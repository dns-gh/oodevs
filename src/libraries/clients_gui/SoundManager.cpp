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
#include "tools/GeneralConfig.h"

#pragma warning( push, 0 )
#include <Qt/qapplication.h>
#include <phonon/mediaobject.h>
#pragma warning( pop )

#include <boost/assign/list_of.hpp>

SoundManager* SoundManager::instance_ = NULL;

namespace
{
    std::map< std::string, std::string > sounds = boost::assign::map_list_of
        ( "directfire", "directfire.wav" )
        ( "indirectsmoke", "indirectsmoke.wav" )
        ( "indirectexplosive", "indirectexplosive.wav" )
        ( "indirectillumination", "indirectillumination.wav" )
        ( "indirecteffect", "indirecteffect.wav" );
}

// -----------------------------------------------------------------------------
// Name: SoundManager constructor
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
SoundManager::SoundManager()
{
    for( auto it = sounds.begin(); it != sounds.end(); ++it )
        volume_[ it->first ] = 0.5;
}


// -----------------------------------------------------------------------------
// Name: SoundManager destructor
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
SoundManager::~SoundManager()
{
    instance_ = 0;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::GetInstance
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
SoundManager* SoundManager::GetInstance()
{
    if ( instance_ == NULL )
    {
        instance_ = new SoundManager;
        instance_->defaultSoundsPath_ = tools::GeneralConfig::BuildResourceChildFile( "sounds" );
        instance_->currentSoundsPath_ = tools::GeneralConfig::BuildResourceChildFile( "sounds" );
    }
    return instance_;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::PlaySound
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
void SoundManager::PlaySound( const std::string& soundName )
{
    tools::Path soundPath = currentSoundsPath_ / sounds[ soundName ].c_str();
    if( !soundPath.Exists() )
        soundPath = defaultSoundsPath_ / sounds[ soundName ].c_str();
    Phonon::MediaObject* mediaObject = new Phonon::MediaObject();
    mediaObject->setCurrentSource( Phonon::MediaSource( soundPath.ToUTF8().c_str() ) );
    Phonon::AudioOutput* audioOutput = new Phonon::AudioOutput( Phonon::MusicCategory );
    audioOutput->setVolume( volume_[ soundName ] );
    Phonon::createPath(mediaObject, audioOutput);
    mediaObject->play();
    canals_[ soundName ] = audioOutput;
}

// -----------------------------------------------------------------------------
// Name: SoundManager::SetVolume
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::SetVolume( const std::string& canal, double value )
{
    if( canals_[ canal ] )
    {
        volume_[ canal ] = value;
        canals_[ canal ]->setVolume( value );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::ChangeSoundsDirectory
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::ChangeSoundsDirectory( const tools::Path& path )
{
    currentSoundsPath_ = path;
}
