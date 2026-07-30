/*
 * IST8310 build-time options.
 */

#pragma once

// Production default: omit all IST8310 print code, strings, counters, and the
// dedicated print task. Override with compiler symbol
// IST8310_ENABLE_DIAGNOSTICS=1, or change the default below, when debugging.
#ifndef IST8310_ENABLE_DIAGNOSTICS
#define IST8310_ENABLE_DIAGNOSTICS 0
#endif

#if (IST8310_ENABLE_DIAGNOSTICS != 0) && (IST8310_ENABLE_DIAGNOSTICS != 1)
#error "IST8310_ENABLE_DIAGNOSTICS must be 0 or 1"
#endif
