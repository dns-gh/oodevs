// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include <host/PortFactory.h>

using namespace host;

namespace
{
    struct Fixture
    {
        Fixture()
            : factory( 1, 0, 10 )
        {
            // NOTHING
        }
    public:
        PortFactory factory;
    };

    template< typename T >
    void RemoveAddBackAndCheck( PortFactory& factory, T& data, int idx )
    {
        int unused = data[ idx ]->Get();
        typename T::iterator it = data.begin();
        std::advance( it, idx );
        data.erase( it );
        data.push_back( factory.Create() );
        BOOST_CHECK_EQUAL( unused, data.back()->Get() );
    }
}

BOOST_FIXTURE_TEST_CASE( port_factory_throws_when_filled, Fixture )
{
    std::vector< std::auto_ptr< Port_ABC > > data;
    for( int i = 0; i < 10; ++i )
        data.push_back( factory.Create() );
    BOOST_CHECK_THROW( factory.Create(), std::exception );
}

BOOST_FIXTURE_TEST_CASE( port_factory_returns_unused_port, Fixture )
{
    std::vector< std::auto_ptr< Port_ABC > > data;
    for( int i = 0; i < 10; ++i )
        data.push_back( factory.Create() );
    // edge-cases
    RemoveAddBackAndCheck( factory, data, data.size() - 1 );
    RemoveAddBackAndCheck( factory, data, 0 );
    // full rotation
    for( int i = 0; i < 10; ++i )
        RemoveAddBackAndCheck( factory, data, i );
}

BOOST_FIXTURE_TEST_CASE( port_factory_is_stable, Fixture )
{
    std::vector< std::auto_ptr< Port_ABC > > data;
    for( int i = 0; i < 10; ++i )
        data.push_back( factory.Create() );
    int unused = data[5]->Get();
    data.erase( data.begin() + 5 );
    for( int i = 0; i < 10; ++i )
        BOOST_CHECK_EQUAL( unused, factory.Create()->Get() );
}

