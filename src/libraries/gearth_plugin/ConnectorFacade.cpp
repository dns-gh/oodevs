// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "ConnectorFacade.h"
#include "ScopeEditor.h"
#include "ReportFactory.h"
#include "ExtensionFactory.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include "dispatcher/Visitors.h"
#include <xeumeuleu/xml.h>

using namespace plugins::gearth;

// -----------------------------------------------------------------------------
// Name: ConnectorFacade constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ConnectorFacade::ConnectorFacade( const dispatcher::Config& config, dispatcher::Model& model, xml::xistream& xis )
    : reportFactory_ ( new ReportFactory( config, model ) )
    , model_ ( model )    
    , config_ ( config )
    , bNeedUpdate_ ( false )
    , factory_( new ExtensionFactory() )
    , vpath_( xml::attribute( xis, "virtual-path", std::string() ) )
{
    model_.RegisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ConnectorFacade::~ConnectorFacade()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::IsRelevant
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
bool ConnectorFacade::IsRelevant( const ASN1T_MsgsSimToClient& asn ) const
{    
    switch ( asn.msg.t )
    {
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
//    case T_MsgsSimToClient_msg_msg_folk_creation:
//    case T_MsgsSimToClient_msg_msg_folk_graph_update:
        return true;            
    case T_MsgsSimToClient_msg_msg_unit_attributes:
        const ASN1T_MsgUnitAttributes* attributes = asn.msg.u.msg_unit_attributes;
        if( attributes->m.positionPresent || attributes->m.vitessePresent || attributes->m.etat_operationnelPresent )
            return true;        
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::IsRelevant
// Created: RDS 2008-04-10
// -----------------------------------------------------------------------------
bool ConnectorFacade::IsRelevant( const ASN1T_MsgsMessengerToClient& asn ) const
{
    switch ( asn.t )
    {
    case T_MsgsMessengerToClient_msg_lima_creation:
    case T_MsgsMessengerToClient_msg_lima_destruction:
    case T_MsgsMessengerToClient_msg_limit_creation:
    case T_MsgsMessengerToClient_msg_limit_destruction:
        return true;
    default:
        return false; 
    }
}


// -----------------------------------------------------------------------------
// Name: ConnectorFacade::Update
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void ConnectorFacade::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{    
    switch ( asnMsg.msg.t )
    {    
    case T_MsgsSimToClient_msg_msg_control_send_current_state_begin:
        bNeedUpdate_ = true;            
        break;
    case T_MsgsSimToClient_msg_msg_control_begin_tick:
        bNeedUpdate_ = false;            
        break;
    case T_MsgsSimToClient_msg_msg_control_send_current_state_end:
    case T_MsgsSimToClient_msg_msg_control_end_tick:
        if ( bNeedUpdate_ )
            UpdateCurrentState();
        break;
    default:
        bNeedUpdate_ |= IsRelevant( asnMsg );
    }
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::OnReceiveMessengerToClient
// Created: RDS 2008-04-10
// -----------------------------------------------------------------------------
void ConnectorFacade::OnReceiveMessengerToClient(const std::string&, const ASN1T_MsgsMessengerToClient& asnMsg)
{
    bNeedUpdate_  = IsRelevant( asnMsg );
}





// -----------------------------------------------------------------------------
// Name: ConnectorFacade::SendCurrentState
// Created: JCR 2007-05-14
// -----------------------------------------------------------------------------
void ConnectorFacade::UpdateCurrentState()
{
    ScopeEditor    editor( config_, model_, *reportFactory_, vpath_ );
    {
        dispatcher::CreationVisitor visitor( editor );        
        editor.VisitModel( visitor );        
    }
    {
        dispatcher::FullUpdateVisitor visitor( editor );        
        editor.VisitModel( visitor );        
    }    
}
