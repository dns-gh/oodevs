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
#include "tools/ExerciseConfig.h"
#include "icons.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef boost::function1< void, int > T_Callback;
    public:
        ConfirmationBox( const QString& title, T_Callback callback )
            : QMessageBox( title, "", QMessageBox::Question, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default, QMessageBox::NoButton )
            , callback_( callback )
        {
            setIcon( MAKE_PIXMAP( csword ) );
            hide();
        }

        virtual void done( int result )
        {
            callback_( result );
            hide();
        }
    private:
        T_Callback callback_;
    };
}

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
    loadBtn_->setTextLabel( tr( "Load order file" ) );
    
    saveBtn_ = new QToolButton( this );
    saveBtn_->setAutoRaise( true );
    saveBtn_->setPixmap( MAKE_PIXMAP( save ) );
    saveBtn_->setDisabled( true );
    saveBtn_->setTextLabel( tr( "Save orders to file" ) );

    recordBtn_ = new QToolButton( this );
    recordBtn_->setAutoRaise( true );
    recordBtn_->setPixmap( pixRecord_ );
    recordBtn_->setTextLabel( tr( "Toggle order recording on/off" ) );

    QToolButton* purgeBtn = new QToolButton( this );
    purgeBtn->setAutoRaise( true );
    purgeBtn->setPixmap( MAKE_PIXMAP( trash2 ) );
    purgeBtn->setTextLabel( tr( "Delete recorded orders" ) );
    
    confirmation_ = new ConfirmationBox( tr( "Actions recorder" ), boost::bind( &ActionsToolbar::PurgeConfirmed, this, _1 ) );

    connect( loadBtn_  , SIGNAL( clicked() ), SLOT( Load()   ) );
    connect( saveBtn_  , SIGNAL( clicked() ), SLOT( Save()   ) );
    connect( recordBtn_, SIGNAL( clicked() ), SLOT( Record() ) );
    connect( purgeBtn  , SIGNAL( clicked() ), SLOT( Purge()  ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::~ActionsToolbar()
{
    delete confirmation_;
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

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Purge
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void ActionsToolbar::Purge()
{
    confirmation_->setText( tr( "Delete recorded orders?" ) );
    confirmation_->show();
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::PurgeConfirmed
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void ActionsToolbar::PurgeConfirmed( int result )
{
    if( result  == QMessageBox::Yes )
        actions_.Purge();
}
