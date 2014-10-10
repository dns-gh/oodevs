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

#include "InfrastructureAttribute.h"
#include "PhysicalAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanColor.h"
#include "UrbanHierarchies.h"
#include "UrbanObject.h"
#include "UrbanPositions.h"

#include "clients_gui/Infrastructure_ABC.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include "clients_gui/Tesselator.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"

// -----------------------------------------------------------------------------
// Name: UrbanFactory constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
UrbanFactory::UrbanFactory( kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::UrbanObject_ABC >& urbanObjects, const StaticModel& staticModel,
                            tools::IdManager& idManager, const tools::Resolver< kernel::Object_ABC >& objects, gui::UrbanDisplayOptions& options )
    : controllers_ ( controllers )
    , urbanObjects_( urbanObjects )
    , staticModel_ ( staticModel )
    , idManager_   ( idManager )
    , objects_     ( objects )
    , options_     ( options )
    , tesselator_  ( new gui::Tesselator() )
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
    gui::PropertiesDictionary& dictionary = pTerrainObject->Get< gui::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );

    UrbanPositions* urbanPosition = new UrbanPositions( xis, dictionary, hierarchies->GetLevel(), *pTerrainObject, staticModel_.coordinateConverter_, *tesselator_ );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *urbanPosition );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( xis, *pTerrainObject, dictionary ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( xis, dictionary, staticModel_.accommodationTypes_, *pTerrainObject, controllers_, staticModel_.objectTypes_ ) );
    if( hierarchies->GetLevel() == eUrbanLevelBlock )
    {
        if( urbanPosition->Vertices().empty() )
        {
            delete hierarchies;
            delete pTerrainObject;
            return 0;
        }
        pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( xis, *pTerrainObject, dictionary ) );
        pTerrainObject->Attach< gui::Infrastructure_ABC >( *new InfrastructureAttribute( xis, controllers_, *pTerrainObject, dictionary, staticModel_.objectTypes_ ) );
        pTerrainObject->Attach< gui::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, xis, true, urbanObjects_, objects_, staticModel_.objectTypes_ ) );
    }
    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    pTerrainObject->Polish();
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
    gui::PropertiesDictionary& dictionary = pTerrainObject->Get< gui::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    assert( hierarchies->GetLevel() == eUrbanLevelCity || hierarchies->GetLevel() == eUrbanLevelDistrict );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( dictionary, hierarchies->GetLevel(), *pTerrainObject, staticModel_.coordinateConverter_, *tesselator_ ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( parent, *pTerrainObject, dictionary ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( parent, dictionary, staticModel_.accommodationTypes_, *pTerrainObject, controllers_, staticModel_.objectTypes_ ) );
    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    pTerrainObject->Polish();
    return pTerrainObject;
}

// -----------------------------------------------------------------------------
// Name: UrbanFactory::Create blocks from buttons
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanFactory::Create( const geometry::Polygon2f& location, kernel::Entity_ABC* parent ) const
{
    assert( parent != 0 );
    UrbanObject* pTerrainObject = new UrbanObject( controllers_, idManager_, tools::translate( "UrbanFactory", "Urban block" ).toStdString(), staticModel_.objectTypes_.StringResolver< kernel::ObjectType >::Get( "urban block" ), staticModel_.accommodationTypes_, options_ );
    gui::PropertiesDictionary& dictionary = pTerrainObject->Get< gui::PropertiesDictionary >();
    UrbanHierarchies* hierarchies = new UrbanHierarchies( controllers_.controller_, *pTerrainObject, parent );
    assert( hierarchies->GetLevel() == eUrbanLevelBlock );
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( location, dictionary, hierarchies->GetLevel(), *pTerrainObject, staticModel_.coordinateConverter_, *tesselator_ ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( parent, *pTerrainObject, dictionary ) );
    pTerrainObject->Attach< kernel::PhysicalAttribute_ABC >( *new PhysicalAttribute( parent, dictionary, staticModel_.accommodationTypes_, *pTerrainObject, controllers_, staticModel_.objectTypes_ ) );

    pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( *pTerrainObject, dictionary ) );
    pTerrainObject->Attach< gui::Infrastructure_ABC >( *new InfrastructureAttribute( controllers_, *pTerrainObject, dictionary ) );
    pTerrainObject->Attach< gui::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, true, urbanObjects_, objects_, staticModel_.objectTypes_ ) );

    pTerrainObject->Attach< kernel::Hierarchies >( *hierarchies );
    pTerrainObject->Polish();

    parent->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
    controllers_.actions_.SetSelected( *parent, false );

    return pTerrainObject;
}
