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
#include "GhostCommandPostAttributes.h"
#include "Ghost.h"
#include "GhostHierarchies.h"
#include "GhostPositions.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticBaseStates.h"
#include "LogisticLevelAttritube.h"
#include "Model.h"
#include "ProfileHierarchies.h"
#include "StaticModel.h"
#include "Symbol.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GhostFactory constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostFactory::GhostFactory( kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel,
                            tools::IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory, kernel::SymbolFactory& symbolsFactory )
    : controllers_          ( controllers )
    , model_                ( model )
    , staticModel_          ( staticModel )
    , idManager_            ( idManager )
    , knowledgeGroupFactory_( knowledgeGroupFactory )
    , symbolsFactory_       ( symbolsFactory )
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
// Name: GhostFactory::Create from creation panel
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostFactory::Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position )
{
    Ghost* result = new Ghost( controllers_.controller_, model_, idManager_, staticModel_.coordinateConverter_, prototype );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach< kernel::TacticalHierarchies >( *new GhostHierarchies( controllers_.controller_, *result, result->GetLevelSymbol(), result->GetSymbol(), &parent ) );

    if( prototype.ghostType_ == eGhostType_Agent )
    {
        result->Attach< CommandPostAttributes_ABC >( *new GhostCommandPostAttributes( *result, false, controllers_.controller_, dico ) );
    }
    else
    {
        assert( prototype.ghostType_ == eGhostType_Automat );
        Entity_ABC* kg = AgentFactory::FindOrCreateKnowledgeGroup( parent );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );
        result->Attach( *new LogisticLevelAttritube( controllers_, *result, dico ) );
        result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_, *result, staticModel_.objectTypes_, dico, false ) );
        result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::Create from xml
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostFactory::Create( kernel::Entity_ABC& parent, xml::xistream& xis )
{
    Ghost* result = new Ghost( controllers_.controller_, model_, idManager_, staticModel_.coordinateConverter_, xis, symbolsFactory_ );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();

    const geometry::Point2f position = model_.ReadPosition( xis, result );
    result->Attach< kernel::Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< kernel::TacticalHierarchies >( *new GhostHierarchies( controllers_.controller_, *result, result->GetLevelSymbol(), result->GetSymbol(), &parent ) );

    if( result->GetGhostType() == eGhostType_Agent )
    {
        result->Attach< CommandPostAttributes_ABC >( *new GhostCommandPostAttributes( *result, xis, controllers_.controller_, dico ) );
    }
    else
    {
        assert( result->GetGhostType() == eGhostType_Automat );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
        result->Attach( *new LogisticLevelAttritube( controllers_, xis, *result, true, dico ) );
        result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_, *result, staticModel_.objectTypes_, dico, result->GetLogisticLevel() != LogisticLevel::none_ ) );
        result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
    }
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: GhostFactory::Create from unknown type convertion
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
kernel::Ghost_ABC* GhostFactory::Create( kernel::Entity_ABC& parent, xml::xistream& xis, E_GhostType ghostType )
{
    Ghost* result = new Ghost( controllers_.controller_, model_, idManager_, staticModel_.coordinateConverter_, xis, parent, ghostType );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();

    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< kernel::TacticalHierarchies >( *new GhostHierarchies( controllers_.controller_, *result, result->GetLevelSymbol(), result->GetSymbol(), &parent ) );

    if( result->GetGhostType() == eGhostType_Agent )
    {
        const geometry::Point2f position = model_.ReadPosition( xis, result );
        result->Attach< kernel::Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
        result->Attach< CommandPostAttributes_ABC >( *new GhostCommandPostAttributes( *result, xis, controllers_.controller_, dico ) );
    }
    else
    {
        assert( result->GetGhostType() == eGhostType_Automat );
        const geometry::Point2f position = model_.ClipPosition( ComputeAutomatPosition( xis ), result );
        result->Attach< Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
        result->Attach< CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
        result->Attach( *new LogisticLevelAttritube( controllers_, xis, *result, true, dico ) );
        result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_, *result, staticModel_.objectTypes_, dico, result->GetLogisticLevel() != LogisticLevel::none_ ) );
        result->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *result, &parent ) );
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
