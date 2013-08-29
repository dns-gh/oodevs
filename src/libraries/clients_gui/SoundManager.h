// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SoundManager_h_
#define __SoundManager_h_

#include <boost/noncopyable.hpp>
#include <tools/Path.h>

#include <boost/shared_ptr.hpp>

namespace Phonon
{

class AudioOutput;
class MediaObject;

}

namespace gui
{

// =============================================================================
/** @class  SoundManager
    @brief  SoundManager
*/
// Created: NPT 2013-07-03
// =============================================================================
class SoundManager : public QObject,
                     private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SoundManager( const std::vector< std::string >& sounds );
    virtual ~SoundManager();
    //@}

    //! @name Operations
    //@{
    void SetSoundState( bool enabled );
    void ChangeSoundsDirectory( const tools::Path& path );
    bool IsPlaying( const std::string& channel );
    bool PlaySound( const std::string& soundName );
    void SetVolume( const std::string& channel, double value );
    void StopSound( const std::string& channel );
    //@}

public slots:
    void PlayPauseAllChannels( bool play );

private:
    //! @name Member data
    //@{
    std::map< std::string, double > volume_;
    std::map< std::string, boost::shared_ptr< Phonon::AudioOutput > > channels_;
    // NULL entries mean we tried to load the sound but failed to do
    // so. They are used to avoid future lookups.
    std::map< std::string, boost::shared_ptr< Phonon::MediaObject > > medias_;
    tools::Path defaultSoundsPath_;
    tools::Path currentSoundsPath_;
    const std::vector< std::string > sounds_;
    bool enabled_;
    //@}
};

}

#endif // __SoundManager_h_
