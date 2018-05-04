#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class CTokenizator {
public:
    class CTokenIterator {
    public:
        std::string operator*() const { return token; }

        bool operator==( const CTokenIterator& other ) const { return id == other.id; }
        bool operator!=( const CTokenIterator& other ) const { return !(*this == other); }

        CTokenIterator& operator++()
        {
            token = tokenizator.getNextToken();
            if( token.empty() ) {
                id = -1;
            }
            return *this;
        }

    private:
        int id;
        static int nextId;
        explicit CTokenIterator( CTokenizator& _tokenizator ) :
            tokenizator( _tokenizator ),
            id( nextId++ )
        {}

        std::string token;
        CTokenizator& tokenizator;

        friend class CTokenizator;
    };

    CTokenizator() : stream( std::cin ) {}
    explicit CTokenizator( std::istream& _stream ) : stream( _stream ) {}

    CTokenIterator begin() { return CTokenIterator( *this ); }
    CTokenIterator end()
    {
        CTokenIterator it( *this );
        it.id = -1;
        return it;
    }

private:
    std::istream& stream;
    std::string getNextToken()
    {
        std::string s;
        stream >> s;
        return s;
    }
};

int CTokenizator::CTokenIterator::nextId = 0;

int main()
{
    std::string s = "Hello, World! I'm cool programmer! But it is not exactly...";
    std::stringstream s1( s );
    CTokenizator t1( s1 );
    CTokenizator t2( s1 );
    //for(auto it = t.begin(); it != t.end(); ++it) {
    //    std::cout << *it << std::endl;
    //}

    auto it1 = t1.begin();
    auto it2 = t2.begin();

    for( auto&& str : t1 ) {
        std::cout << str << std::endl;
    }
    return 0;
}
