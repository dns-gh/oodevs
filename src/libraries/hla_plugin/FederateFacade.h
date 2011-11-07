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
#include "Federate_ABC.h"
#include "AgentListener_ABC.h"
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
    class CallsignResolver_ABC;
    class FederateAmbassadorFactory_ABC;
    class Federate_ABC;
    class RtiAmbassadorFactory_ABC;
    class LocalAgentResolver_ABC;
    class MarkingFactory_ABC;
    class HlaClass;

// =============================================================================
/** @class  FederateFacade
    @brief  Federate facade
*/
// Created: SBO 2008-02-18
// =============================================================================
class FederateFacade : public RemoteAgentSubject_ABC
                     , public Federate_ABC
                     , private AgentListener_ABC
                     , private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( xml::xisubstream xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                             AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver, const RtiAmbassadorFactory_ABC& rtiFactory,
                             const FederateAmbassadorFactory_ABC& federateFactory, const std::string& pluginDirectory, CallsignResolver_ABC& callsignResolver );
    virtual ~FederateFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Register( RemoteAgentListener_ABC& listener );
    virtual void Unregister( RemoteAgentListener_ABC& listener );
    //@}

    //! @name Operations
    //@{
    virtual bool Connect();
    virtual void Disconnect();

    virtual bool Create( const std::string& federation, const std::string& fomFile );
    virtual bool Destroy( const std::string& federation );

    virtual bool Join( const std::string& federation, bool timeConstrained, bool timeRegulating );
    virtual void Resign();

    virtual void Step();

    virtual void Register( const ::hla::ClassIdentifier& classID, ::hla::Class_ABC& objectClass, bool publish, bool subscribe );
    virtual void Register( const ::hla::InteractionIdentifier& interactionID, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe );

    virtual void Register( ::hla::FederateAmbassador_ABC& listener );
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

    //! @name Operations
    //@{
    virtual void AggregateCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol );
    virtual void SurfaceVesselCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type );
    //@}

private:
    //! @name Types
    //@{
    struct FederationDestructor;
    //@}

private:
    //! @name Member data
    //@{
    AgentSubject_ABC& subject_;
    std::auto_ptr< MarkingFactory_ABC > markingFactory_;
    std::auto_ptr< ::hla::TimeFactory_ABC > timeFactory_;
    std::auto_ptr< ::hla::TimeIntervalFactory_ABC > intervalFactory_;
    std::auto_ptr< ::hla::RtiAmbassador_ABC > ambassador_;
    std::auto_ptr< Federate_ABC > federate_;
    std::auto_ptr< FederationDestructor > destructor_;
    std::auto_ptr< HlaClass > aggregateClass_;
    std::auto_ptr< HlaClass > surfaceVesselClass_;
    std::auto_ptr< HlaClass > aircraftClass_;
    //@}
};

}
}

#endif // plugins_hla_FederateFacade_h
