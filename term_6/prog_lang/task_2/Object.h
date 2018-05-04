#ifndef TASK_2_OBJECT_H
#define TASK_2_OBJECT_H


class CObject {

public:
    CObject();
    CObject( const CObject& obj );
    virtual ~CObject();

    static CObject* GetHead();

private:
    CObject* prev;
    static CObject* head;

    void pushMe();
};

#endif //TASK_2_OBJECT_H
