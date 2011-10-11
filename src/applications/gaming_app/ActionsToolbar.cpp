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
#include "Config.h"
#include "gaming/Services.h"
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
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions, const Config& config, kernel::Controllers& controllers )
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

    // Load
    CreateToolButton( tr( "Load actions file" ), MAKE_PIXMAP( open ), SLOT( Load() ) );
    // Save
    CreateToolButton( tr( "Save actions in active timeline to file" ), MAKE_PIXMAP( save ), SLOT( Save() ) );

    // Planning
    QToolButton* planningBtn = new gui::BooleanOptionButton( MakePixmap( "actions_designmode" ), tr( "Planning mode on/off" ), this, controllers.options_, "DesignMode" );
    planningBtn->setAutoRaise( true );
    QAction* action = new QAction( this );
    planningBtn->addAction( action );
    action->setCheckable( true );
    connect( action, SIGNAL( triggered() ), this, SIGNAL( PlanificationModeChange() ) );

    // Purge
    CreateToolButton( tr( "Delete recorded actions" ), MAKE_PIXMAP( trash2 ), SLOT( Purge() ) );
    confirmation_ = new ConfirmationBox( tr( "Actions recorder" ), boost::bind( &ActionsToolbar::PurgeConfirmed, this, _1 ) );
    // Reload
    QToolButton* reloadButton = CreateToolButton( tr( "Reload" ), MAKE_PIXMAP( refresh ), SLOT( Reload() ) );
    connect( this, SIGNAL( activeRefreshButton( bool ) ), reloadButton, SLOT( setEnabled( bool ) ) );

    // Command line order file
    if( !config_.GetOrderFile().empty() )
        filename_ = ( bfs::path( config_.GetExerciseDir( config_.GetExerciseName() ) ) / bfs::path( config_.GetOrderFile() ) ).string();
    emit activeRefreshButton( !filename_.empty() );

    controllers_.Register( *this );
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
// Name: ActionsToolbar::CreateToolButton
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
QToolButton* ActionsToolbar::CreateToolButton( const QString label, const QPixmap& pixmap, const char* slot )
{
    QToolButton* button = new QToolButton( this );
    button->setAutoRaise( true );
    button->setPixmap( pixmap );
    button->setTextLabel( label );
    connect( button, SIGNAL( clicked() ), slot );
    return button;
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
    filename_ = Q3FileDialog::getOpenFileName( rootDir.c_str(), tr( "Actions files (*.ord)" ), topLevelWidget(), 0, tr( "Load" ) ).ascii();
    DoLoad( filename_ );
    emit activeRefreshButton( !filename_.empty() );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::DoLoad
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
void ActionsToolbar::DoLoad( const std::string filename )
{
    if( filename.empty() )
        return;
    try
    {
        actions_.Load( filename, config_.GetLoader(), hasReplay_ );
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
    filename_ = filename.ascii();
    emit activeRefreshButton( !filename_.empty() );
    actions_.Save( filename_, filter_ );
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

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Reload
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
void ActionsToolbar::Reload()
{
    PurgeFilter filter;
    actions_.Purge( &filter );

    assert( !filename_.empty() );
    DoLoad( filename_ );
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
    if( simulation.IsInitialized() != initialized_ )
    {
        initialized_ = simulation.IsInitialized();
        if( initialized_ )
        {
            if( hasReplay_ )
            {
                // todo charger
                std::vector< std::string > files;
                bfs::path path = bfs::path( config_.GetSessionDir(), bfs::native );
                GetOrdFilesList( path, files );
                for( std::vector< std::string >::const_iterator it = files.begin(); it != files.end(); ++it )
                    DoLoad( *it );
            }
            else
                DoLoad( filename_ );
        }
        else
        {
            actions_.Purge();
            filename_.clear();
            emit activeRefreshButton( !filename_.empty() );
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
