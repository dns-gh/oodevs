// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionsModel.h"
#include "Action_ABC.h"
#include "ActionFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionsModel constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::ActionsModel( ActionFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsModel destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsModel::~ActionsModel()
{
    // $$$$ SBO 2007-03-12: check for unsaved actions? prompt user?
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::Purge
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void ActionsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission )
{
    Action_ABC* action = factory_.CreateAction( target, mission );
    Register( action->GetId(), *action );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionsModel::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionsModel::CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder )
{
    Action_ABC* action = factory_.CreateAction( target, fragOrder );
    Register( action->GetId(), *action );
    return action;
}
