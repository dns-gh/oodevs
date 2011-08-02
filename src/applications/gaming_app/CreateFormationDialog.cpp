// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "CreateFormationDialog.h"
#include "moc_CreateFormationDialog.cpp"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog constructor
// Created: LDC 2010-10-12
// -----------------------------------------------------------------------------
CreateFormationDialog::CreateFormationDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::FormationLevels& levels, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel, const kernel::Time_ABC& time )
    : QDialog       ( parent )
    , controllers_  ( controllers )
    , profile_      ( profile )
    , levels_       ( levels )
    , actionsModel_ ( actionsModel )
    , currentEntity_( 0 )
    , time_         ( time )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog destructor
// Created: LDC 2010-10-12
// -----------------------------------------------------------------------------
CreateFormationDialog::~CreateFormationDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog::NotifyContextMenu
// Created: LDC 2010-10-12
// -----------------------------------------------------------------------------
void CreateFormationDialog::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    const kernel::HierarchyLevel_ABC* level = levels_.GetRoot();
    NotifyContextMenu( entity, level, menu );
}

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog::NotifyContextMenu
// Created: LDC 2010-10-12
// -----------------------------------------------------------------------------
void CreateFormationDialog::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    const kernel::HierarchyLevel_ABC* level = &entity.GetLevel();
    NotifyContextMenu( entity, level, menu );
}

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog::NotifyContextMenu
// Created: LDC 2010-10-13
// -----------------------------------------------------------------------------
void CreateFormationDialog::NotifyContextMenu( const kernel::Entity_ABC& entity, const kernel::HierarchyLevel_ABC* level, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        currentEntity_ = &entity;
        Q3PopupMenu* subMenu = menu.SubMenu( "Creation", tr( "Create formation" ) );
        while( level && ( level = level->GetNext() ) )
            subMenu->insertItem( level->GetName(), this, SLOT( OnCreateFormation( int ) ), 0, level->GetId() );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog::OnCreateFormation
// Created: LDC 2010-10-13
// -----------------------------------------------------------------------------
void CreateFormationDialog::OnCreateFormation( int level )
{
    actions::Action_ABC* action = actionsModel_.CreateFormationCreationAction( level, *currentEntity_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, time_ ) );
    action->Attach( *new actions::ActionTasker( currentEntity_, false ) );
    actionsModel_.Publish( *action );
}
