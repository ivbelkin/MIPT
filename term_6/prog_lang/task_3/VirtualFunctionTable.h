#ifndef TASK_3_VIRTUALFUNCTIONTABLE_H
#define TASK_3_VIRTUALFUNCTIONTABLE_H

#include <vector>
#include <map>

class CVirtualFunctionTable {

public:

    int DefineMethod( const char* methodName,
                      void (*methodBody)( void* const _self ) );

    void VirtualCall( const char* methodName, void* const object );

private:
    std::map<const char*, int> methodIds;
    std::vector<void (*)(void* const)> methods;
};

#endif //TASK_3_VIRTUALFUNCTIONTABLE_H
