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

#include "gaming/Simulation.h"
#include "clients_gui/SoundPanel.h"
#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Entity_ABC;
}

namespace gui
{
    class SoundEvent;
    class SoundManager;
}

// =============================================================================
/** @class  FirePlayer
    @brief  FirePlayer
*/
// Created: NPT 2013-07-16
// =============================================================================
class FirePlayer : public QObject
                 , public gui::SoundPlayer
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< gui::SoundEvent >
                 , public tools::ElementObserver_ABC< Simulation::sStartTick >
                 , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FirePlayer( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const Simulation& simulation );
    virtual ~FirePlayer();
    //@}

    //! @name Operations
    //@{
    void PlayPauseSoundControl( bool play );
    virtual void SetVolume( const std::string& channel, double value );
    virtual void ChangeSoundsDirectory( const tools::Path& path );
    virtual void SetSoundState( bool enabled );
    //@}

private:
    //! @name Helpers
    //@{
    void PlaySound( const gui::SoundEvent& soundEvent );
    bool CanPlaySound( const std::string& channel );
    virtual void NotifyUpdated( const gui::SoundEvent& soundEvent );
    virtual void NotifyUpdated( const Simulation::sStartTick& tick );
    //@}

private slots:
    void OnManageSounds();

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profileFilter_;
    std::unique_ptr< gui::SoundManager > soundManager_;
    const Simulation& simulation_;
    unsigned int lastTick_;
    std::map< std::string, std::map< const kernel::Entity_ABC*, int > > loopingSounds_;
    boost::mutex mutex_;
    std::vector< gui::SoundEvent > currentEvents_;
    bool isTimerLaunched_;
    //@}
};

#endif // __FirePlayer_h_
