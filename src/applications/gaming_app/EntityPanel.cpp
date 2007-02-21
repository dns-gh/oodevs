// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EntityPanel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: EntityPanel constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
EntityPanel::EntityPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const QString& title )
    : gui::InfoPanel_ABC( parent, panel, title )
    , controllers_( controllers )
    , selected_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityPanel destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
EntityPanel::~EntityPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityPanel::showEvent
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void EntityPanel::showEvent( QShowEvent* )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: EntityPanel::NotifySelected
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void EntityPanel::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity || ! entity )
    {
        selected_ = entity;
        if( selected_ )
            Show();
        else
            Hide();
    }
}
