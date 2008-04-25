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
#include "tools/ExerciseConfig.h"

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions, ActionsScheduler& scheduler, const tools::ExerciseConfig& config )
    : QHBox     ( parent )
    , actions_  ( actions )
    , scheduler_( scheduler )
    , config_   ( config )
    , pixRecord_( MAKE_PIXMAP( recrec ) )
    , pixStop_  ( MAKE_PIXMAP( recstop ) )
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

    recordBtn_ = new QToolButton( this );
    recordBtn_->setAutoRaise( true );
    recordBtn_->setPixmap( pixRecord_ );
    
    connect( loadBtn_  , SIGNAL( clicked() ), SLOT( Load()   ) );
    connect( saveBtn_  , SIGNAL( clicked() ), SLOT( Save()   ) );
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
    saveBtn_->setDisabled( isRecording );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Load
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionsToolbar::Load()
{
    const std::string rootDir = config_.BuildExerciseChildFile( "orders" );
    const QString filename = QFileDialog::getOpenFileName( rootDir.c_str(), tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Load" ) );
    if( filename.isEmpty() )
        return;
    try
    {
        actions_.Load( filename.ascii() );
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
    const std::string rootDir = config_.BuildExerciseChildFile( "orders" );
    QString filename = QFileDialog::getSaveFileName( rootDir.c_str(), tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
    if( filename == QString::null )
        return;
    if( filename.right( 4 ) != ".ord" )
        filename += ".ord";
    actions_.Save( filename.ascii() );
}
