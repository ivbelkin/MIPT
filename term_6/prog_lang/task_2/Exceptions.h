#include <csetjmp>

#include "ExceptionManager.h"
#include "TryCatchBlock.h"
#include "Definitions.h"

#ifndef EXCEPTIONS
#define EXCEPTIONS
extern CExceptionManager _excManager;
#endif
