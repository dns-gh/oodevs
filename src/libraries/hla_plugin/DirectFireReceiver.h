// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef hla_plugin_DirectFireReceiver_h
#define hla_plugin_DirectFireReceiver_h

#include <hla/InteractionNotification_ABC.h>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace hla
{
namespace interactions
{
    struct MunitionDetonation;
}
    class RemoteAgentResolver_ABC;
    class LocalAgentResolver_ABC;
    class ContextFactory_ABC;

// =============================================================================
/** @class  DirectFireReceiver
    @brief  Direct fire receiver
*/
// Created: SLI 2011-09-26
// =============================================================================
class DirectFireReceiver : public ::hla::InteractionNotification_ABC< interactions::MunitionDetonation >
{
public:
    //! @name Constructors/Destructor
    //@{
             DirectFireReceiver( dispatcher::SimulationPublisher_ABC& publisher, const RemoteAgentResolver_ABC& remoteResolver,
                                 const LocalAgentResolver_ABC& localResolver, const ContextFactory_ABC& factory );
    virtual ~DirectFireReceiver();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( interactions::MunitionDetonation& interaction );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    const RemoteAgentResolver_ABC& remoteResolver_;
    const LocalAgentResolver_ABC& localResolver_;
    const ContextFactory_ABC& factory_;
    //@}
};

}
}

#endif // hla_plugin_DirectFireReceiver_h
