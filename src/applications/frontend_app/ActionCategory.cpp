// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ActionCategory.h"
#include "moc_ActionCategory.cpp"
#include "ActionItem.h"
#include <qtoolbox.h>

// -----------------------------------------------------------------------------
// Name: ActionCategory constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionCategory::ActionCategory( QToolBox* parent )
    : QVButtonGroup( parent )
    , parent_( parent )
{
    setExclusive( true );
    setInsideMargin( 1 );
    setInsideSpacing( 0 );
    setPaletteBackgroundColor( Qt::white );
}

// -----------------------------------------------------------------------------
// Name: ActionCategory destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionCategory::~ActionCategory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionCategory::AddAction
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void ActionCategory::AddAction( QAction& action )
{
    ActionItem* item = new ActionItem( this, action );
    connect( item, SIGNAL( toggled( bool ) ), SLOT( OnItemToggled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionCategory::OnItemToggled
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
void ActionCategory::OnItemToggled( bool toggled )
{
    if( toggled && !isVisible() )
        parent_->setCurrentItem( this );
}
