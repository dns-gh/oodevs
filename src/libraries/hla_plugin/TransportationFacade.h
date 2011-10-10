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

#include <boost/noncopyable.hpp>
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
namespace interactions
{
    struct NetnRequestConvoy;
}

// =============================================================================
/** @class  TransportationFacade
    @brief  Transportation facade
*/
// Created: SLI 2011-10-10
// =============================================================================
class TransportationFacade : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationFacade( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                   tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                   const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                   Federate_ABC& federate, const ContextFactory_ABC& contextFactory );
    virtual ~TransportationFacade();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< TransportationController_ABC > pTransportationController_;
    std::auto_ptr< InteractionSender_ABC< interactions::NetnRequestConvoy > > pNetnRequestConvoy_;
    std::auto_ptr< NetnRequestConvoySender > pNetnRequestConvoySender_;
    //@}
};

}
}

#endif // plugins_hla_TransportationFacade_h
