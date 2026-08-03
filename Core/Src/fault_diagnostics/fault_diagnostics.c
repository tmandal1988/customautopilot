#include "fault_diagnostics/fault_diagnostics.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "stm32h7xx.h"
#include "task.h"

volatile FaultDiagnosticsRecord g_fault_diagnostics_record
    __attribute__((section(".fault_record"), used, aligned(8)));

// FreeRTOS deliberately exports this pointer from tasks.c. Treat it as an
// opaque address: fault capture must not dereference a possibly damaged TCB.
extern void *volatile pxCurrentTCB;

static bool RangeContains(uintptr_t address, size_t bytes,
                          uintptr_t begin, uintptr_t end) {
  return (address >= begin) && (bytes <= (end - begin)) &&
         (address <= (end - bytes));
}

static bool IsReadableStackFrame(const uint32_t *frame) {
  const uintptr_t address = (uintptr_t)frame;
  const size_t bytes = 8U * sizeof(uint32_t);

  if ((address & (sizeof(uint32_t) - 1U)) != 0U) {
    return false;
  }

  return RangeContains(address, bytes, 0x20000000UL, 0x20020000UL) ||
         RangeContains(address, bytes, 0x24000000UL, 0x24080000UL) ||
         RangeContains(address, bytes, 0x30000000UL, 0x30048000UL) ||
         RangeContains(address, bytes, 0x38000000UL, 0x38010000UL);
}

static void CaptureCoreState(uint32_t reason) {
  // Invalidate first, then publish magic only after every field is stored.
  const uint32_t previous_sequence =
      ((g_fault_diagnostics_record.magic == FAULT_DIAGNOSTICS_MAGIC) &&
       (g_fault_diagnostics_record.version == FAULT_DIAGNOSTICS_VERSION))
          ? g_fault_diagnostics_record.sequence
          : 0U;
  g_fault_diagnostics_record.magic = 0U;
  g_fault_diagnostics_record.version = FAULT_DIAGNOSTICS_VERSION;
  g_fault_diagnostics_record.reason = reason;
  g_fault_diagnostics_record.sequence = previous_sequence + 1U;

  g_fault_diagnostics_record.exception_return = 0U;
  g_fault_diagnostics_record.exception_stack_pointer = 0U;
  g_fault_diagnostics_record.stacked_r0 = 0U;
  g_fault_diagnostics_record.stacked_r1 = 0U;
  g_fault_diagnostics_record.stacked_r2 = 0U;
  g_fault_diagnostics_record.stacked_r3 = 0U;
  g_fault_diagnostics_record.stacked_r12 = 0U;
  g_fault_diagnostics_record.stacked_lr = 0U;
  g_fault_diagnostics_record.stacked_pc = 0U;
  g_fault_diagnostics_record.stacked_xpsr = 0U;
  g_fault_diagnostics_record.assert_file = 0U;
  g_fault_diagnostics_record.assert_line = 0U;
  g_fault_diagnostics_record.stack_task = 0U;
  g_fault_diagnostics_record.stack_task_name = 0U;

  g_fault_diagnostics_record.cfsr = SCB->CFSR;
  g_fault_diagnostics_record.hfsr = SCB->HFSR;
  g_fault_diagnostics_record.dfsr = SCB->DFSR;
  g_fault_diagnostics_record.afsr = SCB->AFSR;
  g_fault_diagnostics_record.mmfar = SCB->MMFAR;
  g_fault_diagnostics_record.bfar = SCB->BFAR;
  g_fault_diagnostics_record.shcsr = SCB->SHCSR;
  g_fault_diagnostics_record.icsr = SCB->ICSR;

  g_fault_diagnostics_record.msp = __get_MSP();
  g_fault_diagnostics_record.psp = __get_PSP();
  g_fault_diagnostics_record.control = __get_CONTROL();
  g_fault_diagnostics_record.basepri = __get_BASEPRI();
  g_fault_diagnostics_record.faultmask = __get_FAULTMASK();
  g_fault_diagnostics_record.ipsr = __get_IPSR();
  g_fault_diagnostics_record.current_tcb = (uint32_t)pxCurrentTCB;
}

static __attribute__((noreturn)) void CommitAndHalt(void) {
  __DMB();
  g_fault_diagnostics_record.magic = FAULT_DIAGNOSTICS_MAGIC;
  __DSB();
  __ISB();

  for (;;) {
    // Keep the core stopped at a stable location for an attach-without-reset
    // SWD inspection. Interrupts are disabled before capture begins.
    __NOP();
  }
}

__attribute__((noreturn, noinline, used))
void FaultDiagnostics_CaptureExceptionImpl(const uint32_t *exception_stack,
                                           uint32_t exception_return,
                                           uint32_t reason,
                                           uint32_t entry_msp) {
  CaptureCoreState(reason);
  g_fault_diagnostics_record.msp = entry_msp;
  g_fault_diagnostics_record.exception_return = exception_return;
  g_fault_diagnostics_record.exception_stack_pointer =
      (uint32_t)exception_stack;

  // If bit 4 is clear, an extended FP frame precedes the eight-word core
  // frame. Do not dereference any frame after a stacking/lazy-stacking fault.
  const uint32_t stacking_fault_mask =
      (1UL << 3) | (1UL << 4) | (1UL << 5) |
      (1UL << 11) | (1UL << 12) | (1UL << 13);
  uintptr_t core_frame_address = (uintptr_t)exception_stack;
  bool core_frame_address_valid = true;
  if ((exception_return & (1UL << 4)) == 0U) {
    const uintptr_t fp_frame_bytes = 18U * sizeof(uint32_t);
    if (core_frame_address > (UINTPTR_MAX - fp_frame_bytes)) {
      core_frame_address_valid = false;
    } else {
      core_frame_address += fp_frame_bytes;
    }
  }
  const uint32_t *const core_frame =
      (const uint32_t *)core_frame_address;

  if (core_frame_address_valid &&
      ((g_fault_diagnostics_record.cfsr & stacking_fault_mask) == 0U) &&
      IsReadableStackFrame(core_frame)) {
    g_fault_diagnostics_record.stacked_r0 = core_frame[0];
    g_fault_diagnostics_record.stacked_r1 = core_frame[1];
    g_fault_diagnostics_record.stacked_r2 = core_frame[2];
    g_fault_diagnostics_record.stacked_r3 = core_frame[3];
    g_fault_diagnostics_record.stacked_r12 = core_frame[4];
    g_fault_diagnostics_record.stacked_lr = core_frame[5];
    g_fault_diagnostics_record.stacked_pc = core_frame[6];
    g_fault_diagnostics_record.stacked_xpsr = core_frame[7];
  }

  CommitAndHalt();
}

__attribute__((naked, noreturn))
void FaultDiagnostics_CaptureException(const uint32_t *exception_stack,
                                       uint32_t exception_return,
                                       uint32_t reason) {
  __asm volatile(
      "mrs r12, primask\n"
      "ldr lr, =g_fault_diagnostics_record\n"
      "str r12, [lr, #104]\n"
      "mrs r3, msp\n"
      "cpsid i\n"
      "ldr r12, =__fault_msp_stack_top__\n"
      "msr msp, r12\n"
      "isb\n"
      "b FaultDiagnostics_CaptureExceptionImpl\n");
}

__attribute__((noreturn, noinline, used))
void FaultDiagnostics_CaptureAssertImpl(const char *file, uint32_t line,
                                        uint32_t entry_msp,
                                        uint32_t entry_psp) {
  CaptureCoreState(FAULT_REASON_CONFIG_ASSERT);
  g_fault_diagnostics_record.msp = entry_msp;
  g_fault_diagnostics_record.psp = entry_psp;
  g_fault_diagnostics_record.assert_file = (uint32_t)file;
  g_fault_diagnostics_record.assert_line = line;
  CommitAndHalt();
}

__attribute__((naked, noreturn))
void FaultDiagnostics_CaptureAssert(const char *file, uint32_t line) {
  __asm volatile(
      "mrs r12, primask\n"
      "ldr lr, =g_fault_diagnostics_record\n"
      "str r12, [lr, #104]\n"
      "mrs r2, msp\n"
      "mrs r3, psp\n"
      "cpsid i\n"
      "ldr r12, =__fault_msp_stack_top__\n"
      "msr msp, r12\n"
      "ldr r12, =__fault_psp_stack_top__\n"
      "msr psp, r12\n"
      "isb\n"
      "b FaultDiagnostics_CaptureAssertImpl\n");
}

__attribute__((noreturn, noinline, used))
void FaultDiagnostics_CaptureErrorImpl(uint32_t entry_msp,
                                       uint32_t entry_psp) {
  CaptureCoreState(FAULT_REASON_ERROR_HANDLER);
  g_fault_diagnostics_record.msp = entry_msp;
  g_fault_diagnostics_record.psp = entry_psp;
  CommitAndHalt();
}

__attribute__((naked, noreturn))
void FaultDiagnostics_CaptureError(void) {
  __asm volatile(
      "mrs r12, primask\n"
      "ldr lr, =g_fault_diagnostics_record\n"
      "str r12, [lr, #104]\n"
      "mrs r0, msp\n"
      "mrs r1, psp\n"
      "cpsid i\n"
      "ldr r12, =__fault_msp_stack_top__\n"
      "msr msp, r12\n"
      "ldr r12, =__fault_psp_stack_top__\n"
      "msr psp, r12\n"
      "isb\n"
      "b FaultDiagnostics_CaptureErrorImpl\n");
}

__attribute__((noreturn, noinline, used))
void FaultDiagnostics_CaptureStackOverflowImpl(TaskHandle_t task,
                                               char *task_name,
                                               uint32_t entry_msp,
                                               uint32_t entry_psp) {
  CaptureCoreState(FAULT_REASON_STACK_OVERFLOW);
  g_fault_diagnostics_record.msp = entry_msp;
  g_fault_diagnostics_record.psp = entry_psp;
  g_fault_diagnostics_record.stack_task = (uint32_t)task;
  g_fault_diagnostics_record.stack_task_name = (uint32_t)task_name;
  CommitAndHalt();
}

__attribute__((naked, noreturn))
void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name) {
  __asm volatile(
      "mrs r12, primask\n"
      "ldr lr, =g_fault_diagnostics_record\n"
      "str r12, [lr, #104]\n"
      "mrs r2, msp\n"
      "mrs r3, psp\n"
      "cpsid i\n"
      "ldr r12, =__fault_msp_stack_top__\n"
      "msr msp, r12\n"
      "ldr r12, =__fault_psp_stack_top__\n"
      "msr psp, r12\n"
      "isb\n"
      "b FaultDiagnostics_CaptureStackOverflowImpl\n");
}

__attribute__((noreturn, noinline, used))
void FaultDiagnostics_CaptureMallocFailedImpl(uint32_t entry_msp,
                                              uint32_t entry_psp) {
  CaptureCoreState(FAULT_REASON_MALLOC_FAILED);
  g_fault_diagnostics_record.msp = entry_msp;
  g_fault_diagnostics_record.psp = entry_psp;
  CommitAndHalt();
}

__attribute__((naked, noreturn))
void vApplicationMallocFailedHook(void) {
  __asm volatile(
      "mrs r12, primask\n"
      "ldr lr, =g_fault_diagnostics_record\n"
      "str r12, [lr, #104]\n"
      "mrs r0, msp\n"
      "mrs r1, psp\n"
      "cpsid i\n"
      "ldr r12, =__fault_msp_stack_top__\n"
      "msr msp, r12\n"
      "ldr r12, =__fault_psp_stack_top__\n"
      "msr psp, r12\n"
      "isb\n"
      "b FaultDiagnostics_CaptureMallocFailedImpl\n");
}
