// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ActionItem.h"

// -----------------------------------------------------------------------------
// Name: ActionItem constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionItem::ActionItem( QWidget* parent, Q3Action& action )
    : QToolButton( parent )
    , action_( action )
{
    setIconSet( action.iconSet().pixmap() );
    setTextLabel( action.text(), ! action.toolTip().isEmpty() );
    setTextPosition( QToolButton::BelowIcon );
    setUsesBigPixmap( true );
    setUsesTextLabel( true );
    setToggleButton( true );
    setAutoRaise( true );
    setPaletteBackgroundColor( Qt::white );
    connect( &action, SIGNAL( toggled( bool ) ), SLOT( setOn( bool ) ) );
    connect( this, SIGNAL( toggled( bool ) ), &action, SIGNAL( toggled( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: ActionItem destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
ActionItem::~ActionItem()
{
    // NOTHING
}
