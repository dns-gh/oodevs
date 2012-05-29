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
#include "PhysicalAttribute.h"
#include "ResourceNetworkAttribute.h"
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
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanFactory::UrbanFactory( kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes,
                            const tools::Resolver< kernel::Object_ABC >& objects, kernel::UrbanDisplayOptions& options,
                            const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanObjects,
                            const kernel::AccommodationTypes& accommodations, const kernel::CoordinateConverter_ABC& converter )
    : controllers_   ( controllers )
    , objectTypes_   ( objectTypes )
    , objects_       ( objects )
    , options_       ( options )
    , urbanObjects_  ( urbanObjects )
    , accommodations_( accommodations )
    , converter_     ( converter )
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
// Name: UrbanFactory::CreateScore
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanFactory::Create( xml::xistream& xis, kernel::UrbanObject_ABC* parent ) const
{
    assert( controllers_.modes_ );
    UrbanObject* pTerrainObject = new UrbanObject( xis, controllers_, objectTypes_.StringResolver< kernel::ObjectType >::Get( "urban block" ), accommodations_, options_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( xis, hierarchies->GetLevel(), *pTerrainObject, converter_ ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( xis ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( xis, dictionary, accommodations_, *pTerrainObject, controllers_.controller_ ) );
    if( hierarchies->GetLevel() == eUrbanLevelBlock )
    {
        pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( controllers_, 100, dictionary ) );
        pTerrainObject->Attach< kernel::Infrastructure_ABC >( *new InfrastructureAttribute( xis, controllers_, *pTerrainObject, dictionary, objectTypes_ ) );
        pTerrainObject->Attach< kernel::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, xis, pTerrainObject->Get< kernel::UrbanPositions_ABC >().Barycenter(), urbanObjects_, objects_, objectTypes_, false ) );
    }
    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    return pTerrainObject;
}
