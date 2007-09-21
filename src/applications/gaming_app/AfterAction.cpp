// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterAction.h"
#include "AfterActionItemsPanel.h"
#include "AfterActionCanvas.h"
#include "AfterActionFunctionList.h"
#include "clients_gui/resources.h"

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
{
    {
        QDockWindow* functionEditionDock = new QDockWindow( window );
        QHBox* container = new QHBox( functionEditionDock );
        AfterActionItemsPanel* panel = new AfterActionItemsPanel( container, controllers );
        QVBox* canvasContainer = new QVBox( container );
        container->setStretchFactor( panel,            1 );
        container->setStretchFactor( canvasContainer, 12 );

        QButtonGroup* buttons = new QButtonGroup( 1, Qt::Vertical, canvasContainer );
        buttons->setExclusive( true );
        AfterActionCanvas* canvas = new AfterActionCanvas( canvasContainer );
        canvasContainer->setStretchFactor( buttons, 1 );
        canvasContainer->setStretchFactor( canvas, 12 );
        {
            buttons->layout()->setAlignment( Qt::AlignCenter );
            buttons->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
            buttons->setBackgroundMode( Qt::PaletteButton );
            buttons->setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );
            {
                QToolButton* btn = new QToolButton( buttons );
                btn->setToggleButton( true );
                btn->setOn( true );
                btn->setIconSet( MAKE_PIXMAP( open ) ); // $$$$ AGE 2007-09-18: Select
                QToolTip::add( btn, tr( "Select" ) );
                connect( btn, SIGNAL( clicked() ), canvas, SLOT( Select() ) );
            }
            {
                QToolButton* btn = new QToolButton( buttons );
                btn->setToggleButton( true );
                btn->setIconSet( MAKE_PIXMAP( open ) ); // $$$$ AGE 2007-09-18: Connect
                QToolTip::add( btn, tr( "Connect" ) );
                connect( btn, SIGNAL( clicked() ), canvas, SLOT( Connect() ) );
            }
        }

        container->setMinimumSize( 800, 550 );
        functionEditionDock->setWidget( container );
        functionEditionDock->setResizeEnabled( true );
        functionEditionDock->setCloseMode( QDockWindow::Always );
        functionEditionDock->setCaption( tr( "After action function edition" ) );
    }
    {
        QDockWindow* aar = new QDockWindow( window );
        AfterActionFunctionList* list = new AfterActionFunctionList( aar, controllers, factory );
        aar->setResizeEnabled( true );
        aar->setWidget( list );
        aar->setCloseMode( QDockWindow::Always );
        aar->setCaption( tr( "After action review" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterAction destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::~AfterAction()
{
    // NOTHING
}
