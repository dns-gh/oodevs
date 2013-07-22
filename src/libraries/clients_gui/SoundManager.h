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

#pragma warning( push, 0 )
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#pragma warning( pop )

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
    void ChangeSoundsDirectory( const tools::Path& path );
    bool FindFile( const tools::Path& path, const std::string& name );
    bool IsPlaying( const std::string& channel );
    void PlayLoopSound( const std::string& soundName );
    void PlaySound( const std::string& soundName );
    void SetVolume( const std::string& channel, double value );
    void StopSound( const std::string& channel );
    //@}

public slots:
    void PlayPauseAllChannels( bool play );

private slots:
    void ReplaySound();
    void KillCurrentMediaObject();

private:
    //! @name Member data
    //@{
    std::map< std::string, double > volume_;
    std::map< std::string, Phonon::AudioOutput* > canals_;
    std::map< std::string, Phonon::MediaObject* > medias_;
    tools::Path defaultSoundsPath_;
    tools::Path currentSoundsPath_;
    tools::Path currentSound_;
    const std::vector< std::string > sounds_;
    //@}
};

}

#endif // __SoundManager_h_
