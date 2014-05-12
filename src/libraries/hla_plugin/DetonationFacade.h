// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_InteractionsFacade_h
#define plugins_hla_InteractionsFacade_h

#include <memory>
#include <string>
#include <hla/InteractionNotification_ABC.h>

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace tools
{
    template< typename Category > class MessageController_ABC;
}

namespace sword
{
    class SimToClient_Content;
}

namespace plugins
{
namespace hla
{
    class RemoteAgentResolver_ABC;
    class LocalAgentResolver_ABC;
    class DotationTypeResolver_ABC;
    class ContextFactory_ABC;
    class RemoteAgentSubject_ABC;
    class DirectFireSender;
    class IndirectFireSender;
    class InteractionBuilder;
    template< typename Interaction > class InteractionSender_ABC;
    class AgentSubject_ABC;

namespace interactions
{
    struct MunitionDetonation;
    struct WeaponFire;
}

// =============================================================================
/** @class  DetonationFacade
    @brief  Interactions facade
*/
// Created: SLI 2011-09-27
// =============================================================================
class DetonationFacade : private ::hla::InteractionNotification_ABC< interactions::MunitionDetonation >
                       , private ::hla::InteractionNotification_ABC< interactions::WeaponFire >
{
public:
    //! @name Constructors/Destructor
    //@{
             DetonationFacade( dispatcher::SimulationPublisher_ABC& publisher, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                               const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                               const ContextFactory_ABC& contextFactory, const DotationTypeResolver_ABC& munitionTypeResolver,
                               RemoteAgentSubject_ABC& remoteAgentSubject, const std::string& federateName, const InteractionBuilder& builder,
                               AgentSubject_ABC& agentSubject );
    virtual ~DetonationFacade();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::MunitionDetonation& interaction );
    virtual void Receive( interactions::WeaponFire& interaction );
    //@}

private:
    //! @name Types
    //@{
    typedef InteractionSender_ABC< interactions::MunitionDetonation > T_MunitionDetonationSender;
    typedef InteractionSender_ABC< interactions::WeaponFire > T_WeaponFireSender;
    typedef ::hla::InteractionNotification_ABC< interactions::MunitionDetonation > T_Notification;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< T_MunitionDetonationSender > pMunitionDetonation_;
    std::unique_ptr< T_WeaponFireSender > pWeaponFire_;
    std::unique_ptr< T_Notification > pDirectFireReceiver_;
    std::unique_ptr< T_Notification > pIndirectFireReceiver_;
    std::unique_ptr< DirectFireSender > pDirectFireSender_;
    std::unique_ptr< IndirectFireSender > pIndirectFireSender_;
    //@}
};

}
}

#endif // plugins_hla_InteractionsFacade_h
