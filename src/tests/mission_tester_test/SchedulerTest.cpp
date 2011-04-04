// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "mission_tester_test_pch.h"
#include "mission_tester/Filter_ABC.h"
#include "mission_tester/Schedulable_ABC.h"
#include "mission_tester/Scheduler.h"

MOCK_BASE_CLASS( MockSchedulable, mission_tester::Schedulable_ABC )
{
    MOCK_METHOD( Trigger, 1 )
    MOCK_METHOD( Matches, 1 )
};

MOCK_BASE_CLASS( MockFilter, mission_tester::Filter_ABC )
{
    MOCK_METHOD( Accepts, 1 )
};

BOOST_AUTO_TEST_CASE( scheduler_schedules_a_schedulable )
{
    MockFilter filter;
    mission_tester::Scheduler scheduler( filter );
    MockSchedulable schedulable;
    MOCK_EXPECT( filter, Accepts ).once().with( mock::same( schedulable ) ).returns( true );
    scheduler.Schedule( schedulable );
}