// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Networker.h"
#include "Messages.h"
#include "Master.h"
#include "network/AsnMessageDecoder.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace frontend;
using namespace network;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Networker constructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Networker::Networker( const std::string& host )
    : ClientNetworker_ABC( host )
    , master_( 0 )
{
    GetMessageService().RegisterReceivedMessage( eMsgOutMaster, *this, &Networker::OnReceiveMsgOutMaster );
    GetMessageService().RegisterReceivedMessage( eMsgInit     , *this, &Networker::OnReceiveMsgInit      );
}

// -----------------------------------------------------------------------------
// Name: Networker destructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Networker::~Networker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Networker::OnConnected
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Networker::OnConnected( DIN::DIN_Link& link )
{
    ClientNetworker_ABC::OnConnected( link );
    master_.reset( new Master( GetMessageService(), link ) );
}

// -----------------------------------------------------------------------------
// Name: Networker::OnConnectionLost
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Networker::OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    ClientNetworker_ABC::OnConnectionLost( link, reason );
    master_.release();
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

#define DECLARE_DIN_CALLBACK( MSG )                                                       \
    void Networker::OnReceiveMsg##MSG( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& msg ) \
    {                                                                                     \
        master_->OnReceive( eMsg##MSG, msg );                                             \
    }

DECLARE_DIN_CALLBACK( Init )

// -----------------------------------------------------------------------------
// Name: Networker::OnReceiveMsgOutMaster
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Networker::OnReceiveMsgOutMaster( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    try
    {
        network::AsnMessageDecoder< ASN1T_MsgsOutMaster, ASN1C_MsgsOutMaster > asnDecoder( input );
        master_->OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& exception )
    {
        // $$$$ SBO 2007-01-25: transform into a non-critical exception
        throw std::exception( exception.what() );
    }
}
