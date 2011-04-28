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
#include "clients_kernel/SymbolFactory.h"
#include "dispatcher/Agent.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    std::auto_ptr< kernel::DecisionalModel > MakeDecisionalModel()
    {
        const tools::Resolver< kernel::FragOrderType, std::string > fragOrdersResolver;
        const tools::Resolver< kernel::MissionType, std::string > missionResolver;
        kernel::MissionFactory factory( missionResolver, missionResolver, missionResolver, fragOrdersResolver );
        const tools::Resolver< kernel::FragOrderType > fragOrders;
        const std::string xml( "<model name='my_model'/>" );
        xml::xistringstream xis( xml );
        xis >> xml::start( "model" );
        return std::auto_ptr< kernel::DecisionalModel >( new kernel::DecisionalModel( xis, factory, &kernel::MissionFactory::CreateAgentMission, fragOrders ) );
    }

    std::auto_ptr< kernel::AgentType > MakeAgentType()
    {
        const tools::Resolver< kernel::ComponentType, std::string > componentResolver;
        tools::Resolver< kernel::DecisionalModel, std::string > modelResolver;
        std::auto_ptr< kernel::DecisionalModel > model( MakeDecisionalModel() );
        modelResolver.Register( model->GetName(), *model );
        const kernel::SymbolFactory symbolFactory;
        const std::string xml(
            "<type name='my_name' type='my_type' id='42' decisional-model='my_model'>"
                "<nature level='iii' nature-app6='undefined/undefined' atlas-nature='none'/>"
                "<equipments/>"
                "<crew-ranks/>"
            "</type>"
        );
        xml::xistringstream xis( xml );
        xis >> xml::start( "type" );
        return std::auto_ptr< kernel::AgentType >( new kernel::AgentType( xis, componentResolver, modelResolver, symbolFactory ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_IsCreatedUnderAnAutomat
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Agent_IsCreatedUnderAnAutomat )
{
    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // agent types
    tools::Resolver< kernel::AgentType > types;
    std::auto_ptr< kernel::AgentType > type( MakeAgentType() );
    types.Register( type->GetId(), *type );

    // models
    MockModel model;
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
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
        MOCK_EXPECT( automat, RegisterAgent ).once();
        std::auto_ptr< dispatcher::Agent_ABC > result( new dispatcher::Agent( model, message, types ) );

        // network serialization
        MockClientPublisher publisher;
        MOCK_EXPECT( publisher, SendSimToClient ).once().with( expected );
        result->SendCreation( publisher );
        publisher.verify();

        // cleaning
        MOCK_EXPECT( automat, RemoveAgent ).once().with( mock::same( *result ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_AttributesCanBeUpdated
// Created: SBO 2010-02-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Agent_AttributesCanBeUpdated )
{
    // agents
    tools::Resolver< dispatcher::Agent_ABC > agents;

    // automats
    tools::Resolver< dispatcher::Automat_ABC > automats;
    MockAutomat automat( 51 );
    MOCK_EXPECT( automat, GetId ).returns( 51 );
    automats.Register( automat.GetId(), automat );

    // agent types
    tools::Resolver< kernel::AgentType > types;
    std::auto_ptr< kernel::AgentType > type( MakeAgentType() );
    types.Register( type->GetId(), *type );

    // models
    MockModel model;
    MOCK_EXPECT( model, Agents ).returns( boost::ref( agents ) );
    MOCK_EXPECT( model, Automats ).returns( boost::ref( automats ) );
    {
        std::auto_ptr< dispatcher::Agent_ABC > result;
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
            MOCK_EXPECT( automat, RegisterAgent ).once();
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
                message.set_height( 23 );
                message.set_altitude( 12 );
                message.set_speed( 69 );
                message.set_raw_operational_state( 99 );
//            message.mutable_pions_renforcant();
                message.mutable_reinforced_unit()->set_id( 0 );
                message.set_dead( true );
                message.set_neutralized( true );
                message.set_stealth( true );
                message.set_embarked( true );
                message.set_transporters_available( true );
                message.set_old_posture( sword::UnitAttributes::moving );
                message.set_new_posture( sword::UnitAttributes::parked_on_engineer_prepared_area );
                message.set_posture_transition( 51 );
                message.set_installation( 60 );
                message.set_protective_suits( true );
//            message.mutable_contamination_agents();
                message.mutable_contamination_state()->set_percentage( 0 );
                message.mutable_contamination_state()->set_quantity( 2.f );
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
                message.mutable_surrendered_unit()->set_id( 0 );
                message.set_prisoner( true );
                message.set_refugees_managed( true );
                message.set_critical_intelligence( "critical intelligence" );
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
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expectedAttributes );
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expectedChangeSuperior );
            MOCK_EXPECT( publisher, SendSimToClient ).once().with( expectedNoMission );
            result->SendFullUpdate( publisher );
        }
        // cleaning
        MOCK_EXPECT( automat, RemoveAgent ).once().with( mock::same( *result ) );
    }
}
