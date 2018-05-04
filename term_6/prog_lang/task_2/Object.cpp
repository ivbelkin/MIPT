#include "Object.h"


CObject* CObject::head = nullptr;

CObject::CObject()
{
    pushMe();
}

CObject::CObject( const CObject& obj )
{
    pushMe();
}

CObject::~CObject()
{
    head = prev;
}

CObject* CObject::GetHead()
{
    return head;
}

void CObject::pushMe()
{
    prev = head;
    head = this;
}
