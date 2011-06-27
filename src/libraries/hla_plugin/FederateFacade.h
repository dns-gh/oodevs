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

#include "tools/MessageObserver.h"

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
    class AggregateEntityClass;
    class AgentSubject_ABC;
    class FederateAmbassadorFactory_ABC;
    class Federate_ABC;
    class RtiAmbassadorFactory_ABC;

// =============================================================================
/** @class  FederateFacade
    @brief  Federate facade
*/
// Created: SBO 2008-02-18
// =============================================================================
    class FederateFacade : private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( xml::xisubstream xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller, AgentSubject_ABC& subject, const RtiAmbassadorFactory_ABC& rtiFactory, const FederateAmbassadorFactory_ABC& federateFactory, const std::string& pluginDirectory );
    virtual ~FederateFacade();
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::ControlEndTick& message );
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
    std::auto_ptr< AggregateEntityClass > agentClass_;
    //@}
};

}
}

#endif // plugins_hla_FederateFacade_h
