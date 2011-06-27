// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationFactory.h"
#include "Formation.h"
#include "FormationHierarchies.h"
#include "TacticalLines.h"
#include "EntityIntelligences.h"
#include "LogisticBaseStates.h"
#include "FormationPositions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "StaticModel.h"
#include "Color.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationFactory::FormationFactory( Controllers& controllers, const StaticModel& staticModel, IdManager& idManager, kernel::SymbolFactory& symbolsFactory )
    : controllers_   ( controllers )
    , staticModel_   ( staticModel )
    , idManager_     ( idManager )
    , symbolsFactory_( symbolsFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationFactory destructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationFactory::~FormationFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationFactory::Create( kernel::Entity_ABC& parent, const kernel::HierarchyLevel_ABC& level, const QString& name )
{
    Formation* formation = new Formation( controllers_.controller_, level, idManager_ );
    PropertiesDictionary& dico = formation->Get< PropertiesDictionary >();
    if( !name.isEmpty() )
        formation->Rename( name );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent, symbolsFactory_ ) );
    formation->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *formation, &parent ) );
    formation->Attach< kernel::Positions >( *new FormationPositions( *formation ) );
    formation->Attach< kernel::LogisticHierarchiesBase>( *new LogisticBaseStates( controllers_.controller_, *formation, staticModel_.objectTypes_, dico ) );
    formation->Attach( *new TacticalLines() );
    formation->Attach< kernel::Color_ABC >( *new Color( parent ) );
    formation->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", staticModel_.extensions_ ) );
    formation->Polish();
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationFactory::Create( xml::xistream& xis, kernel::Entity_ABC& parent, const FormationLevels& levels )
{
    Formation* formation = new Formation( xis, controllers_.controller_, levels, idManager_ );
    PropertiesDictionary& dico = formation->Get< PropertiesDictionary >();
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent, symbolsFactory_ ) );
    formation->Attach< kernel::IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *formation, &parent ) );
    formation->Attach< kernel::Positions >( *new FormationPositions( *formation ) );
    formation->Attach< kernel::LogisticHierarchiesBase>( *new LogisticBaseStates( controllers_.controller_, *formation, staticModel_.objectTypes_, dico ) );
    formation->Attach( *new TacticalLines() );
    formation->Attach< kernel::Color_ABC >( *new Color( xis ) );
    formation->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, staticModel_.extensions_ ) );
    formation->Polish();
    return formation;
}
