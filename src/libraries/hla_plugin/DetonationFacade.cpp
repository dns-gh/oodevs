// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "DetonationFacade.h"
#include "Interactions.h"
#include "InteractionSender.h"
#include "InteractionBuilder.h"
#include "DirectFireReceiver.h"
#include "DirectFireSender.h"
#include "IndirectFireReceiver.h"
#include "IndirectFireSender.h"
#include <hla/Interaction.h>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: DetonationFacade constructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
DetonationFacade::DetonationFacade( dispatcher::SimulationPublisher_ABC& publisher, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                    const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                                    const ContextFactory_ABC& contextFactory, const DotationTypeResolver_ABC& munitionTypeResolver,
                                    RemoteAgentSubject_ABC& remoteAgentSubject, const std::string& federateName, const InteractionBuilder& builder,
                                    AgentSubject_ABC& agentSubject, dispatcher::Logger_ABC& logger )
    : pMunitionDetonation_  ( new InteractionSender< interactions::MunitionDetonation >( *this, builder ) )
    , pWeaponFire_          ( new InteractionSender< interactions::WeaponFire >( *this, builder ) )
    , pDirectFireReceiver_  ( new DirectFireReceiver( publisher, remoteResolver, localResolver, contextFactory ) )
    , pIndirectFireReceiver_( new IndirectFireReceiver( publisher, contextFactory, munitionTypeResolver, logger  ) )
    , pDirectFireSender_    ( new DirectFireSender( *pMunitionDetonation_, *pWeaponFire_, remoteResolver, localResolver, remoteAgentSubject, controller, federateName, munitionTypeResolver, agentSubject ) )
    , pIndirectFireSender_  ( new IndirectFireSender( *pMunitionDetonation_, controller, federateName, munitionTypeResolver, localResolver) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetonationFacade destructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
DetonationFacade::~DetonationFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetonationFacade::Receive
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void DetonationFacade::Receive( interactions::MunitionDetonation& interaction )
{
    pDirectFireReceiver_->Receive( interaction );
    pIndirectFireReceiver_->Receive( interaction );
}

// -----------------------------------------------------------------------------
// Name: DetonationFacade::Receive
// Created: AHC 2012-11-15
// -----------------------------------------------------------------------------
void DetonationFacade::Receive( interactions::WeaponFire&  )
{
    // NOTHING
}
