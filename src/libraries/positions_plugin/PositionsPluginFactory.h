// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PositionsPluginFactory_h_
#define __PositionsPluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace positions
{
// =============================================================================
/** @class  PositionsPluginFactory
    @brief  PositionsPluginFactory
*/
// Created: ABR 2011-04-04
// =============================================================================
class PositionsPluginFactory : public dispatcher::PluginFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PositionsPluginFactory();
    virtual ~PositionsPluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< dispatcher::Plugin_ABC > Create(
        const std::string& name, xml::xistream& xis, const dispatcher::Config& config,
        dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel,
        dispatcher::SimulationPublisher_ABC& simulation,
        dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client,
        dispatcher::LinkResolver_ABC& resolver,
        dispatcher::CompositeRegistrable& registrables ) const;
    //@}
};

}
}

#endif // __PositionsPluginFactory_h_
