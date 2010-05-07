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
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/Action_ABC.h"
#include "clients_gui/BooleanOptionButton.h"
#include "clients_kernel/Controllers.h"
#include "gaming/ActionTasker.h"
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

    QPixmap MakePixmap( const std::string& name )
    {
        return QImage( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gaming/" ) + name + ".png" ).c_str() );
    }
}

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions, const tools::ExerciseConfig& config, kernel::Controllers& controllers )
    : QHBox       ( parent, "ActionsToolbar" )
    , controllers_( controllers )
    , actions_    ( actions )
    , config_     ( config )
    , filter_     ( 0 )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

    setMaximumHeight( 32 );
    loadBtn_ = new QToolButton( this );
    loadBtn_->setAutoRaise( true );
    loadBtn_->setPixmap( MAKE_PIXMAP( open ) );
    loadBtn_->setDisabled( false );
    loadBtn_->setTextLabel( tr( "Load actions file" ) );
    
    saveBtn_ = new QToolButton( this );
    saveBtn_->setAutoRaise( true );
    saveBtn_->setPixmap( MAKE_PIXMAP( save ) );
    saveBtn_->setTextLabel( tr( "Save actions in active timeline to file" ) );

    QToolButton* planningBtn = new gui::BooleanOptionButton( MakePixmap( "actions_designmode" ), tr( "Planning mode on/off" ), this, controllers.options_, "DesignMode" );
    planningBtn->setAutoRaise( true );

    purgeBtn_ = new QToolButton( this );
    purgeBtn_->setAutoRaise( true );
    purgeBtn_->setPixmap( MAKE_PIXMAP( trash2 ) );
    purgeBtn_->setTextLabel( tr( "Delete recorded actions" ) );
    
    confirmation_ = new ConfirmationBox( tr( "Actions recorder" ), boost::bind( &ActionsToolbar::PurgeConfirmed, this, _1 ) );

    connect( loadBtn_ , SIGNAL( clicked() ), SLOT( Load()  ) );
    connect( saveBtn_ , SIGNAL( clicked() ), SLOT( Save()  ) );
    connect( purgeBtn_, SIGNAL( clicked() ), SLOT( Purge() ) );
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
// Name: ActionsToolbar::SetFilter
// Created: SBO 2010-05-06
// -----------------------------------------------------------------------------
void ActionsToolbar::SetFilter( const actions::ActionsFilter_ABC& filter )
{
    filter_ = &filter;
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Load
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionsToolbar::Load()
{
    const std::string rootDir = config_.BuildExerciseChildFile( "orders" );
    const QString filename = QFileDialog::getOpenFileName( rootDir.c_str(), tr( "Actions files (*.ord)" ), topLevelWidget(), 0, tr( "Load" ) );
    if( filename.isEmpty() )
        return;
    try
    {
        actions_.Load( filename.ascii() );
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( this, tr( "Error loading actions file" ), e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Save()
{
    const std::string rootDir = config_.BuildExerciseChildFile( "orders" );
    QString filename = QFileDialog::getSaveFileName( rootDir.c_str(), tr( "Actions files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
    if( filename == QString::null )
        return;
    if( filename.right( 4 ) != ".ord" )
        filename += ".ord";
    actions_.Save( filename.ascii(), filter_ );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Purge
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void ActionsToolbar::Purge()
{
    confirmation_->setText( tr( "Delete recorded actions?" ) );
    confirmation_->show();
}

namespace
{
    struct PurgeFilter : public actions::ActionsFilter_ABC
    {
        virtual bool Allows( const actions::Action_ABC& action ) const
        {
            // $$$$ SBO 2010-05-06: actions from decisional cannot be purged
            const ActionTasker* tasker = action.Retrieve< ActionTasker >();
            return !tasker || !tasker->IsSimulation();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::PurgeConfirmed
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
void ActionsToolbar::PurgeConfirmed( int result )
{
    if( result  == QMessageBox::Yes )
    {
        PurgeFilter filter;
        actions_.Purge( &filter );
    }
}
