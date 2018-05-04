#ifndef TASK_2_TRYCATCHBLOCK_H
#define TASK_2_TRYCATCHBLOCK_H

#include <csetjmp>

#include "Object.h"

struct CTryCatchBlock {
    CTryCatchBlock();
    ~CTryCatchBlock();

    bool CheckType( int exceptionType );

    int exc;
    jmp_buf env;
    CTryCatchBlock* prev;
    CObject* headObj;
    enum { IN_TRY, IN_CATCH } status;
};

#endif //TASK_2_TRYCATCHBLOCK_H
