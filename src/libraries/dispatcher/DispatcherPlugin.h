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
#include "protocol/Protocol.h"

namespace tools
{
    class MessageDispatcher_ABC;
}

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
    class LinkResolver_ABC;
    class RotatingLog;
    class OrderResolver_ABC;

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
             DispatcherPlugin( Model& model, SimulationPublisher_ABC& simulation,
                               tools::MessageDispatcher_ABC& clientCommands,
                               LinkResolver_ABC& links, OrderResolver_ABC& order, RotatingLog& log );
    virtual ~DispatcherPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );

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
    void OnReceive( const std::string& link, const sword::ClientToSim& asnMsg );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< ClientPublisher_ABC* > T_Clients;
    typedef T_Clients::const_iterator      CIT_Clients;
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    SimulationPublisher_ABC& simulation_;
    LinkResolver_ABC& links_;
    OrderResolver_ABC& order_;
    T_Clients clients_;
    //@}
};

}

#endif // __DispatcherPlugin_h_
