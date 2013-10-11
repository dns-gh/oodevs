// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileUnitRights.h"
#include "moc_UserProfileUnitRights.cpp"
#include "PreparationProfile.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::UserProfileUnitRights( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons, const QString& name )
    : HierarchyTreeView< kernel::TacticalHierarchies >( objectName, controllers, PreparationProfile::GetProfile(), observer_, icons, parent )
    , UserProfileRights_ABC( *this, dataModel_, name )
{
    controllers_.Update( *this );
    connect( this, SIGNAL( clicked( const QModelIndex& ) ), SLOT( OnItemClicked( const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::~UserProfileUnitRights()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::Display
// Created: LGY 2011-12-09
// -----------------------------------------------------------------------------
void UserProfileUnitRights::Display( UserProfile& profile )
{
    selectionModel()->clearSelection();
    UserProfileRights_ABC::Display( profile );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::AdditionalUpdateItem
// Created: JSR 2012-09-25
// -----------------------------------------------------------------------------
void UserProfileUnitRights::AdditionalUpdateItem( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    gui::longname::SetItemLongName( entity, entityItem );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::OnItemClicked
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnItemClicked( const QModelIndex& index )
{
    UserProfileRights_ABC::OnItemClicked( index );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::OnSelect
// Created: NPT 2012-12-10
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnSelect( const QItemSelection& , const QItemSelection&  )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::NotifyUpdated
// Created: SBO 2008-08-26
// -----------------------------------------------------------------------------
void UserProfileUnitRights::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
    {
        gui::longname::SetItemLongName( entity, *item);
        proxyModel_->invalidate();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::contextMenuEvent
// Created: JSR 2012-09-25
// -----------------------------------------------------------------------------
void UserProfileUnitRights::contextMenuEvent( QContextMenuEvent* /*event*/ )
{
    // NOTHING
}

namespace
{
    bool IsCommandPost( const kernel::Entity_ABC& entity )
    {
        if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
            return pAttributes->IsCommandPost();
        return false;
    }
}

bool UserProfileUnitRights::LessThan( const QModelIndex& left, const QModelIndex& right, bool& valid ) const
{
    const kernel::Entity_ABC* entity1 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( left );
    const kernel::Entity_ABC* entity2 = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( right );
    if( !entity1 || !entity2 )
        return false;
    valid = true;
    if( IsCommandPost( *entity1 ) )
        return true;
    if( IsCommandPost( *entity2 ) )
        return false;
    return entity1->GetId() < entity2->GetId();
}
