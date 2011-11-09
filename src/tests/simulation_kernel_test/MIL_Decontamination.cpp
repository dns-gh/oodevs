// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "Entities/Populations/MIL_DecontaminationEffect.h"
#include "Entities/Populations/MIL_PopulationHumans.h"

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : humans_( 100u, 100u, 100u, 100u )
            , effect_( humans_, 10u )
        {
            // NOTHING
        }
        MIL_PopulationHumans humans_;
        MIL_DecontaminationEffect effect_;

        void Check( unsigned int healthy, unsigned int wounded, unsigned int contaminated, unsigned int dead )
        {
            BOOST_CHECK_EQUAL( humans_.GetHealthyHumans(), healthy );
            BOOST_CHECK_EQUAL( humans_.GetWoundedHumans(), wounded );
            BOOST_CHECK_EQUAL( humans_.GetContaminatedHumans(), contaminated );
            BOOST_CHECK_EQUAL( humans_.GetDeadHumans(), dead );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( decontamination_effect, Fixture )
{
    effect_.Apply( 1., 10u );
    Check( 101u, 100u, 99u, 100u );
    effect_.Apply( 1., 19u );
    Check( 101u, 100u, 99u, 100u );
    effect_.Apply( 0.5, 29u );
    Check( 101u, 100u, 99u, 100u );
    effect_.Apply( 0.5, 30u );
    Check( 102u, 100u, 98u, 100u );
}

BOOST_FIXTURE_TEST_CASE( no_decontamination_effect, Fixture )
{
    effect_.Apply( 0., 100u );
    Check( 101u, 100u, 10u, 100u );
}
