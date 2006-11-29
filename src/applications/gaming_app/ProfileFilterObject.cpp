// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ProfileFilterObject.h"
#include "moc_ProfileFilterObject.cpp"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ProfileFilterObject constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ProfileFilterObject::ProfileFilterObject( QObject* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& forward )
    : QObject( parent )
    , ProfileFilter( controllers, forward )
    , controllers_( controllers )
    , entity_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterObject destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ProfileFilterObject::~ProfileFilterObject()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterObject::NotifyContextMenu
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void ProfileFilterObject::NotifyContextMenu( const kernel::Entity_ABC& entity, ContextMenu& menu )
{
    if( entity_ )
        menu.InsertItem( "Interface", tr( "Remove filter" ), this, SLOT( OnRemove() ) );
    entity_ = &entity;
    menu.InsertItem( "Interface", tr( "Filter view" ), this, SLOT( OnFilter() ) );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterObject::OnFilter
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void ProfileFilterObject::OnFilter()
{
    if( entity_ )
        SetFilter( *entity_ );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterObject::OnRemove
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
void ProfileFilterObject::OnRemove()
{
    RemoveFilter();
}
