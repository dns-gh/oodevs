// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "Panel_ABC.h"
#include "resources.h"
#include <qaction.h>
#include <qregion.h>
#include <qwidgetstack.h>

// -----------------------------------------------------------------------------
// Name: Panel_ABC constructor
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
Panel_ABC::Panel_ABC( QWidgetStack* widget, QAction& action )
    : QVBox( widget )
    , stack_( widget )
{
    setPaletteBackgroundColor( Qt::white );
    layout()->setAlignment( Qt::AlignTop );
    QHBox* banner = new QHBox( this );
    {
        QLabel* label = new QLabel( banner );
        label->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
        label->setFixedSize( 91, 150 );
        label->setMargin( 20 );
        label->setPixmap( action.iconSet().pixmap() );
        label->setErasePixmap( MAKE_PIXMAP( banner_left ) );
    }
    {
        QLabel* label = new QLabel( banner );
        label->setAlignment( Qt::AlignTop | Qt::AlignLeft );
        label->setFixedHeight( 150 );
        label->setErasePixmap( MAKE_PIXMAP( banner_fill ) );
        label->setText( "\n" + action.text() );
        QFont font;
        font.setBold( true );
        font.setPointSize( 20 );
        label->setFont( font );
    }
    {
        QLabel* label = new QLabel( banner );
        label->setFixedSize( 383, 150 );
        label->setPixmap( MAKE_PIXMAP( banner_right ) );
    }
    setSpacing( 10 );
    connect( &action, SIGNAL( toggled( bool ) ), SLOT( setShown( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: Panel_ABC destructor
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
Panel_ABC::~Panel_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Panel_ABC::showEvent
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void Panel_ABC::showEvent( QShowEvent* event )
{
    stack_->raiseWidget( this );
    QVBox::showEvent( event );
}
