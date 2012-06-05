// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanFactory.h"
#include "Architecture.h"
#include "InfrastructureAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "PhysicalAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanColor.h"
#include "UrbanHierarchies.h"
#include "UrbanObject.h"
#include "UrbanPositions.h"
#include "Usages.h"
#include "clients_kernel/Architecture.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UrbanExtensions.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanObject.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Usages.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UrbanFactory constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanFactory::UrbanFactory( kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanObjects, const StaticModel& staticModel,
                            IdManager& idManager, const tools::Resolver< kernel::Object_ABC >& objects, kernel::UrbanDisplayOptions& options )
    : controllers_   ( controllers )
    , urbanObjects_  ( urbanObjects )
    , staticModel_   ( staticModel )
    , idManager_     ( idManager )
    , objects_       ( objects )
    , options_       ( options )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanFactory destructor
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanFactory::~UrbanFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanFactory::Create anything from xis
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanFactory::Create( xml::xistream& xis, kernel::Entity_ABC* parent ) const
{
    UrbanObject* pTerrainObject = new UrbanObject( xis, controllers_, idManager_, staticModel_.objectTypes_.StringResolver< kernel::ObjectType >::Get( "urban block" ), staticModel_.accommodationTypes_, options_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( xis, dictionary, hierarchies->GetLevel(), *pTerrainObject, staticModel_.coordinateConverter_ ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( xis, controllers_, dictionary ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( xis, dictionary, staticModel_.accommodationTypes_, *pTerrainObject, controllers_, staticModel_.objectTypes_ ) );
    if( hierarchies->GetLevel() == eUrbanLevelBlock )
    {
        pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( controllers_, dictionary ) );
        pTerrainObject->Attach< kernel::Infrastructure_ABC >( *new InfrastructureAttribute( xis, controllers_, *pTerrainObject, dictionary, staticModel_.objectTypes_ ) );
        pTerrainObject->Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( staticModel_.objectTypes_, dictionary, &controllers_, pTerrainObject ) );
        pTerrainObject->Attach< kernel::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, xis, pTerrainObject->Get< kernel::UrbanPositions_ABC >().Barycenter(), urbanObjects_, objects_, staticModel_.objectTypes_, false ) );
        //assert( parent != 0 );
        //static_cast< kernel::UrbanObject_ABC* >( parent )->ComputeConvexHull(); // needed ?
    }
    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    return pTerrainObject;
}

// -----------------------------------------------------------------------------
// Name: UrbanFactory::Create city or district from list view context menu
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanFactory::Create( kernel::Entity_ABC* parent ) const
{
    const std::string name = ( parent ) ? tools::translate( "UrbanFactory", "District" ) : tools::translate( "UrbanFactory", "City" );
    UrbanObject* pTerrainObject = new UrbanObject( controllers_, idManager_, name, staticModel_.objectTypes_.StringResolver< kernel::ObjectType >::Get( "urban block" ), staticModel_.accommodationTypes_, options_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    assert( hierarchies->GetLevel() == eUrbanLevelCity || hierarchies->GetLevel() == eUrbanLevelDistrict );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( dictionary, hierarchies->GetLevel(), *pTerrainObject, staticModel_.coordinateConverter_ ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( parent, controllers_, dictionary ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( parent, dictionary, staticModel_.accommodationTypes_, *pTerrainObject, controllers_, staticModel_.objectTypes_ ) );
    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    return pTerrainObject;
}

// -----------------------------------------------------------------------------
// Name: UrbanFactory::Create blocs from buttons
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanFactory::Create( const geometry::Polygon2f& location, kernel::Entity_ABC* parent ) const
{
    assert( parent != 0 );
    UrbanObject* pTerrainObject = new UrbanObject( controllers_, idManager_, tools::translate( "UrbanFactory", "Urban block" ).toStdString(), staticModel_.objectTypes_.StringResolver< kernel::ObjectType >::Get( "urban block" ), staticModel_.accommodationTypes_, options_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    assert( hierarchies->GetLevel() == eUrbanLevelBlock );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( location, dictionary, hierarchies->GetLevel(), *pTerrainObject, staticModel_.coordinateConverter_ ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( parent, controllers_, dictionary ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( parent, dictionary, staticModel_.accommodationTypes_, *pTerrainObject, controllers_, staticModel_.objectTypes_ ) );

    pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( controllers_, dictionary ) );
    pTerrainObject->Attach< kernel::Infrastructure_ABC >( *new InfrastructureAttribute( controllers_, *pTerrainObject, dictionary ) );
    pTerrainObject->Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( staticModel_.objectTypes_, dictionary, &controllers_, pTerrainObject ) );
    pTerrainObject->Attach< kernel::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, pTerrainObject->Get< kernel::UrbanPositions_ABC >().Barycenter(), urbanObjects_, objects_, staticModel_.objectTypes_, false ) );

    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    //static_cast< kernel::UrbanObject_ABC* >( parent )->ComputeConvexHull(); // needed ?

    parent->Select( controllers_.actions_ );
    return pTerrainObject;
}
