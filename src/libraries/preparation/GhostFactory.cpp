// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostFactory.h"
#include "AgentFactory.h"
#include "AgentCommunications.h"
#include "AgentHierarchies.h"
#include "AutomatHierarchies.h"
#include "AutomatCommunications.h"
#include "Color.h"
#include "Ghost.h"
#include "GhostPositions.h"
#include "KnowledgeGroupsModel.h"
#include "Model.h"
#include "StaticModel.h"
#include "Symbol.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GhostFactory constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostFactory::GhostFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                            IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : controllers_          ( controllers )
    , model_                ( model )
    , staticModel_          ( staticModel )
    , idManager_            ( idManager )
    , knowledgeGroupFactory_( knowledgeGroupFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostFactory destructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostFactory::~GhostFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::Create
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostFactory::Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position )
{
    Ghost* result = new Ghost( controllers_.controller_, idManager_, prototype );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetLevel(), result->GetSymbol(), &parent ) );

    if( prototype.ghostType_ == eGhostType_Agent )
    {
        result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    }
    else
    {
        assert( prototype.ghostType_ == eGhostType_Automat );
        result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol() );
        Entity_ABC* kg = AgentFactory::FindorCreateKnowledgeGroup( parent, knowledgeGroupFactory_ );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::Create
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostFactory::Create( kernel::Entity_ABC& parent, xml::xistream& xis )
{
    Ghost* result = new Ghost( controllers_.controller_, idManager_, xis );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new GhostPositions( xis, *result, staticModel_.coordinateConverter_, controllers_.controller_, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetLevel(), result->GetSymbol(), &parent ) );

    if( result->GetGhostType() == eGhostType_Agent )
    {
        result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    }
    else
    {
        assert( result->GetGhostType() == eGhostType_Automat );
        result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::Create
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostFactory::Create( kernel::Entity_ABC& parent, xml::xistream& xis, E_GhostType ghostType )
{
    Ghost* result = new Ghost( controllers_.controller_, idManager_, xis, parent, ghostType );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();

    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< kernel::TacticalHierarchies >( *new AgentHierarchies( controllers_.controller_, *result, result->GetLevel(), result->GetSymbol(), &parent ) );

    if( result->GetGhostType() == eGhostType_Agent )
    {
        result->Attach< Positions >( *new GhostPositions( xis, *result, staticModel_.coordinateConverter_, controllers_.controller_, dico ) );
        result->Attach< CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
    }
    else
    {
        assert( result->GetGhostType() == eGhostType_Automat );

        const geometry::Point2f position = ComputeAutomatPosition( xis );
        result->Attach< Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );

        result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
    }
    result->Polish();
    return result;
}

namespace
{
    geometry::Point2f ReadPosition( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
    {
        const std::string position = xis.attribute< std::string >( "position" );
        return converter.ConvertToXY( position );
    }
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::ComputeAutomatPosition
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
geometry::Point2f GhostFactory::ComputeAutomatPosition( xml::xistream& xis ) const
{
    geometry::Polygon2f points;
    InternalComputeAutomatPosition( xis, points );
    return points.Barycenter();
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::InternalComputeAutomatPosition
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
void GhostFactory::InternalComputeAutomatPosition( xml::xistream& xis, geometry::Polygon2f& points ) const
{
    xis >> xml::list( "automat", *this, &GhostFactory::InternalComputeAutomatPosition, points )
        >> xml::list( "unit"   , *this, &GhostFactory::ReadUnitPosition,               points )
        >> xml::list( "phantom", *this, &GhostFactory::ReadUnitPosition,               points );
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::ReadUnitPosition
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
void GhostFactory::ReadUnitPosition( xml::xistream& xis, geometry::Polygon2f& points ) const
{
    points.Add( ReadPosition( xis, staticModel_.coordinateConverter_ ) );
}
