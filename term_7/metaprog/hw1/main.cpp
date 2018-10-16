#include <iostream>
#include <vector>

// у пользователей ВК есть новостная лента - INewsFeed
// будем считать, что для того, чтобы пользователь смог пользоваться ей,
// ему нужно сначала подписаться на одну группу. Получать новости из группы можно как
// непосредственно, так и через прокси. Но одна группа это не интересно, поэтому посредствам
// медиатора предоставим пользователю возможность получать их из разных групп. Но вручную это
// делать лень, поэтому с помощью наблюдателя добавим уведомления

enum LinkerMode { PROXY, MEDIATOR, OBSERVER };

class IGroup;

// пользователь, по умолчанию может взаимодействовать только с одним каналом
class INewsFeed {
public:
    // этот метод сообщает ленте о наличии обновлений в группе
    virtual void UpdateNotificationFrom( IGroup& group ) = 0;

    // текущее состояние ленты новостей
    virtual int GetState() = 0;
};

// группа ВК
class IGroup {
public:
    // отвечает доступны ли обновления для ленты, и если да,
    // то обновляет содержимое переменной newState
    virtual bool CheckUpdatesFor( INewsFeed& newsFeed, int& newState ) = 0;

    // метод для админа канала
    virtual void SetState( int newState ) = 0;
};

class CNewsFeed : public INewsFeed {
public:
    explicit CNewsFeed( const char* _name ) :
        name( _name )
    {}

    void UpdateNotificationFrom( IGroup& group )
    {
        group.CheckUpdatesFor( *this, state );
    }

    int GetState()
    {
        return state;
    }

private:
    const char* name;
    int state;
};

class CConcreteGroup : public IGroup {
public:
    explicit CConcreteGroup( const char* _name ) :
            name( _name ),
            state( 0 )
    {}

    bool CheckUpdatesFor( INewsFeed& newsFeed, int& newState )
    {
        bool hasUpdates = state != newsFeed.GetState();
        if( hasUpdates ) {
            newState = newsFeed.GetState();
        }
        return hasUpdates;
    }

    void SetState( int newState )
    {
        state = newState;
    }

private:
    const char* name;
    int state;
};

// не предоставляем никакой реализации, он остается абстрактным и создание объектов невозможно
template<LinkerMode mode>
class Linker : public IGroup {
};

// прокси будет обслуживать только первые maxRequests запросов
// а дальше говорить что обновлений нет и возврашать в качестве состояния последнее доступное
template<>
class Linker<PROXY> : public IGroup {
public:
    Linker( IGroup& _group, int _maxRequests ) :
        group( _group ),
        maxRequests( _maxRequests ),
        nRequests( 0 ),
        state( -1 )
    {}

    bool CheckUpdatesFor( INewsFeed& newsFeed, int& newState )
    {
        if( nRequests < maxRequests ) {
            ++nRequests;
            return group.CheckUpdatesFor( newsFeed, newState );
        } else {
            return false;
        }
    }

private:
    IGroup& group;
    int maxRequests;
    int nRequests;
    int state;

    // переводим в private, т.к. админить через прокси нельзя (пусть будет так)
    void SetState( int newState ) {}
};


template<>
class Linker<MEDIATOR> : public IGroup {
public:
    bool CheckUpdatesFor( INewsFeed& newsFeed, int& newState )
    {
        for( IGroup* group : groups ) {
            bool hasUpdates = group->CheckUpdatesFor( newsFeed, newState );
            if( hasUpdates ) {
                return true;
            }
        }
        return false;
    }

    void AddGroup( IGroup& group )
    {
        groups.push_back( &group );
    }

private:
    std::vector<IGroup*> groups;

    void SetState( int newState ) {}
};

template<>
class Linker<OBSERVER> : public IGroup {
public:
    Linker( IGroup& _group ) :
            group( _group )
    {}

    void SetState( int newState )
    {
        group.SetState( newState );
        for( INewsFeed* subscriber : subscribers ) {
            subscriber->UpdateNotificationFrom( group );
        }
    }

    void Subscribe( INewsFeed& newsFeed )
    {
        subscribers.push_back( &newsFeed );
    }

private:
    IGroup& group;
    std::vector<INewsFeed*> subscribers;

    // теперь, в общем то, не нужна
    bool CheckUpdatesFor( INewsFeed& newsFeed, int& newState ) {}
};

int main()
{
    int state;

    CConcreteGroup g1( "c1" );
    CConcreteGroup g2( "c2" );

    CNewsFeed n1( "n1" );
    CNewsFeed n2( "n2" );

    g1.SetState( 2 );
    g1.CheckUpdatesFor( n1, state );

    n1.GetState();
    n1.UpdateNotificationFrom( g1 );

    Linker<PROXY> l1( g1, 10 );
    l1.CheckUpdatesFor( n2, state );

    Linker<MEDIATOR> l2;
    l2.AddGroup( g1 );
    l2.AddGroup( g2 );
    l2.CheckUpdatesFor( n2, state );

    Linker<OBSERVER> l3( g2 );
    l3.Subscribe( n1 );
    l3.Subscribe( n2 );
    l3.SetState( 3 );

    return 0;
}
