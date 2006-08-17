// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Network.h"
#include "Net_Def.h"
#include "clients_kernel/Types.h"
#include "AgentServerMsgMgr.h"
#include "ASN_Messages.h"
#include "Simulation.h"

#include "DIN/DIN_Engine.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"

using namespace DIN; using namespace NEK;
using namespace tools::thread;

// -----------------------------------------------------------------------------
// Name: Network constructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::Network( Controllers& controllers, Simulation& simu )
    : simu_( simu )
    , engine_( new DIN::DIN_Engine() )
    , manager_( new AgentServerMsgMgr( controllers, *engine_, simu, mutex_ ) )
    , session_( 0 )
    , terminated_( false )
{
    DIN_ConnectionProtocols connProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 );

    DIN_ConnectorGuest connector( eConnector_SIM_MOS );

    pConnService_ = new DIN_ConnectionServiceClientUserCbk< Network >( 
                                      *this
                                    , *engine_
                                    , connector
                                    , connProtocols
                                    , eConnMagicMOSServerAgentServer
                                    , "MOS Server to agent server"); 

	pConnService_->SetCbkOnConnectionSuccessful( &Network::OnConnected      );
    pConnService_->SetCbkOnConnectionFailed    ( &Network::OnNotConnected   );
    pConnService_->SetCbkOnConnectionLost      ( &Network::OnConnectionLost );

    Thread::Start();
}
    
// -----------------------------------------------------------------------------
// Name: Network destructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::~Network()
{
    terminated_ = true;
    Join();
    delete pConnService_;
    delete manager_;
    delete engine_;
}

// -----------------------------------------------------------------------------
// Name: Network::Connect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Connect( const std::string& strHostName, uint16 nPort )
{
    boost::mutex::scoped_lock locker( mutex_ );
    if( IsConnected() )
        return false;
    
    NEK_AddressINET addr( strHostName.c_str(), nPort );
    pConnService_->JoinHost( addr );
    return true;
}


// -----------------------------------------------------------------------------
// Name: Network::Disconnect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Disconnect()
{
    boost::mutex::scoped_lock locker( mutex_ );
    if( ! IsConnected() )
        return false;
    session_->Close( false );
    simu_.Disconnect();
    return true;
}

// -----------------------------------------------------------------------------
// Name: Network::Run
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void Network::Run()
{
    while( ! terminated_ )
    {
        {
            boost::mutex::scoped_lock locker( mutex_ );
            engine_->Update();
            manager_->Flush();
        }
        Thread::Sleep( 10 );
    }
}

// -----------------------------------------------------------------------------
// Name: Network::IsConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::IsConnected() const
{
    return session_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Network::Update
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::Update()
{
    {
        boost::mutex::scoped_lock locker( mutex_ );
        T_Messages messages;
        std::swap( messages, messages_ );
        for( CIT_Messages it = messages.begin(); it != messages.end(); ++it )
        {
            session_ = it->link_;
            if( it->error_.empty() )
            {
                MT_LOG_INFO_MSG( "Connecté à " << it->link_->GetRemoteAddress().GetAddressAsString() );
                manager_->Enable( *session_ );
                ASN_MsgCtrlClientAnnouncement asnMsg;
                asnMsg.GetAsnMsg() = MsgCtrlClientAnnouncement::mos_light;
                asnMsg.Send( *manager_ );
                
                simu_.Connect( it->address_ );
            }
            else if( it->lost_ )
            {
                MT_LOG_WARNING_MSG( "Connexion à " << it->address_ << " perdue (raison :" << it->error_ << ")" );   
                simu_.Disconnect();
            }
            else
            {
                MT_LOG_WARNING_MSG( "Non connecté à " << it->address_ << " (raison :" << it->error_ << ")" );   
                simu_.Disconnect();
            }
        }
    }
    manager_->DoUpdate();
}

// -----------------------------------------------------------------------------
// Name: Network::GetMessageMgr
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
AgentServerMsgMgr& Network::GetMessageMgr()
{
    return *manager_;
}

// -----------------------------------------------------------------------------
// Name: Network::OnConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::OnConnected( DIN::DIN_Link& link )
{
    messages_.push_back( T_Message() );
    messages_.back().link_ = &link;
    messages_.back().address_ = link.GetRemoteAddress().GetAddressAsString();
}

// -----------------------------------------------------------------------------
// Name: Network::OnNotConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::OnNotConnected( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    messages_.push_back( T_Message() );
    messages_.back().lost_ = false;
    messages_.back().link_ = 0;
    messages_.back().address_ = link.GetRemoteAddress().GetAddressAsString();
    messages_.back().error_ = reason.GetInfo();
}

// -----------------------------------------------------------------------------
// Name: Network::OnConnectionLost
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    messages_.push_back( T_Message() );
    messages_.back().lost_ = true;
    messages_.back().link_ = 0;
    messages_.back().address_ = link.GetRemoteAddress().GetAddressAsString();
    messages_.back().error_ = reason.GetInfo();
}
