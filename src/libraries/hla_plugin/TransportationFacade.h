// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationFacade_h
#define plugins_hla_TransportationFacade_h

#include <hla/InteractionNotification_ABC.h>
#include <memory>

namespace xml
{
    class xisubstream;
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
    class MissionResolver_ABC;
    class CallsignResolver_ABC;
    class Subordinates_ABC;
    class Federate_ABC;
    class ContextFactory_ABC;
    class TransportationController_ABC;
    template< typename T > class InteractionSender_ABC;
    class NetnRequestConvoySender;
    class NetnOfferConvoySender;
    class Transporters_ABC;

namespace interactions
{
    struct NetnRequestConvoy;
    struct NetnOfferConvoy;
}

// =============================================================================
/** @class  TransportationFacade
    @brief  Transportation facade
*/
// Created: SLI 2011-10-10
// =============================================================================
class TransportationFacade : private ::hla::InteractionNotification_ABC< interactions::NetnRequestConvoy >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationFacade( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                   tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                   const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                   Federate_ABC& federate, const ContextFactory_ABC& contextFactory, const Transporters_ABC& transporters );
    virtual ~TransportationFacade();
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( interactions::NetnRequestConvoy& request );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< TransportationController_ABC > pTransportationController_;
    std::auto_ptr< InteractionSender_ABC< interactions::NetnRequestConvoy > > pNetnRequestConvoy_;
    std::auto_ptr< NetnRequestConvoySender > pNetnRequestConvoySender_;
    std::auto_ptr< InteractionSender_ABC< interactions::NetnOfferConvoy > > pNetnOfferConvoy_;
    std::auto_ptr< NetnOfferConvoySender > pNetnOfferConvoySender_;
    //@}
};

}
}

#endif // plugins_hla_TransportationFacade_h
