// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Player.h"
#include "Melody.h"

// -----------------------------------------------------------------------------
// Name: Player constructor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
Player::Player()
{
    Start();
    Melody base;
    base.SetLights( 1, 1, 1, 4 );
    Handle( base );
}

// -----------------------------------------------------------------------------
// Name: Player destructor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
Player::~Player()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Player::Process
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void Player::Process( const Melody& melody )
{
    melody.Play( lights_ );
}

// -----------------------------------------------------------------------------
// Name: Player::IsValid
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
bool Player::IsValid() const
{
    return lights_.IsValid();
}
