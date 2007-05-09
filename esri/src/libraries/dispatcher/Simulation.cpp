// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Simulation.h"

#include "Network_Def.h"
#include "game_asn/Asn.h"
#include "Dispatcher.h"
#include "Model.h"
#include "ClientsNetworker.h"
#include "ProfileManager.h"
#include "SaverFacade.h"
#include "esri/ESRI_ConnectorFacade.h"
#include "SimulationDispatcher.h"
#include "tools/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"
#include "esri/ESRI_Config.h"
#include "xeumeuleu/xml.h"

using namespace dispatcher;
using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Simulation::Simulation( Dispatcher& dispatcher, DIN_MessageService_ABC& messageService, DIN_Link& link, const std::string& configFile, esri::Config& config )
    : Server_ABC ( messageService, link )
    , dispatcher_( dispatcher )
    , saver_         ( 0 )
    , esriConnector_ ( 0 )    
{
    AsnMsgMiddleToSimCtrlClientAnnouncement().Send( *this );

    CreateSaver( configFile );
    CreateEsriConnector( config, configFile );
    simDispatch_ = new SimulationDispatcher( dispatcher.GetClientsNetworker(), dispatcher.GetModel() );
}

//-----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Simulation::~Simulation()
{
    if ( saver_ )
        delete saver_;
    if ( esriConnector_ )
        delete esriConnector_;
    delete simDispatch_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const ASN1T_MsgsSimToClient& asnMsg )
{
    simDispatch_->OnReceive( asnMsg );

    switch( asnMsg.msg.t )
    {
        case T_MsgsSimToClient_msg_msg_ctrl_send_current_state_begin:
        {
            dispatcher_.GetClientsNetworker().DenyConnections();
            dispatcher_.GetModel           ().Reset();
            MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
            break;
        }
        case T_MsgsSimToClient_msg_msg_ctrl_send_current_state_end:  
        {
            MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
            dispatcher_.GetProfileManager  ().Reset(); // Profiles initialization
            dispatcher_.GetClientsNetworker().AllowConnections();            
            if ( esriConnector_ )
                esriConnector_->Update();
            break;
        }
    };
    
    if( saver_ )
        saver_->Save( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const ASN1T_MsgsSimToMiddle& asnMsg )
{
    // NOTHING
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg )
{
    dispatcher_.GetClientsNetworker().Send( nMsgID, dinMsg );            
    dispatcher_.GetModel().Update( nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsMiddleToSim& asnMsg )
{
    AsnMessageEncoder< ASN1T_MsgsMiddleToSim, ASN1C_MsgsMiddleToSim > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgMiddleToSim, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsMiddleToSim& asnMsg, const DIN::DIN_BufferedMessage& dinMsg )
{
    messageService_.Send( link_, eMsgMiddleToSim, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsClientToSim& asnMsg )
{
    AsnMessageEncoder< ASN1T_MsgsClientToSim, ASN1C_MsgsClientToSim > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgClientToSim, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsClientToSim& /*asnMsg*/, const DIN_BufferedMessage& dinMsg )
{
    messageService_.Send( link_, eMsgClientToSim, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::Send( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg )
{
    messageService_.Send( link_, nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Simulation::GetSimulationFromLink
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
Simulation& Simulation::GetSimulationFromLink( const DIN::DIN_Link& link )
{
    DIN::DIN_UserData_ABC* pTmp = link.GetUserData();
    assert( pTmp );
    return *static_cast< Simulation* >( pTmp );    
}

// -----------------------------------------------------------------------------
// Name: Simulation::CreateSaver
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void Simulation::CreateSaver( const std::string& configFile )
{
    xml::xifstream xis( configFile );
    bool enable = false; std::string directory;
    xis >> xml::start( "config" )
            >> xml::start( "dispatcher" )
                >> xml::start( "recorder" )
                    >> xml::attribute( "enabled", enable )
                    >> xml::optional() >> xml::attribute( "directory", directory );
    if( enable )
        saver_ = new SaverFacade( dispatcher_.GetModel(), directory );
}

// -----------------------------------------------------------------------------
// Name: Simulation::CreateEsriConnector
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
void Simulation::CreateEsriConnector( esri::Config& config, const std::string& configFile )
{
    xml::xifstream xis( configFile );
    bool enable = false; 
    std::string geodatabase;    
    xis >> xml::start( "config" )
            >> xml::start( "dispatcher" )
                >> xml::start( "esri-service" )
                    >> xml::attribute( "enabled", enable )
                    >> xml::optional() >> xml::attribute( "geodatabase", geodatabase );
    config.UpdateGeodatabase( geodatabase );
    if( enable )
        esriConnector_ = new esri::ConnectorFacade( dispatcher_.GetModel(), config );
}
