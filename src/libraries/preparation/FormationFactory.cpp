// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationFactory.h"
#include "Color.h"
#include "Formation.h"
#include "FormationHierarchies.h"
#include "FormationPositions.h"
#include "LogisticBaseStates.h"
#include "LogisticLevelAttribute.h"
#include "FormationPositions.h"
#include "StaticModel.h"
#include "Symbol.h"
#include "TacticalLines.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationFactory::FormationFactory( kernel::Controllers& controllers, const ::StaticModel& staticModel, tools::IdManager& idManager, kernel::SymbolFactory& symbolsFactory )
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
kernel::Formation_ABC* FormationFactory::Create( kernel::Entity_ABC& parent, E_NatureLevel level, const QString& name )
{
    Formation* formation = new Formation( controllers_.controller_, level, idManager_ );
    kernel::PropertiesDictionary& dico = formation->Get< kernel::PropertiesDictionary >();
    if( !name.isEmpty() )
        formation->Rename( name );
    const kernel::Karma& karma = parent.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    formation->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( symbolsFactory_.GetSymbolBase( karma ) ) );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent, symbolsFactory_ ) );
    formation->Attach< kernel::Positions >( *new FormationPositions( *formation ) );
    formation->Attach< LogisticHierarchiesBase>( *new LogisticBaseStates( controllers_.controller_, *formation, staticModel_.objectTypes_, dico ) );
    formation->Attach( *new TacticalLines() );
    formation->Attach< kernel::Color_ABC >( *new Color( parent ) );
    formation->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", staticModel_.extensions_ ) );
    formation->Attach( *new LogisticLevelAttribute( controllers_.controller_, *formation, dico ) );
    formation->Polish();
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationFactory::Create( xml::xistream& xis, kernel::Entity_ABC& parent )
{
    Formation* formation = new Formation( xis, controllers_.controller_, idManager_ );
    kernel::PropertiesDictionary& dico = formation->Get< kernel::PropertiesDictionary >();
    const kernel::Karma& karma = parent.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma();
    formation->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis, symbolsFactory_.GetSymbolBase( karma ) ) );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent, symbolsFactory_ ) );
    formation->Attach< kernel::Positions >( *new FormationPositions( *formation ) );
    formation->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *formation, staticModel_.objectTypes_, dico ) );
    formation->Attach( *new TacticalLines() );
    formation->Attach< kernel::Color_ABC >( *new Color( xis ) );
    formation->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, staticModel_.extensions_ ) );
    formation->Attach( *new LogisticLevelAttribute( controllers_.controller_, xis, *formation, true, dico ) );
    formation->Polish();
    return formation;
}
