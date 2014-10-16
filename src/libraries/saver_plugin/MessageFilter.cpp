// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MessageFilter.h"
#include "protocol/Protocol.h"

using namespace plugins::saver;

// -----------------------------------------------------------------------------
// Name: MessageFilter constructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
MessageFilter::MessageFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageFilter destructor
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
MessageFilter::~MessageFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::SimToClient& wrapper )
{
 if( wrapper.message().has_unit_magic_action_ack()
    || wrapper.message().has_object_magic_action_ack()
    || wrapper.message().has_crowd_magic_action_ack()
    || wrapper.message().has_control_information()
    || wrapper.message().has_control_begin_tick()
    || wrapper.message().has_control_end_tick()
    || wrapper.message().has_control_stop_ack()
    || wrapper.message().has_control_pause_ack()
    || wrapper.message().has_control_resume_ack()
    || wrapper.message().has_control_change_time_factor_ack()
    || wrapper.message().has_control_global_weather_ack()
    || wrapper.message().has_control_local_weather_ack()
    || wrapper.message().has_control_checkpoint_save_begin()
    || wrapper.message().has_control_checkpoint_save_end()
    || wrapper.message().has_control_checkpoint_set_frequency_ack()
    || wrapper.message().has_control_checkpoint_save_now_ack()
    || wrapper.message().has_control_send_current_state_begin()
    || wrapper.message().has_control_send_current_state_end()
    || wrapper.message().has_unit_pathfind()
    || wrapper.message().has_action_create_fire_order_ack()
    || wrapper.message().has_burning_cell_request_ack()
    || wrapper.message().has_compute_pathfind_ack()
    || wrapper.message().has_control_checkpoint_delete_ack()
    || wrapper.message().has_control_checkpoint_list_ack()
    || wrapper.message().has_control_date_time_change_ack()
    || wrapper.message().has_control_enable_vision_cones_ack()
    || wrapper.message().has_knowledge_group_creation_ack()
    || wrapper.message().has_knowledge_group_magic_action_ack()
    || wrapper.message().has_knowledge_group_update_ack()
    || wrapper.message().has_list_enabled_vision_cones_ack()
    || wrapper.message().has_magic_action_ack()
    || wrapper.message().has_segment_request_ack()
    || wrapper.message().has_unit_change_superior_ack()
    || wrapper.message().has_unit_creation_request_ack() )
        return false;
    if( wrapper.message().has_report() ||
        wrapper.message().has_invalidate_report() )
        return true;
    if( wrapper.message().has_trace() )            // $$$$ AGE 2007-04-13: délicat : pas de delete cr !
        return false;
    if( wrapper.message().has_decisional_state() )
        return true;
    if( wrapper.message().has_debug_points() ||
        wrapper.message().has_object_detection() ||
        wrapper.message().has_crowd_concentration_detection() ||
        wrapper.message().has_crowd_flow_detection() )
        return false;
    if( wrapper.message().has_unit_order() ||
        wrapper.message().has_automat_order() ||
        wrapper.message().has_crowd_order() )
        return true;
    if( wrapper.message().has_order_ack() ||
        wrapper.message().has_frag_order_ack() ||
        wrapper.message().has_set_automat_mode_ack() ||
        wrapper.message().has_change_diplomacy_ack() )
        return true;
    if( wrapper.message().has_automat_change_knowledge_group_ack() ||
        wrapper.message().has_automat_change_logistic_links_ack() ||
        wrapper.message().has_automat_change_superior_ack() )
        return false;
    if( wrapper.message().has_log_supply_push_flow_ack() ||
        wrapper.message().has_log_supply_pull_flow_ack() ||
        wrapper.message().has_log_supply_change_quotas_ack() ||
        wrapper.message().has_formation_creation() ||
        wrapper.message().has_formation_update() ||
        wrapper.message().has_knowledge_group_creation() ||
        wrapper.message().has_party_creation() ||
        wrapper.message().has_party_update() ||
        wrapper.message().has_automat_creation() ||
        wrapper.message().has_unit_creation() ||
        wrapper.message().has_unit_destruction() ||
        wrapper.message().has_change_diplomacy() ||
        wrapper.message().has_unit_change_superior() ||
        wrapper.message().has_automat_change_logistic_links() ||
        wrapper.message().has_automat_change_knowledge_group() ||
        wrapper.message().has_automat_change_superior() ||
        wrapper.message().has_unit_knowledge_creation() ||
        wrapper.message().has_unit_knowledge_destruction() ||
        wrapper.message().has_unit_detection() ||
        wrapper.message().has_unit_vision_cones() ||
        wrapper.message().has_start_unit_fire() ||
        wrapper.message().has_stop_unit_fire() ||
        wrapper.message().has_start_crowd_fire() ||
        wrapper.message().has_stop_crowd_fire() ||
        wrapper.message().has_explosion() ||
        wrapper.message().has_start_fire_effect() ||
        wrapper.message().has_stop_fire_effect() ||
        wrapper.message().has_object_creation() ||
        wrapper.message().has_object_destruction() ||
        wrapper.message().has_object_knowledge_creation() ||
        wrapper.message().has_object_knowledge_destruction() ||
        wrapper.message().has_log_medical_handling_creation() ||
        wrapper.message().has_log_medical_handling_update() ||
        wrapper.message().has_log_medical_handling_destruction() ||
        wrapper.message().has_log_medical_state() ||
        wrapper.message().has_log_maintenance_handling_creation() ||
        wrapper.message().has_log_maintenance_handling_update() ||
        wrapper.message().has_log_maintenance_handling_destruction() ||
        wrapper.message().has_log_maintenance_state() ||
        wrapper.message().has_log_supply_handling_creation() ||
        wrapper.message().has_log_supply_handling_update() ||
        wrapper.message().has_log_supply_handling_destruction() ||
        wrapper.message().has_log_supply_state() ||
        wrapper.message().has_log_supply_quotas() ||
        wrapper.message().has_crowd_creation() ||
        wrapper.message().has_population_creation() ||
        wrapper.message().has_crowd_concentration_creation() ||
        wrapper.message().has_crowd_concentration_destruction() ||
        wrapper.message().has_crowd_flow_creation() ||
        wrapper.message().has_crowd_flow_destruction() ||
        wrapper.message().has_crowd_knowledge_creation() ||
        wrapper.message().has_crowd_knowledge_destruction() ||
        wrapper.message().has_crowd_concentration_knowledge_creation() ||
        wrapper.message().has_crowd_concentration_knowledge_destruction() ||
        wrapper.message().has_crowd_flow_knowledge_creation() ||
        wrapper.message().has_crowd_flow_knowledge_destruction() ||
        wrapper.message().has_automat_attributes() ||
        wrapper.message().has_list_logistic_requests_ack() ||
        wrapper.message().has_logistic_history_ack() )
        return true;
    if( wrapper.message().has_unit_attributes() )
        return IsRelevant( wrapper.message().unit_attributes() );
    if( wrapper.message().has_unit_knowledge_update() )
        return IsRelevant( wrapper.message().unit_knowledge_update() );
    if( wrapper.message().has_object_update() )
        return IsRelevant( wrapper.message().object_update() );
    if( wrapper.message().has_object_knowledge_update() )
        return IsRelevant( wrapper.message().object_knowledge_update() );
    if( wrapper.message().has_crowd_update() )
        return IsRelevant( wrapper.message().crowd_update() );
    if( wrapper.message().has_crowd_concentration_update() )
        return IsRelevant( wrapper.message().crowd_concentration_update() );
    if( wrapper.message().has_crowd_flow_update() )
        return IsRelevant( wrapper.message().crowd_flow_update() );
    if( wrapper.message().has_crowd_knowledge_update() )
        return IsRelevant( wrapper.message().crowd_knowledge_update() );
    if( wrapper.message().has_crowd_concentration_knowledge_update() )
        return IsRelevant( wrapper.message().crowd_concentration_knowledge_update() );
    if( wrapper.message().has_crowd_flow_knowledge_update() )
        return IsRelevant( wrapper.message().crowd_flow_knowledge_update() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::UnitAttributes& message )
{
    return message.has_altitude()
        || message.has_meeting_engagement()
        || message.has_communications()
        || message.has_contamination_agents()
        || message.has_direction()
        || message.has_indirect_fire_availability()
        || message.has_equipment_dotations()
        || message.has_human_dotations()
//        || message.has_resource_dotations()
        || message.has_embarked()
        || message.has_protective_suits()
        || message.has_borrowed_equipments()
        || message.has_lent_equipments()
        || message.has_contamination_state()
        || message.has_installation()
        || message.has_operational_state()
        || message.has_raw_operational_state()
        || message.has_experience()
        || message.has_tiredness()
        || message.has_height()
        || message.has_stealth()
        || message.has_underground()
        || message.has_morale()
        || message.has_dead()
        || message.has_neutralized()
        || message.has_reinforced_unit()
        || message.has_transporting_unit()
        || message.has_reinforcements()
        || message.has_transported_units()
        || message.has_position()
        || message.has_new_posture()
        || message.has_old_posture()
//        || message.has_posture_transition()
        || message.has_prisoner()
        || message.has_radar_active()
        || message.has_force_ratio()
        || message.has_refugees_managed()
        || message.has_surrendered_unit()
        || message.has_roe()
        || message.has_roe_crowd()
        || message.has_radio_receiver_disabled()
        || message.has_radio_emitter_disabled()
        || message.has_transporters_available()
        || message.has_speed()
        || message.has_extension()
        || message.has_satisfaction()
        || message.has_critical_intelligence()
        || message.has_adhesions()
        || message.has_stress()
        || message.has_transported_crowd()
        || message.has_headquarters()
        || message.has_name()
    ;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::UnitKnowledgeUpdate& message )
{
    return    message.has_party()
           || message.has_direction()
           || message.has_operational_state()
           || message.has_identification_level()
           || message.has_max_identification_level()
           || message.has_dead()
           || message.has_command_post()
//           || message.has_oid()
//           || message.has_oid_groupe_possesseur()
           || message.has_perceptions()
//           || message.has_pertinence()
           || message.has_position()
           || message.has_prisoner()
           || message.has_refugees_managed()
           || message.has_surrendered_unit()
//           || message.has_speed()
           ;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::ObjectUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::ObjectKnowledgeUpdate& message )
{
    return  message.has_attributes()
         || message.has_perceiving_automats()
         || message.has_location()
         || message.has_knowledge()
         || message.has_perceived()
         || message.has_object()
//         || message.has_relevance()
         || message.has_party()
    ;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::CrowdUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::CrowdConcentrationUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::CrowdFlowUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::CrowdKnowledgeUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::CrowdConcentrationKnowledgeUpdate& message )
{
    return  message.has_attitude()
         || message.has_perceived()
         || message.has_dead()
         || message.has_alive()
//         || message.has_oid_concentration_reelle()
//         || message.has_oid_connaissance_concentration()
//         || message.has_oid_connaissance_crowd()
//         || message.has_oid_groupe_possesseur()
//         || message.has_pertinence()
    ;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const sword::CrowdFlowKnowledgeUpdate& )
{
    return true;
}
