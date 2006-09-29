// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationModel.h"
#include "FormationFactory_ABC.h"
#include "FormationLevels.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Hierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationModel constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationModel::FormationModel( kernel::Controllers& controllers, FormationFactory_ABC& formationFactory )
    : controllers_( controllers )
    , factory_( formationFactory )
    , levels_( *new FormationLevels() )
{
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: FormationModel destructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
FormationModel::~FormationModel()
{
    delete &levels_;
    Purge();
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: FormationModel::Create
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
void FormationModel::Create( kernel::Team_ABC& parent, unsigned int levelId )
{
    const HierarchyLevel_ABC* level = levels_.Resolve( levelId );
    if( !level )
        return;
    Formation_ABC* formation = factory_.Create( parent, *level );
    Register( formation->GetId(), *formation );
}
    
// -----------------------------------------------------------------------------
// Name: FormationModel::Create
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
void FormationModel::Create( kernel::Formation_ABC& parent, unsigned int levelId )
{
    const HierarchyLevel_ABC* level = levels_.Resolve( levelId );
    if( !level )
        return;
    Formation_ABC* formation = factory_.Create( parent, *level );
    Register( formation->GetId(), *formation );
}
    
// -----------------------------------------------------------------------------
// Name: FormationModel::Purge
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void FormationModel::Purge()
{
    DeleteAll();
}
    
// -----------------------------------------------------------------------------
// Name: FormationModel::NotifyDeleted
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
void FormationModel::NotifyDeleted( const Formation_ABC& formation )
{
    Remove( formation.GetId() );
}
