// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA GROUP
//
// *****************************************************************************

#ifndef __plugins_timelinen_TimelinePluginFactory_h_
#define __plugins_timelinen_TimelinePluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace timeline
{

// =============================================================================
/** @class  TimelinePluginFactory
    @brief  TimelinePluginFactory
*/
// Created: JCR 2010-05-29
// =============================================================================
class TimelinePluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePluginFactory();
    virtual ~TimelinePluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name,
                                                            xml::xistream& xis,
                                                            const dispatcher::Config& config,
                                                            dispatcher::Model_ABC& model,
                                                            const kernel::StaticModel& staticModel, 
                                                            dispatcher::SimulationPublisher_ABC& simulationPublisher,
                                                            dispatcher::ClientPublisher_ABC& clients,
                                                            tools::MessageDispatcher_ABC& client,
                                                            dispatcher::LinkResolver_ABC& resolver,
                                                            dispatcher::CompositeRegistrable& registrables ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelinePluginFactory( const TimelinePluginFactory& );            //!< Copy constructor
    TimelinePluginFactory& operator=( const TimelinePluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __plugins_timelinen_TimelinePluginFactory_h_
