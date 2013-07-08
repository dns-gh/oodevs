// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FomBuildery_h
#define plugins_hla_FomBuildery_h

#include <memory>
#include <xeumeuleu/xml.hpp>
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{

class HlaClass;
class HlaTacticalObjectClass;
class Federate_ABC;
class LocalAgentResolver_ABC;
class HlaObjectNameFactory_ABC;
class CallsignResolver_ABC;
class MarkingFactory_ABC;
class EntityIdentifierResolver_ABC;
class FOM_Serializer_ABC;
class OwnershipStrategy_ABC;

// =============================================================================
/** @class  FomBuilder
    @brief  FomBuilder
*/
// Created: AHC 2012-11-12
// =============================================================================
class FomBuilder : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
    FomBuilder( xml::xistream& xis, Federate_ABC& federate, LocalAgentResolver_ABC& resolver, HlaObjectNameFactory_ABC& nameFactory,
                CallsignResolver_ABC& callsignResolver, MarkingFactory_ABC& markingFactory, EntityIdentifierResolver_ABC& entityIdentifierResolver,
                FOM_Serializer_ABC& fomSerializer, OwnershipStrategy_ABC& ownershipStrategy );
    ~FomBuilder();
    //@}

    //! @name Operations
    //@{
    std::auto_ptr< HlaClass > CreateAggregateClass();
    std::auto_ptr< HlaClass > CreateSurfaceVesselClass();
    std::auto_ptr< HlaClass > CreateAircraftClass();
    std::auto_ptr< HlaClass > CreateGroundVehicleClass();
    std::auto_ptr< HlaClass > CreateHumanClass();
    std::auto_ptr< HlaClass > CreateRprAggregateClass();
    std::auto_ptr< HlaClass > CreateRprSurfaceVesselClass();
    std::auto_ptr< HlaClass > CreateRprAircraftClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateMinefieldClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateCulturalFeatureClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateBreachablePointObjectClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateBreachableLinearObjectClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateOtherPointObjectClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateOtherArealObjectClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateATP45HazardAreaClass();
    std::auto_ptr< HlaTacticalObjectClass > CreateRawDataHazardContourGroupClass();
    //@}

private:
    //! @name Attributes
    //@{
    xml::xisubstream xis_;
    Federate_ABC& federate_;
    LocalAgentResolver_ABC& resolver_;
    HlaObjectNameFactory_ABC& nameFactory_;
    CallsignResolver_ABC& callsignResolver_;
    MarkingFactory_ABC& markingFactory_;
    EntityIdentifierResolver_ABC& entityIdentifierResolver_;
    FOM_Serializer_ABC& fomSerializer_;
    OwnershipStrategy_ABC& ownershipStrategy_;
    //@}

};

}
}

#endif // plugins_hla_FomBuildery_h
