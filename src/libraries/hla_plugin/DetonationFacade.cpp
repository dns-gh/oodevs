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
#include "InteractionSender_ABC.h"
#include "InteractionBuilder.h"
#include "DirectFireReceiver.h"
#include "DirectFireSender.h"
#include "IndirectFireReceiver.h"
#include "IndirectFireSender.h"
#include <hla/Interaction.h>

using namespace plugins::hla;

namespace
{
    template< typename T >
    class InteractionSender : public InteractionSender_ABC< T >
    {
    public:
        InteractionSender( ::hla::InteractionNotification_ABC< T >& receiver, Federate_ABC& federate )
            : interaction_( new ::hla::Interaction< T >( receiver ) )
        {
            InteractionBuilder builder;
            builder.Build( federate, *interaction_ );
        }
        virtual ~InteractionSender() {}
        virtual void Send( const T& interaction )
        {
            interaction_->Send( interaction );
        }
    private:
        std::auto_ptr< ::hla::Interaction< T > > interaction_;
    };
}

// -----------------------------------------------------------------------------
// Name: DetonationFacade constructor
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
DetonationFacade::DetonationFacade( Federate_ABC& federate, dispatcher::SimulationPublisher_ABC& publisher, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                        const RemoteAgentResolver_ABC& remoteResolver, const LocalAgentResolver_ABC& localResolver,
                                        const ContextFactory_ABC& contextFactory, const MunitionTypeResolver_ABC& munitionTypeResolver,
                                        RemoteAgentSubject_ABC& remoteAgentSubject, const std::string& federateName )
    : pMunitionDetonation_  ( new InteractionSender< interactions::MunitionDetonation >( *this, federate ) )
    , pDirectFireReceiver_  ( new DirectFireReceiver( publisher, remoteResolver, localResolver, contextFactory ) )
    , pIndirectFireReceiver_( new IndirectFireReceiver( publisher, contextFactory, munitionTypeResolver ) )
    , pDirectFireSender_    ( new DirectFireSender( *pMunitionDetonation_, remoteResolver, localResolver, remoteAgentSubject, controller, federateName ) )
    , pIndirectFireSender_  ( new IndirectFireSender( *pMunitionDetonation_, controller, federateName, munitionTypeResolver, localResolver ) )
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
