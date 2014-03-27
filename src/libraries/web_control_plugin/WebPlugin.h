// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WEB_PLUGIN_H
#define WEB_PLUGIN_H

#include "dispatcher/Plugin_ABC.h"
#include <memory>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace sword
{
    class SimToClient;
}

namespace xml
{
    class xistream;
};

namespace plugins
{
namespace web_control
{
    class WebControl;
    class MongooseServer;

// =============================================================================
/** @class  WebPlugin
    @brief  WebPlugin
*/
// Created: BAX 2012-02-28
// =============================================================================
class WebPlugin : public dispatcher::Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             WebPlugin( dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis );
    virtual ~WebPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& client );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, const std::string& link,
                                            dispatcher::Profile_ABC& profile, unsigned int clientId, bool uncounted );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client, const std::string& link, bool uncounted );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< WebControl > control_;
    std::auto_ptr< MongooseServer > server_;
    //@}
};
}
}

#endif // WEB_CONTROL_PLUGIN_H
