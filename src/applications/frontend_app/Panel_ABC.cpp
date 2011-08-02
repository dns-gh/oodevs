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
#include "frontend/SpawnCommand.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3action.h>
#include <Qt3Support/q3hbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qregion.h>
#include <Qt3Support/q3widgetstack.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Panel_ABC constructor
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
Panel_ABC::Panel_ABC( Q3WidgetStack* widget, Q3Action& action, ActionsContext& context, const char* name )
    : Q3VBox   ( widget, name )
    , context_( context )
    , stack_  ( widget )
    , action_ ( action )
    , next_   ( 0 )
{
    setPaletteBackgroundColor( Qt::white );
    layout()->setAlignment( Qt::AlignTop );
    Q3HBox* banner = new Q3HBox( this );
    banner->setMaximumHeight( 100 );
    {
        QLabel* label = new QLabel( banner );
        label->setAlignment( Qt::AlignTop | Qt::AlignHCenter );
        label->setFixedWidth( 90 );
        label->setMargin( 10 );
        label->setPixmap( action.iconSet().pixmap() );
    }
    {
        Q3VBox* box = new Q3VBox( banner );
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
// Name: Panel_ABC::setShown
// Created: SBO 2008-03-17
// -----------------------------------------------------------------------------
void Panel_ABC::setShown( bool show )
{
    Q3VBox::setShown( show );
}

// -----------------------------------------------------------------------------
// Name: Panel_ABC::showEvent
// Created: SBO 2007-10-05
// -----------------------------------------------------------------------------
void Panel_ABC::showEvent( QShowEvent* event )
{
    stack_->raiseWidget( this );
    Update();
    Q3VBox::showEvent( event );
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

// -----------------------------------------------------------------------------
// Name: Panel_ABC::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void Panel_ABC::Update()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Panel_ABC::Start
// Created: RDS 2008-08-25
// -----------------------------------------------------------------------------
void Panel_ABC::Start( frontend::SpawnCommand* command )
{
    commands_.push_back( command ) ;
    command->Start();
}
