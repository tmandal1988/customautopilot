# RTOS hardware baseline

The firmware can record a low-overhead hardware timing baseline in binary log
Topic 12. The instrumentation is disabled by default and is bench-only until
each stage below passes on the target board.

## Recover the uninstrumented firmware

Build the normal firmware with every metrics and intrusive diagnostic path
disabled:

```sh
cmake --preset debug \
  -DENABLE_RTOS_METRICS=OFF \
  -DENABLE_RTOS_METRICS_LOGGING=OFF \
  -DENABLE_RTOS_CONTEXT_SWITCH_METRICS=OFF \
  -DENABLE_RTOS_STACK_WATERMARK_METRICS=OFF \
  -DENABLE_RTOS_STACK_OVERFLOW_DIAGNOSTICS=OFF
cmake --build --preset debug
```

With this configuration there is no DWT initialization, executor probe,
Topic 12 writer, scheduler hook, stack fill scan, or stack-overflow check in
the running firmware.

## Re-enable in isolated stages

First enable only the task-owned, out-of-line DWT probes. This stage does not
write Topic 12 and does not touch the scheduler or scan stacks:

```sh
cmake --preset debug \
  -DENABLE_RTOS_METRICS=ON \
  -DENABLE_RTOS_METRICS_LOGGING=OFF \
  -DENABLE_RTOS_CONTEXT_SWITCH_METRICS=OFF \
  -DENABLE_RTOS_STACK_WATERMARK_METRICS=OFF
cmake --build --preset debug
```

After that image passes a sustained bench run, enable paced Topic 12 output:

```sh
cmake --preset debug \
  -DENABLE_RTOS_METRICS=ON \
  -DENABLE_RTOS_METRICS_LOGGING=ON \
  -DENABLE_RTOS_CONTEXT_SWITCH_METRICS=OFF \
  -DENABLE_RTOS_STACK_WATERMARK_METRICS=OFF
cmake --build --preset debug
```

Add context-switch counting only after the logged executor build passes. The
stack-watermark option is last and remains optional. It scans one task per
second only while the FCS state is `INACTIVE`; the retained FreeRTOS fill
pattern still reports peak stack use from the complete preceding flight.

Every metrics-enabled application task receives a 256-byte diagnostic stack
reserve. This changes allocation only during task creation and adds no
steady-state execution.

## Collect a representative baseline

1. Complete the staged bench validation above, then build the Topic 12 stage
   required for the measurement. Power-cycle the board so lifetime maxima and
   counters begin at zero.
2. Start the normal SD log, exercise representative ground and flight modes for
   at least five minutes, return the FCS to `INACTIVE` for at least one complete
   14-task rotation, and then issue the normal `STOP` command before removing
   power. Stack scanning remains optional; without it, stack fields are marked
   as not sampled.
3. Decode the copied log:

```sh
python3 tools/rtos_metrics/decode_rtos_metrics.py log_file.bin \
  --csv rtos_metrics.csv
```

The summary reports, per registered application task:

- `max elapsed`: observed inclusive WCET between the executor's release and
  completion probes. DWT continues during interrupts and preemption, so this is
  a conservative response-time observation, not pure task CPU time or a formal
  static WCET proof.
- `max response`: completion relative to the expected periodic release. It is
  elapsed time plus positive release latency.
- `max jitter`: maximum absolute error from the DWT periodic release phase.
- `misses/releases`: completion later than the configured deadline, with the
  deadline set to the task period for current periodic executors.
- `stack free`: minimum-ever unused stack bytes returned by FreeRTOS's fill
  pattern scan. Stack use is `stack size - stack free`.
- context-switch mean/max rate: true TCB changes per second, including the
  FreeRTOS Idle and Timer tasks.

All 14 currently active `TaskBase` tasks are covered. Stack high-water records
apply to those application tasks; the FreeRTOS Idle/Timer tasks and the optional
IST diagnostics print task are not registered and therefore have no stack row.

Event-driven executors have elapsed-time and stack data but no artificial
period, release-jitter, or deadline-miss number. GPS's multi-second recovery and
interactive SD `LIST`/`COPY` work are deliberately outside bounded DWT regions;
the STM32H743's 32-bit 480 MHz CYCCNT wraps every 8.95 seconds. Period and
deadline values must remain below the unambiguous signed half-wrap (4.47 s).
The firmware detects a longer future configuration, flags it in the log, and
retains elapsed samples while suppressing invalid jitter/response/miss data.
An SD-card fault can reach the driver's 30-second timeout, so the SD event
sample from such a fault is modulo-wrapped and must not be used as WCET; the
SD task's existing tick-based `max_write_ticks`/`max_sync_ticks` telemetry is
the source for timeout-path diagnosis.
The current firmware does not use tickless idle. If clock-gated/tickless sleep
is enabled later, CYCCNT must be revalidated or release timing moved to a timer
that continues running in that power state.

## Runtime cost and isolation

- A measured dispatch performs two DWT register reads and fixed, relaxed
  32-bit counter/max updates. It performs no allocation, lock, logging, print,
  catalog scan, or I/O.
- When explicitly enabled, the context-switch hook makes one call to a short
  optimized leaf function on each scheduler decision. It compares one cached
  TCB pointer and increments one wrapping 32-bit counter only on a true task
  change. FreeRTOS runtime statistics are intentionally disabled.
- The heartbeat LED task retains its original priority and behavior.
- Topic 12 logging writes one fixed 128-byte task row per second. If the
  independently gated stack option is enabled, Logger scans only that row's
  task and only in `INACTIVE`; no stack fill scan runs while armed or in
  flight. There is no metrics burst, all-task scan, or scheduler suspension.
- All counters are cumulative and are never reset in flight. The logger and
  optional inactive-only scanner read fixed per-task slots; control tasks never
  wait for them.

The logged payload is permanently fixed at 128 bytes because the current
binary framing has no payload-length field. Future incompatible metrics schemas
must receive a new TopicID.

## If the board freezes

The normal image now records failure-only evidence in
`g_fault_diagnostics_record`. This path has no task, timer, polling, logging,
or success-path I/O. Exception and assert entry switch to separate uncached
512-byte DTCM emergency MSP/PSP stacks before any C code runs.

Attach SWD without resetting or power-cycling and inspect the record with the
matching ELF:

```gdb
p/x g_fault_diagnostics_record
info symbol (void *)g_fault_diagnostics_record.stacked_pc
info symbol (void *)g_fault_diagnostics_record.stacked_lr
```

Valid records have `magic == 0x4641554c`. Reason values are: 2 HardFault,
3 MemManage, 4 BusFault, 5 UsageFault, 6 `Error_Handler`, 7 `configASSERT`, and
8 stack overflow. `cfsr`, `hfsr`, `stacked_pc`, `stacked_lr`, `current_tcb`,
and the assert file/line fields identify the stopped path. The NOLOAD record
can survive a warm software reset, but not a power cycle.

`ENABLE_RTOS_STACK_OVERFLOW_DIAGNOSTICS=ON` enables FreeRTOS level-2 stack
checking for a bench diagnosis. It adds scheduler work and therefore remains
off in the normal flight build.
