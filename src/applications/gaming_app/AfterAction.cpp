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
#include "moc_AfterAction.cpp"
#include "AfterActionItemsPanel.h"
#include "AfterActionCanvas.h"
#include "AfterActionFunctionList.h"
#include "AfterActionRequestList.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/resources.h"
#include "gaming/AfterActionModel.h"
#include "gaming/Simulation.h"

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model, Publisher_ABC& publisher )
    : window_      ( window )
    , controllers_ ( controllers )
    , model_       ( model )
    , functionsTab_( 0 )
{
    CreateEditionDock    ( window, controllers, factory );
    CreateAfterActionDock( window, controllers, factory, publisher );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterAction destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::~AfterAction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterAction::CreateEditionDock
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterAction::CreateEditionDock( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& )
{
    editionDock_ = new QDockWindow( window );
    QHBox* container = new QHBox( editionDock_ );
    AfterActionItemsPanel* panel = new AfterActionItemsPanel( container, controllers );
    QVBox* canvasContainer = new QVBox( container );
    container->setStretchFactor( panel,            1 );
    container->setStretchFactor( canvasContainer, 12 );

    QButtonGroup* buttons = new QButtonGroup( 1, Qt::Vertical, canvasContainer );
    buttons->setExclusive( true );
    canvas_ = new AfterActionCanvas( canvasContainer );
    canvasContainer->setStretchFactor( buttons, 1 );
    canvasContainer->setStretchFactor( canvas_, 12 );
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
            connect( btn, SIGNAL( clicked() ), canvas_, SLOT( Select() ) );
        }
        {
            QToolButton* btn = new QToolButton( buttons );
            btn->setToggleButton( true );
            btn->setIconSet( MAKE_PIXMAP( open ) ); // $$$$ AGE 2007-09-18: Connect
            QToolTip::add( btn, tr( "Connect" ) );
            connect( btn, SIGNAL( clicked() ), canvas_, SLOT( Connect() ) );
        }
    }

    container->setMinimumSize( 800, 550 );
    editionDock_->setWidget( container );
    editionDock_->setResizeEnabled( true );
    editionDock_->setCloseMode( QDockWindow::Always );
    editionDock_->setCaption( tr( "After action function edition" ) );
    editionDock_->hide();
    window_->setAppropriate( editionDock_, false );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::CreateAfterActionDock
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterAction::CreateAfterActionDock( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, Publisher_ABC& publisher )
{
    aarDock_ = new QDockWindow( window );
    QVBox* box = new QVBox( aarDock_ );
    functionsTab_ = new QTabWidget( box );

    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab_, controllers, factory );
    connect( list, SIGNAL( EditFunction( const AfterActionFunction* ) ), SLOT( OnEditFunction( const AfterActionFunction* ) ) );
    connect( list, SIGNAL( NewFunction() ), SLOT( OnNewFunction() ) );
    connect( list, SIGNAL( CreateRequest( const AfterActionFunction* ) ), SLOT( OnCreateRequest( const AfterActionFunction* ) ) );
    functionsTab_->addTab( list, tr( "Functions" ) );

    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab_, window, controllers, factory, publisher );
    functionsTab_->addTab( requests, tr( "Requests" ) );

    aarDock_->setResizeEnabled( true );
    aarDock_->setWidget( box );
    aarDock_->setCloseMode( QDockWindow::Always );
    aarDock_->setCaption( tr( "After action review" ) );
    aarDock_->hide();
    window_->setAppropriate( aarDock_, false );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::OnEditFunction
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterAction::OnEditFunction( const AfterActionFunction* function )
{
    editionDock_->show();
    canvas_->Edit( function );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::OnNewFunction
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
void AfterAction::OnNewFunction()
{
    OnEditFunction( model_.CreateNewFunction() );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::OnCreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterAction::OnCreateRequest( const AfterActionFunction* function )
{
    if( function )
    {
        model_.CreateRequest( *function );
        functionsTab_->showPage( functionsTab_->page( 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterAction::NotifyUpdated
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterAction::NotifyUpdated( const Simulation& simu )
{
    window_->setAppropriate( editionDock_, simu.IsReplayer() );
    window_->setAppropriate( aarDock_,     simu.IsReplayer() );
    aarDock_->setShown( simu.IsReplayer() );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::hide
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterAction::hide()
{
    editionDock_->hide();
    aarDock_->hide();
}
