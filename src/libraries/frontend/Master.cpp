// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Master.h"
#include "Publisher.h"
#include "Messages.h"
#include "network/AsnMessageEncoder.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/DIN_Link.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Master constructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Master::Master( DIN::DIN_MessageService_ABC& messageService, DIN::DIN_Link& link )
    : Server_ABC ( messageService, link )
{
    AsnMsgInMasterClientAnnouncement message;
    Publisher publisher( *this );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Master destructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Master::~Master()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceive
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Master::OnReceive( const ASN1T_MsgsOutMaster& message )
{
    // $$$$ SBO 2007-01-25: process
    switch( message.msg.t )
    {
    case T_MsgsOutMaster_msg_msg_authentication_response:
    case T_MsgsOutMaster_msg_msg_exercise_creation:
    case T_MsgsOutMaster_msg_msg_exercise_creation_ack:
    case T_MsgsOutMaster_msg_msg_exercise_update:
    case T_MsgsOutMaster_msg_msg_exercise_update_ack:
    case T_MsgsOutMaster_msg_msg_exercise_destruction:
    case T_MsgsOutMaster_msg_msg_exercise_destruction_ack:
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: Master::OnReceive
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Master::OnReceive( unsigned int id, DIN::DIN_Input& message )
{
    // $$$$ SBO 2007-01-25: process message
}

// -----------------------------------------------------------------------------
// Name: Master::Send
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Master::Send( const ASN1T_MsgsInMaster& message )
{
    network::AsnMessageEncoder< ASN1T_MsgsInMaster, ASN1C_MsgsInMaster > asnEncoder( messageService_, message );
    messageService_.Send( link_, eMsgInMaster, asnEncoder.GetDinMsg() );
}
