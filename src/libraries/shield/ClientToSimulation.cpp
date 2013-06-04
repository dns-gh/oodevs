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
void ClientToSimulation::Convert( const Common::MsgControlResume& from, sword::ControlResume* to )
{
    CONVERT( tick );
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
    ConvertOrder( from, to, false );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgAutomatOrder& from, sword::AutomatOrder* to )
{
    ConvertOrder( from, to, false );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgCrowdOrder& from, sword::CrowdOrder* to )
{
    ConvertOrder( from, to, false );
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
void ClientToSimulation::Convert( const MsgsClientToSim::MsgUnitMagicAction& from, sword::UnitMagicAction* to )
{
    CONVERT_CB( tasker, ConvertTasker );
    CONVERT_ENUM( type, ( Common::move_to, sword::move_to )
                        ( Common::surrender_to, sword::surrender_to )
                        ( Common::cancel_surrender, sword::cancel_surrender )
                        ( Common::recover_transporters, sword::recover_transporters )
                        ( Common::destroy_component, sword::destroy_component )
                        ( Common::recover_all, sword::recover_all )
                        ( Common::recover_troops, sword::recover_troops )
                        ( Common::recover_equipments, sword::recover_equipments )
                        ( Common::recover_resources, sword::recover_resources )
                        ( Common::recover_all_except_log, sword::recover_all_except_log )
                        ( Common::recover_troops_except_log, sword::recover_troops_except_log )
                        ( Common::recover_equipment_except_log, sword::recover_equipments_except_log )
                        ( Common::recover_resources_except_log, sword::recover_resources_except_log )
                        ( Common::destroy_all, sword::destroy_all )
                        ( Common::change_human_factors, sword::change_human_factors )
                        ( Common::partial_recovery, sword::partial_recovery )
                        ( Common::unit_creation, sword::unit_creation )
                        ( Common::create_fire_order, sword::create_fire_order )
                        ( Common::crowd_total_destruction, sword::crowd_total_destruction )
                        ( Common::crowd_change_health_state, sword::crowd_change_health_state )
                        ( Common::crowd_change_attitude, sword::crowd_change_attitude )
                        ( Common::crowd_change_affinities, sword::crowd_change_affinities )
                        ( Common::crowd_change_armed_individuals, sword::crowd_change_armed_individuals )
                        ( Common::change_knowledge_group, sword::change_knowledge_group )
                        ( Common::change_logistic_links, sword::change_logistic_links )
                        ( Common::unit_change_superior, sword::unit_change_superior )
                        ( Common::change_automat_superior, sword::change_automat_superior )
                        ( Common::change_formation_superior, sword::change_formation_superior )
                        ( Common::knowledge_group_update, sword::knowledge_group_update )
                        ( Common::log_supply_push_flow, sword::log_supply_push_flow )
                        ( Common::log_supply_change_quotas, sword::log_supply_change_quotas )
                        ( Common::automat_creation, sword::automat_creation )
                        ( Common::formation_creation, sword::formation_creation )
                        ( Common::crowd_creation, sword::crowd_creation )
                        ( Common::log_supply_pull_flow, sword::log_supply_pull_flow )
                        ( Common::inhabitant_change_health_state, sword::inhabitant_change_health_state )
                        ( Common::inhabitant_change_affinities, sword::inhabitant_change_affinities )
                        ( Common::unit_change_affinities, sword::unit_change_affinities )
                        ( Common::change_extension, sword::change_extension )
                        ( Common::change_critical_intelligence, sword::change_critical_intelligence )
                        ( Common::transfer_equipment, sword::transfer_equipment )
                        ( Common::change_equipment_human_size, sword::change_equipment_human_size )
                        ( Common::create_breakdowns, sword::create_breakdowns )
                        ( Common::create_wounds, sword::create_wounds )
                        ( Common::change_symbol, sword::change_symbol ) );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgObjectMagicAction& from, sword::ObjectMagicAction* to )
{
    CONVERT_ID( object );
    CONVERT_ENUM( type, ( Common::create, sword::create )
                        ( Common::update, sword::update )
                        ( Common::destroy, sword::destroy ) );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgKnowledgeMagicAction& from, sword::KnowledgeMagicAction* to )
{
    CONVERT_ID( knowledge_group );
    CONVERT_ENUM( type, ( Common::enable, sword::enable )
                        ( Common::update_party, sword::update_party )
                        ( Common::update_party_parent, sword::update_party_parent )
                        ( Common::update_type, sword::update_type ) );
    CONVERT_LIST( parameters, elem, ConvertMissionParameter );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: MCO 2010-11-25
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgMagicAction& from, sword::MagicAction* to )
{
    CONVERT_ENUM( type, ( Common::global_weather_type, sword::global_weather_type )
                        ( Common::local_weather, sword::local_weather )
                        ( Common::local_weather_destruction, sword::local_weather_destruction )
                        ( Common::change_diplomacy, sword::change_diplomacy )
                        ( Common::create_knowledge_group, sword::create_knowledge_group )
                        ( Common::change_resource_network_properties, sword::change_resource_network_properties )
                        ( Common::create_fire_order_on_location, sword::create_fire_order_on_location ) );
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

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: LGY 2011-09-02
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const MsgsClientToSim::MsgOrderStream& from, sword::OrderStream* to )
{
    CONVERT( serialized_order );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: LDC 2012-02-15
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgControlExportRequest& from, sword::ControlExportRequest* to )
{
    CONVERT( directory_name );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: JSR 2012-08-06
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgControlExportRequestAck& from, sword::ControlExportRequestAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ControlExportRequestAck::success, Common::MsgControlExportRequestAck::success )
                              ( sword::ControlExportRequestAck::failure, Common::MsgControlExportRequestAck::failure ) );
    CONVERT( directory_name );
}

// -----------------------------------------------------------------------------
// Name: ClientToSimulation::Convert
// Created: LDC 2012-12-14
// -----------------------------------------------------------------------------
void ClientToSimulation::Convert( const Common::MsgLogHistoryRequestForPlay& from, sword::LogHistoryRequestForPlay* to )
{
    CONVERT( exercise );
    CONVERT( profile );
    CONVERT_DATE( date_time );
    CONVERT( session );
}
