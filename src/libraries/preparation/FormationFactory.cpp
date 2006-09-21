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
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/InstanciationComplete.h"
#include "Formation.h"

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
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation_ABC* FormationFactory::Create( Entity_ABC& parent, const QString& level )
{
    Formation_ABC* formation = new Formation( controllers_.controller_, level );
    formation->Attach< Hierarchies >( *new FormationHierarchies( controllers_.controller_, *formation, &parent ) );
    formation->Update( InstanciationComplete() );
    return formation;
}
