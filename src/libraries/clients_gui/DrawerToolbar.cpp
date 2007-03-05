// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerToolbar.h"
#include "moc_DrawerToolbar.cpp"
#include "DrawerPanel.h"
#include "DrawerLayer.h"
#include "ExclusiveEventStrategy.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawerToolbar constructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
DrawerToolbar::DrawerToolbar( QMainWindow* mainWindow, ExclusiveEventStrategy& strategy, DrawerLayer& layer, kernel::GlTools_ABC& tools )
    : QToolBar( mainWindow )
    , strategy_( strategy )
    , layer_   ( layer )
{
    setLabel( tr( "Drawing" ) );

    dock_ = new QDockWindow( mainWindow );
    DrawerPanel* panel = new DrawerPanel( dock_, layer_, tools );
    dock_->setWidget( panel );
    dock_->setResizeEnabled( true );
    dock_->setCloseMode( QDockWindow::Always );
    dock_->setCaption( tr( "Drawing" ) );
    dock_->setMaximumWidth( 140 );
    mainWindow->moveDockWindow( dock_, Qt::DockTornOff );
    dock_->hide();

    draw_ = new QToolButton( this, "draw" );
    draw_->setIconSet( MAKE_ICON( pencil ) );
    draw_->setTextLabel( tr( "Draw" ) );
    draw_->setToggleButton( true );
    draw_->setOn( false );
    connect( draw_, SIGNAL( toggled( bool ) ), this, SLOT( OnDraw( bool ) ) );

    show_ = new QToolButton( this, "show" );
    show_->setIconSet( MAKE_ICON( pencil_checkbox ) );
    show_->setTextLabel( tr( "Display drawings" ) );
    show_->setToggleButton( true );
    show_->setOn( true );
    connect( show_, SIGNAL( toggled( bool ) ), this, SLOT( OnShow( bool ) ) );
}
 
// -----------------------------------------------------------------------------
// Name: DrawerToolbar destructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
DrawerToolbar::~DrawerToolbar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawerToolbar::OnShow
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DrawerToolbar::OnShow( bool show )
{
    layer_.Show( show );
    if( ! show )
        draw_->setOn( false );
}

// -----------------------------------------------------------------------------
// Name: DrawerToolbar::OnDraw
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void DrawerToolbar::OnDraw( bool draw )
{
    layer_.TakeFocus( draw );
    if( draw )
    {
        show_->setOn( true );
        dock_->show();
        strategy_.TakeExclusiveFocus( layer_ );
    }
    else
    {
        dock_->hide();
        strategy_.ReleaseExclusiveFocus();   
    }
}

// -----------------------------------------------------------------------------
// Name: DrawerToolbar::OnPanelClosed
// Created: AGE 2006-09-07
// -----------------------------------------------------------------------------
void DrawerToolbar::OnPanelClosed()
{
    OnDraw( false );
}
