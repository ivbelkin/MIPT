#ifndef TASK_3_VIRTUALFUNCTOOLS_H
#define TASK_3_VIRTUALFUNCTOOLS_H

#include "VirtualFunctionTable.h"


#define VIRTUAL_CLASS( className ) \
class className { \
private: \
    CVirtualFunctionTable* vptr;


#define END_CLASS( className ) \
protected: \
    static CVirtualFunctionTable vtable; \
}; \
CVirtualFunctionTable className::vtable;


#define VIRTUAL_CLASS_DERIVED( className, baseName ) \
class className : public baseName {


#define END_DERIVED( className, baseName ) \
protected: \
    static CVirtualFunctionTable vtable; \
}; \
CVirtualFunctionTable className::vtable = baseName::vtable;


#define DEFINE_CONSTRUCTOR( className, ... ) \
className::className( __VA_ARGS__ )


#define BEGIN_CONSTRUCTOR {


#define END_CONSTRUCTOR \
*reinterpret_cast<CVirtualFunctionTable**>( this ) = &vtable; \
}


#define DECLARE_METHOD( method ) \
static const int method


#define DEFINE_METHOD( className, method ) \
const int className::method = className::vtable.DefineMethod( #method, \
[]( void* const _self ) { \
    auto self = *reinterpret_cast<className*>( _self ); \


#define END_METHOD });


#define VIRTUAL_CALL( objectPtr, method ) \
(*reinterpret_cast<CVirtualFunctionTable**>(objectPtr))->VirtualCall( #method, reinterpret_cast<void*>( objectPtr ) )


#endif //TASK_3_VIRTUALFUNCTOOLS_H
