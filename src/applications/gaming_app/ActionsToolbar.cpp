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
#include "clients_kernel/Controllers.h"
#include "clients_gui/FileDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichAction.h"
#include "GamingConfig.h"
#include "gaming/Services.h"
#include "icons.h"
#include "protocol/Dispatcher.h"
#include "protocol/ReplaySenders.h"
#include "ENT/ENT_Enums.h"
#include <boost/bind.hpp>

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef std::function< void( int ) > T_Callback;
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

    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gaming" ) / name + ".png" ) );
    }
}

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionsToolbar constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::ActionsToolbar( QWidget* parent, ActionsModel& actions, const GamingConfig& config, kernel::Controllers& controllers )
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
    QAction* action = new QAction( MakePixmap( "actions_designmode" ), tr( "Planning mode on/off" ), this );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnModeChanged() ) );
    QToolButton* planningBtn = new QToolButton( this );
    planningBtn->setDefaultAction( action );
    planningBtn->setAutoRaise( true );
    planningBtn->setCheckable( true );
    planningAction_ = new gui::RichAction( action, controllers_ );
    planningAction_->SetModes( eModes_Replay, eModes_Gaming | eModes_Planning, true );

    // Purge
    CreateToolButton( tr( "Delete recorded actions" ), MAKE_PIXMAP( trash2 ), SLOT( Purge() ) );
    confirmation_ = new ConfirmationBox( tr( "Actions recorder" ), boost::bind( &ActionsToolbar::PurgeConfirmed, this, _1 ) );
    // Reload
    QToolButton* reloadButton = CreateToolButton( tr( "Reload" ), MAKE_PIXMAP( refresh ), SLOT( Reload() ) );
    connect( this, SIGNAL( activeRefreshButton( bool ) ), reloadButton, SLOT( setEnabled( bool ) ) );

    // Command line order file
    if( !config_.GetOrderFile().IsEmpty() )
        filename_ = config_.GetExerciseDir( config_.GetExerciseName() ) / config_.GetOrderFile();
    emit activeRefreshButton( !filename_.IsEmpty() );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsToolbar::~ActionsToolbar()
{
    controllers_.Unregister( *this );
    delete confirmation_;
    delete planningAction_;
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
    tools::Path defaultPath = config_.BuildExerciseChildFile( config_.GetExerciseName() + "-" + tools::Path::FromUnicode( tr( "orders" ).toStdWString() ) + ".ord" );
    filename_ = gui::FileDialog::getOpenFileName( topLevelWidget(), tr( "Load" ), defaultPath, tr( "Actions files (*.ord)" ) );
    DoLoad( filename_ );
    emit activeRefreshButton( !filename_.IsEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::DoLoad
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
void ActionsToolbar::DoLoad( const tools::Path& filename )
{
    if( filename.IsEmpty() )
        return;
    try
    {
        actions_.Load( filename, config_.GetLoader() );
    }
    catch( const std::exception& e )
    {
        QMessageBox::critical( this, tr( "Error loading actions file" ), tools::GetExceptionMsg( e ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::Save
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ActionsToolbar::Save()
{
    tools::Path defaultPath = config_.BuildExerciseChildFile( config_.GetExerciseName() + "-" + tools::Path::FromUnicode( tr( "orders" ).toStdWString() ) );
    tools::Path filename = gui::FileDialog::getSaveFileName( topLevelWidget(), tr( "Save" ), defaultPath, tr( "Actions files (*.ord)" ) );
    if( filename.IsEmpty() )
        return;
    if( filename.Extension() != ".ord" )
        filename.ReplaceExtension( ".ord" );
    filename_ = filename;
    emit activeRefreshButton( !filename_.IsEmpty() );
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

    assert( !filename_.IsEmpty() );
    DoLoad( filename_ );
}

namespace
{
    bool IsOrderFile( const tools::Path& path )
    {
        return path.IsRegularFile() && path.Extension() == ".ord";
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
                tools::Path::T_Paths files = config_.GetSessionDir().ListElements( boost::bind( &IsOrderFile, _1 ), false, false );
                for( auto it = files.begin(); it != files.end(); ++it )
                    DoLoad( *it );
            }
            else
                DoLoad( filename_ );
        }
        else
        {
            actions_.Purge();
            filename_.Clear();
            emit activeRefreshButton( !filename_.IsEmpty() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::NotifyUpdated
// Created: JSR 2010-05-12
// -----------------------------------------------------------------------------
void ActionsToolbar::NotifyUpdated( const Services& services )
{
    hasReplay_ = services.HasService( sword::service_replay );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::NotifyUpdated
// Created: HBD 2010-11-03
// -----------------------------------------------------------------------------
void ActionsToolbar::NotifyUpdated( const Simulation::sCheckPoint& checkPoint )
{
    if( !checkPoint.name_.IsEmpty() )
        try
        {
            actions_.Save( config_.BuildOnLocalCheckpointChildFile( checkPoint.name_, "timeline.ord" ) );
        }
        catch( ... )
        {
            // NOTHING
        }
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::NotifyModeChanged
// Created: NPT 2013-02-27
// -----------------------------------------------------------------------------
void ActionsToolbar::NotifyModeChanged( E_Modes newMode )
{
    kernel::ModesObserver_ABC::NotifyModeChanged( newMode );
    setVisible( newMode != eModes_Replay );
}

// -----------------------------------------------------------------------------
// Name: ActionsToolbar::OnModeChanged
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void ActionsToolbar::OnModeChanged()
{
    controllers_.ChangeMode( GetCurrentMode() == eModes_Gaming ? eModes_Planning : eModes_Gaming );
}
