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
    : QVBox  ( widget )
    , stack_ ( widget )
    , action_( action )
    , next_  ( 0 )
{
    setPaletteBackgroundColor( Qt::white );
    layout()->setAlignment( Qt::AlignTop );
    QHBox* banner = new QHBox( this );
    banner->setMaximumHeight( 100 );
    {
        QLabel* label = new QLabel( banner );
        label->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
        label->setFixedWidth( 90 );
        label->setMargin( 10 );
        label->setPixmap( action.iconSet().pixmap() );
    }
    {
        QVBox* box = new QVBox( banner );
        {
            QLabel* label = new QLabel( box );
            label->setFixedHeight( 60 );
            label->setAlignment( Qt::AlignBottom | Qt::AlignRight );
            label->setText( action.text() );
            label->setMargin( 5 );
            QFont font( "Arial", 24, QFont::Bold );
            label->setFont( font );
        }
        {
            QLabel* label = new QLabel( box );
            label->setMargin( 5 );
            label->setAlignment( Qt::AlignTop | Qt::AlignRight );
            label->setText( action.toolTip() );
            QFont font( "Arial", 8, QFont::Normal, true );
            label->setFont( font );
        }
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

// -----------------------------------------------------------------------------
// Name: Panel_ABC::LinkTo
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void Panel_ABC::LinkTo( Panel_ABC& next )
{
    next_ = & next.action_;
}

// -----------------------------------------------------------------------------
// Name: Panel_ABC::ShowNext
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void Panel_ABC::ShowNext()
{
    if( next_ )
        next_->setOn( true );
}
