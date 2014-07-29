// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioLauncherPage.h"
#include "moc_ScenarioLauncherPage.cpp"
#include "DebugConfigPanel.h"
#include "OrbatConfigPanel.h"
#include "ExerciseContainer.h"
#include "ExerciseList.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "Registry.h"
#include "frontend/AdvancedConfigPanel.h"
#include "frontend/CheckpointConfigPanel.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/EditExercise.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/JoinExercise.h"
#include "frontend/PluginConfigBuilder.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/RandomPluginConfigPanel.h"
#include "frontend/SessionConfigPanel.h"
#include "frontend/StartExercise.h"
#include "frontend/StartTimeline.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "tools/Loader_ABC.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    QString MakeLink( const tools::Path& file )
    {
        const QString protocol = file.Extension() == "exe" ? "cmd" : "file";
        return QString( "%1://%2" ).arg( protocol ).arg( file.ToUTF8().c_str() );
    }

    bool IsValid( const tools::Path& path, const tools::Loader_ABC& fileLoader, const Config& config )
    {
        bool isValid = true;
        tools::Path terrain, dataSet, physical;
        const tools::Path fileName = config.GetExerciseFile( path );
        try
        {
            std::unique_ptr< xml::xistream > xis = fileLoader.LoadFile( fileName );
            *xis >> xml::start( "exercise" )
                    >> xml::start( "terrain" )
                        >> xml::attribute( "name", terrain )
                    >> xml::end
                    >> xml::start( "model" )
                        >> xml::attribute( "dataset", dataSet )
                        >> xml::attribute( "physical", physical )
                    >> xml::end
                    >> xml::optional >> xml::attribute( "valid", isValid );
        }
        catch( ... )
        {
            // NOTHING
        }
        return isValid && config.GetTerrainDir( terrain ).Exists() &&
             config.GetPhysicalsDir( dataSet, physical ).Exists();
    }

    struct ResourcesLoadingWrapper
    {
        ResourcesLoadingWrapper( QStringList& list ) : stringList_ ( list ) { }
        void ReadResource( xml::xistream& xis )
        {
            std::string item ;
            xis >> xml::attribute( "file", item ) ;
            stringList_.append( item.c_str() ) ;
        }
    private:
        ResourcesLoadingWrapper& operator=( const ResourcesLoadingWrapper& );
        QStringList& stringList_;
    };

    QStringList GetResources( const tools::Path& fileName, const tools::Loader_ABC& fileLoader )
    {
        QStringList result;
        try
        {
            std::unique_ptr< xml::xistream > xis = fileLoader.LoadFile( fileName );
            ResourcesLoadingWrapper loadingWrapper( result );
            *xis >> xml::start( "exercise" )
                    >> xml::optional >> xml::start( "meta" )
                        >> xml::optional >> xml::start( "resources" )
                            >> xml::list( "resource", loadingWrapper, &ResourcesLoadingWrapper::ReadResource );
        }
        catch( ... )
        {
            // NOTHING
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( Application& app, QStackedWidget* pages,
        Page_ABC& previous, kernel::Controllers& controllers, const Config& config,
        const tools::Loader_ABC& fileLoader, ExerciseContainer& exercises )
    : ContentPage( pages, previous, eButtonBack | eButtonStart )
    , config_           ( config )
    , fileLoader_       ( fileLoader )
    , controllers_      ( controllers )
    , exerciseContainer_( exercises )
    , progressPage_     ( new ProgressPage( app, pages, *this ) )
    , exercise_         ( 0 )
    , hasClient_        ( !registry::ReadBool( "NoClientSelected" ) )
    , configPanel_      ( 0 )
{
    setWindowTitle( "ScenarioLauncherPage" );

    //general panel
    QWidget* box = new QWidget( this );
    QHBoxLayout* boxLayout = new QHBoxLayout( box );
    boxLayout->setMargin( 5 );
    tabs_ = new QTabWidget();
    boxLayout->addWidget( tabs_ );

    //general tab
    exercises_ = new ExerciseList( app, tabs_, config_, fileLoader_, controllers, true, true, true, false );
    connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
    connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
    tabs_->addTab( exercises_, "" ); // General

    // Settings tab
    configTabs_ = new QTabWidget();

    frontend::CheckpointConfigPanel* checkpointPanel = AddPlugin( new frontend::CheckpointConfigPanel( configTabs_, config_ ) );
    connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), checkpointPanel, SLOT( Select( const frontend::Exercise_ABC& ) ) );
    connect( exercises_, SIGNAL( ClearSelection() ), checkpointPanel, SLOT( ClearSelection() ) );
    connect( checkpointPanel, SIGNAL( CheckpointSelected( const tools::Path&, const tools::Path& ) ), SLOT( OnSelectCheckpoint( const tools::Path&, const tools::Path& ) ) );

    //session config config panel
    auto* panel = AddPlugin( new frontend::SessionConfigPanel( configTabs_, config_ ) );

    //random config panel
    AddPlugin( new frontend::RandomPluginConfigPanel( configTabs_, config_ ) );

    //advanced config panel
    frontend::AdvancedConfigPanel* advancedPanel = AddPlugin( new frontend::AdvancedConfigPanel( configTabs_, config_, hasClient_ ) );
    connect( advancedPanel, SIGNAL( OnClientEnabled( bool ) ), SLOT( OnClientEnabled( bool ) ) );

    //orbat config panel
    OrbatConfigPanel* pOrbatConfigPanel = AddPlugin( new OrbatConfigPanel( configTabs_, config_ ) );
    connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), pOrbatConfigPanel, SLOT( Select( const frontend::Exercise_ABC& ) ) );
    connect( exercises_, SIGNAL( ClearSelection() ), pOrbatConfigPanel, SLOT( ClearSelection() ) );

    //debug config panel
    auto parent = config.IsOnDebugMode() ? configTabs_ : nullptr;
    configPanel_ = AddPlugin( new DebugConfigPanel( parent, config_ ) );
    connect( panel, SIGNAL( exerciseNumberChanged( int ) ), configPanel_, SLOT( OnExerciseNumberChanged( int ) ) );

    //general settings tab
    QWidget* configBox = new QWidget();
    QVBoxLayout* configBoxLayout = new QVBoxLayout( configBox );
    configBoxLayout->setMargin( 5 );
    configBoxLayout->addWidget( configTabs_ );
    tabs_->addTab( configBox, "" );

    //plugins tab
    frontend::PluginConfigBuilder builder( config_, tabs_ );
    plugins_.push_back( builder.BuildFromXml().Finalize() );

    //general configuration
    EnableButton( eButtonStart, false );
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::AddPlugin
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
template< typename T >
T* ScenarioLauncherPage::AddPlugin( T* ptr )
{
    std::unique_ptr< T > plugin( ptr );
    if( plugin.get() && plugin->IsAvailable() )
    {
        if( plugin->IsVisible() )
            configTabs_->addTab( plugin.get(), "" );
        plugins_.push_back( plugin.get() );
        return plugin.release();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::~ScenarioLauncherPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnLanguageChanged()
{
    tabs_->setTabText( 0, tools::translate( "ScenarioLauncherPage", "General" ) );
    tabs_->setTabText( 1, tools::translate( "ScenarioLauncherPage", "Settings" ) );
    SetTitle( tools::translate( "ScenarioLauncherPage", "Start" ) );
    progressPage_->SetTitle( tools::translate( "ScenarioLauncherPage", "Starting scenario" ) );
    for( int i = 0; i < configTabs_->count() && i < static_cast< int >( plugins_.size()); ++i )
        if( plugins_[ i ]->IsVisible() )
            configTabs_->setTabText( i, plugins_[ i ]->GetName() );
    ContentPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::Update()
{
    exercises_->Clear();
    exerciseContainer_.Refresh();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnStart
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnStart()
{
    if( !CanBeStarted() || ! dialogs::KillRunningProcesses( parentWidget()->parentWidget() ) )
        return;
    const tools::Path exerciseName = exercise_->GetName();
    const auto session = BuildSessionName();
    if( session.second )
        CreateSession( exerciseName, session.first );

    std::map< std::string, std::string > arguments = boost::assign::map_list_of
            ( "checkpoint", checkpoint_.ToUTF8().c_str() );
    boost::optional< DebugConfig > debug;
    if( configPanel_ )
        debug = configPanel_->GetConfig();
    if( debug && !debug->sim.integrationDir.IsEmpty() )
    {
        if( !debug->sim.integrationDir.IsEmpty() )
            arguments[ "integration-dir" ] = debug->sim.integrationDir.ToUTF8();
        if( !debug->sim.pathfindDumpDir.IsEmpty() )
            arguments[ "dump-pathfinds" ] = debug->sim.pathfindDumpDir.ToUTF8();
        if( !debug->sim.pathfindFilter.empty() )
            arguments[ "filter-pathfinds" ] = debug->sim.pathfindFilter;
    }

    auto process = boost::make_shared< frontend::ProcessWrapper >( *progressPage_ );
    process->Add( boost::make_shared< frontend::StartExercise >(
        config_, exerciseName, session.first, arguments, true, "" ) );
    process->Add( boost::make_shared< frontend::StartTimeline >(
        config_, exerciseName, session.first ) );
    if( hasClient_ )
    {
        auto profile = profile_.GetLogin();
        QString devFeatures = configPanel_ ? configPanel_->GetDevFeatures() : QString();
        process->Add( boost::make_shared< frontend::JoinExercise >(
            config_, exerciseName, session.first, &profile, devFeatures, tools::Path(),
            config_.GetCefLog() ) );
    }
    progressPage_->Attach( process );
    frontend::ProcessWrapper::Start( process );
    progressPage_->show();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::CreateSession
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::CreateSession( const tools::Path& exercise, const tools::Path& session ) const
{
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetDefaultValues();
        action.Commit();
    }
    BOOST_FOREACH( const T_Plugins::value_type& plugin, plugins_ )
        plugin->Commit( exercise, session );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::BuildSessionName
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
std::pair< tools::Path, bool > ScenarioLauncherPage::BuildSessionName() const
{
    if( ! session_.IsEmpty() )
        return std::make_pair( session_, true );
    if( config_.GetSession().IsEmpty() )
    {
        return std::make_pair( tools::Path::FromUTF8(
            boost::posix_time::to_iso_string(
                boost::posix_time::second_clock::local_time() ) ), true );
    }
    return std::make_pair( config_.GetSession(), false );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSelect
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSelect( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile )
{
    exercise_ = &exercise;
    profile_ = profile;
    EnableButton( eButtonStart, CanBeStarted() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::ClearSelection
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::ClearSelection()
{
    exercise_ = 0;
    profile_ = frontend::Profile::Invalid;
    EnableButton( eButtonStart, false );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::CanBeStarted
// Created: SBO 2010-08-04
// -----------------------------------------------------------------------------
bool ScenarioLauncherPage::CanBeStarted() const
{
    if( !exercise_ )
        return false;
    if( !IsValid( exercise_->GetName(), fileLoader_, config_ ) )
        return false;
    return profile_.IsValid() || !hasClient_;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSelectCheckpoint
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSelectCheckpoint( const tools::Path& session, const tools::Path& checkpoint )
{
    session_ = session;
    checkpoint_ = checkpoint;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnClientEnabled
// Created: RBA 2012-10-17
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnClientEnabled( bool enabled )
{
    registry::WriteBool( "NoClientSelected", !enabled );
    hasClient_ = enabled;
}
