// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PluginFactory_ABC_h_
#define __PluginFactory_ABC_h_

#include <memory>

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
    class Model;
    class Plugin_ABC;
    class SimulationPublisher_ABC;
    class ClientPublisher_ABC;
    class LinkResolver_ABC; 

// =============================================================================
/** @class  PluginFactory_ABC
    @brief  PluginFactory_ABC
*/
// Created: SBO 2008-02-28
// =============================================================================
class PluginFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PluginFactory_ABC() {}
    virtual ~PluginFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< Plugin_ABC > Create( const std::string& name, xml::xistream& xis, const Config& config, Model& model, SimulationPublisher_ABC& simulation, ClientPublisher_ABC& clients, tools::MessageDispatcher_ABC& client, LinkResolver_ABC& resolver) const = 0;
    //@}
};

}

#endif // __PluginFactory_ABC_h_
