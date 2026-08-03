#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  FAULT_DIAGNOSTICS_MAGIC = 0x4641554CUL,  // "FAUL"
  FAULT_DIAGNOSTICS_VERSION = 1U,
};

typedef enum {
  FAULT_REASON_NONE = 0U,
  FAULT_REASON_NMI = 1U,
  FAULT_REASON_HARDFAULT = 2U,
  FAULT_REASON_MEMMANAGE = 3U,
  FAULT_REASON_BUSFAULT = 4U,
  FAULT_REASON_USAGEFAULT = 5U,
  FAULT_REASON_ERROR_HANDLER = 6U,
  FAULT_REASON_CONFIG_ASSERT = 7U,
  FAULT_REASON_STACK_OVERFLOW = 8U,
  FAULT_REASON_MALLOC_FAILED = 9U,
} FaultDiagnosticsReason;

// Read `g_fault_diagnostics_record` through SWD after a freeze. `magic` is
// committed last; ignore a record unless it equals FAULT_DIAGNOSTICS_MAGIC.
typedef struct {
  uint32_t magic;
  uint32_t version;
  uint32_t reason;
  uint32_t sequence;

  uint32_t exception_return;
  uint32_t exception_stack_pointer;
  uint32_t stacked_r0;
  uint32_t stacked_r1;
  uint32_t stacked_r2;
  uint32_t stacked_r3;
  uint32_t stacked_r12;
  uint32_t stacked_lr;
  uint32_t stacked_pc;
  uint32_t stacked_xpsr;

  uint32_t cfsr;
  uint32_t hfsr;
  uint32_t dfsr;
  uint32_t afsr;
  uint32_t mmfar;
  uint32_t bfar;
  uint32_t shcsr;
  uint32_t icsr;

  uint32_t msp;
  uint32_t psp;
  uint32_t control;
  uint32_t basepri;
  uint32_t primask;
  uint32_t faultmask;
  uint32_t ipsr;
  uint32_t current_tcb;

  uint32_t assert_file;
  uint32_t assert_line;
  uint32_t stack_task;
  uint32_t stack_task_name;
} FaultDiagnosticsRecord;

#if defined(__cplusplus)
static_assert(sizeof(FaultDiagnosticsRecord) == 136U);
static_assert(offsetof(FaultDiagnosticsRecord, reason) == 8U);
static_assert(offsetof(FaultDiagnosticsRecord, stacked_pc) == 48U);
static_assert(offsetof(FaultDiagnosticsRecord, cfsr) == 56U);
static_assert(offsetof(FaultDiagnosticsRecord, primask) == 104U);
static_assert(offsetof(FaultDiagnosticsRecord, assert_file) == 120U);
#else
_Static_assert(sizeof(FaultDiagnosticsRecord) == 136U,
               "fault record ABI changed");
_Static_assert(offsetof(FaultDiagnosticsRecord, reason) == 8U,
               "fault reason offset changed");
_Static_assert(offsetof(FaultDiagnosticsRecord, stacked_pc) == 48U,
               "fault PC offset changed");
_Static_assert(offsetof(FaultDiagnosticsRecord, cfsr) == 56U,
               "fault status offset changed");
_Static_assert(offsetof(FaultDiagnosticsRecord, primask) == 104U,
               "fault mask offset changed");
_Static_assert(offsetof(FaultDiagnosticsRecord, assert_file) == 120U,
               "fault assert offset changed");
#endif

extern volatile FaultDiagnosticsRecord g_fault_diagnostics_record;

__attribute__((noreturn)) void FaultDiagnostics_CaptureException(
    const uint32_t *exception_stack, uint32_t exception_return,
    uint32_t reason);
__attribute__((noreturn)) void FaultDiagnostics_CaptureAssert(
    const char *file, uint32_t line);
__attribute__((noreturn)) void FaultDiagnostics_CaptureError(void);

#ifdef __cplusplus
}
#endif
