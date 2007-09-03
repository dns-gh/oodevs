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
ConnectorFacade::ConnectorFacade( const dispatcher::Model& model, const dispatcher::Config& config, dispatcher::SimulationPublisher_ABC& publisher )
    : orderTypes_( new kernel::OrderTypes( config ) )
    , connector_ ( new Connector( config, model ) )
    , bLoaded_   ( false )
{
    listeners_.push_back( T_SharedListener( new OrderListener( *connector_, publisher, *orderTypes_, model ) ) );
    listeners_.push_back( T_SharedListener( new StatusListener( *connector_, publisher ) ) );
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
    case T_MsgsSimToClient_msg_msg_unit_creation:
    case T_MsgsSimToClient_msg_msg_unit_destruction:        
    case T_MsgsSimToClient_msg_msg_report:
    case T_MsgsSimToClient_msg_msg_folk_creation:
    case T_MsgsSimToClient_msg_msg_folk_graph_update:
        return true;            
    case T_MsgsSimToClient_msg_msg_unit_attributes:
        const ASN1T_MsgUnitAttributes* attributes = asn.msg.u.msg_unit_attributes;
        if( attributes->m.positionPresent || attributes->m.vitessePresent || attributes->m.etat_operationnelPresent )
            return true;        
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void ConnectorFacade::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_begin ||
        asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_begin_tick )
    {
        connector_->Lock();
    }
	UpdateOnMessage( asnMsg );
    if( asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_end ||
        asnMsg.msg.t == T_MsgsSimToClient_msg_msg_control_end_tick )
    {
        connector_->Unlock();
    }    
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::UpdateOnTick
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
void ConnectorFacade::UpdateOnMessage( const ASN1T_MsgsSimToClient& asnMsg )
{
	switch ( asnMsg.msg.t )
	{
	case T_MsgsSimToClient_msg_msg_control_begin_tick:
		connector_->Initialize();
        UpdateListeners(); // $$$$ JCR 2007-08-02: Push it out of here ...
		break;
	case T_MsgsSimToClient_msg_msg_control_end_tick:
		connector_->Finalize();        
		break;
	default:
		if( IsRelevant( asnMsg ) )
			connector_->Send( asnMsg );
	}    
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::UpdateListeners
// Created: JCR 2007-08-01
// -----------------------------------------------------------------------------
void ConnectorFacade::UpdateListeners() const
{
    for( CIT_ListenerList it = listeners_.begin(); it != listeners_.end(); ++it )
        (*it)->Listen();
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
