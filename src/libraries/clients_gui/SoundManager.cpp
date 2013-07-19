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
    if( medias_[ soundName ]->remainingTime() <= 0 )
    {
        medias_[ soundName ]->setCurrentSource( Phonon::MediaSource( currentSound_.Normalize().ToUTF8().c_str() ) );
        Phonon::AudioOutput* audio = new Phonon::AudioOutput( Phonon::MusicCategory );
        audio->setVolume( volume_[ soundName ] );
        Phonon::createPath( medias_[ soundName ], audio );
        medias_[ soundName ]->play();
        canals_[ soundName ] = audio;
    }
}

// -----------------------------------------------------------------------------
// Name: SoundManager::SetVolume
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundManager::SetVolume( const std::string& canal, double value )
{
    if( canals_[ canal ] )
        canals_[ canal ]->setVolume( value );
    volume_[ canal ] = value;
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
