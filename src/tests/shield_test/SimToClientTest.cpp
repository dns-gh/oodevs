// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "shield_test_pch.h"
#include "SimTools.h"

using namespace shield;

BOOST_FIXTURE_TEST_CASE( unit_order_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_order_ack()->mutable_tasker()->mutable_unit()->set_id( 7 );
    content.mutable_order_ack()->set_error_code( sword::OrderAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_order_ack { tasker { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_order_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_order_ack()->mutable_tasker()->mutable_automat()->set_id( 7 );
    content.mutable_order_ack()->set_error_code( sword::OrderAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_order_ack { tasker { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_order_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_order_ack()->mutable_tasker()->mutable_crowd()->set_id( 7 );
    content.mutable_order_ack()->set_error_code( sword::OrderAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_order_ack { tasker { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( frag_order_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillTasker( content.mutable_frag_order_ack()->mutable_tasker() );
    content.mutable_frag_order_ack()->set_error_code( sword::OrderAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { frag_order_ack { tasker { automat { id: 7 } formation { id: 8 } crowd { id: 9 } unit { id: 10 } party { id: 11 } } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( set_automat_mode_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_set_automat_mode_ack()->mutable_automate()->set_id( 7 );
    content.mutable_set_automat_mode_ack()->set_error_code( sword::SetAutomatModeAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { set_automat_mode_ack { automate { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_creation_request_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_creation_request_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_creation_request_ack { error: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( magic_action_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_magic_action_ack()->set_error_code( sword::MagicActionAck::error_invalid_parameter );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { magic_action_ack { error_code: error_invalid_attribute } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_magic_action_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_magic_action_ack()->mutable_unit()->set_id( 7 );
    content.mutable_unit_magic_action_ack()->set_error_code( sword::UnitActionAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_magic_action_ack { unit { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( object_magic_action_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_magic_action_ack()->set_error_code( sword::ObjectMagicActionAck::error_invalid_object );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_magic_action_ack { error_code: error_invalid_object } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_magic_action_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_magic_action_ack()->mutable_crowd()->set_id( 7 );
    content.mutable_crowd_magic_action_ack()->set_error_code( sword::CrowdMagicActionAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_magic_action_ack { crowd { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( change_diplomacy_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_change_diplomacy_ack()->mutable_party1()->set_id( 7 );
    content.mutable_change_diplomacy_ack()->mutable_party2()->set_id( 8 );
    content.mutable_change_diplomacy_ack()->set_diplomacy( sword::friendly );
    content.mutable_change_diplomacy_ack()->set_error_code( sword::ChangeDiplomacyAck::error_invalid_party_diplomacy );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { change_diplomacy_ack { party1 { id: 7 } party2 { id: 8 } diplomatie: friend_diplo error_code: error_invalid_camp_diplomacy } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_knowledge_group_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_knowledge_group_ack()->set_error_code( sword::HierarchyModificationAck::error_invalid_agent );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_knowledge_group_ack { error_code: error_invalid_pion } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_logistic_links_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_logistic_links_ack()->set_error_code( sword::HierarchyModificationAck::error_invalid_agent );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_logistic_links_ack { error_code: error_invalid_pion } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_superior_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_superior_ack()->set_error_code( sword::HierarchyModificationAck::error_invalid_agent );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_superior_ack { error_code: error_invalid_pion } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_change_superior_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_change_superior_ack()->set_error_code( sword::HierarchyModificationAck::error_invalid_agent );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_change_superior_ack { error_code: error_invalid_pion } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_supply_push_flow_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_push_flow_ack()->set_error_code( sword::LogSupplyPushFlowAck::error_invalid_supplier );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_push_flow_ack { ack: error_invalid_donneur_pushflow } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_supply_pull_flow_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_pull_flow_ack()->set_error_code( sword::LogSupplyPullFlowAck::error_invalid_supplier );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_pull_flow_ack { ack: error_invalid_provider_pullflow } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_supply_change_quotas_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_change_quotas_ack()->set_error_code( sword::LogSupplyChangeQuotasAck::error_invalid_supplier );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_change_quotas_ack { ack: error_invalid_supplier } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_information_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_information()->set_current_tick( 7 );
    content.mutable_control_information()->mutable_initial_date_time()->set_data( "data" );
    content.mutable_control_information()->mutable_date_time()->set_data( "data2" );
    content.mutable_control_information()->set_tick_duration( 3 );
    content.mutable_control_information()->set_time_factor( 4 );
    content.mutable_control_information()->set_checkpoint_frequency( 12 );
    content.mutable_control_information()->set_status( sword::paused );
    content.mutable_control_information()->set_send_vision_cones( true );
    content.mutable_control_information()->set_profiling_enabled( true );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_information { current_tick: 7 initial_date_time { data: \"data\" } date_time { data: \"data2\" } tick_duration: 3 time_factor: 4 checkpoint_frequency: 12 status: paused send_vision_cones: true profiling_enabled: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_profiling_information_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_profiling_information()->set_perception( 7 );
    content.mutable_control_profiling_information()->set_decision( 8 );
    content.mutable_control_profiling_information()->set_action( 9 );
    content.mutable_control_profiling_information()->set_main_loop( 10 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_profiling_information { perception: 7 decision: 8 action: 9 main_loop: 10 } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_begin_tick_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_begin_tick()->set_current_tick( 7 );
    content.mutable_control_begin_tick()->mutable_date_time()->set_data( "data" );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_begin_tick { current_tick: 7 date_time { data: \"data\" } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_end_tick_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_end_tick()->set_current_tick( 7 );
    content.mutable_control_end_tick()->set_tick_duration( 8 );
    content.mutable_control_end_tick()->set_long_pathfinds( 9 );
    content.mutable_control_end_tick()->set_short_pathfinds( 10 );
    content.mutable_control_end_tick()->set_memory( 11 );
    content.mutable_control_end_tick()->set_virtual_memory( 12 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_end_tick { current_tick: 7 tick_duration: 8 long_pathfinds: 9 short_pathfinds: 10 memory: 11 virtual_memory: 12 } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_date_time_change_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_date_time_change_ack()->set_error_code( sword::ControlAck::error_not_started );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_date_time_change_ack { error_code: error_not_started } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_save_end_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_save_end()->set_name( "name" );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_save_end { name: \"name\" } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename E >
    void FillExtension( E* e )
    {
        e->set_name( "name2" );
        e->set_value( "value" );
    }
    template< typename C >
    void FillRgbColor( C* c )
    {
        c->set_red( 12 );
        c->set_green( 42 );
        c->set_blue( 77 );
    }
    template< typename C >
    void FillRgbaColor( C* c )
    {
        FillRgbColor( c );
        c->set_alpha( 99 );
    }
}

BOOST_FIXTURE_TEST_CASE( formation_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_formation_creation()->mutable_formation()->set_id( 6 );
    content.mutable_formation_creation()->mutable_party()->set_id( 7 );
    content.mutable_formation_creation()->mutable_parent()->set_id( 8 );
    content.mutable_formation_creation()->set_level( sword::ooo );
    content.mutable_formation_creation()->set_name( "name" );
    content.mutable_formation_creation()->set_app6symbol( "app6" );
    content.mutable_formation_creation()->set_logistic_level( sword::logistic_base );
    FillRgbColor( content.mutable_formation_creation()->mutable_color() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { formation_creation { formation { id: 6 } party { id: 7 } parent { id: 8 } level: ooo name: \"name\" app6symbol: \"app6\" logistic_level: logistic_base color { red: 12 green: 42 blue: 77 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( formation_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_formation_update()->mutable_formation()->set_id( 6 );
    FillExtension( content.mutable_formation_update()->mutable_extension()->add_entries() );
    FillExtension( content.mutable_formation_update()->mutable_extension()->add_entries() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { formation_update { formation { id: 6 } extension { entries { name: \"name2\" value: \"value\" } entries { name: \"name2\" value: \"value\" } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( party_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_party_creation()->mutable_party()->set_id( 7 );
    content.mutable_party_creation()->set_name( "name" );
    content.mutable_party_creation()->set_type( sword::friendly );
    FillExtension( content.mutable_party_creation()->mutable_extension()->add_entries() );
    FillExtension( content.mutable_party_creation()->mutable_extension()->add_entries() );
    FillRgbColor( content.mutable_party_creation()->mutable_color() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { party_creation { party { id: 7 } name: \"name\" type: friend_diplo extension { entries { name: \"name2\" value: \"value\" } entries { name: \"name2\" value: \"value\" } } color { red: 12 green: 42 blue: 77 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_creation()->mutable_automat()->set_id( 7 );
    content.mutable_automat_creation()->mutable_type()->set_id( 8 );
    content.mutable_automat_creation()->set_name( "name" );
    content.mutable_automat_creation()->mutable_parent()->mutable_automat()->set_id( 9 );
    content.mutable_automat_creation()->mutable_parent()->mutable_formation()->set_id( 10 );
    content.mutable_automat_creation()->mutable_party()->set_id( 11 );
    content.mutable_automat_creation()->mutable_knowledge_group()->set_id( 12 );
    content.mutable_automat_creation()->set_app6symbol( "app6" );
    content.mutable_automat_creation()->set_logistic_level( sword::logistic_base );
    FillRgbColor( content.mutable_automat_creation()->mutable_color() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_creation { automat { id: 7 } type { id: 8 } nom: \"name\" parent { automat { id: 9 } formation { id: 10 } } party { id: 11 } knowledge_group { id: 12 } app6symbol: \"app6\" logistic_level: logistic_base color { red: 12 green: 42 blue: 77 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_attributes_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_attributes()->mutable_automat()->set_id( 7 );
    content.mutable_automat_attributes()->set_mode( sword::engaged );
    content.mutable_automat_attributes()->set_meeting_engagement( sword::avoiding );
    content.mutable_automat_attributes()->set_operational_state( sword::operational );
    content.mutable_automat_attributes()->set_roe( sword::RulesOfEngagement::free_fire );
    FillExtension( content.mutable_automat_attributes()->mutable_extension()->add_entries() );
    FillExtension( content.mutable_automat_attributes()->mutable_extension()->add_entries() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_attributes { automat { id: 7 } etat_automate: embraye combat_de_rencontre: etat_esquive etat_operationnel: operationnel roe: tir_libre extension { entries { name: \"name2\" value: \"value\" } entries { name: \"name2\" value: \"value\" } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_creation()->mutable_unit()->set_id( 7 );
    content.mutable_unit_creation()->mutable_type()->set_id( 8 );
    content.mutable_unit_creation()->set_name( "name" );
    content.mutable_unit_creation()->mutable_automat()->set_id( 9 );
    content.mutable_unit_creation()->set_pc( true );
    FillRgbColor( content.mutable_unit_creation()->mutable_color() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_creation { unit { id: 7 } type { id: 8 } nom: \"name\" automat { id: 9 } pc: true color { red: 12 green: 42 blue: 77 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename D, typename R >
    void FillHumanDotation( D* d, R r )
    {
        d->set_quantity( 10 );
        d->set_rank( r );
        d->set_state( sword::healthy );
        d->set_location( sword::battlefield );
        sword::Injury* injury = d->mutable_injuries()->Add();
        injury->set_id( 0 );
        injury->set_seriousness( sword::wounded_u1 );
        d->set_mentally_wounded( true );
        d->set_contaminated( true );
    }
    template< typename D >
    void FillEquipmentDotation( D* d )
    {
        d->mutable_type()->set_id( 20 );
        d->set_available( 21 );
        d->set_unavailable( 22 );
        d->set_repairable( 23 );
        d->set_on_site_fixable( 24 );
        d->set_repairing( 25 );
        d->set_captured( 26 );
    }
    template< typename D >
    void FillResourceDotation( D* d )
    {
        d->mutable_type()->set_id( 30 );
        d->set_quantity( 31 );
    }
    template< typename E >
    void FillLentEquipment( E* e )
    {
        e->mutable_borrower()->set_id( 40 );
        e->mutable_type()->set_id( 41 );
        e->set_quantity( 42 );
    }
    template< typename E >
    void FillBorrowedEquipment( E* e )
    {
        e->mutable_owner()->set_id( 50 );
        e->mutable_type()->set_id( 51 );
        e->set_quantity( 52 );
    }
}

BOOST_FIXTURE_TEST_CASE( unit_attributes_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_attributes()->mutable_unit()->set_id( 7 );
    FillHumanDotation( content.mutable_unit_attributes()->mutable_human_dotations()->add_elem(), sword::officer );
    FillHumanDotation( content.mutable_unit_attributes()->mutable_human_dotations()->add_elem(), sword::sub_officer );
    FillHumanDotation( content.mutable_unit_attributes()->mutable_human_dotations()->add_elem(), sword::trooper );
    FillHumanDotation( content.mutable_unit_attributes()->mutable_human_dotations()->add_elem(), sword::trooper );
    FillEquipmentDotation( content.mutable_unit_attributes()->mutable_equipment_dotations()->add_elem() );
    FillEquipmentDotation( content.mutable_unit_attributes()->mutable_equipment_dotations()->add_elem() );
    FillResourceDotation( content.mutable_unit_attributes()->mutable_resource_dotations()->add_elem() );
    FillResourceDotation( content.mutable_unit_attributes()->mutable_resource_dotations()->add_elem() );
    FillLentEquipment( content.mutable_unit_attributes()->mutable_lent_equipments()->add_elem() );
    FillLentEquipment( content.mutable_unit_attributes()->mutable_lent_equipments()->add_elem() );
    FillBorrowedEquipment( content.mutable_unit_attributes()->mutable_borrowed_equipments()->add_elem() );
    FillBorrowedEquipment( content.mutable_unit_attributes()->mutable_borrowed_equipments()->add_elem() );
    FillCoordLatLong( content.mutable_unit_attributes()->mutable_position() );
    content.mutable_unit_attributes()->mutable_direction()->set_heading( 77 );
    content.mutable_unit_attributes()->set_height( 78 );
    content.mutable_unit_attributes()->set_altitude( 79 );
    content.mutable_unit_attributes()->set_speed( 80 );
    content.mutable_unit_attributes()->set_raw_operational_state( 81 );
    content.mutable_unit_attributes()->mutable_reinforcements()->add_elem()->set_id( 82 );
    content.mutable_unit_attributes()->mutable_reinforcements()->add_elem()->set_id( 83 );
    content.mutable_unit_attributes()->mutable_reinforced_unit()->set_id( 84 );
    content.mutable_unit_attributes()->set_dead( true );
    content.mutable_unit_attributes()->set_neutralized( true );
    content.mutable_unit_attributes()->set_stealth( true );
    content.mutable_unit_attributes()->set_embarked( true );
    content.mutable_unit_attributes()->set_transporters_available( true );
    content.mutable_unit_attributes()->set_old_posture( sword::UnitAttributes::stopping );
    content.mutable_unit_attributes()->set_new_posture( sword::UnitAttributes::stopping );
    content.mutable_unit_attributes()->set_posture_transition( 90 );
    content.mutable_unit_attributes()->set_installation( 91 );
    content.mutable_unit_attributes()->set_protective_suits( true );
    content.mutable_unit_attributes()->mutable_contamination_agents()->add_elem()->set_id( 100 );
    content.mutable_unit_attributes()->mutable_contamination_agents()->add_elem()->set_id( 101 );
    content.mutable_unit_attributes()->mutable_contamination_state()->set_percentage( 102 );
    content.mutable_unit_attributes()->mutable_contamination_state()->set_quantity( 103.4f );
    content.mutable_unit_attributes()->mutable_communications()->set_jammed( true );
    content.mutable_unit_attributes()->mutable_communications()->mutable_knowledge_group()->set_id( 104 );
    content.mutable_unit_attributes()->set_radio_emitter_disabled( true );
    content.mutable_unit_attributes()->set_radio_receiver_disabled( true );
    content.mutable_unit_attributes()->set_radar_active( true );
    content.mutable_unit_attributes()->mutable_transported_units()->add_elem()->set_id( 110 );
    content.mutable_unit_attributes()->mutable_transported_units()->add_elem()->set_id( 111 );
    content.mutable_unit_attributes()->mutable_transporting_unit()->set_id( 112 );
    content.mutable_unit_attributes()->set_force_ratio( sword::ForceRatio::neutral );
    content.mutable_unit_attributes()->set_meeting_engagement( sword::avoiding );
    content.mutable_unit_attributes()->set_operational_state( sword::operational );
    content.mutable_unit_attributes()->set_indirect_fire_availability( sword::UnitAttributes::fire_ready );
    content.mutable_unit_attributes()->set_roe( sword::RulesOfEngagement::free_fire );
    content.mutable_unit_attributes()->set_roe_crowd( sword::UnitAttributes::no_force );
    content.mutable_unit_attributes()->set_tiredness( sword::UnitAttributes::exhausted );
    content.mutable_unit_attributes()->set_morale( sword::UnitAttributes::high );
    content.mutable_unit_attributes()->set_experience( sword::UnitAttributes::novice );
    content.mutable_unit_attributes()->mutable_surrendered_unit()->set_id( 120 );
    content.mutable_unit_attributes()->set_prisoner( true );
    content.mutable_unit_attributes()->set_refugees_managed( true );
    FillExtension( content.mutable_unit_attributes()->mutable_extension()->add_entries() );
    FillExtension( content.mutable_unit_attributes()->mutable_extension()->add_entries() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_attributes { unit { id: 7 } dotation_eff_personnel { elem { rang: officier nb_total: 10 nb_operationnels: 0 nb_morts: 0 nb_blesses: 0 nb_blesses_mentaux: 10 nb_contamines_nbc: 10 nb_dans_chaine_sante: 0 nb_utilises_pour_maintenance: 0 nb_blesses_non_evacues: 0 nb_morts_dans_chaine_mortuaire: 0 } elem { rang: sous_officier nb_total: 10 nb_operationnels: 0 nb_morts: 0 nb_blesses: 0 nb_blesses_mentaux: 10 nb_contamines_nbc: 10 nb_dans_chaine_sante: 0 nb_utilises_pour_maintenance: 0 nb_blesses_non_evacues: 0 nb_morts_dans_chaine_mortuaire: 0 } elem { rang: mdr nb_total: 20 nb_operationnels: 0 nb_morts: 0 nb_blesses: 0 nb_blesses_mentaux: 20 nb_contamines_nbc: 20 nb_dans_chaine_sante: 0 nb_utilises_pour_maintenance: 0 nb_blesses_non_evacues: 0 nb_morts_dans_chaine_mortuaire: 0 } } dotation_eff_materiel { elem { type { id: 20 } nb_disponibles: 21 nb_indisponibles: 22 nb_reparables: 47 nb_dans_chaine_maintenance: 25 nb_prisonniers: 26 } elem { type { id: 20 } nb_disponibles: 21 nb_indisponibles: 22 nb_reparables: 47 nb_dans_chaine_maintenance: 25 nb_prisonniers: 26 } } dotation_eff_ressource { elem { type { id: 30 } quantite_disponible: 31 } elem { type { id: 30 } quantite_disponible: 31 } } equipements_pretes { elem { borrower { id: 40 } type { id: 41 } nombre: 42 } elem { borrower { id: 40 } type { id: 41 } nombre: 42 } } equipements_empruntes { elem { owner { id: 50 } type { id: 51 } nombre: 52 } elem { owner { id: 50 } type { id: 51 } nombre: 52 } } position { latitude: 17.23 longitude: 23.17 } direction { heading: 77 } hauteur: 78 altitude: 79 vitesse: 80 etat_operationnel_brut: 81 reinforcements { elem { id: 82 } elem { id: 83 } } reinforced_unit { id: 84 } mort: true neutralise: true mode_furtif_actif: true embarque: true transporteurs_disponibles: true posture_old: arret posture_new: arret posture_pourcentage: 90 etat_installation: 91 en_tenue_de_protection_nbc: true contamine_par_agents_nbc { elem { id: 100 } elem { id: 101 } } etat_contamination { percentage: 102 quantity: 103.4 } communications { jammed: true knowledge_group { id: 104 } } radio_emitter_disabled: true radio_receiver_disabled: true radar_actif: true transported_units { elem { id: 110 } elem { id: 111 } } transporting_unit { id: 112 } rapport_de_force: neutre combat_de_rencontre: etat_esquive etat_operationnel: operationnel disponibilite_au_tir_indirect: pret_au_tir roe: tir_libre roe_crowd: emploi_force_interdit fatigue: epuise moral: bon experience: conscrit surrendered_to_party { id: 120 } prisonnier: true refugie_pris_en_compte: true extension { entries { name: \"name2\" value: \"value\" } entries { name: \"name2\" value: \"value\" } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_pathfind_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_pathfind()->mutable_unit()->set_id( 7 );
    FillLocation( content.mutable_unit_pathfind()->mutable_path()->mutable_location() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_pathfind { unit { id: 7 } itineraire { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_destruction()->mutable_unit()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_destruction { unit { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_environment_type_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_environment_type()->mutable_unit()->set_id( 7 );
    content.mutable_unit_environment_type()->set_area( 8 );
    content.mutable_unit_environment_type()->set_left( 9 );
    content.mutable_unit_environment_type()->set_right( 10 );
    content.mutable_unit_environment_type()->set_linear( 11 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_environment_type { unit { id: 7 } area: 8 left: 9 right: 10 linear: 11 } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( change_diplomacy_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_change_diplomacy()->mutable_party1()->set_id( 7 );
    content.mutable_change_diplomacy()->mutable_party2()->set_id( 8 );
    content.mutable_change_diplomacy()->set_diplomacy( sword::friendly );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { change_diplomacy { party1 { id: 7 } party2 { id: 8 } diplomatie: friend_diplo } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_change_superior_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_change_superior()->mutable_unit()->set_id( 7 );
    content.mutable_unit_change_superior()->mutable_parent()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_change_superior { unit { id: 7 } parent { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_logistic_links_to_client_is_converted_1, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_logistic_links()->mutable_requester()->mutable_automat()->set_id( 7 );
    content.mutable_automat_change_logistic_links()->add_superior()->mutable_formation()->set_id( 13 );
    content.mutable_automat_change_logistic_links()->add_superior()->mutable_automat()->set_id( 12 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_logistic_links { requester { automat { id: 7 } } superior { formation { id: 13 } } superior { automat { id: 12 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_logistic_links_to_client_is_converted_2, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_logistic_links()->mutable_requester()->mutable_formation()->set_id( 8 );
    content.mutable_automat_change_logistic_links()->clear_superior();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_logistic_links { requester { formation { id: 8 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_logistic_links_to_client_is_converted_3, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_logistic_links()->mutable_requester()->mutable_formation()->set_id( 8 );
    content.mutable_automat_change_logistic_links()->add_superior()->mutable_formation()->set_id( 13 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_logistic_links { requester { formation { id: 8 } } superior { formation { id: 13 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_knowledge_group_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_knowledge_group()->mutable_automat()->set_id( 7 );
    content.mutable_automat_change_knowledge_group()->mutable_party()->set_id( 8 );
    content.mutable_automat_change_knowledge_group()->mutable_knowledge_group()->set_id( 9 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_knowledge_group { automat { id: 7 } party { id: 8 } knowledge_group { id: 9 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_change_superior_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_change_superior()->mutable_automat()->set_id( 7 );
    content.mutable_automat_change_superior()->mutable_superior()->mutable_formation()->set_id( 8 );
    content.mutable_automat_change_superior()->mutable_superior()->mutable_automat()->set_id( 9 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_change_superior { automat { id: 7 } superior { formation { id: 8 } automat { id: 9 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_knowledge_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_knowledge_creation()->mutable_knowledge()->set_id( 7 );
    content.mutable_unit_knowledge_creation()->mutable_knowledge_group()->set_id( 8 );
    content.mutable_unit_knowledge_creation()->mutable_unit()->set_id( 9 );
    content.mutable_unit_knowledge_creation()->mutable_type()->set_id( 10 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_knowledge_creation { knowledge { id: 7 } knowledge_group { id: 8 } unit { id: 9 } type { id: 10 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename P >
    void FillAutomatPerception( P* p )
    {
        p->mutable_automat()->set_id( 100 );
        p->set_identification_level( sword::UnitIdentification::unseen );
    }
}

BOOST_FIXTURE_TEST_CASE( unit_knowledge_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_knowledge_update()->mutable_knowledge()->set_id( 7 );
    content.mutable_unit_knowledge_update()->mutable_knowledge_group()->set_id( 8 );
    content.mutable_unit_knowledge_update()->set_pertinence( 9 );
    content.mutable_unit_knowledge_update()->set_identification_level( sword::UnitIdentification::recognized );
    content.mutable_unit_knowledge_update()->set_max_identification_level( sword::UnitIdentification::detected );
    content.mutable_unit_knowledge_update()->set_operational_state( 10 );
    content.mutable_unit_knowledge_update()->set_dead( true );
    FillCoordLatLong( content.mutable_unit_knowledge_update()->mutable_position() );
    content.mutable_unit_knowledge_update()->mutable_direction()->set_heading( 77 );
    content.mutable_unit_knowledge_update()->set_speed( 78 );
    content.mutable_unit_knowledge_update()->mutable_party()->set_id( 79 );
    content.mutable_unit_knowledge_update()->set_command_post( true );
    FillAutomatPerception( content.mutable_unit_knowledge_update()->mutable_perceptions()->add_elem() );
    FillAutomatPerception( content.mutable_unit_knowledge_update()->mutable_perceptions()->add_elem() );
    content.mutable_unit_knowledge_update()->mutable_surrendered_unit()->set_id( 80 );
    content.mutable_unit_knowledge_update()->set_prisoner( true );
    content.mutable_unit_knowledge_update()->set_refugees_managed( true );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_knowledge_update { knowledge { id: 7 } knowledge_group { id: 8 } pertinence: 9 identification_level: reconnue max_identification_level: detectee etat_op: 10 mort: true position { latitude: 17.23 longitude: 23.17 } direction { heading: 77 } speed: 78 party { id: 79 } nature_pc: true perception_par_compagnie { elem { automat { id: 100 } identification_level: signale } elem { automat { id: 100 } identification_level: signale } } surrendered_unit { id: 80 } prisonnier: true refugie_pris_en_compte: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( unit_knowledge_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_knowledge_destruction()->mutable_knowledge()->set_id( 7 );
    content.mutable_unit_knowledge_destruction()->mutable_knowledge_group()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_knowledge_destruction { knowledge { id: 7 } knowledge_group { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( start_unit_fire_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_start_unit_fire()->mutable_fire()->set_id( 7 );
    content.mutable_start_unit_fire()->mutable_firing_unit()->set_id( 8 );
    content.mutable_start_unit_fire()->mutable_target()->mutable_unit()->set_id( 9 );
    content.mutable_start_unit_fire()->mutable_target()->mutable_crowd()->set_id( 10 );
    FillCoordLatLong( content.mutable_start_unit_fire()->mutable_target()->mutable_position() );
    content.mutable_start_unit_fire()->set_type( sword::StartUnitFire::indirect );
    content.mutable_start_unit_fire()->mutable_ammunition()->set_id( 11 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { start_unit_fire { fire { id: 7 } firing_unit { id: 8 } target { unit { id: 9 } crowd { id: 10 } position { latitude: 17.23 longitude: 23.17 } } type: indirect ammunition { id: 11 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename D >
    void FillUnitHumanFireDamage( D* d )
    {
        d->set_rank( sword::trooper );
        d->set_alive( 10 );
        d->set_dead( 11 );
        d->set_wounded_u1( 12 );
        d->set_wounded_u2( 13 );
        d->set_wounded_u3( 14 );
        d->set_wounded_ue( 15 );
    }
    template< typename D >
    void FillUnitEquipmentFireDamage( D* d )
    {
        d->mutable_equipement()->set_id( 20 );
        d->set_available( 21 );
        d->set_unavailable( 22 );
        d->set_repairable( 23 );
    }
    template< typename D >
    void FillUnitFireDamages( D* d )
    {
        d->mutable_target()->set_id( 8 );
        FillUnitHumanFireDamage( d->mutable_humans()->add_elem() );
        FillUnitHumanFireDamage( d->mutable_humans()->add_elem() );
        FillUnitEquipmentFireDamage( d->mutable_equipments()->add_elem() );
        FillUnitEquipmentFireDamage( d->mutable_equipments()->add_elem() );
    }
    template< typename D >
    void FillCrowdFireDamages( D* d )
    {
        d->mutable_target()->set_id( 30 );
        d->set_dead( 31 );
        d->set_wounded( 22 );
        d->set_scattered( 13 );
    }
}

BOOST_FIXTURE_TEST_CASE( stop_unit_fire_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_stop_unit_fire()->mutable_fire()->set_id( 7 );
    FillUnitFireDamages( content.mutable_stop_unit_fire()->mutable_units_damages()->add_elem() );
    FillUnitFireDamages( content.mutable_stop_unit_fire()->mutable_units_damages()->add_elem() );
    FillCrowdFireDamages( content.mutable_stop_unit_fire()->mutable_crowds_damages()->add_elem() );
    FillCrowdFireDamages( content.mutable_stop_unit_fire()->mutable_crowds_damages()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { stop_unit_fire { fire { id: 7 } units_damages { elem { target { id: 8 } humans { elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } } equipments { elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } } } elem { target { id: 8 } humans { elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } } equipments { elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } } } } crowds_damages { elem { target { id: 30 } dead_nbr: 31 } elem { target { id: 30 } dead_nbr: 31 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( start_crowd_fire_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_start_crowd_fire()->mutable_fire()->set_id( 7 );
    content.mutable_start_crowd_fire()->mutable_firing_crowd()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { start_crowd_fire { fire { id: 7 } firing_crowd { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( stop_crowd_fire_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_stop_crowd_fire()->mutable_fire()->set_id( 7 );
    FillUnitFireDamages( content.mutable_stop_crowd_fire()->mutable_units_damages()->add_elem() );
    FillUnitFireDamages( content.mutable_stop_crowd_fire()->mutable_units_damages()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { stop_crowd_fire { fire { id: 7 } units_damages { elem { target { id: 8 } humans { elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } } equipments { elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } } } elem { target { id: 8 } humans { elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } } equipments { elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } } } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( explosion_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_explosion()->mutable_object()->set_id( 7 );
    FillUnitFireDamages( content.mutable_explosion()->mutable_units_damages()->add_elem() );
    FillUnitFireDamages( content.mutable_explosion()->mutable_units_damages()->add_elem() );
    FillCrowdFireDamages( content.mutable_explosion()->mutable_crowds_damages()->add_elem() );
    FillCrowdFireDamages( content.mutable_explosion()->mutable_crowds_damages()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { explosion { object { id: 7 } units_damages { elem { target { id: 8 } humans { elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } } equipments { elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } } } elem { target { id: 8 } humans { elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } elem { rank: mdr alive_nbr: 10 dead_nbr: 11 wounded_u1_nbr: 12 wounded_u2_nbr: 13 wounded_u3_nbr: 14 wounded_ue_nbr: 15 } } equipments { elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } elem { equipement_type { id: 20 } available_nbr: 21 unavailable_nbr: 22 repairable_nbr: 23 } } } } crowds_damages { elem { target { id: 30 } dead_nbr: 31 } elem { target { id: 30 } dead_nbr: 31 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( start_fire_effect_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_start_fire_effect()->mutable_fire_effect()->set_id( 7 );
    FillLocation( content.mutable_start_fire_effect()->mutable_location() );
    content.mutable_start_fire_effect()->set_type( sword::StartFireEffect::light );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { start_fire_effect { fire_effect { id: 7 } location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type: light } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( stop_fire_effect_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_stop_fire_effect()->mutable_fire_effect()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { stop_fire_effect { fire_effect { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename W >
    void FillPlannedWork( W* w )
    {
        w->set_type( "type" );
        FillLocation( w->mutable_position() );
        w->set_type_obstacle( sword::ObstacleType::reserved );
        w->set_density( 1.23f );
        w->mutable_combat_train()->set_id( 40 );
        w->set_activity_time( 41 );
    }
    template< typename O >
    void FillMissionObjective( O* o )
    {
        FillLocation( o->mutable_location() );
        o->mutable_time()->set_data( "horaire" );
    }
    template< typename O >
    void FillLimaOrder( O* o )
    {
        FillLocation( o->mutable_line()->mutable_location() );
        o->mutable_time()->set_data( "horaire2" );
        o->add_fonctions( sword::PhaseLineOrder::handover_line );
        o->add_fonctions( sword::PhaseLineOrder::objective_line );
    }
    template< typename V >
    void FillParameterValue( V* v )
    {
        v->set_booleanvalue( true );
        v->set_intvalue( 2 );
        v->mutable_heading()->set_heading( 3 );
        v->set_enumeration( 4 );
        v->mutable_datetime()->set_data( "data2" );
        FillLocation( v->mutable_point()->mutable_location() );
        FillLocation( v->mutable_area()->mutable_location() );
        FillLocation( v->mutable_path()->mutable_location() );
        FillLocation( v->mutable_limit()->mutable_location() );
        FillLimaOrder( v->mutable_phaseline()->add_elem() );
        FillLimaOrder( v->mutable_phaseline()->add_elem() );
        v->mutable_automat()->set_id( 5 );
        v->mutable_agent()->set_id( 6 );
        v->mutable_agentknowledge()->set_id( 7 );
        v->mutable_crowdknowledge()->set_id( 8 );
        v->mutable_objectknowledge()->set_id( 9 );
        v->mutable_urbanknowledge()->set_id( 10 );
        FillPlannedWork( v->mutable_plannedwork() );
        v->mutable_nature()->set_flags( 11 );
        v->mutable_resourcetype()->set_id( 50 );
        v->mutable_logmaintenancepriorities()->add_elem()->set_id( 51 );
        v->mutable_logmaintenancepriorities()->add_elem()->set_id( 52 );
        v->mutable_logmedicalpriorities()->add_elem( sword::dead );
        v->mutable_logmedicalpriorities()->add_elem( sword::unwounded );
        v->set_areal( 3.14f );
        FillLocation( v->mutable_pathlist()->add_elem()->mutable_location() );
        FillLocation( v->mutable_pathlist()->add_elem()->mutable_location() );
        FillLocation( v->mutable_pointlist()->add_elem()->mutable_location() );
        FillLocation( v->mutable_pointlist()->add_elem()->mutable_location() );
        FillLocation( v->mutable_polygonlist()->add_elem()->mutable_location() );
        FillLocation( v->mutable_polygonlist()->add_elem()->mutable_location() );
        FillLocation( v->mutable_locationlist()->add_elem() );
        FillLocation( v->mutable_locationlist()->add_elem() );
        v->mutable_unitlist()->add_elem()->set_id( 60 );
        v->mutable_unitlist()->add_elem()->set_id( 61 );
        v->mutable_automatlist()->add_elem()->set_id( 63 );
        v->mutable_automatlist()->add_elem()->set_id( 64 );
        v->mutable_unitknowledgelist()->add_elem()->set_id( 66 );
        v->mutable_unitknowledgelist()->add_elem()->set_id( 67 );
        v->mutable_objectknowledgelist()->add_elem()->set_id( 69 );
        v->mutable_objectknowledgelist()->add_elem()->set_id( 70 );
        v->mutable_equipmenttype()->set_id( 71 );
        v->mutable_indirectfire()->set_id( 72 );
        v->set_acharstr( "acharstr" );
        FillMissionObjective( v->mutable_missionobjective() );
        FillMissionObjective( v->mutable_missionobjectivelist()->add_elem() );
        FillMissionObjective( v->mutable_missionobjectivelist()->add_elem() );
        v->mutable_object()->set_id( 73 );
        v->mutable_party()->set_id( 74 );
        v->mutable_formation()->set_id( 75 );
        v->set_identifier( 76u );
        v->set_quantity( 77 );
        v->mutable_knowledgegroup()->set_id( 78 );
        v->add_list()->set_booleanvalue( true );
        v->add_list()->set_booleanvalue( false );
    }
    template< typename P >
    void FillParameter( P* p )
    {
        p->set_null_value( true );
        FillParameterValue( p->add_value() );
    }
}

BOOST_FIXTURE_TEST_CASE( report_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_report()->mutable_report()->set_id( 7 );
    FillTasker( content.mutable_report()->mutable_source() );
    content.mutable_report()->mutable_type()->set_id( 8 );
    content.mutable_report()->set_category( sword::Report::warning );
    content.mutable_report()->mutable_time()->set_data( "data" );
    FillParameter( content.mutable_report()->mutable_parameters()->add_elem() );
    FillParameter( content.mutable_report()->mutable_parameters()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { report { report { id: 7 } source { automat { id: 7 } formation { id: 8 } crowd { id: 9 } unit { id: 10 } party { id: 11 } } type { id: 8 } category { id: 3 } time { data: \"data\" } parameters { elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( invalidate_report_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_invalidate_report()->mutable_report()->set_id( 7 );
    FillTasker( content.mutable_invalidate_report()->mutable_source() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { invalidate_report { report { id: 7 } source { automat { id: 7 } formation { id: 8 } crowd { id: 9 } unit { id: 10 } party { id: 11 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( trace_to_client_is_ignored, ContextFixture< sword::SimToClient > )
{
    FillTasker( content.mutable_trace()->mutable_source() );
    content.mutable_trace()->set_message( "message" );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( decisional_state_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillTasker( content.mutable_decisional_state()->mutable_source() );
    content.mutable_decisional_state()->set_key( "key" );
    content.mutable_decisional_state()->set_value( "value" );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { decisional_state { source { automat { id: 7 } formation { id: 8 } crowd { id: 9 } unit { id: 10 } party { id: 11 } } key: \"key\" value: \"value\" } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( debug_points_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillTasker( content.mutable_debug_points()->mutable_source() );
    FillCoordLatLong( content.mutable_debug_points()->mutable_coordinates()->add_elem() );
    FillCoordLatLong( content.mutable_debug_points()->mutable_coordinates()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { debug_points { source { automat { id: 7 } formation { id: 8 } crowd { id: 9 } unit { id: 10 } party { id: 11 } } coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename C >
    void FillVisionCone( C* c )
    {
        FillCoordLatLong( c->mutable_origin() );
        c->set_height( 42 );
        c->set_sensor( "sensor" );
        c->mutable_directions()->add_elem()->set_heading( 43 );
        c->mutable_directions()->add_elem()->set_heading( 44 );
    }
}

BOOST_FIXTURE_TEST_CASE( unit_vision_cones_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_unit_vision_cones()->mutable_unit()->set_id( 7 );
    FillVisionCone( content.mutable_unit_vision_cones()->mutable_cones()->add_elem() );
    FillVisionCone( content.mutable_unit_vision_cones()->mutable_cones()->add_elem() );
    content.mutable_unit_vision_cones()->set_elongation( 8.3f );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_vision_cones { unit { id: 7 } cones { elem { origin { latitude: 17.23 longitude: 23.17 } height: 42 sensor: \"sensor\" directions { elem { heading: 43 } elem { heading: 44 } } } elem { origin { latitude: 17.23 longitude: 23.17 } height: 42 sensor: \"sensor\" directions { elem { heading: 43 } elem { heading: 44 } } } } elongation: 8.3 } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename O >
    void FillOrder( O* o )
    {
        o->mutable_tasker()->set_id( 7 );
        o->mutable_type()->set_id( 8 );
        FillParameter( o->mutable_parameters()->add_elem() );
        FillParameter( o->mutable_parameters()->add_elem() );
        o->set_label( "label" );
        o->mutable_start_time()->set_data( "data" );
        FillLocation( o->mutable_symbollocation() );
    }
}

BOOST_FIXTURE_TEST_CASE( unit_order_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillOrder( content.mutable_unit_order() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { unit_order { tasker { id: 7 } type { id: 8 } parameters { elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } } label: \"label\" symbolLocation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } start_time { data: \"data\" } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_order_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillOrder( content.mutable_automat_order() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_order { tasker { id: 7 } type { id: 8 } parameters { elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } } label: \"label\" symbolLocation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } start_time { data: \"data\" } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_order_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillOrder( content.mutable_crowd_order() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_order { tasker { id: 7 } type { id: 8 } parameters { elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } elem { null_value: true value { booleanValue: true intValue: 2 heading { heading: 3 } enumeration: 4 dateTime { data: \"data2\" } point { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } area { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } path { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } limit { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } phaseLine { elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } elem { lima { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } horaire { data: \"horaire2\" } fonctions: ligne_recueil fonctions: ligne_objectif } } automat { id: 5 } agent { id: 6 } agentKnowledge { id: 7 } crowdKnowledge { id: 8 } objectKnowledge { id: 9 } urbanKnowledge { id: 10 } plannedWork { type: \"type\" position { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } type_obstacle: reserved densite: 1.23 tc2 { id: 40 } activity_time: 41 } atlasNature { nature: 11 } resourceType { id: 50 } logMaintenancePriorities { elem { id: 51 } elem { id: 52 } } logMedicalPriorities { elem: mort elem: non_blesse } aReal: 3.14 pathList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } pointList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } polygonList { elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } elem { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } } locationList { elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } elem { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } unitList { elem { id: 60 } elem { id: 61 } } automatList { elem { id: 63 } elem { id: 64 } } unitKnowledgeList { elem { id: 66 } elem { id: 67 } } objectKnowledgeList { elem { id: 69 } elem { id: 70 } } equipmentType { id: 71 } tirIndirect { id: 72 } aCharStr: \"acharstr\" missionObjective { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } missionObjectiveList { elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } elem { localisation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } horaire { data: \"horaire\" } } } object { id: 73 } party { id: 74 } formation { id: 75 } identifier: 76 quantity: 77 knowledgeGroup { id: 78 } list { booleanValue: true } list { booleanValue: false } } } } label: \"label\" symbolLocation { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } start_time { data: \"data\" } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename Q >
    void FillLocatedQuantity( Q* q )
    {
        FillCoordLatLong( q->mutable_coordinate() );
        q->set_quantity( 32 );
    }
    template< typename C >
    void FillBurningCell( C* c )
    {
        c->set_origin_x( 1 );
        c->set_origin_y( 2 );
        c->set_phase( sword::decline );
        c->mutable_decline()->set_current_heat( 3 );
    }
    template< typename R >
    void FillStockResource( R* r )
    {
        r->mutable_resource()->set_id( 42 );
        r->set_current( 43 );
        r->set_maximum( 44 );
    }
    template< typename L >
    void FillResourceNetworkLink( L* l )
    {
        l->mutable_object()->set_id( 50 );
        l->set_capacity( 51 );
        l->set_flow( 52u );
    }
    template< typename N >
    void FillResourceNetwork( N* n )
    {
        n->mutable_resource()->set_name( "name" );
        FillResourceNetworkLink( n->add_link() );
        FillResourceNetworkLink( n->add_link() );
        n->set_enabled( true );
        n->set_max_stock( 53 );
        n->set_stock( 54 );
        n->set_production( 55 );
        n->set_consumption( 56 );
        n->set_critical( true );
        n->set_initial_stock( 57u );
        n->set_max_production( 58u );
        n->set_max_consumption( 59u );
    }
    template< typename A >
    void FillObjectAttributes( A* a )
    {
        a->mutable_construction()->mutable_resource()->set_id( 10 );
        a->mutable_construction()->set_dotation( 11 );
        a->mutable_construction()->set_density( 12 );
        a->mutable_construction()->set_percentage( 13 );
        a->mutable_obstacle()->set_type( sword::ObstacleType::reserved );
        a->mutable_obstacle()->set_activated( true );
        a->mutable_obstacle()->set_activation_time( 14 );
        a->mutable_obstacle()->set_activity_time( 142 );
        a->mutable_mine()->mutable_resource()->set_id( 15 );
        a->mutable_mine()->set_dotation( 16 );
        a->mutable_mine()->set_density( 17 );
        a->mutable_mine()->set_percentage( 18 );
        a->mutable_bypass()->set_percentage( 20 );
        a->mutable_logistic()->mutable_logistic_superior()->mutable_automat()->set_id( 21 );
        a->mutable_nbc()->set_danger_level( 22 );
        a->mutable_nbc()->add_nbc_agents()->set_id( 23 );
        a->mutable_nbc()->add_nbc_agents()->set_id( 24 );
        a->mutable_crossing_site()->set_width( 25 );
        a->mutable_crossing_site()->set_depth( 26 );
        a->mutable_crossing_site()->set_flow_rate( 27 );
        a->mutable_crossing_site()->set_banks_require_fitting( true );
        a->mutable_supply_route()->set_equipped( true );
        a->mutable_supply_route()->set_max_weight( 28 );
        a->mutable_supply_route()->set_width( 29 );
        a->mutable_supply_route()->set_length( 30 );
        a->mutable_supply_route()->set_flow_rate( 31 );
        FillLocatedQuantity( a->mutable_toxic_cloud()->mutable_quantities()->add_elem() );
        FillLocatedQuantity( a->mutable_toxic_cloud()->mutable_quantities()->add_elem() );
        a->mutable_fire()->set_class_name( "Class A" );
        a->mutable_fire()->set_max_combustion_energy( 2000 );
        a->mutable_burn()->set_current_heat( 34 );
        a->mutable_burn()->set_combustion_energy( 34 );
        a->mutable_burn_surface()->set_cell_size( 100 );
        FillBurningCell( a->mutable_burn_surface()->add_burning_cells() );
        a->mutable_interaction_height()->set_height( 41 );
        FillStockResource( a->mutable_stock()->add_resources() );
        FillStockResource( a->mutable_stock()->add_resources() );
        a->mutable_nbc_agent()->mutable_agent()->set_id( 45 );
        a->mutable_nbc_agent()->set_concentration( 46 );
        a->mutable_nbc_agent()->set_source_life_duration( 47 );
        a->mutable_effect_delay()->set_value( 48 );
        FillResourceNetwork( a->mutable_resource_networks()->add_network() );
        FillResourceNetwork( a->mutable_resource_networks()->add_network() );
    }
}

BOOST_FIXTURE_TEST_CASE( object_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_creation()->mutable_object()->set_id( 7 );
    content.mutable_object_creation()->mutable_type()->set_id( "id" );
    content.mutable_object_creation()->set_name( "name" );
    content.mutable_object_creation()->mutable_party()->set_id( 9 );
    FillLocation( content.mutable_object_creation()->mutable_location() );
    FillObjectAttributes( content.mutable_object_creation()->mutable_attributes() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_creation { object { id: 7 } type { id: \"id\" } name: \"name\" party { id: 9 } location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } attributes { construction { resource { id: 10 } dotation_nbr: 11 density: 12 percentage: 13 } obstacle { type: reserved activated: true } valorisation { resource { id: 15 } dotation_nbr: 16 density: 17 percentage: 18 } activity_time { activity_time: 142 activation_time: 14 } bypass { percentage: 20 } logistic { logistic_superior { automat { id: 21 } } } nbc { danger_level: 22 nbc_agents { id: 23 } nbc_agents { id: 24 } } crossing_site { width: 25 depth: 26 flow_rate: 27 banks_require_fitting: true } supply_route { equipped: true max_weight: 28 width: 29 length: 30 flow_rate: 31 } toxic_cloud { quantities { elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } } } fire { class_name: \"Class A\" max_combustion_energy: 2000 } interaction_height { height: 41 } stock { resources { resource { id: 42 } current: 43 maximum: 44 } resources { resource { id: 42 } current: 43 maximum: 44 } } nbc_agent { agent { id: 45 } concentration: 46 source_life_duration: 47 } effect_delay { value: 48 } resource_networks { network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } } burn { current_heat: 34 combustion_energy: 34 } burn_surface { cell_size: 100 burning_cells { origin_x: 1 origin_y: 2 phase: decline decline { current_heat: 3 } } } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( object_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_destruction()->mutable_object()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_destruction { object { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( object_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_update()->mutable_object()->set_id( 7 );
    FillLocation( content.mutable_object_update()->mutable_location() );
    FillObjectAttributes( content.mutable_object_update()->mutable_attributes() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_update { object { id: 7 } location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } attributes { construction { resource { id: 10 } dotation_nbr: 11 density: 12 percentage: 13 } obstacle { type: reserved activated: true } valorisation { resource { id: 15 } dotation_nbr: 16 density: 17 percentage: 18 } activity_time { activity_time: 142 activation_time: 14 } bypass { percentage: 20 } logistic { logistic_superior { automat { id: 21 } } } nbc { danger_level: 22 nbc_agents { id: 23 } nbc_agents { id: 24 } } crossing_site { width: 25 depth: 26 flow_rate: 27 banks_require_fitting: true } supply_route { equipped: true max_weight: 28 width: 29 length: 30 flow_rate: 31 } toxic_cloud { quantities { elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } } } fire { class_name: \"Class A\" max_combustion_energy: 2000 } interaction_height { height: 41 } stock { resources { resource { id: 42 } current: 43 maximum: 44 } resources { resource { id: 42 } current: 43 maximum: 44 } } nbc_agent { agent { id: 45 } concentration: 46 source_life_duration: 47 } effect_delay { value: 48 } resource_networks { network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } } burn { current_heat: 34 combustion_energy: 34 } burn_surface { cell_size: 100 burning_cells { origin_x: 1 origin_y: 2 phase: decline decline { current_heat: 3 } } } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( object_knowledge_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_knowledge_creation()->mutable_knowledge()->set_id( 7 );
    content.mutable_object_knowledge_creation()->mutable_party()->set_id( 8 );
    content.mutable_object_knowledge_creation()->mutable_object()->set_id( 9 );
    content.mutable_object_knowledge_creation()->mutable_type()->set_id( "type" );
    FillObjectAttributes( content.mutable_object_knowledge_creation()->mutable_attributes() );
    content.mutable_object_knowledge_creation()->mutable_knowledge_group()->set_id( 10 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_knowledge_creation { knowledge { id: 7 } party { id: 8 } object { id: 9 } type { id: \"type\" } attributes { construction { resource { id: 10 } dotation_nbr: 11 density: 12 percentage: 13 } obstacle { type: reserved activated: true } valorisation { resource { id: 15 } dotation_nbr: 16 density: 17 percentage: 18 } activity_time { activity_time: 142 activation_time: 14 } bypass { percentage: 20 } logistic { logistic_superior { automat { id: 21 } } } nbc { danger_level: 22 nbc_agents { id: 23 } nbc_agents { id: 24 } } crossing_site { width: 25 depth: 26 flow_rate: 27 banks_require_fitting: true } supply_route { equipped: true max_weight: 28 width: 29 length: 30 flow_rate: 31 } toxic_cloud { quantities { elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } } } fire { class_name: \"Class A\" max_combustion_energy: 2000 } interaction_height { height: 41 } stock { resources { resource { id: 42 } current: 43 maximum: 44 } resources { resource { id: 42 } current: 43 maximum: 44 } } nbc_agent { agent { id: 45 } concentration: 46 source_life_duration: 47 } effect_delay { value: 48 } resource_networks { network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } } burn { current_heat: 34 combustion_energy: 34 } burn_surface { cell_size: 100 burning_cells { origin_x: 1 origin_y: 2 phase: decline decline { current_heat: 3 } } } } knowledge_group { id: 10 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( object_knowledge_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_knowledge_update()->mutable_knowledge()->set_id( 7 );
    content.mutable_object_knowledge_update()->mutable_party()->set_id( 8 );
    content.mutable_object_knowledge_update()->mutable_object()->set_id( 9 );
    content.mutable_object_knowledge_update()->set_relevance( 10 );
    FillLocation( content.mutable_object_knowledge_update()->mutable_location() );
    FillObjectAttributes( content.mutable_object_knowledge_update()->mutable_attributes() );
    content.mutable_object_knowledge_update()->set_perceived( true );
    content.mutable_object_knowledge_update()->mutable_perceiving_automats()->add_elem()->set_id( 11 );
    content.mutable_object_knowledge_update()->mutable_perceiving_automats()->add_elem()->set_id( 12 );
    content.mutable_object_knowledge_update()->mutable_knowledge_group()->set_id( 13 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_knowledge_update { knowledge { id: 7 } party { id: 8 } object { id: 9 } relevance: 10 location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } attributes { construction { resource { id: 10 } dotation_nbr: 11 density: 12 percentage: 13 } obstacle { type: reserved activated: true } valorisation { resource { id: 15 } dotation_nbr: 16 density: 17 percentage: 18 } activity_time { activity_time: 142 activation_time: 14 } bypass { percentage: 20 } logistic { logistic_superior { automat { id: 21 } } } nbc { danger_level: 22 nbc_agents { id: 23 } nbc_agents { id: 24 } } crossing_site { width: 25 depth: 26 flow_rate: 27 banks_require_fitting: true } supply_route { equipped: true max_weight: 28 width: 29 length: 30 flow_rate: 31 } toxic_cloud { quantities { elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } elem { coordinate { latitude: 17.23 longitude: 23.17 } quantity: 32 } } } fire { class_name: \"Class A\" max_combustion_energy: 2000 } interaction_height { height: 41 } stock { resources { resource { id: 42 } current: 43 maximum: 44 } resources { resource { id: 42 } current: 43 maximum: 44 } } nbc_agent { agent { id: 45 } concentration: 46 source_life_duration: 47 } effect_delay { value: 48 } resource_networks { network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } } burn { current_heat: 34 combustion_energy: 34 } burn_surface { cell_size: 100 burning_cells { origin_x: 1 origin_y: 2 phase: decline decline { current_heat: 3 } } } } perceived: true perceiving_automats { elem { id: 11 } elem { id: 12 } } knowledge_group { id: 13 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( object_knowledge_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_object_knowledge_destruction()->mutable_knowledge()->set_id( 7 );
    content.mutable_object_knowledge_destruction()->mutable_party()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { object_knowledge_destruction { knowledge { id: 7 } party { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_medical_handling_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_medical_handling_creation()->mutable_request()->set_id( 7 );
    content.mutable_log_medical_handling_creation()->mutable_unit()->set_id( 8 );
    content.mutable_log_medical_handling_creation()->set_tick( 9u );
    content.mutable_log_medical_handling_creation()->set_rank( sword::trooper );
    content.mutable_log_medical_handling_creation()->set_wound( sword::dead );
    content.mutable_log_medical_handling_creation()->set_mental_wound( true );
    content.mutable_log_medical_handling_creation()->set_nbc_contaminated( true );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_medical_handling_creation { request { id: 7 } unit { id: 8 } tick_creation: 9 rang: mdr blessure: mort blesse_mental: true contamine_nbc: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_medical_handling_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_medical_handling_update()->mutable_request()->set_id( 7 );
    content.mutable_log_medical_handling_update()->mutable_unit()->set_id( 8 );
    content.mutable_log_medical_handling_update()->mutable_provider()->set_id( 9 );
    content.mutable_log_medical_handling_update()->set_wound( sword::dead );
    content.mutable_log_medical_handling_update()->set_mental_wound( true );
    content.mutable_log_medical_handling_update()->set_nbc_contaminated( true );
    content.mutable_log_medical_handling_update()->set_state( sword::LogMedicalHandlingUpdate::triaging );
    content.mutable_log_medical_handling_update()->set_diagnosed( true );
    content.mutable_log_medical_handling_update()->set_current_state_end_tick( 11 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_medical_handling_update { request { id: 7 } unit { id: 8 } provider { id: 9 } blessure: mort blesse_mental: true contamine_nbc: true etat: tri current_state_end_tick: 11 diagnostique_effectue: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_medical_handling_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_medical_handling_destruction()->mutable_request()->set_id( 7 );
    content.mutable_log_medical_handling_destruction()->mutable_unit()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_medical_handling_destruction { request { id: 7 } unit { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename A >
    void FillLogMedicalEquipmentAvailability( A* a )
    {
        a->mutable_equipment()->set_id( 10 );
        a->set_total( 11 );
        a->set_available( 12 );
        a->set_working( 13 );
        a->set_lent( 14 );
        a->set_resting( 15 );
    }
}

BOOST_FIXTURE_TEST_CASE( log_medical_state_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_medical_state()->mutable_unit()->set_id( 7 );
    content.mutable_log_medical_state()->set_chain( true );
    content.mutable_log_medical_state()->mutable_priorities()->add_elem( sword::dead );
    content.mutable_log_medical_state()->mutable_priorities()->add_elem( sword::unwounded );
    content.mutable_log_medical_state()->mutable_tactical_priorities()->add_elem()->set_id( 8 );
    content.mutable_log_medical_state()->mutable_tactical_priorities()->add_elem()->set_id( 9 );
    FillLogMedicalEquipmentAvailability( content.mutable_log_medical_state()->mutable_evacuation_ambulances()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_medical_state()->mutable_evacuation_ambulances()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_medical_state()->mutable_collection_ambulances()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_medical_state()->mutable_collection_ambulances()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_medical_state()->mutable_doctors()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_medical_state()->mutable_doctors()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_medical_state { unit { id: 7 } chaine_activee: true priorites { elem: mort elem: non_blesse } tactical_priorities { elem { id: 8 } elem { id: 9 } } disponibilites_ambulances_releve { elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } } disponibilites_ambulances_ramassage { elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } } disponibilites_medecins { elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_maintenance_handling_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_maintenance_handling_creation()->mutable_request()->set_id( 7 );
    content.mutable_log_maintenance_handling_creation()->mutable_unit()->set_id( 8 );
    content.mutable_log_maintenance_handling_creation()->set_tick( 9 );
    content.mutable_log_maintenance_handling_creation()->mutable_equipement()->set_id( 10 );
    content.mutable_log_maintenance_handling_creation()->mutable_breakdown()->set_id( 11 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_maintenance_handling_creation { request { id: 7 } unit { id: 8 } tick_creation: 9 equipement { id: 10 } breakdown { id: 11 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_maintenance_handling_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_maintenance_handling_update()->mutable_request()->set_id( 7 );
    content.mutable_log_maintenance_handling_update()->mutable_unit()->set_id( 8 );
    content.mutable_log_maintenance_handling_update()->mutable_provider()->set_id( 9 );
    content.mutable_log_maintenance_handling_update()->set_state( sword::LogMaintenanceHandlingUpdate::repairing );
    content.mutable_log_maintenance_handling_update()->set_diagnosed( true );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_maintenance_handling_update { request { id: 7 } unit { id: 8 } provider { id: 9 } etat: reparation current_state_end_tick: 1 diagnostique_effectue: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_maintenance_handling_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_maintenance_handling_destruction()->mutable_request()->set_id( 7 );
    content.mutable_log_maintenance_handling_destruction()->mutable_unit()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_maintenance_handling_destruction { request { id: 7 } unit { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_maintenance_state_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_maintenance_state()->mutable_unit()->set_id( 7 );
    content.mutable_log_maintenance_state()->set_chain( true );
    content.mutable_log_maintenance_state()->set_work_rate( sword::rate_1 );
    content.mutable_log_maintenance_state()->mutable_priorities()->add_elem()->set_id( 8 );
    content.mutable_log_maintenance_state()->mutable_priorities()->add_elem()->set_id( 9 );
    content.mutable_log_maintenance_state()->mutable_tactical_priorities()->add_elem()->set_id( 10 );
    content.mutable_log_maintenance_state()->mutable_tactical_priorities()->add_elem()->set_id( 11 );
    FillLogMedicalEquipmentAvailability( content.mutable_log_maintenance_state()->mutable_haulers()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_maintenance_state()->mutable_haulers()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_maintenance_state()->mutable_repairers()->add_elem() );
    FillLogMedicalEquipmentAvailability( content.mutable_log_maintenance_state()->mutable_repairers()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_maintenance_state { unit { id: 7 } chaine_activee: true regime_travail: regime_1 priorites { elem { id: 8 } elem { id: 9 } } priorites_tactiques { elem { id: 10 } elem { id: 11 } } disponibilites_remorqueurs { elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } } disponibilites_reparateurs { elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } elem { equipment_type { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename Q >
    void FillDotationQuery( Q& q )
    {
        q.mutable_resource()->set_id( 10 );
        q.set_requested( 11 );
        q.set_granted( 12 );
        q.set_convoyed( 13 );
    }

    template< typename Q >
    void FillSupplyRequest( Q& req )
    {
        req.mutable_recipient()->set_id( 34 );
        FillDotationQuery( *req.add_resources() );
        FillDotationQuery( *req.add_resources() );
    }
}

BOOST_FIXTURE_TEST_CASE( log_supply_handling_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_handling_creation()->mutable_request()->set_id( 7 );
    content.mutable_log_supply_handling_creation()->set_tick( 9 );
    content.mutable_log_supply_handling_creation()->mutable_supplier()->mutable_automat()->set_id( 7 );
    content.mutable_log_supply_handling_creation()->mutable_transporters_provider()->mutable_formation()->set_id( 8 );    
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_handling_creation { request { id: 7 } supplier { automat { id: 7 } } tick_creation: 9 transporters_provider { formation { id: 8 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_supply_handling_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_handling_update()->mutable_request()->set_id( 7 );
    content.mutable_log_supply_handling_update()->mutable_convoyer()->set_id( 13 );
    FillSupplyRequest( *content.mutable_log_supply_handling_update()->mutable_requests()->add_requests() );
    content.mutable_log_supply_handling_update()->set_state( sword::LogSupplyHandlingUpdate::convoy_loading );
    content.mutable_log_supply_handling_update()->set_current_state_end_tick( 666 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_handling_update { request { id: 7 } convoying_unit { id: 13 } etat: convoi_chargement current_state_end_tick: 666 requests { requests { recipient { id: 34 } resources { resource { id: 10 } requested: 11 granted: 12 convoyed: 13 } resources { resource { id: 10 } requested: 11 granted: 12 convoyed: 13 } } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_supply_handling_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_handling_destruction()->mutable_request()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_handling_destruction { request { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_funeral_handling_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_funeral_handling_creation()->mutable_request()->set_id( 7 );
    content.mutable_log_funeral_handling_creation()->mutable_unit()->set_id( 8 );
    content.mutable_log_funeral_handling_creation()->set_tick( 9 );
    content.mutable_log_funeral_handling_creation()->set_rank( sword::officer );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_funeral_handling_creation { request { id: 7 } requesting_unit { id: 8 } tick_creation: 9 rank: officier } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_funeral_handling_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    // NB: requesting_unit '8' comes from 'log_funeral_handling_creation_to_client_is_converted' ...
    content.mutable_log_funeral_handling_update()->mutable_request()->set_id( 7 );
    content.mutable_log_funeral_handling_update()->mutable_handling_unit()->mutable_automat()->set_id( 9 );
    content.mutable_log_funeral_handling_update()->mutable_convoying_unit()->set_id( 10 );
    content.mutable_log_funeral_handling_update()->set_state( sword::LogFuneralHandlingUpdate::waiting_for_transporter );
    content.mutable_log_funeral_handling_update()->set_current_state_end_tick( 666 );
    content.mutable_log_funeral_handling_update()->mutable_packaging_resource()->set_id( 11 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_funeral_handling_update { request { id: 7 } requesting_unit { id: 8 } handling_unit { automat { id: 9 } } convoying_unit { id: 10 } status: attente_transporteur conditioning_resource { id: 11 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( log_funeral_handling_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    // NB: requesting_unit '8' comes from 'log_funeral_handling_creation_to_client_is_converted' ...
    content.mutable_log_funeral_handling_destruction()->mutable_request()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_funeral_handling_destruction { request { id: 7 } requesting_unit { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename S >
    void FillDotationStock( S* s )
    {
        s->mutable_resource()->set_id( 10 );
        s->set_quantity( 11 );
    }
    template< typename A >
    void FillLogSupplyEquimentAvailability( A* a )
    {
        a->mutable_equipment()->set_id( 10 );
        a->set_total( 11 );
        a->set_available( 12 );
        a->set_working( 13 );
        a->set_lent( 14 );
        a->set_resting( 15 );
    }
}

BOOST_FIXTURE_TEST_CASE( log_supply_state_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_state()->mutable_unit()->set_id( 7 );
    content.mutable_log_supply_state()->set_chain( true );
    FillDotationStock( content.mutable_log_supply_state()->mutable_stocks()->add_elem() );
    FillDotationStock( content.mutable_log_supply_state()->mutable_stocks()->add_elem() );
    FillLogSupplyEquimentAvailability( content.mutable_log_supply_state()->mutable_transporters()->add_elem() );
    FillLogSupplyEquimentAvailability( content.mutable_log_supply_state()->mutable_transporters()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_state { unit { id: 7 } chaine_activee: true stocks { elem { ressource_id { id: 10 } quantite_disponible: 11 } elem { ressource_id { id: 10 } quantite_disponible: 11 } } disponibilites_transporteurs_convois { elem { equipment { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } elem { equipment { id: 10 } nbr_total: 11 nbr_disponibles: 12 nbr_au_travail: 13 nbr_pretes: 14 nbr_au_repos: 15 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename Q >
    void FillDotationQuota( Q* q )
    {
        q->mutable_resource()->set_id( 10 );
        q->set_quantity( 11 );
    }
}

BOOST_FIXTURE_TEST_CASE( log_supply_quotas_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_log_supply_quotas()->mutable_supplied()->mutable_automat()->set_id( 7 );
    content.mutable_log_supply_quotas()->mutable_supplier()->mutable_formation()->set_id( 8 );
    FillDotationQuota( content.mutable_log_supply_quotas()->mutable_quotas()->add_elem() );
    FillDotationQuota( content.mutable_log_supply_quotas()->mutable_quotas()->add_elem() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { log_supply_quotas { supplied { automat { id: 7 } } supplier { formation { id: 8 } } quotas { elem { ressource_id { id: 10 } quota_disponible: 11 } elem { ressource_id { id: 10 } quota_disponible: 11 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_creation()->mutable_crowd()->set_id( 7 );
    content.mutable_crowd_creation()->mutable_type()->set_id( 8 );
    content.mutable_crowd_creation()->set_name( "name" );
    content.mutable_crowd_creation()->mutable_party()->set_id( 9 );
    content.mutable_crowd_creation()->mutable_repartition()->set_male( 0.5f );
    content.mutable_crowd_creation()->mutable_repartition()->set_female( 0.3f );
    content.mutable_crowd_creation()->mutable_repartition()->set_children( 0.2f );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_creation { crowd { id: 7 } type { id: 8 } nom: \"name\" party { id: 9 } repartition { male: 0.5 female: 0.3 children: 0.2 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_update()->mutable_crowd()->set_id( 7 );
    content.mutable_crowd_update()->set_domination( 8 );
    FillExtension( content.mutable_crowd_update()->mutable_extension()->add_entries() );
    FillExtension( content.mutable_crowd_update()->mutable_extension()->add_entries() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_update { crowd { id: 7 } domination: 8 extension { entries { name: \"name2\" value: \"value\" } entries { name: \"name2\" value: \"value\" } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_concentration_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_concentration_creation()->mutable_concentration()->set_id( 7 );
    content.mutable_crowd_concentration_creation()->mutable_crowd()->set_id( 8 );
    FillCoordLatLong( content.mutable_crowd_concentration_creation()->mutable_position() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_concentration_creation { concentration { id: 7 } crowd { id: 8 } position { latitude: 17.23 longitude: 23.17 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_concentration_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_concentration_destruction()->mutable_concentration()->set_id( 7 );
    content.mutable_crowd_concentration_destruction()->mutable_crowd()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_concentration_destruction { concentration { id: 7 } crowd { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_concentration_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_concentration_update()->mutable_concentration()->set_id( 7 );
    content.mutable_crowd_concentration_update()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_concentration_update()->set_healthy( 9 );
    content.mutable_crowd_concentration_update()->set_wounded( 10 );
    content.mutable_crowd_concentration_update()->set_contaminated( 11 );
    content.mutable_crowd_concentration_update()->set_dead( 12 );
    content.mutable_crowd_concentration_update()->set_attitude( sword::agitated );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_concentration_update { concentration { id: 7 } crowd { id: 8 } healthy: 9 wounded: 10 contaminated: 11 dead: 12 attitude: agitee } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_flow_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_flow_creation()->mutable_flow()->set_id( 7 );
    content.mutable_crowd_flow_creation()->mutable_crowd()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_flow_creation { flow { id: 7 } crowd { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_flow_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_flow_destruction()->mutable_flow()->set_id( 7 );
    content.mutable_crowd_flow_destruction()->mutable_crowd()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_flow_destruction { flow { id: 7 } crowd { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_flow_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_flow_update()->mutable_flow()->set_id( 7 );
    content.mutable_crowd_flow_update()->mutable_crowd()->set_id( 8 );
    FillLocation( content.mutable_crowd_flow_update()->mutable_parts()->mutable_location() );
    content.mutable_crowd_flow_update()->mutable_direction()->set_heading( 20 );
    content.mutable_crowd_flow_update()->set_speed( 21 );
    content.mutable_crowd_flow_update()->set_healthy( 22 );
    content.mutable_crowd_flow_update()->set_wounded( 23 );
    content.mutable_crowd_flow_update()->set_contaminated( 24 );
    content.mutable_crowd_flow_update()->set_dead( 25 );
    content.mutable_crowd_flow_update()->set_attitude( sword::agitated );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_flow_update { flow { id: 7 } crowd { id: 8 } parts { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } direction { heading: 20 } speed: 21 healthy: 22 wounded: 23 contaminated: 24 dead: 25 attitude: agitee } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_knowledge_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_knowledge_creation()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_knowledge_creation()->mutable_knowledge_group()->set_id( 8 );
    content.mutable_crowd_knowledge_creation()->mutable_crowd()->set_id( 9 );
    content.mutable_crowd_knowledge_creation()->mutable_party()->set_id( 10 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_knowledge_creation { knowledge { id: 7 } knowledge_group { id: 8 } crowd { id: 9 } party { id: 10 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_knowledge_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_knowledge_update()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_knowledge_update()->mutable_knowledge_group()->set_id( 8 );
    content.mutable_crowd_knowledge_update()->set_domination( 9 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_knowledge_update { knowledge { id: 7 } knowledge_group { id: 8 } etat_domination: 9 } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_knowledge_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_knowledge_destruction()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_knowledge_destruction()->mutable_knowledge_group()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_knowledge_destruction { knowledge { id: 7 } knowledge_group { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_concentration_knowledge_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_concentration_knowledge_creation()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_concentration_knowledge_creation()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_concentration_knowledge_creation()->mutable_knowledge_group()->set_id( 9 );
    content.mutable_crowd_concentration_knowledge_creation()->mutable_concentration()->set_id( 10 );
    FillCoordLatLong( content.mutable_crowd_concentration_knowledge_creation()->mutable_position() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_concentration_knowledge_creation { knowledge { id: 7 } crowd { id: 8 } knowledge_group { id: 9 } concentration { id: 10 } position { latitude: 17.23 longitude: 23.17 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_concentration_knowledge_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_concentration_knowledge_destruction()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_concentration_knowledge_destruction()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_concentration_knowledge_destruction()->mutable_knowledge_group()->set_id( 9 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_concentration_knowledge_destruction { knowledge { id: 7 } crowd { id: 8 } knowledge_group { id: 9 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_concentration_knowledge_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_concentration_knowledge_update()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_concentration_knowledge_update()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_concentration_knowledge_update()->mutable_knowledge_group()->set_id( 9 );
    content.mutable_crowd_concentration_knowledge_update()->mutable_concentration()->set_id( 10 );
    content.mutable_crowd_concentration_knowledge_update()->set_alive( 22 );
    content.mutable_crowd_concentration_knowledge_update()->set_dead( 23 );
    content.mutable_crowd_concentration_knowledge_update()->set_attitude( sword::agitated );
    content.mutable_crowd_concentration_knowledge_update()->set_pertinence( 24 );
    content.mutable_crowd_concentration_knowledge_update()->set_perceived( true );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_concentration_knowledge_update { knowledge { id: 7 } crowd { id: 8 } knowledge_group { id: 9 } concentration { id: 10 } nb_humains_vivants: 22 nb_humains_morts: 23 attitude: agitee pertinence: 24 est_percu: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_flow_knowledge_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_flow_knowledge_creation()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_flow_knowledge_creation()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_flow_knowledge_creation()->mutable_knowledge_group()->set_id( 9 );
    content.mutable_crowd_flow_knowledge_creation()->mutable_flow()->set_id( 10 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_flow_knowledge_creation { knowledge { id: 7 } crowd { id: 8 } knowledge_group { id: 9 } flow { id: 10 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_flow_knowledge_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_flow_knowledge_destruction()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_flow_knowledge_destruction()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_flow_knowledge_destruction()->mutable_knowledge_group()->set_id( 9 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_flow_knowledge_destruction { knowledge { id: 7 } crowd { id: 8 } knowledge_group { id: 9 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename P >
    void FillFlowPart( P* p )
    {
        FillLocation( p->mutable_shape()->mutable_location() );
        p->set_pertinence( 19 );
    }
}

BOOST_FIXTURE_TEST_CASE( crowd_flow_knowledge_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_flow_knowledge_update()->mutable_knowledge()->set_id( 7 );
    content.mutable_crowd_flow_knowledge_update()->mutable_crowd()->set_id( 8 );
    content.mutable_crowd_flow_knowledge_update()->mutable_knowledge_group()->set_id( 9 );
    content.mutable_crowd_flow_knowledge_update()->mutable_flow()->set_id( 10 );
    FillFlowPart( content.mutable_crowd_flow_knowledge_update()->mutable_parts()->add_elem() );
    FillFlowPart( content.mutable_crowd_flow_knowledge_update()->mutable_parts()->add_elem() );
    content.mutable_crowd_flow_knowledge_update()->mutable_direction()->set_heading( 20 );
    content.mutable_crowd_flow_knowledge_update()->set_speed( 21 );
    content.mutable_crowd_flow_knowledge_update()->set_alive( 22 );
    content.mutable_crowd_flow_knowledge_update()->set_dead( 23 );
    content.mutable_crowd_flow_knowledge_update()->set_attitude( sword::agitated );
    content.mutable_crowd_flow_knowledge_update()->set_perceived( true );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_flow_knowledge_update { knowledge { id: 7 } crowd { id: 8 } knowledge_group { id: 9 } flow { id: 10 } portions_flux { elem { forme { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } pertinence: 19 } elem { forme { location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } } pertinence: 19 } } direction { heading: 20 } vitesse: 21 nb_humains_vivants: 22 nb_humains_morts: 23 attitude: agitee est_percu: true } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_global_weather_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_global_weather_ack();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_global_weather_ack { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_local_weather_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_local_weather_ack();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_local_weather_ack { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_save_begin_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_save_begin();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_save_begin { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_set_frequency_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_set_frequency_ack();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_set_frequency_ack { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_save_now_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_save_now_ack();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_save_now_ack { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_send_current_state_begin_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_send_current_state_begin();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_send_current_state_begin { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_send_current_state_end_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_send_current_state_end();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_send_current_state_end { } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename A >
    void FillUrbanAttributes( A* a )
    {
        a->mutable_architecture()->set_height( 8.f );
        a->mutable_architecture()->set_floor_number( 9 );
        a->mutable_architecture()->set_roof_shape( "shape" );
        a->mutable_architecture()->set_material( "material" );
        a->mutable_architecture()->set_occupation( 10.f );
        a->mutable_architecture()->set_trafficability( 11.f );
        a->mutable_architecture()->set_parking_floors( 14 );
        a->mutable_structure()->set_state( 12u );
        FillRgbaColor( a->mutable_color() );
        FillResourceNetwork( a->mutable_infrastructures()->add_resource_network() );
        FillResourceNetwork( a->mutable_infrastructures()->add_resource_network() );
        a->mutable_infrastructures()->mutable_infrastructure()->set_type( "type" );
        a->mutable_infrastructures()->mutable_infrastructure()->set_active( true );
        a->mutable_infrastructures()->mutable_infrastructure()->set_threshold( 13 );
        FillUrbanUsage( a->add_usages() );
        FillUrbanUsage( a->add_usages() );
    }
}

BOOST_FIXTURE_TEST_CASE( urban_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_urban_creation()->mutable_object()->set_id( 7 );
    content.mutable_urban_creation()->set_name( "name" );
    FillLocation( content.mutable_urban_creation()->mutable_location() );
    FillUrbanAttributes( content.mutable_urban_creation()->mutable_attributes() );
    content.mutable_urban_creation()->mutable_parent()->set_id( 20 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { urban_creation { object { id: 7 } name: \"name\" location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } attributes { architecture { height: 8 floor_number: 9 roof_shape: \"shape\" material: \"material\" occupation: 10 trafficability: 11 parking_available: true } structure { state: 12 } color { red: 12 green: 42 blue: 77 alpha: 99 } infrastructures { resource_network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } resource_network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } infrastructure { type: \"type\" active: true threshold: 13 } } usages { role: \"office\" percentage: 11 } usages { role: \"office\" percentage: 11 } } parent { id: 20 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( urban_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_urban_update()->mutable_object()->set_id( 7 );
    FillLocation( content.mutable_urban_update()->mutable_location() );
    FillUrbanAttributes( content.mutable_urban_update()->mutable_attributes() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { urban_update { object { id: 7 } location { type: rectangle coordinates { elem { latitude: 17.23 longitude: 23.17 } elem { latitude: 17.23 longitude: 23.17 } } } attributes { architecture { height: 8 floor_number: 9 roof_shape: \"shape\" material: \"material\" occupation: 10 trafficability: 11 parking_available: true } structure { state: 12 } color { red: 12 green: 42 blue: 77 alpha: 99 } infrastructures { resource_network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } resource_network { resource { name: \"name\" } link { object { id: 50 } capacity: 51 flow: 52 } link { object { id: 50 } capacity: 51 flow: 52 } enabled: true max_stock: 53 stock: 54 production: 55 consumption: 56 critical: true initial_stock: 57 max_production: 58 max_consumption: 59 } infrastructure { type: \"type\" active: true threshold: 13 } } usages { role: \"office\" percentage: 11 } usages { role: \"office\" percentage: 11 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( urban_knowledge_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_urban_knowledge_creation()->mutable_knowledge()->set_id( 7 );
    content.mutable_urban_knowledge_creation()->mutable_party()->set_id( 8 );
    content.mutable_urban_knowledge_creation()->mutable_object()->set_id( 9 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { urban_knowledge_creation { knowledge { id: 7 } party { id: 8 } object { id: 9 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( urban_knowledge_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_urban_knowledge_update()->mutable_knowledge()->set_id( 7 );
    content.mutable_urban_knowledge_update()->mutable_party()->set_id( 8 );
    content.mutable_urban_knowledge_update()->mutable_object()->set_id( 9 );
    content.mutable_urban_knowledge_update()->set_progress( 10 );
    content.mutable_urban_knowledge_update()->set_max_progress( 11 );
    content.mutable_urban_knowledge_update()->set_perceived( true );
    content.mutable_urban_knowledge_update()->mutable_automat_perceptions()->add_elem()->set_id( 13u );
    content.mutable_urban_knowledge_update()->mutable_automat_perceptions()->add_elem()->set_id( 14u );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { urban_knowledge_update { knowledge { id: 7 } party { id: 8 } object { id: 9 } progress: 10 maxProgress: 11 perceived: true automat_perceptions { elem { id: 13 } elem { id: 14 } } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( stock_resource_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    FillStockResource( content.mutable_stock_resource() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { stock_resource { resource { id: 42 } current: 43 maximum: 44 } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( knowledge_group_magic_action_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_knowledge_group_magic_action_ack()->mutable_knowledge_group()->set_id( 7 );
    content.mutable_knowledge_group_magic_action_ack()->set_error_code( sword::KnowledgeGroupAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { knowledge_group_magic_action_ack { knowledge_group { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( knowledge_group_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_knowledge_group_creation()->mutable_knowledge_group()->set_id( 7 );
    content.mutable_knowledge_group_creation()->mutable_party()->set_id( 8 );
    content.mutable_knowledge_group_creation()->mutable_parent()->set_id( 9 );
    content.mutable_knowledge_group_creation()->set_type( "type" );
    content.mutable_knowledge_group_creation()->set_jam( true );
    content.mutable_knowledge_group_creation()->set_name( "totogroup" );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { knowledge_group_creation { knowledge_group { id: 7 } party { id: 8 } parent { id: 9 } type: \"type\" jam: true name: \"totogroup\" } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( knowledge_group_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_knowledge_group_update()->mutable_knowledge_group()->set_id( 7 );
    content.mutable_knowledge_group_update()->mutable_party()->set_id( 8 );
    content.mutable_knowledge_group_update()->mutable_parent()->set_id( 9 );
    content.mutable_knowledge_group_update()->set_enabled( true );
    content.mutable_knowledge_group_update()->set_type( "type" );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { knowledge_group_update { knowledge_group { id: 7 } party { id: 8 } parent { id: 9 } enabled: true type: \"type\" } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( knowledge_group_update_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_knowledge_group_update_ack()->mutable_knowledge_group()->set_id( 7 );
    content.mutable_knowledge_group_update_ack()->set_error_code( sword::KnowledgeGroupAck::error_invalid_perception );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { knowledge_group_magic_action_ack { knowledge_group { id: 7 } error_code: error_invalid_type } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( knowledge_group_creation_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_knowledge_group_creation_ack()->mutable_knowledge_group()->set_id( 7 );
    content.mutable_knowledge_group_creation_ack()->set_error_code( sword::KnowledgeGroupAck::error_invalid_unit );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { knowledge_group_creation_ack { knowledge_group { id: 7 } error_code: error_invalid_unit } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( knowledge_group_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_knowledge_group_destruction()->mutable_knowledge_group()->set_id( 7 );
    content.mutable_knowledge_group_destruction()->mutable_party()->set_id( 8 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { knowledge_group_destruction { knowledge_group { id: 7 } party { id: 8 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( action_create_fire_order_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_action_create_fire_order_ack()->set_error_code( sword::ActionCreateFireOrderAck::error_invalid_target );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { action_create_fire_order_ack { error_code: error_invalid_target } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_global_weather_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_global_weather()->mutable_weather()->set_id( 7 );
    content.mutable_control_global_weather()->mutable_attributes()->set_temperature( 8 );
    content.mutable_control_global_weather()->mutable_attributes()->set_wind_speed( 9 );
    content.mutable_control_global_weather()->mutable_attributes()->mutable_wind_direction()->set_heading( 10 );
    content.mutable_control_global_weather()->mutable_attributes()->set_cloud_floor( 11 );
    content.mutable_control_global_weather()->mutable_attributes()->set_cloud_ceiling( 12 );
    content.mutable_control_global_weather()->mutable_attributes()->set_cloud_density( 13 );
    content.mutable_control_global_weather()->mutable_attributes()->set_precipitation( sword::WeatherAttributes::snow );
    content.mutable_control_global_weather()->mutable_attributes()->set_lighting( sword::WeatherAttributes::artificial_light );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_global_weather { weather { id: 7 } attributes { temperature: 8 wind_speed: 9 wind_direction { heading: 10 } cloud_floor: 11 cloud_ceiling: 12 cloud_density: 13 precipitation: neige lighting: eclairant } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_local_weather_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_local_weather_creation()->mutable_weather()->set_id( 7 );
    FillCoordLatLong( content.mutable_control_local_weather_creation()->mutable_top_left() );
    FillCoordLatLong( content.mutable_control_local_weather_creation()->mutable_bottom_right() );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_temperature( 8 );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_wind_speed( 9 );
    content.mutable_control_local_weather_creation()->mutable_attributes()->mutable_wind_direction()->set_heading( 10 );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_cloud_floor( 11 );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_cloud_ceiling( 12 );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_cloud_density( 13 );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_precipitation( sword::WeatherAttributes::snow );
    content.mutable_control_local_weather_creation()->mutable_attributes()->set_lighting( sword::WeatherAttributes::artificial_light );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_local_weather_creation { weather { id: 7 } top_left_coordinate { latitude: 17.23 longitude: 23.17 } bottom_right_coordinate { latitude: 17.23 longitude: 23.17 } attributes { temperature: 8 wind_speed: 9 wind_direction { heading: 10 } cloud_floor: 11 cloud_ceiling: 12 cloud_density: 13 precipitation: neige lighting: eclairant } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_local_weather_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_local_weather_destruction()->mutable_weather()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_local_weather_destruction { weather { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_list_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_list_ack();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_list_ack { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_list_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_list()->add_checkpoint( "checkpoint1" );
    content.mutable_control_checkpoint_list()->add_checkpoint( "checkpoint2" );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_list { checkpoint: \"checkpoint1\" checkpoint: \"checkpoint2\" } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( control_checkpoint_delete_ack_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_control_checkpoint_delete_ack();
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { control_checkpoint_delete_ack { } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( formation_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_formation_destruction()->mutable_formation()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { formation_destruction { formation { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( automat_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_automat_destruction()->mutable_automat()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { automat_destruction { automat { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( crowd_destruction_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_crowd_destruction()->mutable_crowd()->set_id( 7 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { crowd_destruction { crowd { id: 7 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

BOOST_FIXTURE_TEST_CASE( population_creation_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_population_creation()->mutable_id()->set_id( 7 );
    content.mutable_population_creation()->mutable_party()->set_id( 8 );
    content.mutable_population_creation()->mutable_type()->set_id( 9 );
    content.mutable_population_creation()->set_name( "name" );
    content.mutable_population_creation()->set_text( "text" );
    content.mutable_population_creation()->add_objects()->set_id( 27 );
    content.mutable_population_creation()->add_objects()->set_id( 28 );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { population_creation { id { id: 7 } party { id: 8 } type { id: 9 } name: \"name\" objects { id: 27 } objects { id: 28 } } }" ) );
    converter.ReceiveSimToClient( msg );
}

namespace
{
    template< typename S >
    void FillResourceSatisfaction( S* s )
    {
        s->mutable_resource()->set_id( 30 );
        s->set_value( 31.f );
    }
    template< typename S >
    void FillMotivationSatisfaction( S* s )
    {
        s->set_motivation( "office" );
        s->set_percentage( 41.f );
    }
    template< typename S >
    void FillSatisfaction( S* s )
    {
        FillResourceSatisfaction( s->add_resources() );
        FillResourceSatisfaction( s->add_resources() );
        FillMotivationSatisfaction( s->add_motivations() );
        FillMotivationSatisfaction( s->add_motivations() );
        s->set_lodging( 50.f );
        s->set_health( 50.f );
        s->set_safety( 50.f );
    }
    template< typename S >
    void FillUsageOccupation( S* s )
    {
        s->set_usage( "office" );
        s->set_number( 42 );
    }
    template< typename O >
    void FillBlockOccupation( O* o )
    {
        o->mutable_object()->set_id( 12 );
        FillUsageOccupation( o->add_persons() );
        FillUsageOccupation( o->add_persons() );
        o->set_alerted( true ); // $$$$ SBO 2011-02-04: in sword but not in shield
        o->set_confined( true ); // $$$$ BCI 2011-02-23: in sword but not in shield
        o->set_evacuated( true ); // $$$$ ABR 2011-03-25: in sword but not in shield
        o->set_angriness( 0.0f ); // $$$$ ABR 2011-03-25: in sword but not in shield
    }
}

BOOST_FIXTURE_TEST_CASE( population_update_to_client_is_converted, ContextFixture< sword::SimToClient > )
{
    content.mutable_population_update()->mutable_id()->set_id( 7 );
    content.mutable_population_update()->set_healthy( 8 );
    content.mutable_population_update()->set_wounded( 9 );
    content.mutable_population_update()->set_dead( 10 );
    FillPartyAdhesion( content.mutable_population_update()->mutable_adhesions()->add_adhesion() );
    FillPartyAdhesion( content.mutable_population_update()->mutable_adhesions()->add_adhesion() );
    FillSatisfaction( content.mutable_population_update()->mutable_satisfaction() );
    content.mutable_population_update()->set_motivation( "office" );
    FillBlockOccupation( content.mutable_population_update()->add_occupations() );
    FillBlockOccupation( content.mutable_population_update()->add_occupations() );
    FillExtension( content.mutable_population_update()->mutable_extension()->add_entries() );
    FillExtension( content.mutable_population_update()->mutable_extension()->add_entries() );
    MOCK_EXPECT( client, SendSimToClient ).once().with( constraint( msg, "context: 42 message { population_update { id { id: 7 } healthy: 8 wounded: 9 dead: 10 adhesions { adhesion { party { id: 20 } value: 21 } adhesion { party { id: 20 } value: 21 } } satisfaction { resources { resource { id: 30 } value: 31 } resources { resource { id: 30 } value: 31 } motivations { motivation: \"office\" percentage: 41 } motivations { motivation: \"office\" percentage: 41 } lodging: 50 health: 50 safety: 50 } motivation: \"office\" occupations { object { id: 12 } persons { usage: \"office\" number: 42 } persons { usage: \"office\" number: 42 } } occupations { object { id: 12 } persons { usage: \"office\" number: 42 } persons { usage: \"office\" number: 42 } } extension { entries { name: \"name2\" value: \"value\" } entries { name: \"name2\" value: \"value\" } } } }" ) );
    converter.ReceiveSimToClient( msg );
}
