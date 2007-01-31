// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Profile.h"
#include "Publisher_ABC.h"
#include "clients_kernel/Controller.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Profile::Profile( kernel::Controller& controller )
    : controller_     ( controller )
    , roleAdaptation_ ( false )
    , rolePreparation_( false )
    , roleGame_       ( false )
    , roleSupervision_( false )
    , loggedIn_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile::DoUpdate
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Profile::DoUpdate( const ASN1T_MsgAuthenticationResponse& message )
{
    loggedIn_ = message.return_code == MsgAuthenticationResponse_return_code::ok;
    if( loggedIn_ )
    {
        login_           = message.profile.login;
        roleAdaptation_  = message.profile.role_adaptation;
        rolePreparation_ = message.profile.role_preparation;
        roleGame_        = message.profile.role_game;
        roleSupervision_ = message.profile.role_supervision;
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void Profile::Login( Publisher_ABC& publisher, const QString& login, const QString& password ) const
{
    AsnMsgInMasterAuthenticationRequest asn;
    asn().login    = login.ascii();
    asn().password = password.ascii();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Profile::IsLoggedIn
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
bool Profile::IsLoggedIn() const
{
    return loggedIn_;
}
