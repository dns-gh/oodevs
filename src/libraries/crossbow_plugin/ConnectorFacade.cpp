// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ConnectorFacade.h"
#include "Connector.h"
#include "AgentTypes.h"
#include "OrderTypes.h"
#include "OrderListener.h"
#include "StatusListener.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include "dispatcher/Visitors.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: ConnectorFacade constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ConnectorFacade::ConnectorFacade( const dispatcher::Model& model, const dispatcher::Config& config, dispatcher::Publisher_ABC& simulation )
    : types_     ( new kernel::AgentTypes( config ) )
    , orderTypes_( new kernel::OrderTypes( config ) )
    , connector_ ( new Connector( config, *types_, *types_,  model ) )
    , bLoaded_   ( false )
    , simulation_( simulation )
{
    listeners_.push_back( T_SharedListener( new OrderListener( *connector_, *orderTypes_, model ) ) );
    listeners_.push_back( T_SharedListener( new StatusListener( *connector_) ) );
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ConnectorFacade::~ConnectorFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::IsRelevant
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
bool ConnectorFacade::IsRelevant( const ASN1T_MsgsSimToClient& asn ) const
{
    bool relevant = false;
    switch ( asn.msg.t )
    {
    case T_MsgsSimToClient_msg_msg_lima_creation:
    case T_MsgsSimToClient_msg_msg_lima_destruction:
    case T_MsgsSimToClient_msg_msg_limit_creation:
    case T_MsgsSimToClient_msg_msg_limit_destruction:
    case T_MsgsSimToClient_msg_msg_object_creation:
    case T_MsgsSimToClient_msg_msg_object_update:
    case T_MsgsSimToClient_msg_msg_object_destruction:
    case T_MsgsSimToClient_msg_msg_formation_creation:
    case T_MsgsSimToClient_msg_msg_automat_creation:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_creation:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_update:
    case T_MsgsSimToClient_msg_msg_unit_knowledge_destruction:
        relevant = true;
        break;
    case T_MsgsSimToClient_msg_msg_unit_creation:
    case T_MsgsSimToClient_msg_msg_unit_destruction:
        relevant = true;
        break;
    case T_MsgsSimToClient_msg_msg_unit_attributes:
        const ASN1T_MsgUnitAttributes* attributes = asn.msg.u.msg_unit_attributes;
        if( attributes->m.positionPresent || attributes->m.vitessePresent || attributes->m.etat_operationnelPresent )
            relevant = true;
        break;
    }
    return relevant;
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void ConnectorFacade::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_end )
        UpdateCurrentState();
    else if( bLoaded_ )
        UpdateOnTick( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::UpdateOnTick
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
void ConnectorFacade::UpdateOnTick( const ASN1T_MsgsSimToClient& asnMsg )
{
    if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_begin_tick )
    {
        connector_->Lock();        
        Send( simulation_ ); // $$$$ JCR 2007-08-02: Push it out of here ...
    }
    else if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
        connector_->Unlock();
    else if( IsRelevant( asnMsg ) )
        connector_->Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::Send
// Created: JCR 2007-08-01
// -----------------------------------------------------------------------------
void ConnectorFacade::Send( dispatcher::Publisher_ABC& publisher ) const
{
    for( CIT_ListenerList it = listeners_.begin(); it != listeners_.end(); ++it )
        if( *it )
            (*it)->Listen( publisher );
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::SendCurrentState
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
void ConnectorFacade::UpdateCurrentState()
{
    {
        dispatcher::CreationVisitor visitor( *connector_ );
        connector_->Lock();
        connector_->VisitModel( visitor );
        connector_->Unlock();
    }
    {
        dispatcher::FullUpdateVisitor visitor( *connector_ );
        connector_->Lock();
        connector_->VisitModel( visitor );
        connector_->Unlock();
    }
    bLoaded_ = true;
}
