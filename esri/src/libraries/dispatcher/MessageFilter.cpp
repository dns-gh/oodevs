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
        case T_MsgsSimToClient_msg_msg_limit_creation_request_ack:
        case T_MsgsSimToClient_msg_msg_limit_destruction_request_ack:
        case T_MsgsSimToClient_msg_msg_limit_update_request_ack:
        case T_MsgsSimToClient_msg_msg_lima_creation_request_ack:
        case T_MsgsSimToClient_msg_msg_lima_destruction_request_ack:
        case T_MsgsSimToClient_msg_msg_lima_update_request_ack:
        case T_MsgsSimToClient_msg_msg_unit_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_object_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_population_magic_action_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_info:
        case T_MsgsSimToClient_msg_msg_ctrl_begin_tick:
        case T_MsgsSimToClient_msg_msg_ctrl_end_tick:
        case T_MsgsSimToClient_msg_msg_ctrl_stop_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_pause_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_resume_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_change_time_factor_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_meteo_globale_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_meteo_locale_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_begin:
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_end:
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_set_frequency_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_checkpoint_save_now_ack:
        case T_MsgsSimToClient_msg_msg_ctrl_send_current_state_begin:
        case T_MsgsSimToClient_msg_msg_ctrl_send_current_state_end:
        case T_MsgsSimToClient_msg_msg_unit_pathfind: // $$$$ AGE 2007-04-13: 
        case T_MsgsSimToClient_msg_msg_cr:            // $$$$ AGE 2007-04-13: délicat : pas de delete cr !
        case T_MsgsSimToClient_msg_msg_pion_order:       // $$$$ AGE 2007-04-13: délicat : état à sauvegarder
        case T_MsgsSimToClient_msg_msg_automate_order:   // $$$$ AGE 2007-04-13: délicat : état à sauvegarder
        case T_MsgsSimToClient_msg_msg_population_order: // $$$$ AGE 2007-04-13: délicat : état à sauvegarder
            return false;
        case T_MsgsSimToClient_msg_msg_pion_order_ack:
        case T_MsgsSimToClient_msg_msg_automate_order_ack:
        case T_MsgsSimToClient_msg_msg_population_order_ack:
        case T_MsgsSimToClient_msg_msg_frag_order_ack:
        case T_MsgsSimToClient_msg_msg_set_automate_mode_ack:
        case T_MsgsSimToClient_msg_msg_change_diplomatie_ack:
        case T_MsgsSimToClient_msg_msg_automate_change_groupe_connaissance_ack:
        case T_MsgsSimToClient_msg_msg_automate_change_liens_logistiques_ack:
        case T_MsgsSimToClient_msg_msg_pion_change_superior_ack:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_pousser_flux_ack:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_change_quotas_ack:
        case T_MsgsSimToClient_msg_msg_limit_creation:
        case T_MsgsSimToClient_msg_msg_limit_update:
        case T_MsgsSimToClient_msg_msg_limit_destruction:
        case T_MsgsSimToClient_msg_msg_lima_creation:
        case T_MsgsSimToClient_msg_msg_lima_update:
        case T_MsgsSimToClient_msg_msg_lima_destruction:
        case T_MsgsSimToClient_msg_msg_formation_creation:
        case T_MsgsSimToClient_msg_msg_knowledge_group_creation:
        case T_MsgsSimToClient_msg_msg_side_creation:
        case T_MsgsSimToClient_msg_msg_automate_creation:
        case T_MsgsSimToClient_msg_msg_pion_creation:
        case T_MsgsSimToClient_msg_msg_change_diplomatie:
        case T_MsgsSimToClient_msg_msg_pion_change_superior:
        case T_MsgsSimToClient_msg_msg_automate_change_liens_logistiques:
        case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_start_pion_fire:
        case T_MsgsSimToClient_msg_msg_stop_pion_fire:
        case T_MsgsSimToClient_msg_msg_start_population_fire:
        case T_MsgsSimToClient_msg_msg_stop_population_fire:
        case T_MsgsSimToClient_msg_msg_explosion:
        case T_MsgsSimToClient_msg_msg_start_fire_effect:
        case T_MsgsSimToClient_msg_msg_stop_fire_effect:
        case T_MsgsSimToClient_msg_msg_object_creation:
        case T_MsgsSimToClient_msg_msg_object_destruction:
        case T_MsgsSimToClient_msg_msg_object_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_object_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_creation:
        case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_update:
        case T_MsgsSimToClient_msg_msg_log_sante_traitement_humain_destruction:
        case T_MsgsSimToClient_msg_msg_log_sante_etat:
        case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_creation:
        case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_update:
        case T_MsgsSimToClient_msg_msg_log_maintenance_traitement_equipement_destruction:
        case T_MsgsSimToClient_msg_msg_log_maintenance_etat:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_creation:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_update:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_traitement_destruction:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_etat:
        case T_MsgsSimToClient_msg_msg_log_ravitaillement_quotas:
        case T_MsgsSimToClient_msg_msg_population_creation:
        case T_MsgsSimToClient_msg_msg_population_concentration_creation:
        case T_MsgsSimToClient_msg_msg_population_concentration_destruction:
        case T_MsgsSimToClient_msg_msg_population_flux_creation:
        case T_MsgsSimToClient_msg_msg_population_flux_destruction:
        case T_MsgsSimToClient_msg_msg_population_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_population_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_creation:
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_destruction:
        case T_MsgsSimToClient_msg_msg_automate_attributes:
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
        case T_MsgsSimToClient_msg_msg_population_flux_update:
            return IsRelevant( *message.msg.u.msg_population_flux_update );
        case T_MsgsSimToClient_msg_msg_population_knowledge_update:
            return IsRelevant( *message.msg.u.msg_population_knowledge_update );
        case T_MsgsSimToClient_msg_msg_population_concentration_knowledge_update:
            return IsRelevant( *message.msg.u.msg_population_concentration_knowledge_update );
        case T_MsgsSimToClient_msg_msg_population_flux_knowledge_update:
            return IsRelevant( *message.msg.u.msg_population_flux_knowledge_update );
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
    useless.m.pertinencePresent = 1;
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
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationFluxUpdate& )
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
bool MessageFilter::IsRelevant( const ASN1T_MsgPopulationFluxKnowledgeUpdate& )
{
    return true;
}
