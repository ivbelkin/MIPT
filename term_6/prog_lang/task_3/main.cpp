#include <iostream>

#include "VirtualFuncTools.h"


VIRTUAL_CLASS( Base )
public:
    Base( int a_ );

    DECLARE_METHOD( Both );
    DECLARE_METHOD( OnlyBase );

protected:
    int a;
END_CLASS( Base )

DEFINE_CONSTRUCTOR( Base, int a_ ) :
    a( a_ )
BEGIN_CONSTRUCTOR
    std::cout << "Base( " << a << " )" << std::endl;
END_CONSTRUCTOR

DEFINE_METHOD( Base, Both )
    std::cout << "Base::Both a = " << self.a << std::endl;
END_METHOD

DEFINE_METHOD( Base, OnlyBase )
    std::cout << "Base::OnlyBase a = " << self.a << std::endl;
END_METHOD


VIRTUAL_CLASS_DERIVED( Derived, Base )
public:
    Derived( int a_, int b_ );

    DECLARE_METHOD( Both );
    DECLARE_METHOD( OnlyDerived );

private:
    int b;
END_DERIVED( Derived, Base )

DEFINE_CONSTRUCTOR( Derived, int a_, int b_ ) :
    Base( a_ ),
    b( b_ )
BEGIN_CONSTRUCTOR
    std::cout << "Derived( " << a << ", " << b << " )" << std::endl;
END_CONSTRUCTOR

DEFINE_METHOD( Derived, Both )
    std::cout << "Derived::Both a = " << self.a << " b = " << self.b << std::endl;
END_METHOD

DEFINE_METHOD( Derived, OnlyDerived )
    std::cout << "Derived::OnlyDerived a = " << self.a << " b = " << self.b << std::endl;
END_METHOD


int main()
{
    Base base( 12 );
    Derived derived( 3, 7 );
    Base* derPtr = &derived;

    std::cout << std::endl;

    VIRTUAL_CALL( &base, Both );
    VIRTUAL_CALL( &base, OnlyBase );
    VIRTUAL_CALL( &base, OnlyDerived );

    std::cout << std::endl;

    VIRTUAL_CALL( &derived, Both );
    VIRTUAL_CALL( &derived, OnlyBase );
    VIRTUAL_CALL( &derived, OnlyDerived );

    std::cout << std::endl;

    VIRTUAL_CALL( derPtr, Both );
    VIRTUAL_CALL( derPtr, OnlyBase );
    VIRTUAL_CALL( derPtr, OnlyDerived );

    return 0;
}
