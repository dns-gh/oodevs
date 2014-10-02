// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "host_test.h"

#include "host/PortFactory.h"

using namespace host;

namespace
{
    typedef std::vector< Port > T_Ports;

    const int period = 17;
    const int offset = 23;

    template< size_t size >
    struct Fixture
    {
        Fixture()
            : factory( period, offset, offset + size*period )
        {
            for( size_t i = 0; i < size; ++i )
                ports.push_back( factory.Create() );
        }
        void RemoveAddCheck( size_t idx )
        {
            int unused = ports[idx]->Get();
            ports.erase( ports.begin() + idx );
            ports.push_back( factory.Create() );
            BOOST_CHECK_EQUAL( unused, ports.back()->Get() );
        }
        PortFactory factory;
        T_Ports ports;
    };
}

BOOST_FIXTURE_TEST_CASE( factory_throws_when_filled, Fixture< 10 > )
{
    BOOST_CHECK_THROW( factory.Create(), std::exception );
}

BOOST_FIXTURE_TEST_CASE( factory_returns_first_unused_port, Fixture< 10 > )
{
    RemoveAddCheck( 0 );
}

BOOST_FIXTURE_TEST_CASE( factory_returns_last_unused_port, Fixture< 10 > )
{
    RemoveAddCheck( ports.size() - 1 );
}

BOOST_FIXTURE_TEST_CASE( factory_returns_any_unused_port, Fixture< 10 > )
{
    for( size_t i = 0; i < ports.size(); ++i )
        RemoveAddCheck( i );
}

BOOST_FIXTURE_TEST_CASE( factory_throws_on_creating_used_custom_port, Fixture< 10 > )
{
    BOOST_CHECK_THROW( factory.Create( ports[0]->Get() ), std::exception );
}

BOOST_FIXTURE_TEST_CASE( factory_creates_unused_custom_port, Fixture< 10 > )
{
    const int unused = ports[ 5 ]->Get();
    ports.erase( ports.begin() + 5 );
    factory.Create( unused );
}
