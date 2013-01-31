// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "simulation_kernel/DefaultPostureComputer.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PHY_Posture.h"
#include "simulation_kernel/Entities/Agents/Units/Postures/PostureTime_ABC.h"
#include "simulation_kernel/MIL_Random_ABC.h"
#include <xeumeuleu/xml.hpp>

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
    struct Fixture
    {
        Fixture()
        {
            MOCK_EXPECT( random.rand_oi ).returns( 0 );
            PHY_Posture::Initialize();
        }
        ~Fixture()
        {
            PHY_Posture::Terminate();
        }
        MockPostureTime time;
        MockMIL_Random random;
    };
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_with_unfinished_completion_does_not_change_posture, Fixture )
{
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, 0, 1, 1, false );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( 10 );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( !result.newPosture_ );
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_but_now_moving_and_loaded_changes_to_moving_posture_and_new_completion_is_empty, Fixture )
{
    const double completion = 0.;
    const double tearDownTime = 0;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completion, 1, 1, false );
    computer.SetPostureMovement();
    computer.NotifyLoaded();
    MOCK_EXPECT( time.GetPostureTearDownTime ).once().returns( tearDownTime );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == &PHY_Posture::mouvement_ );
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, completion, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_but_now_moving_must_wait_completion_decrease_before_changing_to_moving_posture, Fixture )
{
    const double completion = 0.5;
    const double tearDownTime = 10;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completion, 1, 1, false );
    computer.SetPostureMovement();
    computer.NotifyLoaded();
    MOCK_EXPECT( time.GetPostureTearDownTime ).once().returns( tearDownTime );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( !result.newPosture_ );
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, completion - 1 / tearDownTime, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_but_now_moving_and_loaded_in_discrete_mode_changes_to_discrete_moving_posture, Fixture )
{
    const bool isInDiscreteMode = true;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, isInDiscreteMode, 0, 1, 1, false );
    computer.SetPostureMovement();
    computer.NotifyLoaded();
    MOCK_EXPECT( time.GetPostureTearDownTime ).once().returns( 10 );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == &PHY_Posture::mouvementDiscret_ );
}

BOOST_FIXTURE_TEST_CASE( stopped_posture_but_now_moving_and_unloaded_changes_to_short_stopping_posture, Fixture )
{
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, 0, 1, 1, false );
    computer.SetPostureMovement();
    MOCK_EXPECT( time.GetPostureTearDownTime ).once().returns( 10 );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == &PHY_Posture::posteReflexe_ );
}

BOOST_FIXTURE_TEST_CASE( moving_posture_but_stopped_changes_to_stopped_posture, Fixture )
{
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::mouvement_, false, false, 0, 1, 1, false );
    computer.UnsetPostureMovement();
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == &PHY_Posture::arret_ );
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 0, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( completion_percent_increase_with_posture_time, Fixture )
{
    const double completionPercent = 0;
    const double timeForNextPosture = 5;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completionPercent, 1, 1, false );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, completionPercent + 1 / timeForNextPosture, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( empty_posture_time_changes_posture_instantly, Fixture )
{
    const double completionPercent = 0;
    const double timeForNextPosture = 0;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completionPercent, 1, 1, false );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == PHY_Posture::arret_.GetNextAutoPosture() );
}

BOOST_FIXTURE_TEST_CASE( modifier_changes_completion_percent_increase_rate, Fixture )
{
    const double completionPercent = 0;
    const double timeForNextPosture = 5;
    const double modifier = 0.5;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completionPercent, 1, 1, false );
    computer.AddCoefficientModifier( modifier );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, completionPercent + 1 / ( timeForNextPosture * modifier ), 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( timing_factor_changes_completion_percent_increase_rate, Fixture )
{
    const double completionPercent = 0;
    const double timeForNextPosture = 5;
    const double timingFactor = 2;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completionPercent, 1, timingFactor, false );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, completionPercent + 1 / ( timeForNextPosture / timingFactor ), 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( urban_modifier_is_applied_only_if_posture_is_parked, Fixture )
{
    const double completionPercent = 0;
    const double timeForNextPosture = 5;
    const double urbanModifier = 0.5;
    const double unmodified = completionPercent + 1 / timeForNextPosture;
    const double modified = completionPercent + 1 / ( timeForNextPosture * urbanModifier );
    {
        posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completionPercent, 1, 1, false );
        computer.AddUrbanCoefficientModifier( urbanModifier );
        MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
        const posture::PostureComputer_ABC::Results& result = computer.Result();
        BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, unmodified, 0.0001 );
    }
    {
        posture::DefaultPostureComputer computer( random, time, PHY_Posture::poste_, false, false, completionPercent, 1, 1, false );
        computer.AddUrbanCoefficientModifier( urbanModifier );
        MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( timeForNextPosture );
        const posture::PostureComputer_ABC::Results& result = computer.Result();
        BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, modified, 0.0001 );
    }
}

BOOST_FIXTURE_TEST_CASE( stealth_is_enabled_if_roll_is_greater_than_stealth_factor, Fixture )
{
    mock::reset( random );
    const double stealthFactor = 0.75;
    const double greatRoll = 0.8;
    const double lowRoll = 0.7;
    {
        posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, 0, stealthFactor, 1, false );
        MOCK_EXPECT( random.rand_oi ).once().returns( greatRoll );
        MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( 1 );
        const posture::PostureComputer_ABC::Results& result = computer.Result();
        BOOST_CHECK( result.bIsStealth_ );
        mock::verify();
    }
    {
        posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, 0, stealthFactor, 1, false );
        MOCK_EXPECT( random.rand_oi ).once().returns( lowRoll );
        MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( 1 );
        const posture::PostureComputer_ABC::Results& result = computer.Result();
        BOOST_CHECK( !result.bIsStealth_ );
    }
}

BOOST_FIXTURE_TEST_CASE( moving_posture_but_dead_is_stopped_and_completed, Fixture )
{
    const bool isDead = true;
    const double completionPercent = 0;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::mouvement_, isDead, false, completionPercent, 1, 1, false );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == &PHY_Posture::arret_ );
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 1, 0.0001 );
}

BOOST_FIXTURE_TEST_CASE( parked_on_engineer_area_changes_to_its_special_posture, Fixture )
{
    const bool parkedOnEngineerArea = true;
    const double completionPercent = 0;
    posture::DefaultPostureComputer computer( random, time, PHY_Posture::arret_, false, false, completionPercent, 1, 1, parkedOnEngineerArea );
    MOCK_EXPECT( time.GetPostureSetupTime ).once().returns( 0 );
    const posture::PostureComputer_ABC::Results& result = computer.Result();
    BOOST_CHECK( result.newPosture_ == &PHY_Posture::postePrepareGenie_ );
    BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 1, 0.0001 );
}

namespace
{
    struct CheckFixture : public Fixture
    {
        void CheckNextPosture( const PHY_Posture& current, const PHY_Posture* next )
        {
            const double completionPercent = 1;
            posture::DefaultPostureComputer computer( random, time, current, false, false, completionPercent, 1, 1, false );
            MOCK_EXPECT( time.GetPostureSetupTime ).returns( 0 );
            const posture::PostureComputer_ABC::Results& result = computer.Result();
            BOOST_CHECK( result.newPosture_ == next );
            BOOST_CHECK_CLOSE( result.postureCompletionPercentage_, 0., 0.0001 );
        }
        void CheckPreviousPosture( const PHY_Posture& current, const PHY_Posture* previous )
        {
            const double completionPercent = 0;
            posture::DefaultPostureComputer computer( random, time, current, false, false, completionPercent, 1, 1, false );
            computer.SetPostureMovement();
            computer.NotifyLoaded();
            MOCK_EXPECT( time.GetPostureTearDownTime ).returns( 0 );
            const posture::PostureComputer_ABC::Results& result = computer.Result();
            BOOST_CHECK( result.newPosture_ == previous );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( completed_posture_changes_to_next_posture, CheckFixture )
{
    CheckNextPosture( PHY_Posture::arret_            , &PHY_Posture::posteReflexe_ );
    CheckNextPosture( PHY_Posture::posteReflexe_     , &PHY_Posture::poste_ );
    CheckNextPosture( PHY_Posture::poste_            , &PHY_Posture::posteAmenage_ );
    CheckNextPosture( PHY_Posture::posteAmenage_     , 0 );
    CheckNextPosture( PHY_Posture::postePrepareGenie_, 0 );
    CheckNextPosture( PHY_Posture::mouvement_        , 0 );
    CheckNextPosture( PHY_Posture::mouvementDiscret_ , 0 );
}

BOOST_FIXTURE_TEST_CASE( moving_entity_changes_to_previous_posture, CheckFixture )
{
    CheckPreviousPosture( PHY_Posture::arret_            , &PHY_Posture::mouvement_ );
    CheckPreviousPosture( PHY_Posture::posteReflexe_     , &PHY_Posture::arret_ );
    CheckPreviousPosture( PHY_Posture::poste_            , &PHY_Posture::posteReflexe_ );
    CheckPreviousPosture( PHY_Posture::posteAmenage_     , &PHY_Posture::poste_ );
    CheckPreviousPosture( PHY_Posture::postePrepareGenie_, &PHY_Posture::mouvement_ );
    CheckPreviousPosture( PHY_Posture::mouvement_        , &PHY_Posture::mouvement_ );
    CheckPreviousPosture( PHY_Posture::mouvementDiscret_ , &PHY_Posture::mouvement_ );
}
