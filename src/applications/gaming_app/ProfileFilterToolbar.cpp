// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ProfileFilterToolbar.h"
#include "moc_ProfileFilterToolbar.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/ProfileFilter.h"
#include "clients_gui/resources.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ProfileFilterToolbar constructor
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
ProfileFilterToolbar::ProfileFilterToolbar( QMainWindow* pParent, Controllers& controllers, ProfileFilter& filter )
     : QToolBar    ( pParent )
     , controllers_( controllers )
     , filter_     ( filter )
     , entity_     ( controllers_ )
{
    label_ = new QLabel( this );
    // $$$$ AGE 2006-12-13: icon cancel 
    QToolButton* button = new QToolButton( MAKE_ICON( cancel ), "", "", this, SLOT( OnClearFilter() ), this );
    QToolTip::add( button, tr( "Remove filter" ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterToolbar destructor
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
ProfileFilterToolbar::~ProfileFilterToolbar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterToolbar::OnSetFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void ProfileFilterToolbar::OnSetFilter()
{
    if( entity_ )
    {
        label_->setText( entity_->GetName() );
        filter_.SetFilter( *entity_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterToolbar::OnClearFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void ProfileFilterToolbar::OnClearFilter()
{
    label_->setText( "" );
    filter_.RemoveFilter();
}

// -----------------------------------------------------------------------------
// Name: ProfileFilterToolbar::NotifyContextMenu
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void ProfileFilterToolbar::NotifyContextMenu( const Entity_ABC& entity, ContextMenu& menu )
{
    entity_ = &entity;
    menu.InsertItem( "Interface", tr( "Filter view" ), this, SLOT( OnSetFilter() ) );
}
