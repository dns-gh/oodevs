// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "InteractionsFacade.h"
#include "MunitionDetonation.h"
#include "DirectFireReceiver.h"
#include "DirectFireSender.h"
#include "IndirectFireReceiver.h"
#include "IndirectFireSender.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: InteractionsFacade constructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
InteractionsFacade::InteractionsFacade( Federate_ABC& federate, dispatcher::SimulationPublisher_ABC& publisher, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                        const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                                        const ContextFactory_ABC& contextFactory, const MunitionTypeResolver_ABC& munitionTypeResolver,
                                        RemoteAgentSubject_ABC& remoteAgentSubject, const std::string& federateName )
    : pMunitionDetonation_  ( new MunitionDetonation( federate, *this ) )
    , pDirectFireReceiver_  ( new DirectFireReceiver( publisher, remoteResolver, localResolver, contextFactory ) )
    , pIndirectFireReceiver_( new IndirectFireReceiver( publisher, contextFactory, munitionTypeResolver ) )
    , pDirectFireSender_    ( new DirectFireSender( *pMunitionDetonation_, remoteResolver, localResolver, remoteAgentSubject, controller, federateName ) )
    , pIndirectFireSender_  ( new IndirectFireSender( *pMunitionDetonation_, controller, federateName, munitionTypeResolver, localResolver ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionsFacade destructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
InteractionsFacade::~InteractionsFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InteractionsFacade::Receive
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void InteractionsFacade::Receive( interactions::MunitionDetonation& interaction )
{
    pDirectFireReceiver_->Receive( interaction );
    pIndirectFireReceiver_->Receive( interaction );
}
