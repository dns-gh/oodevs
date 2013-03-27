// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfilePopulationRights.h"
#include "moc_UserProfilePopulationRights.cpp"
#include "PreparationProfile.h"

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::UserProfilePopulationRights( const QString& objectName, QWidget* pParent, kernel::Controllers& controllers, const QString& name )
    : gui::PopulationTreeView(objectName, controllers, PreparationProfile::GetProfile(), observer_, pParent )
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
// Name: UserProfilePopulationRights::OnItemClicked
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::OnItemClicked( const QModelIndex& index )
{
    UserProfileRights_ABC::OnItemClicked( index );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::contextMenuEvent
// Created: JSR 2012-09-25
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::contextMenuEvent( QContextMenuEvent* /*event*/ )
{
    // NOTHING
}
