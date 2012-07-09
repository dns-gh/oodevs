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

// -----------------------------------------------------------------------------
// Name: GhostFactory constructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
GhostFactory::GhostFactory( kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel,
                            IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory, kernel::SymbolFactory& symbolsFactory )
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
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( parent ) );
    result->Attach< kernel::TacticalHierarchies >( *new GhostHierarchies( controllers_.controller_, *result, result->GetLevelSymbol(), result->GetSymbol(), &parent ) );

    if( prototype.ghostType_ == eGhostType_Agent )
    {
        result->Attach< kernel::CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
        result->Attach< kernel::CommandPostAttributes_ABC >( *new GhostCommandPostAttributes( *result, false, controllers_.controller_, dico ) );
    }
    else
    {
        assert( prototype.ghostType_ == eGhostType_Automat );
        const kernel::Karma& karma = parent.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
        result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbolsFactory_.GetSymbolBase( karma ) ) );
        kernel::Entity_ABC* kg = AgentFactory::FindorCreateKnowledgeGroup( parent, knowledgeGroupFactory_ );
        result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( controllers_.controller_, *result, kg ) );
        result->Attach( *new LogisticLevelAttritube( controllers_.controller_, *result, dico ) );
        result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, staticModel_.objectTypes_, dico, false ) );
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
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();
    result->Attach< kernel::Positions >( *new GhostPositions( xis, *result, staticModel_.coordinateConverter_, controllers_.controller_, dico ) );
    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< kernel::TacticalHierarchies >( *new GhostHierarchies( controllers_.controller_, *result, result->GetLevelSymbol(), result->GetSymbol(), &parent ) );

    if( result->GetGhostType() == eGhostType_Agent )
    {
        result->Attach< kernel::CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
        result->Attach< kernel::CommandPostAttributes_ABC >( *new GhostCommandPostAttributes( *result, xis, controllers_.controller_, dico ) );
    }
    else
    {
        assert( result->GetGhostType() == eGhostType_Automat );
        result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
        result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
        result->Attach( *new LogisticLevelAttritube( controllers_.controller_, xis, *result, true, dico ) );
        result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, staticModel_.objectTypes_, dico, result->GetLogisticLevel() != kernel::LogisticLevel::none_ ) );
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
    kernel::PropertiesDictionary& dico = result->Get< kernel::PropertiesDictionary >();

    result->Attach< kernel::Color_ABC >( *new Color( xis ) );
    result->Attach< kernel::TacticalHierarchies >( *new GhostHierarchies( controllers_.controller_, *result, result->GetLevelSymbol(), result->GetSymbol(), &parent ) );

    if( result->GetGhostType() == eGhostType_Agent )
    {
        result->Attach< kernel::Positions >( *new GhostPositions( xis, *result, staticModel_.coordinateConverter_, controllers_.controller_, dico ) );
        result->Attach< kernel::CommunicationHierarchies >( *new AgentCommunications( controllers_.controller_, *result, &parent ) );
        result->Attach< kernel::CommandPostAttributes_ABC >( *new GhostCommandPostAttributes( *result, xis, controllers_.controller_, dico ) );
    }
    else
    {
        assert( result->GetGhostType() == eGhostType_Automat );
        const geometry::Point2f position = ComputeAutomatPosition( xis );
        result->Attach< kernel::Positions >( *new GhostPositions( *result, staticModel_.coordinateConverter_, controllers_.controller_, position, dico ) );
        result->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
        result->Attach< kernel::CommunicationHierarchies >( *new AutomatCommunications( xis, controllers_.controller_, *result, model_.knowledgeGroups_ ) );
        result->Attach( *new LogisticLevelAttritube( controllers_.controller_, xis, *result, true, dico ) );
        result->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *result, staticModel_.objectTypes_, dico, result->GetLogisticLevel() != kernel::LogisticLevel::none_ ) );
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
