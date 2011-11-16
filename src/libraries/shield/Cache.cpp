// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Cache.h"
#include "Converter.h"
#include "Utf8Converter.h"
#include "Model_ABC.h"
#include "tools/MessageSender_ABC.h"
#pragma warning( push, 0 )
#include "proto/SimToClient.pb.h"
#pragma warning( pop )
#include <boost/preprocessor/stringize.hpp>

using namespace shield;

// -----------------------------------------------------------------------------
// Name: Cache constructor
// Created: MCO 2011-11-09
// -----------------------------------------------------------------------------
Cache::Cache( tools::MessageSender_ABC& sender, const Model_ABC& model, bool utf8StringEncoding )
    : sender_            ( sender )
    , model_             ( model )
    , utf8StringEncoding_( utf8StringEncoding )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Cache destructor
// Created: MCO 2011-11-09
// -----------------------------------------------------------------------------
Cache::~Cache()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Cache::Send
// Created: MCO 2011-11-09
// -----------------------------------------------------------------------------
void Cache::Send( const std::string& link )
{
    if( messages_.empty() )
        model_.Send( *this );
    static const unsigned long tag = tools::MessageIdentifierFactory::GetIdentifier< MsgsSimToClient::MsgSimToClient >();
    for( IT_Messages it = messages_.begin(); it != messages_.end(); ++it )
        sender_.Send( link, tag, *it );
}

// -----------------------------------------------------------------------------
// Name: Cache::Clear
// Created: MCO 2011-11-09
// -----------------------------------------------------------------------------
void Cache::Clear()
{
    messages_.clear();
}

// -----------------------------------------------------------------------------
// Name: Cache::Send
// Created: MCO 2011-11-09
// -----------------------------------------------------------------------------
void Cache::Send( const sword::SimToClient& message )
{
    Converter converter( *this, *this );
    converter.ReceiveSimToClient( message );
}

// -----------------------------------------------------------------------------
// Name: Cache::Send
// Created: MCO 2011-11-09
// -----------------------------------------------------------------------------
void Cache::Send( MsgsSimToClient::MsgSimToClient& message )
{
    if( utf8StringEncoding_ )
        Utf8Converter::ConvertCP1252StringsToUtf8( message );
    tools::Message m;
    sender_.Serialize( message, m );
    messages_.push_back( m );
}

#define NO_CACHE( MESSAGE ) \
    void Cache::Send( MESSAGE& ) \
    { \
        throw std::runtime_error( "unable to cache message of type " BOOST_PP_STRINGIZE( MESSAGE ) ); \
    } \

NO_CACHE( const sword::AuthenticationToClient )
NO_CACHE( const sword::ReplayToClient )
NO_CACHE( const sword::AarToClient )
NO_CACHE( const sword::MessengerToClient )
NO_CACHE( const sword::DispatcherToClient )
NO_CACHE( sword::ClientToSim )
NO_CACHE( sword::ClientToAuthentication )
NO_CACHE( sword::ClientToReplay )
NO_CACHE( sword::ClientToAar )
NO_CACHE( sword::ClientToMessenger )
NO_CACHE( sword::AdminToLauncher )
NO_CACHE( MsgsAarToClient::MsgAarToClient )
NO_CACHE( MsgsAuthenticationToClient::MsgAuthenticationToClient )
NO_CACHE( MsgsDispatcherToClient::MsgDispatcherToClient )
NO_CACHE( MsgsMessengerToClient::MsgMessengerToClient )
NO_CACHE( MsgsReplayToClient::MsgReplayToClient )
NO_CACHE( MsgsLauncherToAdmin::MsgLauncherToAdmin )
