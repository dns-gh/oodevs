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
#include "RemoteTacticalObjectSubject_ABC.h"
#include "Federate_ABC.h"
#include "AgentListener_ABC.h"
#include "TacticalObjectListener_ABC.h"
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

namespace tools
{
    class Path;
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
    class ClassListener_ABC;
    class HlaClass;
    class HlaTacticalObjectClass;
    class HlaObjectNameFactory_ABC;
    class TacticalObjectSubject_ABC;
    class OwnershipStrategy_ABC;
    class CommentMissionSender;
    class EntityIdentifierResolver_ABC;
    class FOM_Serializer_ABC;
    class FomBuilder;

// =============================================================================
/** @class  FederateFacade
    @brief  Federate facade
*/
// Created: SBO 2008-02-18
// =============================================================================
class FederateFacade : public RemoteAgentSubject_ABC
                     , public Federate_ABC
                     , public RemoteTacticalObjectSubject_ABC
                     , private AgentListener_ABC
                     , private TacticalObjectListener_ABC
                     , private tools::MessageObserver< sword::ControlEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( xml::xisubstream xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                             AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver, const RtiAmbassadorFactory_ABC& rtiFactory,
                             const FederateAmbassadorFactory_ABC& federateFactory, const tools::Path& pluginDirectory, CallsignResolver_ABC& callsignResolver,
                             TacticalObjectSubject_ABC& tacticalObjectSubject,
                             OwnershipStrategy_ABC& ownershipStrategy, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer );
    virtual ~FederateFacade();
    //@}

    //! @name Operations
    //@{
    virtual void Register( ClassListener_ABC& listener );
    virtual void Unregister( ClassListener_ABC& listener );
    virtual void RegisterTactical( ClassListener_ABC& listener );
    virtual void UnregisterTactical( ClassListener_ABC& listener );
    //@}

    //! @name Operations
    //@{
    virtual bool Connect();
    virtual void Disconnect();

    virtual bool Create( const std::string& federation, const std::string& fomFile );
    virtual bool Create( const std::string& federation, const T_FomFiles& fomFiles );
    virtual bool Destroy( const std::string& federation );

    virtual bool Join( const std::string& federation, bool timeConstrained, bool timeRegulating );
    virtual void Resign();

    virtual void Step();
    virtual void Tick();

    virtual void Register( const ::hla::ClassIdentifier& classID, ::hla::Class_ABC& objectClass, bool publish, bool subscribe );
    virtual void Register( const ::hla::InteractionIdentifier& interactionID, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe );

    virtual void Register( ::hla::FederateAmbassador_ABC& listener );

    const ::hla::FederateIdentifier& GetFederateHandle() const;

    // Ownership control
    virtual void DivestRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    virtual void UnconditionalDivest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes );
    virtual void AcquisitionRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes );
    virtual void UnconditionalAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag );
    //@}

private:
    //! @name Messages
    //@{
    virtual void Notify( const sword::ControlEndTick& message, int context );
    //@}

    //! @name Operations
    //@{
    virtual void AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, bool isLocal, const std::vector< char >& uniqueId );
    virtual void PlatformCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol, const std::vector< char >& uniqueId );
    virtual void ObjectCreated( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
            bool isBreachable, TacticalObjectListener_ABC::GeometryType geometry, bool isPropagation );
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
    TacticalObjectSubject_ABC& tacticalObjectSubject_;
    const RtiAmbassadorFactory_ABC& rtiFactory_;
    std::unique_ptr< MarkingFactory_ABC > markingFactory_;
    std::unique_ptr< ::hla::TimeFactory_ABC > timeFactory_;
    std::unique_ptr< ::hla::TimeIntervalFactory_ABC > intervalFactory_;
    ::hla::RtiAmbassador_ABC* ambassador_;
    std::unique_ptr< Federate_ABC > federate_;
    std::unique_ptr< FederationDestructor > destructor_;
    std::unique_ptr< HlaObjectNameFactory_ABC > nameFactory_;
    std::unique_ptr< FomBuilder > fomBuilder_;
    std::unique_ptr< HlaClass > aggregateClass_;
    std::unique_ptr< HlaClass > surfaceVesselClass_;
    std::unique_ptr< HlaClass > aircraftClass_;
    std::unique_ptr< HlaClass > groundVehicleClass_;
    std::unique_ptr< HlaClass > humanClass_;
    std::unique_ptr< HlaClass > rprAggregateClass_;
    std::unique_ptr< HlaClass > rprSurfaceVesselClass_;
    std::unique_ptr< HlaClass > rprAircraftClass_;
    std::unique_ptr< HlaTacticalObjectClass > minefieldClass_;
    std::unique_ptr< HlaTacticalObjectClass > culturalFeatureClass_;
    std::unique_ptr< HlaTacticalObjectClass > breachableLinearObjectClass_;
    std::unique_ptr< HlaTacticalObjectClass > breachablePointObjectClass_;
    std::unique_ptr< HlaTacticalObjectClass > otherPointObjectClass_;
    std::unique_ptr< HlaTacticalObjectClass > otherArealObjectClass_;
    std::unique_ptr< HlaTacticalObjectClass > rawDataHazardContourGroupClass_;
    std::unique_ptr< HlaTacticalObjectClass > atp45HazardAreaClass_;
    //@}
};

}
}

#endif // plugins_hla_FederateFacade_h
