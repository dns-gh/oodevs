// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockAgent.h"
#include "MockMIL_Time_ABC.h"
#include "simulation_kernel/DefaultPostureComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PostureTime_ABC.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentType_ABC.h"
#include "simulation_kernel/Entities/Agents/Units/PHY_UnitType.h"
#include "simulation_kernel/MIL_Random_ABC.h"
#include <xeumeuleu/xml.hpp>

#define CHECK_POSTURE( expected, actual ) \
    BOOST_CHECK_MESSAGE( expected == actual, \
        (expected ? (expected)->GetName() : "0") + " != " + (actual ? (actual)->GetName() : "0") )

namespace
{
    MOCK_BASE_CLASS( MockPostureTime, posture::PostureTime_ABC )
    {
        MOCK_METHOD( GetPostureSetupTime, 1 )
        MOCK_METHOD( GetPostureTearDownTime, 1 )
    };
    MOCK_BASE_CLASS( MockMIL_Random, MIL_Random_ABC )
    {
        MOCK_METHOD( rand_oi, 3 )
    };
    MOCK_BASE_CLASS( MockAgentType, MIL_AgentType_ABC )
    {
        MOCK_METHOD( IsMilitia, 0 )
        MOCK_METHOD( IsRefugee, 0 )
        MOCK_METHOD( IsTerrorist, 0 )
        MOCK_METHOD( IsNBC, 0 )
        MOCK_METHOD( GetUnitType, 0 )
        MOCK_METHOD( GetFeedbackTime, 0 )
        MOCK_METHOD( RegisterFunctions, 2 )
        MOCK_METHOD( GetModel, 0 )
        MOCK_METHOD( GetDistanceAvantPoint, 1 )
        MOCK_METHOD( GetDistanceAvantLima, 0 )
        MOCK_METHOD( IsWrittenInODB, 0 )
    };
    struct Fixture
    {
        Fixture()
        {
            MOCK_EXPECT( random.rand_oi ).returns( 0 );
            PHY_Posture::Initialize();
            // Initialize variables to the role initialisation values:
            currentPosture = &PHY_Posture::arret_;
            previousPosture = &PHY_Posture::arret_;
            completionPercentage = 0.;
            stealthFactor = 1.;
            timingFactor = 1.;
            bIsDead = false;
            bDiscreteModeEnabled = false;
            parkedOnEngineerArea = false;
            movement = false;
            loaded = true;
        }
        ~Fixture()
        {
            PHY_Posture::Terminate();
        }
        MockPostureTime time;
        MockMIL_Random random;
        // Emulate PHY_RolePion_Posture :
        const PHY_Posture* currentPosture;
        const PHY_Posture* previousPosture;
        double completionPercentage;
        double stealthFactor;
        double timingFactor;
        bool bIsDead;
        bool bDiscreteModeEnabled;
        bool parkedOnEngineerArea;
        bool movement;
        bool loaded;
        void Tick( const PHY_Posture* expectedPosture, double expectedCompletion )
        {
            posture::DefaultPostureComputer computer( random, time, *previousPosture, *currentPosture, bIsDead, bDiscreteModeEnabled, completionPercentage, stealthFactor, timingFactor, parkedOnEngineerArea );
            if( movement )
                computer.SetPostureMovement();
            else
                computer.UnsetPostureMovement();
            if( loaded )
                computer.NotifyLoaded();
            const posture::PostureComputer_ABC::Results& result = computer.Result();
            CHECK_POSTURE( expectedPosture, result.newPosture_ );
            BOOST_CHECK_CLOSE( expectedCompletion, result.postureCompletionPercentage_, 0.0001 );
            if( currentPosture != result.newPosture_ )
            {
                previousPosture = currentPosture;
                currentPosture = result.newPosture_;
            }
            completionPercentage = result.postureCompletionPercentage_;
        }
    };
}

BOOST_AUTO_TEST_SUITE( posture )

BOOST_FIXTURE_TEST_CASE( newly_created_unit_increases_posture, Fixture )
{
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 2 );
    Tick( &PHY_Posture::posteReflexe_, 0.5 );
    Tick( &PHY_Posture::posteReflexe_, 1. );
    Tick( &PHY_Posture::poste_, 0.5 );
    Tick( &PHY_Posture::poste_, 1. );
    Tick( &PHY_Posture::posteAmenage_, 0.5 );
    Tick( &PHY_Posture::posteAmenage_, 1. );
    Tick( &PHY_Posture::posteAmenage_, 1. );
}

BOOST_FIXTURE_TEST_CASE( stopped_with_movement_decreases_posture, Fixture )
{
    previousPosture = &PHY_Posture::poste_;
    currentPosture = &PHY_Posture::posteAmenage_;
    completionPercentage = 1.;
    movement = true;
    MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 2 );
    Tick( &PHY_Posture::poste_, 0.5 );
    Tick( &PHY_Posture::poste_, 1 ); // current = poste_, previous = posteAmenage_
    Tick( &PHY_Posture::posteReflexe_, 0.5 );
    Tick( &PHY_Posture::posteReflexe_, 1. );
    Tick( &PHY_Posture::arret_, 0.5 );
    Tick( &PHY_Posture::arret_, 1. );
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 0 );
    Tick( &PHY_Posture::mouvement_, 1. );
}

BOOST_FIXTURE_TEST_CASE( moving_posture_but_stopped_changes_to_stopped_posture, Fixture )
{
    previousPosture = &PHY_Posture::arret_;
    currentPosture = &PHY_Posture::mouvement_;
    completionPercentage = 1.;
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 0 );
    Tick( &PHY_Posture::arret_, 1. );
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_but_now_moving_and_unloaded_changes_to_short_stopping_posture, Fixture )
{
    previousPosture = &PHY_Posture::posteReflexe_;
    currentPosture = &PHY_Posture::arret_;
    completionPercentage = 1.;
    movement = true;
    loaded = false;
    MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 0 );
    Tick( &PHY_Posture::posteReflexe_, 1. );
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_but_now_moving_and_loaded_in_discrete_mode_changes_to_discrete_moving_posture, Fixture )
{
    previousPosture = &PHY_Posture::posteReflexe_;
    currentPosture = &PHY_Posture::arret_;
    completionPercentage = 1.;
    movement = true;
    bDiscreteModeEnabled = true;
    MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 0 );
    Tick( &PHY_Posture::mouvementDiscret_, 1. );
    Tick( &PHY_Posture::mouvementDiscret_, 1. ); // Keep moving, don't go into mouvement_
}

BOOST_FIXTURE_TEST_CASE( cancel_move_order_increases_posture, Fixture )
{
    previousPosture = &PHY_Posture::poste_;
    currentPosture = &PHY_Posture::posteReflexe_;
    completionPercentage = 0.4;
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 5 );
    Tick( &PHY_Posture::poste_, 0.8 );
    Tick( &PHY_Posture::poste_, 1. );
    Tick( &PHY_Posture::posteAmenage_, 0.2 );
}

BOOST_FIXTURE_TEST_CASE( move_order_decreases_posture, Fixture )
{
    previousPosture = &PHY_Posture::posteReflexe_;
    currentPosture = &PHY_Posture::poste_;
    completionPercentage = 0.4;
    movement = true;
    MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 5 );
    Tick( &PHY_Posture::posteReflexe_, 0.8 );
    Tick( &PHY_Posture::posteReflexe_, 1. );
    Tick( &PHY_Posture::arret_, 0.2 );
}

BOOST_FIXTURE_TEST_CASE( parked_on_engineer_area_changes_to_its_special_posture, Fixture )
{
    parkedOnEngineerArea = true;
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 2 );
    Tick( &PHY_Posture::postePrepareGenie_, 0.5 );
    Tick( &PHY_Posture::postePrepareGenie_, 1. );
    movement = true;
    MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 2 );
    Tick( &PHY_Posture::mouvement_, 0.5 ); // FIXME ? arret_ intermediaire?
}

BOOST_FIXTURE_TEST_CASE( modifier_changes_completion_percent_increase_rate, Fixture )
{
    const double timeForNextPosture = 5;
    const double modifier = 0.5;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, PHY_Posture::arret_, false, false, 0, 1, 1, false );
    computer.AddCoefficientModifier( modifier );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 1 / ( timeForNextPosture * modifier ), 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( timing_factor_changes_completion_percent_increase_rate, Fixture )
{
    const double timeForNextPosture = 5;
    const double timingFactor = 2;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, PHY_Posture::arret_, false, false, 0, 1, timingFactor, false );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 1 / ( timeForNextPosture / timingFactor ), 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( stealth_is_enabled_if_roll_is_greater_than_stealth_factor, Fixture )
{
    mock::reset( random );
    const double stealthFactor = 0.75;
    const double greatRoll = 0.8;
    const double lowRoll = 0.7;
    {
        posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, PHY_Posture::arret_, false, false, 0, stealthFactor, 1, false );
        MOCK_EXPECT( random.rand_oi ).once().returns( greatRoll );
        MOCK_EXPECT( time.GetPostureSetupTime ).returns( 1 );
        const posture::PostureComputer_ABC::Results& result = computer.Result();
        BOOST_CHECK( result.bIsStealth_ );
        mock::verify();
    }
    {
        posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, PHY_Posture::arret_, false, false, 0, stealthFactor, 1, false );
        MOCK_EXPECT( random.rand_oi ).once().returns( lowRoll );
        MOCK_EXPECT( time.GetPostureSetupTime ).returns( 1 );
        const posture::PostureComputer_ABC::Results& result = computer.Result();
        BOOST_CHECK( !result.bIsStealth_ );
    }
}

BOOST_FIXTURE_TEST_CASE( moving_posture_but_dead_is_stopped_and_completed, Fixture )
{
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, PHY_Posture::mouvement_, true, false, 1, 1, 1, false );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    CHECK_POSTURE( &PHY_Posture::arret_, result.newPosture_ );
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 1, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( reinforcing_agent_posture_time_modifies_completion, Fixture )
{
    MockMIL_Time_ABC t;
    MOCK_EXPECT( t.GetTickDuration ).returns( 10u );
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, PHY_Posture::arret_, false, false, 0, 1, 1, false );
    MockAgent agent;
    MockAgentType type;
    MOCK_EXPECT( agent.GetType ).returns( boost::cref( type ) );
    xml::xistringstream xis( "<root>"
                             "  <equipments/>"
                             "  <crew-ranks/>"
                             "  <postures>"
                             "    <posture name='PosteReflexe' setup-time='100s' tear-down-time='100s'/>"
                             "  </postures>"
                             "  <nbc decontamination-delay='0' suit=''/>"
                             "</root>");
    xis >> xml::start( "root" );
    PHY_UnitType utype( xis );
    MOCK_EXPECT( type.GetUnitType ).returns( boost::cref( utype ) );
    computer.ApplyOnReinforcement( agent );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( 5 );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 0.1, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( empty_posture_time_increases_posture_instantly, Fixture )
{
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 0 );
    Tick( &PHY_Posture::posteAmenage_, 1. );
}

BOOST_FIXTURE_TEST_CASE( empty_posture_time_increases_posture_from_move_posture_instantly, Fixture )
{
    previousPosture = &PHY_Posture::poste_;
    currentPosture = &PHY_Posture::posteReflexe_;
    completionPercentage = 0.4;
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 0 );
    Tick( &PHY_Posture::posteAmenage_, 1. );
}

BOOST_FIXTURE_TEST_CASE( empty_posture_time_decreases_posture_instantly, Fixture )
{
    movement = true;
    previousPosture = &PHY_Posture::poste_;
    currentPosture = &PHY_Posture::posteAmenage_;
    MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 0 );
    Tick( &PHY_Posture::mouvement_, 1. );
}

BOOST_FIXTURE_TEST_CASE( short_posture_time_increases_two_posture_per_tick, Fixture )
{
    MOCK_EXPECT( time.GetPostureSetupTime ).returns( 0.5 );
    Tick( &PHY_Posture::poste_, 1. ); // move to psteReflexe_ and poste_ in 2*0.5 = 1 tick.
}

BOOST_AUTO_TEST_SUITE_END()
