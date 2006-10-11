// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Profile.h"
#include "clients_kernel/Controller.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::Profile( kernel::Controller& controller )
    : controller_( controller )
    , loggedIn_( false )
{
//    login_    = "test";
//    password_ = "cock";
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( Publisher_ABC& publisher ) const
{
    ASN_MsgAuthLogin asnMsg;
    asnMsg.GetAsnMsg().login    = login_.c_str();
    asnMsg.GetAsnMsg().password = password_.c_str();
    asnMsg.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Profile::Login
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Login( Publisher_ABC& publisher, const std::string& login, const std::string& password ) const
{
    login_    = login;
    password_ = password;
    Login( publisher );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void Profile::Update( const ASN1T_MsgAuthLoginAck& message )
{
    loggedIn_ = message.etat == 0;
    if( message.m.profilePresent )
    {
        // $$$$ AGE 2006-10-11: 
    };
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Profile::IsLoggedIn
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
bool Profile::IsLoggedIn() const
{
    return loggedIn_;
}

