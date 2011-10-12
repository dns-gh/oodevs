// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TransportationController_h
#define plugins_hla_TransportationController_h

#include "TransportationController_ABC.h"
#include "tools/MessageObserver.h"
#include <vector>
#include <set>

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
    class AutomatOrder;
}

namespace plugins
{
namespace hla
{
    class MissionResolver_ABC;
    class CallsignResolver_ABC;
    class Subordinates_ABC;
    class ContextFactory_ABC;

// =============================================================================
/** @class  TransportationController
    @brief  Transportation controller
*/
// Created: SLI 2011-10-06
// =============================================================================
class TransportationController : public TransportationController_ABC
                               , private tools::MessageObserver< sword::AutomatOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportationController( xml::xisubstream xis, const MissionResolver_ABC& missionResolver,
                                       tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                       const CallsignResolver_ABC& callsignResolver, const Subordinates_ABC& subordinates,
                                       const ContextFactory_ABC& contextFactory );
    virtual ~TransportationController();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Register( TransportationListener_ABC& listener );
    virtual void Unregister( TransportationListener_ABC& listener );
    virtual void OfferReceived( unsigned int context, bool fullOffer, const std::string& provider );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Notify(  const sword::AutomatOrder& message, int context );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< TransportationListener_ABC* > T_Listeners;
    typedef std::set< unsigned int > T_Requests;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int transportIdentifier_;
    const CallsignResolver_ABC& callsignResolver_;
    const Subordinates_ABC& subordinates_;
    const ContextFactory_ABC& contextFactory_;
    T_Listeners listeners_;
    T_Requests pendingRequests_;
    T_Requests acceptedRequests_;
    //@}
};

}
}

#endif // plugins_hla_TransportationController_h
