// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MessageFilter.h"

using namespace dispatcher;

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
bool MessageFilter::IsRelevant( const ASN1T_MsgsSimToClient& message )
{
    switch( message.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_unit_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_object_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_population_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_control_information:
        case T_MsgsSimToClient_msg_msg_control_begin_tick:
        case T_MsgsSimToClient_msg_msg_control_end_tick:
        case T_MsgsSimToClient_msg_msg_control_stop_ack:
        case T_MsgsSimToClient_msg_msg_control_pause_ack:
        case T_MsgsSimToClient_msg_msg_control_resume_ack:
        case T_MsgsSimToClient_msg_msg_control_change_time_factor_ack:
        case T_MsgsSimToClient_msg_msg_control_global_meteo_ack:
        case T_MsgsSimToClient_msg_msg_control_local_meteo_ack:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_begin:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_end:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_set_frequency_ack:
        case T_MsgsSimToClient_msg_msg_control_checkpoint_save_now_ack:
        case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:
        case T_MsgsSimToClient_msg_msg_control_send_current_state_end:
        case T_MsgsSimToClient_msg_msg_unit_pathfind:    // $$$$ AGE 2007-04-13: 
            return false;
        case T_MsgsSimToClient_msg_msg_report:
        case T_MsgsSimToClient_msg_msg_invalidate_report:
            return true;
        case T_MsgsSimToClient_msg_msg_trace:            // $$$$ AGE 2007-04-13: délicat : pas de delete cr !
            return false;
        case T_MsgsSimToClient_msg_msg_decisional_state:
            return true;
        case T_MsgsSimToClient_msg_msg_debug_points:
        case T_MsgsSimToClient_msg_msg_unit_vision_cones:
        case T_MsgsSimToClient_msg_msg_unit_detection:
        case T_MsgsSimToClient_msg_msg_object_detection:
        case T_MsgsSimToClient_msg_msg_population_concentration_detection:
        case T_MsgsSimToClient_msg_msg_population_flow_detection:
            return false;
        case T_MsgsSimToClient_msg_msg_unit_order:
        case T_MsgsSimToClient_msg_msg_automat_order:
        case T_MsgsSimToClient_msg_msg_population_order:
            return true;
        case T_MsgsSimToClient_msg_msg_unit_order_ack:
        case T_MsgsSimToClient_msg_msg_automat_order_ack:
        case T_MsgsSimToClient_msg_msg_population_order_ack:
        case T_MsgsSimToClient_msg_msg_frag_order_ack:
        case T_MsgsSimToClient_msg_msg_set_automat_mode_ack:
        case T_MsgsSimToClient_msg_msg_change_diplomacy_ack:
            return true;
        case T_MsgsSimToClient_msg_msg_automat_change_knowledge_group_ack:
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links_ack:
        case T_MsgsSimToClient_msg_msg_automat_change_superior_ack:
            return false;
        case T_MsgsSimToClient_msg_msg_log_supply_push_flow_ack:
        case T_MsgsSimToClient_msg_msg_log_supply_change_quotas_ack:
        case T_MsgsSimToClient_msg_msg_formation_creation:
        case T_MsgsSimToClient_msg_msg_knowledge_group_creation:
        case T_MsgsSimToClient_msg_msg_side_creation:
        case T_MsgsSimToClient_msg_msg_automat_creation:
        case T_MsgsSimToClient_msg_msg_unit_creation:
        case T_MsgsSimToClient_msg_msg_change_diplomacy:
        case T_MsgsSimToClient_msg_msg_unit_change_superior:
        case T_MsgsSimToClient_msg_msg_automat_change_logistic_links:
        case T_MsgsSimToClient_msg_msg_automat_change_knowledge_group:
        case T_MsgsSimToClient_msg_msg_automat_change_superior:
        case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_start_unit_fire:
        case T_MsgsSimToClient_msg_msg_stop_unit_fire:
        case T_MsgsSimToClient_msg_msg_start_population_fire:
        case T_MsgsSimToClient_msg_msg_stop_population_fire:
        case T_MsgsSimToClient_msg_msg_explosion:
        case T_MsgsSimToClient_msg_msg_start_fire_effect:
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:
        case T_MsgsSimToClient_msg_msg_object_creation:
        case T_MsgsSimToClient_msg_msg_object_destruction:
        case T_MsgsSimToClient_msg_msg_object_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_log_medical_handling_creation:
        case T_MsgsSimToClient_msg_msg_log_medical_handling_update:
        case T_MsgsSimToClient_msg_msg_log_medical_handling_destruction:
        case T_MsgsSimToClient_msg_msg_log_medical_state:
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_creation:
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_update:
        case T_MsgsSimToClient_msg_msg_log_maintenance_handling_destruction:
        case T_MsgsSimToClient_msg_msg_log_maintenance_state:
        case T_MsgsSimToClient_msg_msg_log_supply_handling_creation:
        case T_MsgsSimToClient_msg_msg_log_supply_handling_update:
        case T_MsgsSimToClient_msg_msg_log_supply_handling_destruction:
        case T_MsgsSimToClient_msg_msg_log_supply_state:
        case T_MsgsSimToClient_msg_msg_log_supply_quotas:
        case T_MsgsSimToClient_msg_msg_population_creation:
        case T_MsgsSimToClient_msg_msg_population_concentration_creation:
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction:
        case T_MsgsSimToClient_msg_msg_population_flow_creation:
        case T_MsgsSimToClient_msg_msg_population_flow_destruction:
        case T_MsgsSimToClient_msg_msg_population_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_automat_attributes:
            return true;
        case T_MsgsSimToClient_msg_msg_unit_attributes:
            return IsRelevant( *message.msg.u.msg_unit_attributes );
        case T_MsgsSimToClient_msg_msg_unit_knowledge_update:
            return IsRelevant( *message.msg.u.msg_unit_knowledge_update );
        case T_MsgsSimToClient_msg_msg_object_update:
            return IsRelevant( *message.msg.u.msg_object_update );
        case T_MsgsSimToClient_msg_msg_object_knowledge_update:
            return IsRelevant( *message.msg.u.msg_object_knowledge_update );
        case T_MsgsSimToClient_msg_msg_population_update:
            return IsRelevant( *message.msg.u.msg_population_update );
        case T_MsgsSimToClient_msg_msg_population_concentration_update:
            return IsRelevant( *message.msg.u.msg_population_concentration_update );
        case T_MsgsSimToClient_msg_msg_population_flow_update:
            return IsRelevant( *message.msg.u.msg_population_flow_update );
        case T_MsgsSimToClient_msg_msg_population_knowledge_update:
            return IsRelevant( *message.msg.u.msg_population_knowledge_update );
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update:
            return IsRelevant( *message.msg.u.msg_population_concentration_knowledge_update );
        case T_MsgsSimToClient_msg_msg_population_flow_knowledge_update:
            return IsRelevant( *message.msg.u.msg_population_flow_knowledge_update );
        default:
            return true;
    }
}

namespace
{
    template< typename T >
    inline
    bool IsNotUseless( const T& message, const T& mask )
    {
        int lhs = *reinterpret_cast< const int* >( &message );
        int rhs = *reinterpret_cast< const int* >( &mask    );
        return ( lhs & rhs ) != lhs;
    }
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgUnitAttributes& message )
{
    ASN1T_MsgUnitAttributes useless;
    useless.m.posture_pourcentagePresent = 1;
    return IsNotUseless( message.m, useless.m );
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    ASN1T_MsgUnitKnowledgeUpdate useless;
    useless.m.pertinencePresent = 1;
    useless.m.speedPresent      = 1;
    return IsNotUseless( message.m, useless.m );
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgObjectUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    ASN1T_MsgObjectKnowledgeUpdate useless;
    useless.m.relevancePresent = 1;
    return IsNotUseless( message.m, useless.m );
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationConcentrationUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationFlowUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationKnowledgeUpdate& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    ASN1T_MsgPopulationConcentrationKnowledgeUpdate useless;
    useless.m.pertinencePresent = 1;
    return IsNotUseless( message.m, useless.m );
}

// -----------------------------------------------------------------------------
// Name: MessageFilter::IsRelevant
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationFlowKnowledgeUpdate& )
{
    return true;
}
