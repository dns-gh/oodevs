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
#include "UrbanColor.h"
#include "UrbanPositions.h"
#include "Architecture.h"
#include "Usages.h"
#include "InfrastructureAttribute.h"
#include "StructuralStateAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "UrbanHierarchies.h"
#include "clients_gui/UrbanObject.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UrbanExtensions.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_gui/Architecture.h"
#include "clients_gui/Usages.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: UrbanFactory constructor
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
UrbanFactory::UrbanFactory( kernel::Controllers& controllers, const kernel::ObjectTypes& objectTypes,
                            const tools::Resolver< kernel::Object_ABC >& objects, gui::UrbanDisplayOptions& options,
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
    // TODO voir quels attributs virer dans le cas d'une ville ou d'un quartier
    kernel::UrbanObject_ABC* pTerrainObject = new gui::UrbanObject( xis, controllers_, objectTypes_.StringResolver< kernel::ObjectType >::Get( "urban block" ), accommodations_, options_ );
    kernel::PropertiesDictionary& dictionary = pTerrainObject->Get< kernel::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( xis ) );
    pTerrainObject->Attach< kernel::Architecture_ABC >( *new Architecture( xis, std::auto_ptr< kernel::Architecture_ABC >( new gui::Architecture( dictionary ) ) ) );
    pTerrainObject->Attach< kernel::Usages_ABC >( *new Usages( xis, std::auto_ptr< kernel::Usages_ABC >( new gui::Usages( dictionary, accommodations_, pTerrainObject->GetLivingSpace() ) ) ) );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( xis, hierarchies->GetLevel(), *pTerrainObject, converter_ ) );

    if( hierarchies->GetLevel() == eUrbanLevelBlock )
    {
        pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( 100, dictionary ) );
        pTerrainObject->Attach< kernel::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, xis,
            pTerrainObject->Get< kernel::UrbanPositions_ABC >().Barycenter(),
            urbanObjects_, objects_, objectTypes_ ) );
        pTerrainObject->Attach< kernel::Infrastructure_ABC >( *new InfrastructureAttribute( xis, *pTerrainObject, dictionary, objectTypes_ ) );
    }

    return pTerrainObject;
}
