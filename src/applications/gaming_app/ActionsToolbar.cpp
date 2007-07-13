// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionsToolbar.h"
#include "moc_ActionsToolbar.cpp"
#include "gaming/ActionsScheduler.h"
#include "gaming/ActionsModel.h"
#include "gaming/Action_ABC.h"
#include "clients_gui/resources.h"

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions, kernel::Controllers& controllers, Publisher_ABC& publisher, const Simulation& simulation )
    : QHBox( parent )
    , actions_( actions )
    , scheduler_( new ActionsScheduler( controllers, simulation, actions, publisher ) )
    , pixRecord_( MAKE_PIXMAP( recrec ) )
    , pixStop_( MAKE_PIXMAP( recstop ) )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

    setMaximumHeight( 32 );
    loadBtn_ = new QToolButton( this );
    loadBtn_->setAutoRaise( true );
    loadBtn_->setPixmap( MAKE_PIXMAP( open ) );
    loadBtn_->setDisabled( false );
    
    saveBtn_ = new QToolButton( this );
    saveBtn_->setAutoRaise( true );
    saveBtn_->setPixmap( MAKE_PIXMAP( save ) );
    saveBtn_->setDisabled( true );

    playBtn_ = new QToolButton( this );
    playBtn_->setAutoRaise( true );
    playBtn_->setPixmap( MAKE_PIXMAP( recplay ) );
    playBtn_->setDisabled( true );

    recordBtn_ = new QToolButton( this );
    recordBtn_->setAutoRaise( true );
    recordBtn_->setPixmap( pixRecord_ );
    
    connect( loadBtn_  , SIGNAL( clicked() ), SLOT( Load()   ) );
    connect( saveBtn_  , SIGNAL( clicked() ), SLOT( Save()   ) );
    connect( playBtn_  , SIGNAL( clicked() ), SLOT( Play()   ) );
    connect( recordBtn_, SIGNAL( clicked() ), SLOT( Record() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::~ActionsToolbar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Record
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Record()
{
    actions_.ToggleRecording();
    const bool isRecording = actions_.IsRecording();
    recordBtn_->setPixmap( isRecording ? pixStop_ : pixRecord_ );
    playBtn_->setDisabled( isRecording );
    saveBtn_->setDisabled( isRecording );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Play
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Play()
{
    scheduler_->Start();
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Load
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionsToolbar::Load()
{
    const QString filename = QFileDialog::getOpenFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Load" ) );
    if( filename == QString::null )
        return;
    try
    {
        scheduler_->Stop();
        actions_.Load( filename.ascii() );
        playBtn_->setDisabled( false );
        saveBtn_->setDisabled( false );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( this, tr( "Error loading order file" ), e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Save()
{
    QString filename = QFileDialog::getSaveFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
    if( filename == QString::null )
        return;
    if( filename.right( 4 ) != ".ord" )
        filename += ".ord";
    actions_.Save( filename.ascii() );
}
