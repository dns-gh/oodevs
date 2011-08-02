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
#include "actions/Action_ABC.h"
#include "actions/ActionsFilter_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "clients_gui/BooleanOptionButton.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Services.h"
#include "tools/SessionConfig.h"
#include "icons.h"
#include "protocol/ReplaySenders.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef boost::function1< void, int > T_Callback;
    public:
        ConfirmationBox( const QString& title, T_Callback callback )
            : QMessageBox( title, "", QMessageBox::Question, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default, Qt::NoButton )
            , callback_( callback )
        {
            setIcon( QMessageBox::Information );
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
        return QPixmap( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gaming/" ) + name + ".png" ).c_str() );
    }
}

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions, const tools::SessionConfig& config, kernel::Controllers& controllers )
    : Q3HBox       ( parent, "ActionsToolbar" )
    , controllers_( controllers )
    , actions_    ( actions )
    , config_     ( config )
    , filter_     ( 0 )
    , initialized_( false )
    , hasReplay_  ( false )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( Q3Frame::StyledPanel | Q3Frame::Raised );

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
    QAction* action = new QAction( this );
    planningBtn->addAction( action );
    action->setCheckable( true );
    planningBtn->setAutoRaise( true );
  //  connect( planningBtn, SIGNAL( QAction::triggered() ), this, SIGNAL( PlanificationModeChange() ) ); $$$$ FPT : stateChanged doesnt exists anymore
    purgeBtn_ = new QToolButton( this );
    purgeBtn_->setAutoRaise( true );
    purgeBtn_->setPixmap( MAKE_PIXMAP( trash2 ) );
    purgeBtn_->setTextLabel( tr( "Delete recorded actions" ) );
    confirmation_ = new ConfirmationBox( tr( "Actions recorder" ), boost::bind( &ActionsToolbar::PurgeConfirmed, this, _1 ) );
    controllers_.Register( *this );

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
    const std::string rootDir = config_.BuildExerciseChildFile( "orders.ord" );
    const QString filename = Q3FileDialog::getOpenFileName( rootDir.c_str(), tr( "Actions files (*.ord)" ), topLevelWidget(), 0, tr( "Load" ) );
    if( filename.isEmpty() )
        return;
    try
    {
        actions_.Load( filename.ascii(), config_.GetLoader(), hasReplay_ );
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
    QString filename = Q3FileDialog::getSaveFileName( rootDir.c_str(), tr( "Actions files (*.ord)" ), topLevelWidget(), 0, tr( "Save" ) );
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

namespace
{
    void GetOrdFilesList( bfs::path& path, std::vector< std::string >& list )
    {
        if( bfs::exists( path ) )
        {
            bfs::directory_iterator end;
            for( bfs::directory_iterator it( path ); it != end; ++it )
            {
                const bfs::path child = *it;
                if( bfs::is_regular_file( child ) && bfs::extension( child ).compare( ".ord" ) == 0 )
                    list.push_back( child.string().c_str() );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::NotifyUpdated
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
void ActionsToolbar::NotifyUpdated( const Simulation& simulation )
{
    if( !hasReplay_ )
        return;

    if( simulation.IsInitialized() != initialized_ )
    {
        initialized_ = simulation.IsInitialized();
        if( initialized_ )
        {
            // todo charger
            std::vector< std::string > files;
            bfs::path path = bfs::path( config_.GetSessionDir(), bfs::native );
            GetOrdFilesList( path, files );
            for( std::vector< std::string >::const_iterator it = files.begin(); it != files.end(); ++it )
            {
                try
                {
                    actions_.Load( *it, config_.GetLoader(), hasReplay_ );
                }
                catch( std::exception& e )
                {
                    QMessageBox::critical( this, tr( "Error loading actions file" ), e.what() );
                }
            }
        }
        else
        {
            actions_.Purge();
            initialized_ = false;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::NotifyUpdated
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
void ActionsToolbar::NotifyUpdated( const Services& services )
{
    hasReplay_ = services.HasService< replay::Service >();
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::NotifyUpdated
// Created: HBD 2010-11-03
// -----------------------------------------------------------------------------
void ActionsToolbar::NotifyUpdated( const Simulation::sCheckPoint& checkPoint )
{
    if( !checkPoint.name_.empty() )
        try
        {
            actions_.Save( config_.BuildOnLocalCheckpointChildFile( checkPoint.name_, "timeline.ord" ) );
        }
        catch( ... )
        {
            // NOTHING
        }
}
