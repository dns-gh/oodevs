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
    class Log;
}

namespace dispatcher
{
    class SimulationPublisher_ABC;
    class AuthenticatedLinkResolver_ABC;
    class OrderResolver_ABC;
    class ClientsNetworker;

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
             DispatcherPlugin( SimulationPublisher_ABC& simulation, ClientsNetworker& clients,
                               AuthenticatedLinkResolver_ABC& resolver, OrderResolver_ABC& order,
                               tools::Log& log );
    virtual ~DispatcherPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Register( dispatcher::Services& services );
    //@}

private:
    //! @name Helpers
    //@{
    void OnReceive( const std::string& link, sword::ClientToSim& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    SimulationPublisher_ABC& simulation_;
    AuthenticatedLinkResolver_ABC& resolver_;
    OrderResolver_ABC& order_;
    ClientsNetworker& clients_;
    //@}
};

}

#endif // __DispatcherPlugin_h_
