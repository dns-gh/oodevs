// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/DetectionComputer.h"
#include "MockAgent.h"

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : computer( agent )
        {}
        MockAgent agent;
        detection::DetectionComputer computer;
    };
}

BOOST_FIXTURE_TEST_CASE( agent_can_be_seen_by_default, Fixture )
{
    BOOST_CHECK( computer.CanBeSeen() );
}

BOOST_FIXTURE_TEST_CASE( underground_agent_cannot_be_seen, Fixture )
{
    computer.SetUnderground( true );
    BOOST_CHECK( !computer.CanBeSeen() );
}

BOOST_FIXTURE_TEST_CASE( already_seen_agent_cannot_be_seen_if_underground, Fixture )
{
    computer.AlreadyPerceived();
    computer.SetUnderground( true );
    BOOST_CHECK( !computer.CanBeSeen() );
}

BOOST_FIXTURE_TEST_CASE( stealth_agent_cannot_be_seen, Fixture )
{
    computer.NotifyStealth();
    BOOST_CHECK( !computer.CanBeSeen() );
}

BOOST_FIXTURE_TEST_CASE( already_seen_agent_can_be_seen_even_if_in_stealth_mode, Fixture )
{
    computer.AlreadyPerceived();
    computer.NotifyStealth();
    BOOST_CHECK( computer.CanBeSeen() );
}
