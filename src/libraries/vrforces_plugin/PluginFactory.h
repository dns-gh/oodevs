// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PluginFactory_h_
#define __PluginFactory_h_

#include "dispatcher/PluginFactory_ABC.h"

namespace plugins
{
namespace vrforces
{

// =============================================================================
/** @class  PluginFactory
    @brief  PluginFactory
*/
// Created: SBO 2011-01-19
// =============================================================================
class PluginFactory : public dispatcher::PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginFactory();
    virtual ~PluginFactory();
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< dispatcher::Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const dispatcher::Config& config, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& simulation, dispatcher::ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, dispatcher::LinkResolver_ABC& resolver, dispatcher::CompositeRegistrable& registrables ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginFactory( const PluginFactory& );            //!< Copy constructor
    PluginFactory& operator=( const PluginFactory& ); //!< Assignment operator
    //@}
};

}
}

#endif // __PluginFactory_h_
