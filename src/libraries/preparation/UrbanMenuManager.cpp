// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanMenuManager.h"
#include "moc_UrbanMenuManager.cpp"
#include "UrbanHierarchies.h"
#include "UrbanModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/UrbanObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager constructor
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
UrbanMenuManager::UrbanMenuManager( kernel::Controllers& controllers, UrbanModel& model )
    : controllers_( controllers )
    , model_      ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager destructor
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
UrbanMenuManager::~UrbanMenuManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::Register
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::Register()
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::Unregister
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::Unregister()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::NotifySelectionChanged
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::NotifySelectionChanged( const T_Elements& elements )
{
    selected_ = elements;
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::NotifyContextMenu
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu )
{
    if( GetCurrentMode() != ePreparationMode_Terrain)
        return;
    const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
    if( hierarchies.GetLevel() == eUrbanLevelCity )
        menu.InsertItem( "Urban", tr( "Create district" ), this, SLOT( OnCreateDistrict() ) );
    menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnCreateDistrict
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnCreateDistrict()
{
    for( CIT_Elements it = selected_.begin(); it != selected_.end(); ++it )
    {
        const kernel::UrbanObject_ABC& object = **it;
        const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        if( hierarchies.GetLevel() == eUrbanLevelCity )
            model_.CreateCityOrDistrict( const_cast< kernel::UrbanObject_ABC* >( &object ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnDelete
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnDelete()
{
    model_.DeleteBlocks( selected_ );
}
