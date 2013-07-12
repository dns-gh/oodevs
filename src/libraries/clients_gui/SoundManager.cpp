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
#include <boost/bind.hpp>

SoundManager* SoundManager::instance_ = NULL;

namespace
{
    const std::vector< std::string > sounds = boost::assign::list_of< std::string >
        ( "directfire" )
        ( "indirectsmoke" )
        ( "indirectexplosive" )
        ( "indirectillumination" )
        ( "indirecteffect" );
}

// -----------------------------------------------------------------------------
// Name: SoundManager constructor
// Created: NPT 2013-07-03
// -----------------------------------------------------------------------------
SoundManager::SoundManager()
{
    for( auto it = ::sounds.begin(); it != ::sounds.end(); ++it )
    {
            volume_[ *it ] = 0.5;
            lastPlayTic_[ *it ] = 0;
    }
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
void SoundManager::PlaySound( const std::string& soundName, int tic )
{
    currentSound_ = tools::Path();
    currentSoundsPath_.Apply( boost::bind( &SoundManager::FindFile, this, _1, boost::cref( soundName ) ), false );
    if( currentSound_.IsEmpty() )
        defaultSoundsPath_.Apply( boost::bind( &SoundManager::FindFile, this, _1, boost::cref( soundName ) ), false );
    if( !currentSound_.Exists() )
        return;
    Phonon::MediaObject* mediaObject = new Phonon::MediaObject();
    mediaObject->setCurrentSource( Phonon::MediaSource( currentSound_.Normalize().ToUTF8().c_str() ) );
    Phonon::AudioOutput* audioOutput = new Phonon::AudioOutput( Phonon::MusicCategory );
    audioOutput->setVolume( volume_[ soundName ] );
    Phonon::createPath(mediaObject, audioOutput);
    if( CanPlaySound( soundName, tic ) )
    {
        mediaObject->play();
        canals_[ soundName ] = audioOutput;
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

// -----------------------------------------------------------------------------
// Name: SoundManager::CanPlaySound
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
bool SoundManager::CanPlaySound( const std::string& canal, int currentTic )
{
    if( lastPlayTic_[ canal ] < currentTic )
    {
        lastPlayTic_[ canal ] = currentTic;
        return true;
    }
    return false;
}
