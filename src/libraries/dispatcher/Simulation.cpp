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
#include "SimulationDispatcher.h"
#include "tools/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"

using namespace dispatcher;
using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Simulation::Simulation( Dispatcher& dispatcher, DIN_MessageService_ABC& messageService, DIN_Link& link, bool bRecord )
    : Server_ABC ( messageService, link )
    , dispatcher_( dispatcher )
    , saver_( 0 )
{
    AsnMsgInSimCtrlClientAnnouncement asnMsg;
    asnMsg.Send( *this );

    // $$$$ AGE 2007-04-10: 
    if( bRecord )
        saver_ = new SaverFacade( dispatcher.GetModel(), "test" );
    simDispatch_ = new SimulationDispatcher( dispatcher.GetClientsNetworker(), dispatcher.GetModel() );
}

//-----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Simulation::~Simulation()
{
    delete simDispatch_;
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Simulation::OnReceive( const ASN1T_MsgsOutSim& asnInMsg )
{
    simDispatch_->OnReceive( asnInMsg );

    switch( asnInMsg.msg.t )
    {
    case T_MsgsOutSim_msg_msg_ctrl_send_current_state_begin:
    {
        dispatcher_.GetClientsNetworker().DenyConnections();
        dispatcher_.GetModel           ().Reset();
        MT_LOG_INFO_MSG( "Dispatcher - Initializing model" );
        break;
    }
    case T_MsgsOutSim_msg_msg_ctrl_send_current_state_end:  
    {
        MT_LOG_INFO_MSG( "Dispatcher - Model initialized" );
        dispatcher_.GetProfileManager  ().Reset(); // Profiles initialization
        dispatcher_.GetClientsNetworker().AllowConnections();
        break;
    }
    };
    
    if( saver_ )
        saver_->Save( const_cast< ASN1T_MsgsOutSim& >( asnInMsg ) );
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
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsInSim& asnMsg )
{
    AsnMessageEncoder< ASN1T_MsgsInSim, ASN1C_MsgsInSim > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgInSim, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Simulation::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Simulation::Send( const ASN1T_MsgsInSim& /*asnMsg*/, const DIN_BufferedMessage& dinMsg )
{
    messageService_.Send( link_, eMsgInSim, dinMsg );
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