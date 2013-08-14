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
#include "moc_FirePlayer.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
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
FirePlayer::FirePlayer( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const Simulation& simulation )
    : controllers_( controllers )
    , profileFilter_( profile )
    , soundManager_( new gui::SoundManager( fireSoundNames ) )
    , simulation_( simulation )
    , lastTick_( 0 )
    , isTimerLaunched_( false )
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
// Name: FirePlayer::PlayPauseSoundControl
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void FirePlayer::PlayPauseSoundControl( bool play )
{
    boost::mutex::scoped_lock locker( mutex_ );
    soundManager_->PlayPauseAllChannels( play );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::SetVolume
// Created: NPT 2013-08-12
// -----------------------------------------------------------------------------
void FirePlayer::SetVolume( const std::string& channel, double value )
{
    boost::mutex::scoped_lock locker( mutex_ );
    soundManager_->SetVolume( channel, value );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::ChangeSoundsDirectory
// Created: NPT 2013-08-12
// -----------------------------------------------------------------------------
void FirePlayer::ChangeSoundsDirectory( const tools::Path& path )
{
    boost::mutex::scoped_lock locker( mutex_ );
    soundManager_->ChangeSoundsDirectory( path );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::CanPlaySound
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
bool FirePlayer::CanPlaySound( const std::string& channel )
{
    return !soundManager_->IsPlaying( channel ) && lastTick_ < simulation_.GetCurrentTick();
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::NotifyUpdated
// Created: NPT 2013-07-16
// -----------------------------------------------------------------------------
void FirePlayer::NotifyUpdated( const gui::SoundEvent& soundEvent )
{
    boost::mutex::scoped_lock locker( mutex_ );
    PlaySound( soundEvent );
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::NotifyUpdated
// Created: NPT 2013-08-01
// -----------------------------------------------------------------------------
void FirePlayer::NotifyUpdated( const Simulation::sStartTick& /*tick*/ )
{
    boost::mutex::scoped_lock locker( mutex_ );
    // When a new event is created the event is stored in the currentEvents vector that is cleared every tick
    // When the tick time is smaller than the timer, the sound is played only if the sound is present in the vector
    // We don't launch the timer if it is already running
    currentEvents_.clear();
    for( auto it = loopingSounds_.begin(); it != loopingSounds_.end(); ++it )
        for( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
            if( it2->second > 0 )
            {
                currentEvents_.push_back( gui::SoundEvent( it2->first, it->first, gui::SoundEvent::eSingleShot ) );
                break;
            }
    if( !isTimerLaunched_ && !currentEvents_.empty() )
    {
        isTimerLaunched_ = true;
        QTimer::singleShot( 500, this, SLOT( OnManageSounds() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::OnManageSounds
// Created: NPT 2013-08-08
// -----------------------------------------------------------------------------
void FirePlayer::OnManageSounds()
{
    boost::mutex::scoped_lock locker( mutex_ );
    for( auto it = currentEvents_.begin(); it != currentEvents_.end(); ++it )
        PlaySound( *it );
    isTimerLaunched_ = false;
}

// -----------------------------------------------------------------------------
// Name: FirePlayer::PlaySound
// Created: NPT 2013-08-12
// -----------------------------------------------------------------------------
void FirePlayer::PlaySound( const gui::SoundEvent& soundEvent )
{
    if( soundEvent.GetSignalType() == gui::SoundEvent::eStop )
    {
        // Do not call StopSound() here, stopping a playing sound is annoying,
        // let it just finish.
        int& count = loopingSounds_[ soundEvent.GetSoundType() ][ soundEvent.GetEntity() ];
        if( count > 0 )
            --count;
        return;
    }

    if( soundEvent.GetSignalType() == gui::SoundEvent::eStart 
        && ( !soundEvent.GetEntity() || profileFilter_.IsPerceived( *soundEvent.GetEntity() ) 
        && profileFilter_.IsVisible( *soundEvent.GetEntity() ) ) )
        loopingSounds_[ soundEvent.GetSoundType() ][ soundEvent.GetEntity() ]++;

    if( !CanPlaySound( soundEvent.GetSoundType() ) )
        return;

    if( !soundEvent.GetEntity() || profileFilter_.IsPerceived( *soundEvent.GetEntity() ) && profileFilter_.IsVisible( *soundEvent.GetEntity() ) )
    {
        soundManager_->PlaySound( soundEvent.GetSoundType() );
        lastTick_ = simulation_.GetCurrentTick();
    }
}

