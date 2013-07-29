// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __FirePlayer_h_
#define __FirePlayer_h_

#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class SoundEvent;
    class SoundManager;
}

class Simulation;

// =============================================================================
/** @class  FirePlayer
    @brief  FirePlayer
*/
// Created: NPT 2013-07-16
// =============================================================================
class FirePlayer : public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< gui::SoundEvent >
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FirePlayer( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const Simulation& simulation );
    virtual ~FirePlayer();
    //@}

    //! @name Operations
    //@{
    gui::SoundManager& GetSoundManager();
    void PlayPauseSoundControl( bool play );
    //@}

private:
    //! @name Helpers
    //@{
    bool CanPlaySound( const std::string& channel );
    virtual void NotifyUpdated( const gui::SoundEvent& soundEvent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profileFilter_;
    std::auto_ptr< gui::SoundManager > soundManager_;
    const Simulation& simulation_;
    unsigned int lastTick_;
    //@}
};

#endif // __FirePlayer_h_
