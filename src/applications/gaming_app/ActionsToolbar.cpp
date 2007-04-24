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
#include "gaming/ActionsModel.h"
#include "clients_gui/resources.h"

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions )
    : QHBox( parent )
    , actions_( actions )
    , pixRecord_( MAKE_PIXMAP( recrec ) )
    , pixStop_( MAKE_PIXMAP( recstop ) )
    , isRecording_( false )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

    setMaximumHeight( 32 );
    playBtn_ = new QToolButton( this );
    playBtn_->setAutoRaise( true );
    playBtn_->setPixmap( MAKE_PIXMAP( recplay ) );
    playBtn_->setDisabled( true );

    recordBtn_ = new QToolButton( this );
    recordBtn_->setAutoRaise( true );
    recordBtn_->setPixmap( pixRecord_ );
    
    saveBtn_ = new QToolButton( this );
    saveBtn_->setAutoRaise( true );
    saveBtn_->setPixmap( MAKE_PIXMAP( save ) );
    saveBtn_->setDisabled( true );

    connect( recordBtn_, SIGNAL( clicked() ), SLOT( Record() ) );
    connect( playBtn_  , SIGNAL( clicked() ), SLOT( Play()   ) );
    connect( saveBtn_  , SIGNAL( clicked() ), SLOT( Save()   ) );
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
// Name: ActionsToolbar::IsRecording
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
bool ActionsToolbar::IsRecording() const
{
    return isRecording_;
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Record
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Record()
{
    isRecording_ = !isRecording_;
    recordBtn_->setPixmap( isRecording_ ? pixStop_ : pixRecord_ );
    playBtn_->setDisabled( isRecording_ );
    saveBtn_->setDisabled( isRecording_ );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Play
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Play()
{
    const QString filename = QFileDialog::getOpenFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Load" ) );
    if( filename == QString::null )
        return;
    actions_.Load( filename.ascii() );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Save()
{
    if( isRecording_ )
        return;
    QString filename = QFileDialog::getSaveFileName( QString::null, tr( "Order files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
    if( filename == QString::null )
        return;
    if( filename.right( 4 ) != ".ord" )
        filename += ".ord";
    actions_.Save( filename.ascii() );
}
