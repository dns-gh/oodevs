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
#include "tools/AsnMessageDecoder.h"
#include "clients_kernel/Controller.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace frontend;
using namespace tools;
using namespace DIN;

static const unsigned int magicCookie_ = 4242;

// -----------------------------------------------------------------------------
// Name: Networker constructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Networker::Networker( const std::string& host, kernel::Controller& controller, Model& model, Profile& profile )
    : ClientNetworker_ABC( magicCookie_, host )
    , controller_( controller )
    , master_( 0 )
    , model_( model )
    , profile_( profile )
{
    GetMessageService().RegisterReceivedMessage( eMsgOutMaster, *this, &Networker::OnReceiveMsgOutMaster );
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
    master_.reset( new Master( GetMessageService(), link, model_, profile_ ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Networker::OnConnectionLost
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Networker::OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    ClientNetworker_ABC::OnConnectionLost( link, reason );
    master_.release();
    controller_.Update( *this );
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

#define DECLARE_DIN_CALLBACK( MSG )                                                       \
    void Networker::OnReceiveMsg##MSG( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& msg ) \
    {                                                                                     \
        master_->OnReceive( eMsg##MSG, msg );                                             \
    }

// -----------------------------------------------------------------------------
// Name: Networker::OnReceiveMsgOutMaster
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Networker::OnReceiveMsgOutMaster( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    try
    {
        AsnMessageDecoder< ASN1T_MsgsOutMaster, ASN1C_MsgsOutMaster > asnDecoder( input );
        master_->OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& exception )
    {
        // $$$$ SBO 2007-01-25: transform into a non-critical exception
        throw std::exception( exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: Networker::Send
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Networker::Send( const ASN1T_MsgsInMaster& message )
{
    if( master_.get() )
        master_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: Networker::BuildDinMsg
// Created: SBO 2007-01-30
// -----------------------------------------------------------------------------
DIN::DIN_BufferedMessage Networker::BuildDinMsg()
{
    if( master_.get() )
        return master_->BuildDinMsg();
    throw std::runtime_error( "Trying to build network message but you're not connected!" );
}
