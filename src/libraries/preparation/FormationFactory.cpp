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
#include "clients_kernel/Controllers.h"
#include "FormationHierarchies.h"
#include "Formation.h"
#include "TeamHierarchy.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/InstanciationComplete.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationFactory constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationFactory::FormationFactory( Controllers& controllers )
    : controllers_( controllers )
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
kernel::Formation_ABC* FormationFactory::Create( kernel::Team_ABC& parent, const QString& level )
{
    Formation_ABC* formation = new Formation( controllers_.controller_, level );
    formation->Attach( *new TeamHierarchy( parent ) );    
    formation->Attach< Hierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent ) );
    formation->Update( InstanciationComplete() );
    return formation;
}
    
// -----------------------------------------------------------------------------
// Name: FormationFactory::Create
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
kernel::Formation_ABC* FormationFactory::Create( kernel::Formation_ABC& parent, const QString& level )
{
    Formation_ABC* formation = new Formation( controllers_.controller_, level );
    if( const TeamHierarchy* team = parent.Retrieve< TeamHierarchy >() )
        formation->Attach( *new TeamHierarchy( team->GetTeam() ) );    
    formation->Attach< Hierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent ) );
    formation->Update( InstanciationComplete() );
    return formation;
}
