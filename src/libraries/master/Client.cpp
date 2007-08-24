// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"

#include "Client.h"

#include "Network_Def.h"
#include "Master.h"
#include "ProfileManager.h"
#include "DataManager.h"
#include "tools/AsnMessageEncoder.h"
#include "tools/ObjectMessageService.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"

using namespace master;
using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Client constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Client::Client( Master& master, ObjectMessageService& messageService, DIN_Link& link )
    : Client_ABC( messageService, link )
    , master_   ( master )
    , pProfile_ ( 0 )
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
bool Client::CheckRights( const ASN1T_MsgsInMaster& asnMsg ) const
{
    if( asnMsg.msg.t == T_MsgsInMaster_msg_msg_authentication_request )
        return true;

    if( !pProfile_ )
        return false;

    return pProfile_->CheckRights( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::CheckRights
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool Client::CheckRights( const ASN1T_MsgsOutMaster& asnMsg ) const
{
    //$$$$ TMP
    if( asnMsg.msg.t == T_MsgsOutMaster_msg_msg_authentication_response )
        return true;

    if( !pProfile_ )
        return false;

    return true;
}

//
// =============================================================================
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Client::OnReceiveMsgAuthenticationRequest
// Created: NLD 2007-01-30
// -----------------------------------------------------------------------------
void Client::OnReceiveMsgAuthenticationRequest( const ASN1T_MsgAuthenticationRequest& msg )
{
    //$$$ TMP
    if( pProfile_ )
    {
        Disconnect();
        return;
    }

    pProfile_ = master_.GetProfileManager().Authenticate( msg.login, msg.password );
    if( !pProfile_ )
    {
        AsnMsgOutMasterAuthenticationResponse ack;
        ack().return_code = MsgAuthenticationResponse_return_code::invalid_login;
        ack.Send( *this );
        return; 
    }

    // Ack message
    AsnMsgOutMasterAuthenticationResponse ack;
    ack().return_code      = MsgAuthenticationResponse_return_code::ok;
    ack().m.profilePresent = 1;
    pProfile_->Send( ack().profile );
    ack.Send( *this );

    master_.GetDataManager().Send( *this );
}

// -----------------------------------------------------------------------------
// Name: Client::OnReceive
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
void Client::OnReceive( const ASN1T_MsgsInMaster& asnInMsg )
{
    if( !CheckRights( asnInMsg ) )
    {
        Disconnect();
        return;
    }

    switch( asnInMsg.msg.t )
    {
        case T_MsgsInMaster_msg_msg_authentication_request       : OnReceiveMsgAuthenticationRequest( *asnInMsg.msg.u.msg_authentication_request ); break;
        case T_MsgsInMaster_msg_msg_exercise_creation_request    : break;
        case T_MsgsInMaster_msg_msg_exercise_update_request      : break;
        case T_MsgsInMaster_msg_msg_exercise_destruction_request : break;
    }
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsOutMaster& asnMsg )
{
    if( !CheckRights( asnMsg ) )
        return;
    messageService_.Send( link_, asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Client::Send
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void Client::Send( const ASN1T_MsgsOutMaster& asnMsg, const DIN_BufferedMessage& dinMsg )
{
    if( !CheckRights( asnMsg ) )
        return;
    messageService_.Send( link_, asnMsg, dinMsg );
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
