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
#pragma warning( pop )

// =============================================================================
/** @class  SoundManager
    @brief  SoundManager
*/
// Created: NPT 2013-07-03
// =============================================================================
class SoundManager : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             SoundManager();
    virtual ~SoundManager();
    //@}

    //! @name Operations
    //@{
    static SoundManager* GetInstance();
    void PlaySound( const std::string& filePath, int tic );
    void SetVolume( const std::string& canal, double value );
    void ChangeSoundsDirectory( const tools::Path& path );
    bool FindFile( const tools::Path& path, const std::string& name );
    bool CanPlaySound( const std::string& canal, int currentTic );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    static SoundManager* instance_;
    std::map< std::string, double > volume_;
    std::map< std::string, double > lastPlayTic_;
    std::map< std::string, Phonon::AudioOutput* > canals_;
    tools::Path defaultSoundsPath_;
    tools::Path currentSoundsPath_;
    tools::Path currentSound_;
    //@}
};

#endif // __SoundManager_h_
