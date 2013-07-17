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
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "ENT/ENT_Tr_Gen.h"

// -----------------------------------------------------------------------------
// Name: CreateFormationDialog constructor
// Created: LDC 2010-10-12
// -----------------------------------------------------------------------------
CreateFormationDialog::CreateFormationDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel, const kernel::Time_ABC& time )
    : QDialog       ( parent )
    , controllers_  ( controllers )
    , profile_      ( profile )
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
void CreateFormationDialog::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    const int level = entity.GetLevel();
    if( entity.GetId() == 0 ) // no side team
        return;
    if( profile_.CanDoMagic( entity ) && level > eNatureLevel_c )
    {
        currentEntity_ = &entity;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Creation", tr( "Create formation" ) );
        for( int levelIt = static_cast< int >( level ); levelIt > 0; --levelIt )
            subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( levelIt ), ENT_Tr_ABC::eToTr ).c_str(), this, SLOT( OnCreateFormation( int ) ), 0, levelIt );
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
    actionsModel_.Publish( *action, 0 );
}
