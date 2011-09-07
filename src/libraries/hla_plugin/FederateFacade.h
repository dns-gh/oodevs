// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_FederateFacade_h
#define plugins_hla_FederateFacade_h

#include "RemoteAgentSubject_ABC.h"
#include "tools/MessageObserver.h"
#include <memory>

namespace xml
{
    class xisubstream;
}

namespace hla
{
    class TimeFactory_ABC;
    class TimeIntervalFactory_ABC;
    class RtiAmbassador_ABC;
}

namespace sword
{
    class ControlEndTick;
    class SimToClient_Content;
}

namespace plugins
{
namespace hla
{
    class AgentSubject_ABC;
    class FederateAmbassadorFactory_ABC;
    class Federate_ABC;
    class RtiAmbassadorFactory_ABC;
    class AggregateFactory_ABC;
    class RemoteAggregateFactory_ABC;
    class ClassBuilder_ABC;

// =============================================================================
/** @class  FederateFacade
    @brief  Federate facade
*/
// Created: SBO 2008-02-18
// =============================================================================
class FederateFacade : public RemoteAgentSubject_ABC
                     , private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( xml::xisubstream xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                             AgentSubject_ABC& subject, const RtiAmbassadorFactory_ABC& rtiFactory,
                             const FederateAmbassadorFactory_ABC& federateFactory, const std::string& pluginDirectory );
    virtual ~FederateFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Register( RemoteAgentListener_ABC& listener );
    virtual void Unregister( RemoteAgentListener_ABC& listener );
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

private:
    //! @name Types
    //@{
    struct FederationDestructor;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ::hla::TimeFactory_ABC > timeFactory_;
    std::auto_ptr< ::hla::TimeIntervalFactory_ABC > intervalFactory_;
    std::auto_ptr< ::hla::RtiAmbassador_ABC > ambassador_;
    std::auto_ptr< Federate_ABC > federate_;
    std::auto_ptr< FederationDestructor > destructor_;
    std::auto_ptr< AggregateFactory_ABC > pAggregateFactory_;
    std::auto_ptr< AggregateFactory_ABC > pNetnAggregateFactory_;
    std::auto_ptr< RemoteAggregateFactory_ABC > pRemoteAggregateFactory_;
    std::auto_ptr< RemoteAggregateFactory_ABC > pNetnRemoteAggregateFactory_;
    std::auto_ptr< ClassBuilder_ABC > pClassBuilder_;
    std::auto_ptr< ClassBuilder_ABC > pNetnClassBuilder_;
    std::auto_ptr< RemoteAgentSubject_ABC > agentClass_;
    //@}
};

}
}

#endif // plugins_hla_FederateFacade_h
