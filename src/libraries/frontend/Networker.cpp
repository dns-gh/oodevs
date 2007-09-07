*****************************************************************************
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

using namespace frontend;
using namespace tools;

static const unsigned int magicCookie_ = 4242;

// -----------------------------------------------------------------------------
// Name: Networker constructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Networker::Networker( const std::string& host, kernel::Controller& controller, Model& model, Profile& profile )
    : ClientNetworker( host, true )
    , controller_( controller )
    , master_( 0 )
    , model_( model )
    , profile_( profile )
{
    RegisterMessage( *this, &Networker::OnReceiveMsgOutMaster );
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
// Name: Networker::ConnectionSucceeded
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
void Networker::ConnectionSucceeded( const std::string& endpoint )
{
    ClientNetworker::ConnectionSucceeded( endpoint );
    master_.reset( new Master( *this, endpoint, model_, profile_ ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Networker::ConnectionError
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
void Networker::ConnectionError( const std::string& address, const std::string& error )
{
    ClientNetworker::ConnectionError( address, error );
    master_.release();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Networker::OnReceiveMsgOutMaster
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
void Networker::OnReceiveMsgOutMaster( const std::string& , const ASN1T_MsgsOutMaster& message )
{
    master_->OnReceive( message );
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
// Name: Networker::IsConnected
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
bool Networker::IsConnected() const
{
    return master_.get() != 0;
}
