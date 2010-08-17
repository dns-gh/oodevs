// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DispatcherPlugin_h_
#define __DispatcherPlugin_h_

#include "Plugin_ABC.h"
#include <protocol/protocol.h>

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
    class LinkResolver_ABC;

// =============================================================================
/** @class  DispatcherPlugin
    @brief  DispatcherPlugin
*/
// Created: AGE 2007-08-24
// =============================================================================
class DispatcherPlugin : public Plugin_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DispatcherPlugin( Model& model, SimulationPublisher_ABC& simulation, tools::MessageDispatcher_ABC& clientCommands, LinkResolver_ABC& links );
    virtual ~DispatcherPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );

    virtual void NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& profile );
    virtual void NotifyClientLeft( ClientPublisher_ABC& client );

    virtual void Register( dispatcher::Services& services );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DispatcherPlugin( const DispatcherPlugin& );            //!< Copy constructor
    DispatcherPlugin& operator=( const DispatcherPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnReceive( const std::string& link, const MsgsClientToSim::MsgClientToSim& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    SimulationPublisher_ABC& simulation_;
    LinkResolver_ABC& links_;
    std::set< ClientPublisher_ABC* > clients_;
    //@}
};

}

#endif // __DispatcherPlugin_h_
