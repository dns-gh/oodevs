// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ClientToSimulation.h"
#include "ClientTools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgControlChangeTimeFactor& from, sword::ControlChangeTimeFactor* to )
{
    CONVERT( time_factor );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgControlStop& /*from*/, sword::ControlStop* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgControlPause& /*from*/, sword::ControlPause* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgControlResume& /*from*/, sword::ControlResume* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgControlDatetimeChange& from, sword::ControlDateTimeChange* to )
{
    CONVERT_DATE( date_time );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgControlCheckPointSaveNow& from, sword::ControlCheckPointSaveNow* to )
{
    CONVERT( name );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgControlCheckPointSetFrequency& from, sword::ControlCheckPointSetFrequency* to )
{
    CONVERT( frequency );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgControlEnableVisionCones& from, sword::ControlEnableVisionCones* to )
{
    CONVERT( vision_cones );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgUnitOrder& from, sword::UnitOrder* to )
{
    ConvertOrder( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgAutomatOrder& from, sword::AutomatOrder* to )
{
    ConvertOrder( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgCrowdOrder& from, sword::CrowdOrder* to )
{
    ConvertOrder( from, to );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgFragOrder& from, sword::FragOrder* to )
{
    CONVERT_CB( tasker, ConvertTasker );
    if( from.has_frag_order() && from.frag_order().has_id() )
        to->mutable_type()->set_id( from.frag_order().id() );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgSetAutomatMode& from, sword::SetAutomatMode* to )
{
    CONVERT_ID( automate );
    CONVERT_AUTOMAT_MODE( mode, mode );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgUnitCreationRequest& from, sword::UnitCreationRequest* to )
{
    CONVERT_ID( type );
    CONVERT_ID( superior );
    CONVERT_CB( position, ConvertCoordLatLong );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgUnitMagicAction& from, sword::UnitMagicAction* to )
{
    CONVERT_CB( tasker, ConvertTasker );
    CONVERT_ENUM( type, ( MsgsClientToSim::MsgUnitMagicAction::move_to, sword::UnitMagicAction::move_to )
                        ( MsgsClientToSim::MsgUnitMagicAction::surrender_to, sword::UnitMagicAction::surrender_to )
                        ( MsgsClientToSim::MsgUnitMagicAction::cancel_surrender, sword::UnitMagicAction::cancel_surrender )
                        ( MsgsClientToSim::MsgUnitMagicAction::recover_transporters, sword::UnitMagicAction::recover_transporters )
                        ( MsgsClientToSim::MsgUnitMagicAction::destroy_component, sword::UnitMagicAction::destroy_component )
                        ( MsgsClientToSim::MsgUnitMagicAction::recover_all, sword::UnitMagicAction::recover_all )
                        ( MsgsClientToSim::MsgUnitMagicAction::recover_troops, sword::UnitMagicAction::recover_troops )
                        ( MsgsClientToSim::MsgUnitMagicAction::recover_equipments, sword::UnitMagicAction::recover_equipments )
                        ( MsgsClientToSim::MsgUnitMagicAction::recover_resources, sword::UnitMagicAction::recover_resources )
                        ( MsgsClientToSim::MsgUnitMagicAction::destroy_all, sword::UnitMagicAction::destroy_all )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_human_factors, sword::UnitMagicAction::change_human_factors )
                        ( MsgsClientToSim::MsgUnitMagicAction::partial_recovery, sword::UnitMagicAction::partial_recovery )
                        ( MsgsClientToSim::MsgUnitMagicAction::unit_creation, sword::UnitMagicAction::unit_creation )
                        ( MsgsClientToSim::MsgUnitMagicAction::create_fire_order, sword::UnitMagicAction::create_fire_order )
                        ( MsgsClientToSim::MsgUnitMagicAction::crowd_total_destruction, sword::UnitMagicAction::crowd_total_destruction )
                        ( MsgsClientToSim::MsgUnitMagicAction::crowd_change_health_state, sword::UnitMagicAction::crowd_change_health_state )
                        ( MsgsClientToSim::MsgUnitMagicAction::crowd_change_attitude, sword::UnitMagicAction::crowd_change_attitude )
                        ( MsgsClientToSim::MsgUnitMagicAction::crowd_change_affinities, sword::UnitMagicAction::crowd_change_affinities )
                        ( MsgsClientToSim::MsgUnitMagicAction::crowd_change_armed_individuals, sword::UnitMagicAction::crowd_change_armed_individuals )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_knowledge_group, sword::UnitMagicAction::change_knowledge_group )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_logistic_links, sword::UnitMagicAction::change_logistic_links )
                        ( MsgsClientToSim::MsgUnitMagicAction::unit_change_superior, sword::UnitMagicAction::unit_change_superior )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_automat_superior, sword::UnitMagicAction::change_automat_superior )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_formation_superior, sword::UnitMagicAction::change_formation_superior )
                        ( MsgsClientToSim::MsgUnitMagicAction::knowledge_group_update, sword::UnitMagicAction::knowledge_group_update )
                        ( MsgsClientToSim::MsgUnitMagicAction::log_supply_push_flow, sword::UnitMagicAction::log_supply_push_flow )
                        ( MsgsClientToSim::MsgUnitMagicAction::log_supply_change_quotas, sword::UnitMagicAction::log_supply_change_quotas )
                        ( MsgsClientToSim::MsgUnitMagicAction::automat_creation, sword::UnitMagicAction::automat_creation )
                        ( MsgsClientToSim::MsgUnitMagicAction::formation_creation, sword::UnitMagicAction::formation_creation )
                        ( MsgsClientToSim::MsgUnitMagicAction::crowd_creation, sword::UnitMagicAction::crowd_creation )
                        ( MsgsClientToSim::MsgUnitMagicAction::log_supply_pull_flow, sword::UnitMagicAction::log_supply_pull_flow )
                        ( MsgsClientToSim::MsgUnitMagicAction::create_wound, sword::UnitMagicAction::create_wound )
                        ( MsgsClientToSim::MsgUnitMagicAction::inhabitant_change_health_state, sword::UnitMagicAction::inhabitant_change_health_state )
                        ( MsgsClientToSim::MsgUnitMagicAction::inhabitant_change_affinities, sword::UnitMagicAction::inhabitant_change_affinities )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_extension, sword::UnitMagicAction::change_extension )
                        ( MsgsClientToSim::MsgUnitMagicAction::change_critical_intelligence, sword::UnitMagicAction::change_critical_intelligence )
                        ( MsgsClientToSim::MsgUnitMagicAction::unit_creation, sword::UnitMagicAction::unit_creation ) );
    switch( from.type() )
    {
        case MsgsClientToSim::MsgUnitMagicAction::change_logistic_links:    ConvertUnitMagicActionChangeLogisticLinks  ( from.parameters(), *to->mutable_parameters() ); break;
        case MsgsClientToSim::MsgUnitMagicAction::log_supply_change_quotas: ConvertUnitMagicActionLogSupplyChangeQuotas( from.parameters(), *to->mutable_parameters() ); break;
        default:
            CONVERT_LIST( parameters, elem, ConvertMissionParameter );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: NLD 2011-02-08
// -----------------------------------------------------------------------------
void ClientToSimulation::ConvertUnitMagicActionChangeLogisticLinks( const Common::MsgMissionParameters& /*from*/, sword::MissionParameters& /*to*/ )
{
    //$$ NLD TO BE DONE if useful: Thales doesn't have the original message "documentation" and probably hasn't implemented it
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: NLD 2011-02-08
// -----------------------------------------------------------------------------
void ClientToSimulation::ConvertUnitMagicActionLogSupplyChangeQuotas( const Common::MsgMissionParameters& /*from*/, sword::MissionParameters& /*to*/ )
{
    //$$ NLD TO BE DONE if useful: Thales doesn't have the original message "documentation" and probably hasn't implemented it
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgObjectMagicAction& from, sword::ObjectMagicAction* to )
{
    CONVERT_ID( object );
    CONVERT_ENUM( type, ( MsgsClientToSim::MsgObjectMagicAction::create, sword::ObjectMagicAction::create )
                        ( MsgsClientToSim::MsgObjectMagicAction::update, sword::ObjectMagicAction::update )
                                  ( MsgsClientToSim::MsgObjectMagicAction::destroy, sword::ObjectMagicAction::destroy ) );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgKnowledgeMagicAction& from, sword::KnowledgeMagicAction* to )
{
    CONVERT_ID( knowledge_group );
    CONVERT_ENUM( type, ( MsgsClientToSim::MsgKnowledgeMagicAction::enable, sword::KnowledgeMagicAction::enable )
                        ( MsgsClientToSim::MsgKnowledgeMagicAction::update_party, sword::KnowledgeMagicAction::update_party )
                        ( MsgsClientToSim::MsgKnowledgeMagicAction::update_party_parent, sword::KnowledgeMagicAction::update_party_parent )
                        ( MsgsClientToSim::MsgKnowledgeMagicAction::update_type, sword::KnowledgeMagicAction::update_type ) );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgMagicAction& from, sword::MagicAction* to )
{
    CONVERT_ENUM( type, ( MsgsClientToSim::MsgMagicAction::global_weather, sword::MagicAction::global_weather )
                        ( MsgsClientToSim::MsgMagicAction::local_weather, sword::MagicAction::local_weather )
                        ( MsgsClientToSim::MsgMagicAction::change_diplomacy, sword::MagicAction::change_diplomacy )
                        ( MsgsClientToSim::MsgMagicAction::create_knowledge_group, sword::MagicAction::create_knowledge_group )
                        ( MsgsClientToSim::MsgMagicAction::change_resource_network_properties, sword::MagicAction::change_resource_network_properties )
                        ( MsgsClientToSim::MsgMagicAction::create_fire_order_on_location, sword::MagicAction::create_fire_order_on_location ) );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgControlCheckPointListRequest& /*from*/, sword::ControlCheckPointListRequest* /*to*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgControlCheckPointDeleteRequest& from, sword::ControlCheckPointDeleteRequest* to )
{
    CONVERT( checkpoint );
}
