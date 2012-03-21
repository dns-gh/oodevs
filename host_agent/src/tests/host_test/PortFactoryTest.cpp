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

    typedef std::vector< std::auto_ptr< Port_ABC > > T_Ports;

    T_Ports MakePorts( PortFactory& factory, size_t size )
    {
        T_Ports reply;
        for( size_t i = 0; i < size; ++i )
            reply.push_back( factory.Create() );
        return reply;
    }

    template< typename T >
    void RemoveAddCheck( PortFactory& factory, T& data, size_t idx )
    {
        int unused = data[idx]->Get();
        data.erase( data.begin() + idx );
        data.push_back( factory.Create() );
        BOOST_CHECK_EQUAL( unused, data.back()->Get() );
    }
}

BOOST_FIXTURE_TEST_CASE( port_factory_throws_when_filled, Fixture )
{
    T_Ports data = MakePorts( factory, 10 );
    BOOST_CHECK_THROW( factory.Create(), std::exception );
}

BOOST_FIXTURE_TEST_CASE( factory_can_return_first_unused_port, Fixture )
{
    T_Ports data = MakePorts( factory, 10 );
    RemoveAddCheck( factory, data, 0 );
}

BOOST_FIXTURE_TEST_CASE( factory_can_return_last_unused_port, Fixture )
{
    T_Ports data = MakePorts( factory, 10 );
    RemoveAddCheck( factory, data, data.size() - 1 );
}

BOOST_FIXTURE_TEST_CASE( factory_can_return_any_unused_port, Fixture )
{
    T_Ports data = MakePorts( factory, 10 );
    for( int i = 0; i < 10; ++i )
        RemoveAddCheck( factory, data, i );
}
