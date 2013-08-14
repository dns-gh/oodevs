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
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#pragma warning( pop )

#include <boost/bind.hpp>

using namespace gui;

namespace
{

bool CheckSoundFile( const tools::Path& path, const std::string& sound, tools::Path& found )
{
    if( path.Exists() && path.BaseName().ToUTF8() == sound )
    {
        found = path;
        return true;
    }
    return false;
}

tools::Path FindSoundFile( const tools::Path& dir, const std::string& sound )
{
    tools::Path found;
    dir.Apply( boost::bind( &CheckSoundFile, _1, boost::cref( sound ), boost::ref( found )));
    return found;
}

}  // namespace

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
        if( !it->second )
            continue;
        if( play )
            it->second->play();
        else
            it->second->pause();
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::PlaySound
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
void SoundManager::PlaySound( const std::string& soundName )
{
    auto it = medias_.find( soundName );
    auto& media = medias_[ soundName ]; // leave a NULL entry if we cannot load
    if( it == medias_.end() )
    {
        tools::Path path = FindSoundFile( currentSoundsPath_, soundName );
        if( path.IsEmpty() )
            path = FindSoundFile( defaultSoundsPath_, soundName );
        if( !path.IsEmpty() )
        {
            media.reset( new Phonon::MediaObject() );
            media->setCurrentSource( QString( path.Normalize().ToUTF8().c_str() ) );
        }
    }
    if( !media )
        // We tried to load it already, and failed.
        return;

    if( !IsPlaying( soundName ) )
    {
        auto& canal = canals_[ soundName ];
        if( !canal )
        {
            canal.reset( new Phonon::AudioOutput( Phonon::MusicCategory ));
            Phonon::createPath( media.get(), canal.get() );
        }
        media->play();
        media->seek( 0 );
        canal->setVolume( volume_[ soundName ] );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::SetVolume
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::SetVolume( const std::string& channel, double value )
{
    auto it = canals_.find( channel );
    if( it != canals_.end() )
        it->second->setVolume( value );
    volume_[ channel ] = value;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::ChangeSoundsDirectory
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::ChangeSoundsDirectory( const tools::Path& path )
{
    canals_.clear();
    medias_.clear();
    currentSoundsPath_ = path;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::IsPlaying
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
bool SoundManager::IsPlaying( const std::string& channel )
{
    auto it = medias_.find( channel );
    if( it == medias_.end() || !it->second )
        return false;
    return it->second->state() == Phonon::PlayingState || it->second->state() == Phonon::BufferingState;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::StopSound
// Created: NPT 2013-07-22
// -----------------------------------------------------------------------------
void SoundManager::StopSound( const std::string& channel )
{
    auto it = medias_.find( channel );
    if( it == medias_.end() || !it->second )
        return;
    it->second->stop();
}
