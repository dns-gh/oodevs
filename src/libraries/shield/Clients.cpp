// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Clients.h"
#include "Client.h"

#pragma warning( disable: 4355 )

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Clients constructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Clients::Clients( const std::string& host, tools::MessageSender_ABC& sender, ClientListener_ABC& listener,
                  bool encodeStringsInUtf8, unsigned long timeOut )
    : host_              ( host )
    , sender_            ( sender )
    , listener_          ( listener )
    , utf8StringEncoding_( encodeStringsInUtf8 )
    , timeOut_           ( timeOut )
    , stopped_           ( false )
    , thread_            ( boost::bind( &Clients::Run, this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Clients destructor
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
Clients::~Clients()
{
    try
    {
        service_.post( boost::bind( &Clients::Stop, this ) );
        thread_.join();
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: Clients::Run
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Clients::Run()
{
    ::SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL );
    while( !stopped_ )
    {
        service_.run();
        ::Sleep( 100 );
        service_.reset();
    }
}

// -----------------------------------------------------------------------------
// Name: Clients::Stop
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
void Clients::Stop()
{
    stopped_ = true;
    clients_.clear();
}

// -----------------------------------------------------------------------------
// Name: Clients::Update
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
void Clients::Update()
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Update();
}

// -----------------------------------------------------------------------------
// Name: Clients::Add
// Created: MCO 2010-11-29
// -----------------------------------------------------------------------------
void Clients::Add( const std::string& from )
{
    clients_.erase( from );
    boost::shared_ptr< Client > client( new Client( service_, from, sender_, listener_, utf8StringEncoding_, timeOut_ ) );
    clients_.insert( std::make_pair( from, client ) );
    client->Connect( host_ );
}

// -----------------------------------------------------------------------------
// Name: Clients::Remove
// Created: MCO 2010-12-01
// -----------------------------------------------------------------------------
void Clients::Remove( const std::string& from )
{
    clients_.erase( from );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToAar
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToAar( const std::string& from, const MsgsClientToAar::MsgClientToAar& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToAar( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToAuthentication
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToAuthentication( const std::string& from, const MsgsClientToAuthentication::MsgClientToAuthentication& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToAuthentication( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToMessenger
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToMessenger( const std::string& from, const MsgsClientToMessenger::MsgClientToMessenger& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToMessenger( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToReplay
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToReplay( const std::string& from, const MsgsClientToReplay::MsgClientToReplay& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToReplay( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveClientToSim
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveClientToSim( const std::string& from, const MsgsClientToSim::MsgClientToSim& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveClientToSim( msg );
}

// -----------------------------------------------------------------------------
// Name: Clients::ReceiveAdminToLauncher
// Created: MCO 2011-07-06
// -----------------------------------------------------------------------------
void Clients::ReceiveAdminToLauncher( const std::string& from, const MsgsAdminToLauncher::MsgAdminToLauncher& msg )
{
    CIT_Clients it = clients_.find( from );
    if( it != clients_.end() )
        it->second->ReceiveAdminToLauncher( msg );
}
