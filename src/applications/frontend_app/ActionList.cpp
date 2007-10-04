// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ActionList.h"
#include "ActionCategory.h"
#include "ActionItem.h"

// -----------------------------------------------------------------------------
// Name: ActionList constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionList::ActionList( QWidget* parent )
    : QToolBox( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionList destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionList::~ActionList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionList::AddAction
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void ActionList::AddAction( const QString& category, QAction& action )
{
    ActionCategory*& widget = categories_[ category ];
    if( !widget )
    {
        widget = new ActionCategory( this );
        insertItem( count(), widget, category );
    }
    new ActionItem( widget, action );
}
