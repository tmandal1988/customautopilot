#include "executor/static_executor.h"

// StaticExecutor is template-based so each executor can use compile-time fixed
// table sizes without heap allocation. This translation unit intentionally
// exists only to make the executor module visible in IDEs and build manifests.
