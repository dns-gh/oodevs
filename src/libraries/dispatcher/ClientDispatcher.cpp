// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ClientDispatcher.h"
#include "SimulationNetworker.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ClientDispatcher constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ClientDispatcher::ClientDispatcher( SimulationNetworker& simu )
    : simu_( simu )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientDispatcher destructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ClientDispatcher::~ClientDispatcher()
{
    // NOTHING
}


#define DISPATCH_ASN_MSG( NAME )                                 \
    case T_MsgsOutClient_msg_msg_##NAME:                         \
    {                                                            \
        ASN1T_MsgsInSim asnOutMsg;                               \
        asnOutMsg.context          = asnInMsg.context;           \
        asnOutMsg.msg.t            = T_MsgsInSim_msg_msg_##NAME; \
        asnOutMsg.msg.u.msg_##NAME = asnInMsg.msg.u.msg_##NAME;  \
        simu_.Send( asnOutMsg );                                 \
        break;                                                   \
    } 

#define DISPATCH_EMPTY_ASN_MSG( NAME )                           \
    case T_MsgsOutClient_msg_msg_##NAME:                         \
    {                                                            \
        ASN1T_MsgsInSim asnOutMsg;                               \
        asnOutMsg.context          = asnInMsg.context;           \
        asnOutMsg.msg.t            = T_MsgsInSim_msg_msg_##NAME; \
        simu_.Send( asnOutMsg );                                 \
        break;                                                   \
    } 


// -----------------------------------------------------------------------------
// Name: ClientDispatcher::OnReceive
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
void ClientDispatcher::OnReceive( const ASN1T_MsgsOutClient& asnInMsg )
{
    switch( asnInMsg.msg.t )
    {
        DISPATCH_EMPTY_ASN_MSG( ctrl_stop );
        DISPATCH_EMPTY_ASN_MSG( ctrl_pause );
        DISPATCH_EMPTY_ASN_MSG( ctrl_resume );
        DISPATCH_ASN_MSG      ( ctrl_change_time_factor );
        DISPATCH_ASN_MSG      ( ctrl_meteo_globale );
        DISPATCH_ASN_MSG      ( ctrl_meteo_locale );
        DISPATCH_ASN_MSG      ( ctrl_checkpoint_save_now );
        DISPATCH_ASN_MSG      ( ctrl_checkpoint_set_frequency );
        DISPATCH_ASN_MSG      ( limit_creation_request );
        DISPATCH_ASN_MSG      ( limit_destruction_request );
        DISPATCH_ASN_MSG      ( limit_update_request );
        DISPATCH_ASN_MSG      ( lima_creation_request );
        DISPATCH_ASN_MSG      ( lima_destruction_request );
        DISPATCH_ASN_MSG      ( lima_update_request );
        DISPATCH_ASN_MSG      ( pion_order );
        DISPATCH_ASN_MSG      ( automate_order);
        DISPATCH_ASN_MSG      ( population_order );
        DISPATCH_ASN_MSG      ( frag_order );
        DISPATCH_ASN_MSG      ( set_automate_mode );
        DISPATCH_ASN_MSG      ( unit_magic_action );
        DISPATCH_ASN_MSG      ( object_magic_action );
        DISPATCH_ASN_MSG      ( population_magic_action );
        DISPATCH_ASN_MSG      ( change_diplomatie );
        DISPATCH_ASN_MSG      ( automate_change_groupe_connaissance );
        DISPATCH_ASN_MSG      ( automate_change_liens_logistiques );
        DISPATCH_ASN_MSG      ( pion_change_superior );
        DISPATCH_ASN_MSG      ( log_ravitaillement_pousser_flux );
        DISPATCH_ASN_MSG      ( log_ravitaillement_change_quotas );
    }
}
