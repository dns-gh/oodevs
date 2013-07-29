// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FirePlayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_gui/SoundManager.h"
#include "clients_gui/SoundEvent.h"

#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp>

namespace
{

    const std::vector< std::string > fireSoundNames = boost::assign::list_of< std::string >
        ( "directfire" )
        ( "indirectsmoke" )
        ( "indirectexplosive" )
        ( "indirectillumination" )
        ( "indirecteffect" );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer constructor
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
FirePlayer::FirePlayer( kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : controllers_( controllers )
    , profileFilter_( profile )
    , soundManager_ ( new gui::SoundManager( fireSoundNames ) )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer destructor
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
FirePlayer::~FirePlayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::GetSoundManager
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
gui::SoundManager& FirePlayer::GetSoundManager()
{
    return *soundManager_;
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::PlayPauseSoundControl
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void FirePlayer::PlayPauseSoundControl( bool play )
{
    if( soundManager_.get() )
        soundManager_->PlayPauseAllChannels( play );
}


// -----------------------------------------------------------------------------
// Name: FirePlayer::CanPlaySound
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
bool FirePlayer::CanPlaySound( const std::string& channel )
{
    return !soundManager_->IsPlaying( channel );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::NotifyUpdated
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
void FirePlayer::NotifyUpdated( const gui::SoundEvent& soundEvent )
{
    if( soundEvent.StopSound() )
        soundManager_->StopSound( soundEvent.GetSoundType() );

    if( !CanPlaySound( soundEvent.GetSoundType() ) )
        return;
    if( !soundEvent.GetEntity() || profileFilter_.IsPerceived( *soundEvent.GetEntity() ) && profileFilter_.IsVisible( *soundEvent.GetEntity() ) )
    {
        if( soundEvent.GetSoundType() == "directfire" )
            soundManager_->PlayLoopSound( soundEvent.GetSoundType() );
        else
            soundManager_->PlaySound( soundEvent.GetSoundType() );
    }
}
