// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef plugins_vision_VisionPlugin_h
#define plugins_vision_VisionPlugin_h

#include "dispatcher/Plugin_ABC.h"
#include <boost/scoped_ptr.hpp>
#include <map>

namespace sword
{
    class SimToClient;
    class ClientToSim;
    class ControlEnableVisionCones;
    class UnitId;
}

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Model_ABC;
    class AuthenticatedLinkResolver_ABC;
}

namespace client
{
    class ListEnabledVisionConesAck;
}

namespace plugins
{
namespace vision
{
    class Clients;
    class Units;

// =============================================================================
/** @class  VisionPlugin
    @brief  Vision cones plugin

    This plugin maintains a list of requested unit vision cones per client and
    dispatches accordingly when receiving the vision cones updates from the
    simulation.
*/
// Created: MCO 2013-08-21
// =============================================================================
class VisionPlugin : public dispatcher::Plugin_ABC
{
public:
             VisionPlugin( const dispatcher::Model_ABC& model, tools::MessageDispatcher_ABC& dispatcher, dispatcher::AuthenticatedLinkResolver_ABC& resolver );
    virtual ~VisionPlugin();

    virtual bool Filter( const sword::SimToClient& message );
    virtual void Receive( const sword::SimToClient& message );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& publisher, const std::string& link );

private:
    void OnReceive( const std::string& link, const sword::ClientToSim& message );

    bool Validate( dispatcher::ClientPublisher_ABC& publisher, const sword::ControlEnableVisionCones& message, int context, unsigned int client ) const;
    void Register( dispatcher::ClientPublisher_ABC& publisher, const sword::UnitId& unitId, bool activate );
    void Register( dispatcher::ClientPublisher_ABC& publisher, bool activate );

private:
    const dispatcher::Model_ABC& model_;
    dispatcher::AuthenticatedLinkResolver_ABC& resolver_;
    std::map< unsigned int, sword::SimToClient > cones_;
    boost::scoped_ptr< Clients > clients_;
    boost::scoped_ptr< Units > units_;
};

}
}

#endif // plugins_vision_VisionPlugin_h
