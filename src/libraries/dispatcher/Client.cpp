// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Client.h"

#include "game_asn/Asn.h"
#include "Model.h"
#include "Profile.h"
#include "ProfileManager.h"
#include "Network_Def.h"
#include "SimulationNetworker.h"
#include "ClientDispatcher.h"
#include "ProfilesDispatcher.h"
#include "LoaderFacade.h"
#include "tools/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"

using namespace dispatcher;
using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Client::Client( const Model& model, ProfileManager& profiles, LoaderFacade& loader, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link )
    : Client_ABC ( messageService, link )
    , model_( model )
    , profiles_( profiles )
    , pProfile_  ( 0 )
    , loader_( &loader )
    , simulation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
Client::Client( const Model& model, ProfileManager& profiles, SimulationNetworker& simulation, DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link )
    : Client_ABC ( messageService, link )
    , model_( model )
    , profiles_( profiles )
    , pProfile_  ( 0 )
    , loader_( 0 )
    , simulation_( &simulation )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: Client destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Client::~Client()
{
    // NOTHING
}

// =============================================================================
// RIGHTS MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsOutClient& asnMsg ) const
{
    if( asnMsg.msg.t == T_MsgsOutClient_msg_msg_authentication_request )
        return true;

    if( !pProfile_ )
        return false;

    return pProfile_->CheckRights( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsInClient& asnMsg ) const
{
    //$$$$ TMP
    if( asnMsg.msg.t == T_MsgsInClient_msg_msg_authentication_response )
        return true;

    if( !pProfile_ )
        return false;

    return true;
}

// =============================================================================
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgAuthenticationRequest
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgAuthenticationRequest( const ASN1T_MsgAuthenticationRequest& msg )
{
    //$$$ TMP
    if( pProfile_ )
    {
        Disconnect();
        return;
    }

    pProfile_ = profiles_.Authenticate( msg.login, msg.password );
    if( !pProfile_ )
    {
        AsnMsgInClientAuthenticationResponse ack;
        ack().error_code = MsgAuthenticationResponse_error_code::invalid_login;
        ack.Send( *this );
        return; 
    }

    // Ack message
    AsnMsgInClientAuthenticationResponse ack;
    ack().error_code      = MsgAuthenticationResponse_error_code::success;
    ack().m.profilePresent = 1;
    pProfile_->Send( ack().profile );
    ack.Send( *this );
    Profile::AsnDelete( ack().profile );

    model_.Send( *this );
    profiles_.Send( *this );
    if( loader_ )
        loader_->Send( *this );
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceive( const ASN1T_MsgsOutClient& asnInMsg )
{
    if( !CheckRights( asnInMsg ) )
    {
        Disconnect();
        return;
    }

    if( asnInMsg.msg.t == T_MsgsOutClient_msg_msg_authentication_request )
        OnReceiveMsgAuthenticationRequest( *asnInMsg.msg.u.msg_authentication_request ); 
    
    ProfilesDispatcher dispatcher( profiles_, *this );
    dispatcher.OnReceive( asnInMsg );

    if( simulation_ )
    {
        ClientDispatcher dispatcher( *simulation_ );
        dispatcher.OnReceive( asnInMsg );
    }
    if( loader_ )
        loader_->OnReceive( asnInMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::OnReceive( unsigned int nMsgID, DIN::DIN_Input& dinMsg )
{
    // $$$ TMP
    if( !pProfile_ || !simulation_ )
        return;
    simulation_->Send( nMsgID, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsInClient& asnMsg )
{
    if( !CheckRights( asnMsg ) )
        return;

    AsnMessageEncoder< ASN1T_MsgsInClient, ASN1C_MsgsInClient > asnEncoder( messageService_, asnMsg );
    messageService_.Send( link_, eMsgInClient, asnEncoder.GetDinMsg() );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsInClient& asnMsg, const DIN_BufferedMessage& dinMsg )
{
    if( !CheckRights( asnMsg ) )
        return;

    messageService_.Send( link_, eMsgInClient, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::Send( unsigned int nMsgID, const DIN::DIN_BufferedMessage& dinMsg )
{
    // $$$ TMP
    if( !pProfile_ )
        return;

    messageService_.Send( link_, nMsgID, dinMsg );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::GetClientFromLink
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
// static
Client& Client::GetClientFromLink( const DIN_Link& link )
{
    DIN_UserData_ABC* pTmp = link.GetUserData();
    assert( pTmp );
    return *static_cast< Client* >( pTmp );    
}
