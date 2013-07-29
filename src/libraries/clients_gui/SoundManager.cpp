// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SoundManager.h"
#include "moc_SoundManager.cpp"
#include "tools/GeneralConfig.h"

#pragma warning( push, 0 )
#include <Qt/qapplication.h>
#include <phonon/mediaobject.h>
#pragma warning( pop )

#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SoundManager constructor
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
SoundManager::SoundManager( const std::vector< std::string >& sounds )
    : sounds_( sounds )
    , defaultSoundsPath_( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) )
    , currentSoundsPath_( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) )
{
    for( auto it = sounds.begin(); it != sounds.end(); ++it )
        volume_[ *it ] = 0.5;
}


// -----------------------------------------------------------------------------
// Name: SoundManager destructor
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
SoundManager::~SoundManager()
{
}

// -----------------------------------------------------------------------------
// Name: SoundManager::PauseAllChannels
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void SoundManager::PlayPauseAllChannels( bool play )
{
    for( auto it = medias_.begin(); it != medias_.end(); ++it )
    {
        if( it->second && play )
            it->second->play();
        if( it->second && !play )
            it->second->pause();
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::PlaySound
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
void SoundManager::PlaySound( const std::string& soundName )
{
    currentSound_ = tools::Path();
    currentSoundsPath_.Apply( boost::bind( &SoundManager::FindFile, this, _1, boost::cref( soundName ) ), false );
    if( currentSound_.IsEmpty() )
        defaultSoundsPath_.Apply( boost::bind( &SoundManager::FindFile, this, _1, boost::cref( soundName ) ), false );
    if( !currentSound_.Exists() )
        return;
    if( !medias_[ soundName ] )
        medias_[ soundName ] = new Phonon::MediaObject();
    if( !IsPlaying( soundName ) )
    {
        medias_[ soundName ]->setCurrentSource( QString( currentSound_.Normalize().ToUTF8().c_str() ) );
        if( !canals_[ soundName ] )
        {
            Phonon::AudioOutput* audio = new Phonon::AudioOutput( Phonon::MusicCategory );
            Phonon::createPath( medias_[ soundName ], audio );
            canals_[ soundName ] = audio;
        }
        medias_[ soundName ]->play();
        canals_[ soundName ]->setVolume( volume_[ soundName ] );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::PlayLoopSound
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void SoundManager::PlayLoopSound( const std::string& soundName )
{
    PlaySound( soundName );
    connect( medias_[ soundName ], SIGNAL( finished() ), SLOT( ReplaySound() ) );
}

// -----------------------------------------------------------------------------
// Name: SoundManager::SetVolume
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::SetVolume( const std::string& channel, double value )
{
    if( canals_[ channel ] )
        canals_[ channel ]->setVolume( value );
    volume_[ channel ] = value;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::ChangeSoundsDirectory
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::ChangeSoundsDirectory( const tools::Path& path )
{
    currentSoundsPath_ = path;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::FindFile
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
bool SoundManager::FindFile( const tools::Path& path, const std::string& name )
{
    if( !path.Exists() )
        return false;
    if( path.BaseName().ToUTF8() == name )
    {
        currentSound_ = path;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::IsPlaying
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
bool SoundManager::IsPlaying( const std::string& channel )
{
    return medias_[ channel ] && 
        ( medias_[ channel ]->state() == Phonon::PlayingState ||
          medias_[ channel ]->state() == Phonon::BufferingState );
}

// -----------------------------------------------------------------------------
// Name: SoundManager::StopSound
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void SoundManager::StopSound( const std::string& channel )
{
    connect( medias_[ channel ], SIGNAL( finished() ), SLOT( KillCurrentMediaObject() ) );
}

// -----------------------------------------------------------------------------
// Name: SoundManager::ReplaySound
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void SoundManager::ReplaySound()
{
    if( Phonon::MediaObject* mediaObject = dynamic_cast< Phonon::MediaObject* >( QObject::sender() ) )
    {
        mediaObject->stop();
        mediaObject->play();
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::KillCurrentMediaObject
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void SoundManager::KillCurrentMediaObject()
{
    if( Phonon::MediaObject* mediaObject = dynamic_cast< Phonon::MediaObject* >( QObject::sender() ) )
    {
        mediaObject->clearQueue();
        mediaObject->clear();
    }
}
