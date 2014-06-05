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
#include "protocol/Protocol.h"

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
    class Logger_ABC;
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
            TimelinePlugin( dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulationPublisher, const tools::ExerciseConfig& config, xml::xistream& xis, dispatcher::Logger_ABC& logger );
    virtual ~TimelinePlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
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
    std::unique_ptr< kernel::Controller > controller_;
    std::unique_ptr< ActionPublisher > actions_;
    std::unique_ptr< ScenarioManager > scenario_;
    std::unique_ptr< PublisherActor > publisher_;
	dispatcher::Logger_ABC*  logger_;
    //@}
};

}
}

#endif // __plugins_timeline_TimelinePlugin_h_
