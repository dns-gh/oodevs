// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_test_pch.h"
#include "Helpers.h"
#include "MockClientPublisher.h"
#include "MockAutomat.h"
#include "MockModel.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/MissionFactory.h"
#include "clients_kernel/MissionType.h"
#include "dispatcher/Agent.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Agent_IsCreatedUnderAnAutomat
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Agent_IsCreatedUnderAnAutomat )
{
    dispatcher_test::StaticModel staticModel;
    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat.GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // agent types
    tools::Resolver< kernel::AgentType > types;
    boost::shared_ptr< kernel::AgentType > type( staticModel.MakeAgentType() );
    types.Register( type->GetId(), *type );

    // models
    MockModel model;
    MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::UnitCreation& message = *expected.mutable_message()->mutable_unit_creation();
        message.mutable_unit()->set_id( 1 );
        message.mutable_type()->set_id( 42 );
        message.set_name( "test" );
        message.mutable_automat()->set_id( automat.GetId() );
        message.set_pc( false );
        BOOST_REQUIRE( message.IsInitialized() );

        // creation
        MOCK_EXPECT( automat.RegisterAgent ).once();
        std::unique_ptr< dispatcher::Agent_ABC > result( new dispatcher::Agent( model, message, types ) );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        mock::verify( publisher );

        // cleaning
        MOCK_EXPECT( automat.RemoveAgent ).once().with( mock::same( *result ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_AttributesCanBeUpdated
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Agent_AttributesCanBeUpdated )
{
    dispatcher_test::StaticModel staticModel;
    // agents
    tools::Resolver< dispatcher::Agent_ABC > agents;

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat.GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // agent types
    tools::Resolver< kernel::AgentType > types;
    boost::shared_ptr< kernel::AgentType > type( staticModel.MakeAgentType() );
    types.Register( type->GetId(), *type );

    // models
    MockModel model;
    MOCK_EXPECT( model.Agents ).returns( boost::ref( agents ) );
    MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
    {
        std::unique_ptr< dispatcher::Agent_ABC > result;
        {
            sword::SimToClient expected;
            expected.set_context( 0 );
            sword::UnitCreation& message = *expected.mutable_message()->mutable_unit_creation();
            message.mutable_unit()->set_id( 1 );
            message.mutable_type()->set_id( 42 );
            message.set_name( "test" );
            message.mutable_automat()->set_id( automat.GetId() );
            message.set_pc( false );
            BOOST_REQUIRE( message.IsInitialized() );

            // creation
            MOCK_EXPECT( automat.RegisterAgent ).once();
            result.reset( new dispatcher::Agent( model, message, types ) );
            agents.Register( result->GetId(), *result );
        }
        {
            // attribute update
            sword::SimToClient expectedAttributes;
            expectedAttributes.set_context( 0 );
            {
                sword::UnitAttributes& message = *expectedAttributes.mutable_message()->mutable_unit_attributes();
                message.mutable_unit()->set_id( 1 );
//            message.mutable_human_dotations();
//            message.mutable_equipment_dotations();
//            message.mutable_resource_dotations();
//            message.mutable_lent_equipments();
//            message.mutable_borrowed_equipments();
                message.mutable_position()->set_latitude( 42. );
                message.mutable_position()->set_longitude( 1. );
                message.mutable_direction()->set_heading( 43 );
                message.mutable_sensors_direction()->set_heading( 181 );
                message.set_height( 23 );
                message.set_altitude( 12 );
                message.set_speed( 69 );
                message.set_raw_operational_state( 99 );
//            message.mutable_pions_renforcant();
                message.mutable_reinforced_unit()->set_id( 0 );
                message.set_dead( true );
                message.set_neutralized( true );
                message.set_stealth( true );
                message.set_underground( false );
                message.set_embarked( true );
                message.set_transporters_available( true );
                message.set_old_posture( sword::UnitAttributes::moving );
                message.set_new_posture( sword::UnitAttributes::parked_on_engineer_prepared_area );
                message.set_posture_transition( 51 );
                message.set_installation( 60 );
                message.set_protective_suits( true );
                message.mutable_contamination_agents();
                message.mutable_contamination_state()->set_decontamination_process( 0 );
                message.mutable_contamination_state()->set_percentage( 100 );
                message.mutable_contamination_state()->set_quantity( 2.f );
                message.mutable_contamination_state()->set_dose( 0.f );
                message.mutable_contamination_state()->set_contaminated( false );
                message.mutable_communications()->set_jammed( true );
                message.mutable_communications()->mutable_knowledge_group()->set_id( 0 );
                message.set_radio_emitter_disabled( true );
                message.set_radio_receiver_disabled( true );
                message.set_radar_active( true );
                message.mutable_transported_units();
                message.mutable_transporting_unit()->set_id( 0 );
                message.set_force_ratio( sword::ForceRatio::favorable );
                message.set_meeting_engagement( sword::pinned_down );
                message.set_operational_state( sword::totally_destroyed );
                message.set_indirect_fire_availability( sword::UnitAttributes::fire_ready );
                message.set_roe( sword::RulesOfEngagement::retaliation_only );
                message.set_roe_crowd( sword::UnitAttributes::non_lethal_force );
                message.set_tiredness( sword::UnitAttributes::tired );
                message.set_morale( sword::UnitAttributes::fanatical );
                message.set_experience( sword::UnitAttributes::veteran );
                message.set_stress( sword::UnitAttributes::calm );
                message.mutable_surrendered_unit()->set_id( 0 );
                message.set_prisoner( true );
                message.set_refugees_managed( true );
                message.set_critical_intelligence( "critical intelligence" );
                message.set_decisional_model( "" );
                message.set_brain_debug( false );
                message.set_headquarters( false );
                BOOST_REQUIRE( message.IsInitialized() );
                agents.Get( 1 ).Update( message );
            }

            // automat change superior
            sword::SimToClient expectedChangeSuperior;
            expectedChangeSuperior.set_context( 0 );
            {
                sword::UnitChangeSuperior& message = *expectedChangeSuperior.mutable_message()->mutable_unit_change_superior();
                message.mutable_unit()->set_id( 1 );
                message.mutable_parent()->set_id( automat.GetId() );
                BOOST_REQUIRE( message.IsInitialized() );
            }

            // order no mission
            sword::SimToClient expectedNoMission;
            expectedNoMission.set_context( 0 );
            {
                sword::UnitOrder& message = *expectedNoMission.mutable_message()->mutable_unit_order();
                message.mutable_tasker()->set_id( result->GetId() );
                message.mutable_type()->set_id( 0 );
                BOOST_REQUIRE( message.IsInitialized() );
            }

            // network serialization
            MockClientPublisher publisher;
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedAttributes );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedChangeSuperior );
            MOCK_EXPECT( publisher.SendSimToClient ).once().with( expectedNoMission );
            result->SendFullUpdate( publisher );
        }
        // cleaning
        MOCK_EXPECT( automat.RemoveAgent ).once().with( mock::same( *result ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_IsCreatedWithExtensions
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Agent_IsCreatedWithExtensions )
{
    dispatcher_test::StaticModel staticModel;
    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat.GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // agent types
    tools::Resolver< kernel::AgentType > types;
    boost::shared_ptr< kernel::AgentType > type( staticModel.MakeAgentType() );
    types.Register( type->GetId(), *type );

    // models
    MockModel model;
    MOCK_EXPECT( model.Automats ).returns( boost::ref( automats ) );
    {
        sword::SimToClient expected;
        expected.set_context( 0 );
        sword::UnitCreation& message = *expected.mutable_message()->mutable_unit_creation();
        message.mutable_unit()->set_id( 1 );
        message.mutable_type()->set_id( 42 );
        message.set_name( "test" );
        message.mutable_automat()->set_id( automat.GetId() );
        message.set_pc( false );
        sword::Extension_Entry* entry = message.mutable_extension()->add_entries();
        entry->set_name( "extension" );
        entry->set_value( "value" );
        BOOST_REQUIRE( message.IsInitialized() );

        // creation
        MOCK_EXPECT( automat.RegisterAgent ).once();
        dispatcher::Agent result( model, message, types );

        std::string extVal;
        BOOST_CHECK( result.GetExtension( "extension", extVal ) );
        BOOST_CHECK_EQUAL( extVal, "value" );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher.SendSimToClient ).once().with( expected );
        result.SendCreation( publisher );
        mock::verify( publisher );

        // cleaning
        MOCK_EXPECT( automat.RemoveAgent ).once().with( mock::same( result ) );
    }
}