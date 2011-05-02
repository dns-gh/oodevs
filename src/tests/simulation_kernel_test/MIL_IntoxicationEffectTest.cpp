// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "Entities/Populations/MIL_IntoxicationEffect.h"
#include "Entities/Populations/MIL_PopulationHumans.h"

namespace
{
    class ConfigurationFixture
    {
    public:
        ConfigurationFixture()
            : humans_      ( 100u, 100u, 100u, 100u )
            , intoxication_( humans_, 10u, 0u )
        {
            // NOTHING
        }
        MIL_PopulationHumans humans_;
        MIL_IntoxicationEffect intoxication_;

        void Check( unsigned int healthy, unsigned int wounded, unsigned int contaminated, unsigned int dead )
        {
            BOOST_CHECK_EQUAL( humans_.GetHealthyHumans(), healthy );
            BOOST_CHECK_EQUAL( humans_.GetWoundedHumans(), wounded );
            BOOST_CHECK_EQUAL( humans_.GetContaminatedHumans(), contaminated );
            BOOST_CHECK_EQUAL( humans_.GetDeadHumans(), dead );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( no_intoxication_effect, ConfigurationFixture )
{
    Check( 100u, 100u, 100u, 100u );
}

BOOST_FIXTURE_TEST_CASE( intoxication_effect, ConfigurationFixture )
{
    intoxication_.Add( 0.5, 0.3 );
    Check( 20u, 224u, 20u, 136u );
}

BOOST_FIXTURE_TEST_CASE( liquid_and_gas_intoxication_effects, ConfigurationFixture )
{
    intoxication_.Add( 0.5, 0.3 );
    intoxication_.Add( 0.2, 0.1 );
    Check( 14u, 233u, 14u, 139u );
}

BOOST_FIXTURE_TEST_CASE( human_number_does_not_change, ConfigurationFixture )
{
    intoxication_.Add( 0.33, 0.21 );
    BOOST_CHECK_EQUAL( humans_.GetAllHumans(), 400u );
}

namespace
{
    class Fixture : public ConfigurationFixture
    {
    public:
        Fixture()
        {
            intoxication_.Add( 0.5, 0.3 );
            Check( 20u, 224u, 20u, 136u );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( no_effect_updated_when_delay_does_not_pass, Fixture )
{
    intoxication_.Update( 1u );
    Check( 20u, 224u, 20u, 136u );
    intoxication_.Update( 5u );
    Check( 20u, 224u, 20u, 136u );
    intoxication_.Update( 9u );
    Check( 20u, 224u, 20u, 136u );
}

BOOST_FIXTURE_TEST_CASE( effect_updated, Fixture )
{
    intoxication_.Update( 10u );
    Check( 4u, 249u, 4u, 143u );
    intoxication_.Update( 16u );
    Check( 4u, 249u, 4u, 143u );
    intoxication_.Update( 19u );
    Check( 4u, 249u, 4u, 143u );
    intoxication_.Update( 21u );
    Check( 0u, 256u, 0u, 144u );
}

