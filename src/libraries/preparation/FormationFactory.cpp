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
#include "clients_kernel/Controllers.h"
#include "FormationHierarchies.h"
#include "Formation.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/InstanciationComplete.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationFactory::FormationFactory( Controllers& controllers, IdManager& idManager )
    : controllers_( controllers )
    , idManager_( idManager )
{
    //NOTHING
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
kernel::Formation_ABC* FormationFactory::Create( kernel::Entity_ABC& parent, const kernel::HierarchyLevel_ABC& level )
{
    Formation_ABC* formation = new Formation( controllers_.controller_, level, idManager_ );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent ) );
    formation->Update( InstanciationComplete() );
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationFactory::Create( xml::xistream& xis, kernel::Entity_ABC& parent, const FormationLevels& levels )
{
    Formation_ABC* formation = new Formation( xis, controllers_.controller_, levels, idManager_ );
    formation->Attach< kernel::TacticalHierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent ) );
    formation->Update( InstanciationComplete() );
    return formation;
}

