#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <queue>



#define CLASS( className ) \
class className : public CTop { \
public: \
    static CClassNode classNode; \
private:


#define DERIVE( className, ... ) \
class className : public CTop, __VA_ARGS__ { \
public: \
    static CClassNode classNode; \
private:


#define END_CLASS( className ) \
public: \
    CClassNode* classNodePtr = &classNode; \
    uint32_t classSize = sizeof( className ); \
    CBottom bottom; \
}; \
CClassNode className::classNode = CClassNode( #className, nullptr );


#define END_DERIVE( className, ... ) \
public: \
    CClassNode* classNodePtr_ = &classNode; \
    uint32_t classSize = sizeof( className ); \
    CBottom bottom; \
}; \
CClassNode className::classNode = CClassNode( #className, #__VA_ARGS__ );




#define DYNAMIC_CAST( objectPtr, className ) \
reinterpret_cast<className*>( _dynamic_cast( \
    reinterpret_cast<CTop*>( objectPtr ), \
    className::classNode.typeInfo ) )


#define TYPEINFO( className ) className::classNode.typeInfo


#define TYPEID( objectPtr ) reinterpret_cast<CTop*>( objectPtr )->getRoot()->getClassNode()->typeInfo;


class CTypeInfo {
public:
    explicit CTypeInfo( const char* name_ ) :
        name( name_ ),
        id( nextId++ )
    {}

    inline bool operator==( const CTypeInfo& obj )
    {
        return id == obj.id;
    }

    inline bool operator!=( const CTypeInfo& obj )
    {
        return id != obj.id;
    }

    const char* const name;
    const int id;

public:
    static int nextId;
};

int CTypeInfo::nextId = 0;


class CClassNode {

public:
    CClassNode( const char* name, const char* base ) :
        typeInfo( name )
    {
        nameToNode[name] = this;
        if( base != nullptr ) {
            std::vector<std::string> baseNames;
            extractNames( base, baseNames );
            updateEdges( baseNames );
        }
    }

    CTypeInfo typeInfo;
    std::vector<CClassNode*> derivedFrom;
    static std::map<std::string, CClassNode*> nameToNode;

private:
    void extractNames( const char* base, std::vector<std::string>& baseNames )
    {
        std::string s( base );
        s.append(",");
        size_t prev_pos = s.find_first_not_of( ", :", 0);
        size_t pos = 1;
        while( ( pos = s.find_first_of( ", ", prev_pos ) ) != std::string::npos ) {
            std::string token = s.substr( prev_pos, pos - prev_pos );
            if( token != "public" ) {
                baseNames.push_back( token );
            }
            prev_pos = s.find_first_not_of( ", ", pos );
        }
    }

    void updateEdges( std::vector<std::string>& baseNames )
    {
        for( auto base : baseNames ) {
            derivedFrom.push_back( nameToNode[base] );
        }
    }
};

std::map<std::string, CClassNode*> CClassNode::nameToNode;



class CTop {
public:
    uint32_t size = 0;
    uint32_t displacement = 0;

    bool isRoot()
    {
        return displacement == 0;
    }

    CTop* getParent()
    {
        if( !isRoot() ) {
            return reinterpret_cast<CTop*>( reinterpret_cast<char*>( this ) - displacement );
        } else {
            return nullptr;
        }
    }

    CClassNode* getClassNode()
    {
        return *reinterpret_cast<CClassNode**>(
                reinterpret_cast<char*>( this ) + size - 2 * sizeof( CClassNode* ) );
    }

    CTop* getRoot()
    {
        CTop* currentTop = this;
        while( !currentTop->isRoot() ) {
            currentTop = currentTop->getParent();
        }
        return currentTop;
    }

    int getNumDerived()
    {
        return getClassNode()->derivedFrom.size();
    }

    static CTop* getFirstDerived( CTop* classTop )
    {
        return classTop + 1;
    }

    static CTop* getNextDerived( CTop* current )
    {
        return reinterpret_cast<CTop*>(
            reinterpret_cast<char*>( current ) + current->size );
    }
};


class CBottom {
public:
    CBottom()
    {
        CTop* classTop = updateClassTop();
        updateDerivedDisp( classTop );
    }

    CTop* updateClassTop()
    {
        uint32_t classSize = getSize();
        CTop* classTop = getTop();
        classTop->size = classSize;
        return classTop;
    }

    void updateDerivedDisp( CTop* classTop )
    {
        CClassNode* classNode = getClassNode();
        classTop += 1;
        uint32_t displacement = sizeof( CTop );
        for( int i = 0; i < classNode->derivedFrom.size(); ++i ) {
            classTop->displacement = displacement;
            displacement += classTop->size;
            classTop = reinterpret_cast<CTop*>( reinterpret_cast<char*>( classTop ) + classTop->size );
        }
    }

    uint32_t getSize()
    {
        return *reinterpret_cast<uint32_t*>(
                reinterpret_cast<char*>( this ) - sizeof( uint32_t ) );
    }

    CTop* getTop()
    {
        uint32_t classSize = getSize();
        return reinterpret_cast<CTop*>(
            reinterpret_cast<char*>( this ) - sizeof( uint32_t ) + sizeof( CClassNode* ) - classSize );
    }

    CClassNode* getClassNode()
    {
        return *reinterpret_cast<CClassNode**>(
                reinterpret_cast<char*>( this ) - sizeof( uint32_t ) - sizeof( CClassNode* ) );
    }
};

CTop* _bfs( CTop* source, CTypeInfo typeInfo, std::vector<bool>& vis )
{
    std::queue<CTop*> q;
    q.push( source );
    while( !q.empty() ) {
        CTop* u = q.front();
        q.pop();
        vis[u->getClassNode()->typeInfo.id] = true;
        if( u->getClassNode()->typeInfo == typeInfo ) {
            return u;
        }
        CTop* v = CTop::getFirstDerived( u );
        for( int i = 0; i < u->getNumDerived(); ++i ) {
            if( !vis[v->getClassNode()->typeInfo.id] ) {
                q.push( v );
            }
            v = CTop::getNextDerived( v );
        }
    }
}

CTop* _dynamic_cast( CTop* u, CTypeInfo typeInfo )
{
    std::vector<bool> vis( typeInfo.nextId, false );
    while( u != nullptr && u->getClassNode()->typeInfo != typeInfo ) {
        CTop* v = _bfs( u, typeInfo, vis );
        if( v != nullptr ) {
            u = v;
        } else {
            u = u->getParent();
        }
    }
    return u;
}

CLASS( A )
END_CLASS( A )

CLASS( B )
END_CLASS( B )

CLASS( H )
END_CLASS( H )

CLASS( G )
END_CLASS( G )

DERIVE( C, public A, public B )
END_DERIVE( C, A, B )

DERIVE( F, public H, public G )
END_DERIVE( F, H, G )

DERIVE( D, public B )
END_DERIVE( D, B )

DERIVE( E, public C, public F )
END_DERIVE( E, C, F )



int main()
{
    E e;

    typedef G POINTER_CLASS;
    typedef B TARGET_CLASS;

    TARGET_CLASS* origin = static_cast<TARGET_CLASS*>( &e );
    POINTER_CLASS* p = static_cast<POINTER_CLASS*>( &e );
    TARGET_CLASS* casted = DYNAMIC_CAST( p, TARGET_CLASS );

    std::cout << origin << " " << casted << std::endl;

    return 0;
}