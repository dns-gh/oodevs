// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __SimulationController_h_
#define __SimulationController_h_

#include "Simulation.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

namespace actions
{
    class Action_ABC;
}

class Publisher_ABC;
class Services;

// =============================================================================
/** @class  SimulationController
    @brief  SimulationController
*/
// Created: JSR 2013-03-26
// =============================================================================
class SimulationController : public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< Services >
                           , public tools::ElementObserver_ABC< Simulation::sEndTick >
                           , public tools::ElementObserver_ABC< Simulation >
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SimulationController( const Simulation& simulation, kernel::Controllers& controllers, Publisher_ABC& publisher );
    virtual ~SimulationController();
    //@}

    //! @name Operations
    //@{
    bool IsPaused() const;
    unsigned int GetFirstTick() const;
    unsigned int GetTickCount() const;
    unsigned int GetTickDuration() const;
    void Pause();
    void Resume();
    void Step();
    void ChangeTimeFactor( int factor );
    void SkipToDate( const QDateTime& dateTime );
    void SkipToTick( unsigned int tick );
    void RequestCheckpoint( const std::string& name );
    void ForceReplayDataRequest();
    void SendEnableVisionCones( bool value ) const;
    void PublishAction( const actions::Action_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Services& services );
    virtual void NotifyUpdated( const Simulation::sEndTick& endTick );
    void NotifyUpdated( const Simulation& simulation );
    //@}

private:
    //! @name Member data
    //@{
    const Simulation& simulation_;
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    bool hasReplay_;
    bool hasSimulation_;
    bool replayRequested_;
    //@}
};

#endif // __SimulationController_h_
