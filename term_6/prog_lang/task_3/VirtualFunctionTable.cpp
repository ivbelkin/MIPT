#include <iostream>

#include "VirtualFunctionTable.h"

int CVirtualFunctionTable::DefineMethod( const char* methodName,
                  void (*methodBody)( void* const _self ) )
{
    auto it = methodIds.find( methodName );
    int id = -1;
    if( it != methodIds.end() ) {
        id = it->second;
        methods[it->second] = methodBody;
    } else {
        id = methods.size();
        methodIds[methodName] = id;
        methods.push_back( methodBody );
    }
    return id;
}

void CVirtualFunctionTable::VirtualCall( const char* methodName, void* const object )
{
    auto it = methodIds.find( methodName );
    if( it != methodIds.end() ) {
        int id = it->second;
        methods[id]( object );
    } else {
        std::cout << "Method " << methodName << " is unavailable" << std::endl;
    }
}
