// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef DISPATCHER_PLUGINCONTAINER_H
#define DISPATCHER_PLUGINCONTAINER_H

#include "Plugin_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace dispatcher
{

// PluginContainer lets you build a plugins tree where a parent node can prevent
// messages to reach its children by overriding the Forward* methods.
// Other notifications and actions pass through unaffected.
class PluginContainer : public Plugin_ABC
{
public:
    //! @name Operations
    //@{
    void Add( const boost::shared_ptr< Plugin_ABC >& plugin );
    void AddHandler( const boost::shared_ptr< MessageHandler_ABC >& handler );

    virtual void Receive( const sword::SimToClient& message );
    virtual void Receive( const sword::AarToClient& message );
    virtual void Receive( const sword::MessengerToClient& message );

    virtual void Register( dispatcher::Services& services );

    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client,
            const std::string& link, Profile_ABC& profile, bool uncounted );
    virtual void NotifyClientLeft( ClientPublisher_ABC& client,
            const std::string& link, bool uncounted );
    virtual void NotifySimulationLeft();

    virtual void Update();
    virtual void Close();
    virtual bool HandleClientToSim( const sword::ClientToSim&, RewritingPublisher_ABC&,
            ClientPublisher_ABC& );
    virtual bool HandleClientToReplay( const sword::ClientToReplay&,
            RewritingPublisher_ABC&, ClientPublisher_ABC& );
    //@}

private:
    virtual bool ForwardSimToClient( const sword::SimToClient& message );

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< Plugin_ABC > > plugins_;
    std::vector< boost::shared_ptr< MessageHandler_ABC > > handlers_;
    //@}
};

}

#endif //DISPATCHER_PLUGINCONTAINER_H

