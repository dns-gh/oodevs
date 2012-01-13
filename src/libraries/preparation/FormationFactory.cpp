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
#include "LogisticBaseStates.h"
#include "LogisticLevelAttritube.h"
#include "FormationPositions.h"
#include "clients_kernel/LogisticLevel.h"
#include "ProfileHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "StaticModel.h"
#include "Color.h"
#include "Symbol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationFactory::FormationFactory( Controllers& controllers, const ::StaticModel& staticModel, IdManager& idManager, kernel::SymbolFactory& symbolsFactory )
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

namespace
{
    void SetNationality( Entity_ABC& entity, const Entity_ABC& parent )
    {
        const DictionaryExtensions* parentExt = parent.Retrieve< DictionaryExtensions >();
        DictionaryExtensions* childExt = entity.Retrieve< DictionaryExtensions >();
        if( parentExt && childExt )
        {
            const std::string& country = parentExt->GetValue( "Nationalite" );
            if( !country.empty() )
            {
                childExt->SetEnabled( true );
                childExt->SetValue( "Nationalite", country );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationFactory::Create( kernel::Entity_ABC& parent, const kernel::HierarchyLevel_ABC& level, const FormationLevels& levels, const QString& name )
{
    Formation* formation = new Formation( controllers_.controller_, level, levels, idManager_ );
    PropertiesDictionary& dico = formation->Get< PropertiesDictionary >();
    if( !name.isEmpty() )
        formation->Rename( name );
    formation->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol() );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent, symbolsFactory_ ) );
    formation->Attach< kernel::Positions >( *new FormationPositions( *formation ) );
    formation->Attach< LogisticHierarchiesBase>( *new LogisticBaseStates( controllers_.controller_, *formation, staticModel_.objectTypes_, dico, true, false ) );
    formation->Attach( *new TacticalLines() );
    formation->Attach< kernel::Color_ABC >( *new Color( parent ) );
    formation->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", staticModel_.extensions_ ) );
    formation->Attach( *new LogisticLevelAttritube( controllers_, *formation, dico ) );
    formation->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *formation, &parent ) );
    SetNationality( *formation, parent );
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
    formation->Attach< kernel::SymbolHierarchy_ABC >( *new Symbol( xis ) );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent, symbolsFactory_ ) );
    formation->Attach< kernel::Positions >( *new FormationPositions( *formation ) );
    formation->Attach( *new LogisticLevelAttritube( controllers_, xis, *formation, true, dico ) );
    formation->Attach< LogisticHierarchiesBase >( *new LogisticBaseStates( controllers_.controller_, *formation, staticModel_.objectTypes_, dico, true, formation->GetLogisticLevel() != LogisticLevel::none_ ) );
    formation->Attach( *new TacticalLines() );
    formation->Attach< kernel::Color_ABC >( *new Color( xis ) );
    formation->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, staticModel_.extensions_ ) );
    formation->Attach< ProfileHierarchies_ABC >( *new ProfileHierarchies( controllers_.controller_, *formation, &parent ) );
    formation->Polish();
    return formation;
}
