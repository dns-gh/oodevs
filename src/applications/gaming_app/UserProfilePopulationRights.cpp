// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfilePopulationRights.h"
#include "moc_UserProfilePopulationRights.cpp"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, const QString& name, const kernel::Profile_ABC& profile )
    : gui::PopulationTreeView( controllers, profile, observer_, pParent )
    , UserProfileRights_ABC( *this, dataModel_, name )
{
    connect( this, SIGNAL( clicked( const QModelIndex& ) ), SLOT( OnItemClicked( const QModelIndex& ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights destructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::~UserProfilePopulationRights()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::showEvent
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::showEvent( QShowEvent* event )
{
    OnShow();
    gui::PopulationTreeView::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::hideEvent
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::hideEvent( QHideEvent* event )
{
    OnHide();
    gui::PopulationTreeView::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::OnItemClicked
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::OnItemClicked( const QModelIndex& index )
{
    UserProfileRights_ABC::OnItemClicked( index );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::NeedsCommit
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
bool UserProfilePopulationRights::NeedsCommit() const
{
    return isVisible();
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::contextMenuEvent
// Created: JSR 2012-06-22
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::contextMenuEvent( QContextMenuEvent* /*event*/ )
{
    // NOTHING
}
