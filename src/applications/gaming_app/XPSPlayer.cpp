// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "XPSPlayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Player.h"
#include "clients_gui/Melody.h"

// -----------------------------------------------------------------------------
// Name: XPSPlayer constructor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
XPSPlayer::XPSPlayer( QObject* parent, kernel::Controllers& controllers )
    : QObject( parent )
    , controllers_( controllers )
    , player_( new Player() )
{
    if( player_->IsValid() )
        controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: XPSPlayer destructor
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
XPSPlayer::~XPSPlayer()
{
    if( player_->IsValid() )
        controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: XPSPlayer::NotifyUpdated
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void XPSPlayer::NotifyUpdated( const Fires& )
{
    player_->Handle( Melody::Explosion() );
}

// -----------------------------------------------------------------------------
// Name: XPSPlayer::NotifyUpdated
// Created: AGE 2006-11-13
// -----------------------------------------------------------------------------
void XPSPlayer::NotifyUpdated( const Simulation::sStartTick& )
{
    player_->Handle( Melody::TickStart() );
}

// -----------------------------------------------------------------------------
// Name: XPSPlayer::NotifyUpdated
// Created: AGE 2006-11-30
// -----------------------------------------------------------------------------
void XPSPlayer::NotifyUpdated( const Simulation& simu )
{
    player_->Handle( simu.IsConnected() ? Melody::Connected() : Melody::Disconnected() );
}
