// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __plugins_timeline_TimelinePlugin_h_
#define __plugins_timeline_TimelinePlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "protocol/protocol.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class StaticModel;
    class Controller;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace timeline
{
    class PublisherActor;
    class ActionPublisher;
    class ScenarioManager;

// =============================================================================
/** @class  TimelinePlugin
    @brief  TimelinePlugin
*/
// Created: JCR 2010-05-29
// =============================================================================
class TimelinePlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            TimelinePlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, const tools::ExerciseConfig& config, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulationPublisher );
    virtual ~TimelinePlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const sword::SimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelinePlugin( const TimelinePlugin& );            //!< Copy constructor
    TimelinePlugin& operator=( const TimelinePlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Load( const tools::ExerciseConfig& config, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::Controller > controller_;
    std::auto_ptr< ActionPublisher > actions_;
    std::auto_ptr< ScenarioManager > scenario_;
    std::auto_ptr< PublisherActor > publisher_;
    //@}
};

}
}

#endif // __plugins_timeline_TimelinePlugin_h_
