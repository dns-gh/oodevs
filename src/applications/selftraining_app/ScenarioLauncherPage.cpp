// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioLauncherPage.h"
#include "moc_ScenarioLauncherPage.cpp"
#include "CompositeProcessWrapper.h"
#include "ExerciseList.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "frontend/AdvancedConfigPanel.h"
#include "frontend/CheckpointConfigPanel.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/EditExercise.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/JoinExercise.h"
#include "frontend/PluginConfigBuilder.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/RandomPluginConfigPanel.h"
#include "frontend/SessionConfigPanel.h"
#include "frontend/StartExercise.h"
#include "frontend/StartReplay.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/LinkInterpreter_ABC.h"
#include "clients_gui/Tools.h"
#include "tools/Loader_ABC.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;
namespace bpt = boost::posix_time;

namespace
{
    std::string MakeLink( const std::string& file )
    {
        const QFileInfo info( file.c_str() );
        const QString protocol = info.extension( false ) == "exe" ? "cmd" : "file";
        return QString( "%1://%2" ).arg( protocol ).arg( info.absFilePath() ).toStdString();
    }

    bool IsValid( const std::string& fileName, const tools::Loader_ABC& fileLoader )
    {
        bool isValid = true;
        try
        {
            std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( fileName );
            *xis >> xml::start( "exercise" )
                    >> xml::optional >> xml::attribute( "valid", isValid )
                 >> xml::end;
        }
        catch( ... )
        {
            // NOTHING
        }
        return isValid;
    }

    std::string ReadTargetApplication( const std::string& fileName, const tools::Loader_ABC& fileLoader )
    {
        std::string target = "gaming";
        try
        {
            std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( fileName );
            *xis >> xml::start( "exercise" )
                    >> xml::optional >> xml::start( "meta" )
                        >> xml::optional >> xml::start( "tutorial" )
                            >> xml::attribute( "target", target )
                    >> xml::end
                >> xml::end;
        }
        catch( ... )
        {
            // NOTHING
        }
        return target;
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

    QStringList GetResources( const std::string& fileName, const tools::Loader_ABC& fileLoader )
    {
        QStringList result;
        try
        {
            std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( fileName );
            ResourcesLoadingWrapper loadingWrapper( result );
            *xis >> xml::start( "exercise" )
                    >> xml::optional >> xml::start( "meta" )
                        >> xml::optional >> xml::start( "resources" )
                            >> xml::list( "resource", loadingWrapper, &ResourcesLoadingWrapper::ReadResource )
                        >> xml::end
                    >> xml::end
                >> xml::end;
        }
        catch( ... )
        {
            // NOTHING
        }
        return result;
    }
    bool ReadRegistry( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.readBoolEntry( ( "/sword/" + key ).c_str(), false );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( Application& app, Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const frontend::Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter )
    : LauncherClientPage( pages, previous, eButtonBack | eButtonStart, launcher )
    , config_      ( config )
    , fileLoader_  ( fileLoader )
    , controllers_ ( controllers )
    , interpreter_ ( interpreter )
    , progressPage_( new ProgressPage( app, pages, *this ) )
    , exercise_    ( 0 )
    , noClient_    ( ReadRegistry( "NoClientSelected" ) )
    , isLegacy_    ( ReadRegistry( "IsLegacy" ) )
{
    setName( "ScenarioLauncherPage" );
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 5 );
    {
        tabs_ = new QTabWidget( box );
        {
            exercises_ = new ExerciseList( tabs_, config_, fileLoader_, controllers, true, true, true, false );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
            tabs_->addTab( exercises_, "" ); // General
        }
        {
            Q3GroupBox* configBox = new Q3GroupBox( 1, Qt::Vertical, tabs_ );
            configBox->setMargin( 5 );
            configBox->setFrameShape( Q3GroupBox::NoFrame );
            configTabs_ = new QTabWidget( configBox );
            tabs_->addTab( configBox, "" ); // Settings
            {
                frontend::CheckpointConfigPanel* checkpointPanel = AddPlugin< frontend::CheckpointConfigPanel >();
                connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), checkpointPanel, SLOT( Select( const frontend::Exercise_ABC& ) ) );
                connect( exercises_, SIGNAL( ClearSelection() ), checkpointPanel, SLOT( ClearSelection() ) );
                connect( checkpointPanel, SIGNAL( CheckpointSelected( const QString&, const QString& ) ), SLOT( OnSelectCheckpoint( const QString&, const QString& ) ) );
                AddPlugin< frontend::SessionConfigPanel >();
                AddPlugin< frontend::RandomPluginConfigPanel >();
                frontend::AdvancedConfigPanel* advancedPanel = AddPlugin< frontend::AdvancedConfigPanel >();
                connect( advancedPanel, SIGNAL( SwordVersionSelected( bool ) ), SLOT( OnSwordVersionSelected( bool ) ) );
                connect( advancedPanel, SIGNAL( NoClientSelected( bool ) ), SLOT( OnNoClientSelected( bool ) ) );
            }
        }
        {
            frontend::PluginConfigBuilder builder( config_, tabs_ );
            plugins_.push_back( builder.BuildFromXml().Finalize() );
        }
    }
    EnableButton( eButtonStart, false );
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::AddPlugin
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
template< typename T >
T* ScenarioLauncherPage::AddPlugin()
{
    std::auto_ptr< T > plugin( new T( configTabs_, config_ ) );
    if( plugin.get() && plugin->IsAvailable() )
    {
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
        configTabs_->setTabText( i, plugins_[ i ]->GetName() );
    LauncherClientPage::OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::Update()
{
    exercises_->Clear();
    Connect( "localhost", config_.GetLauncherPort() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::BuildSessionName
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
std::string ScenarioLauncherPage::BuildSessionName() const
{
    return bpt::to_iso_string( bpt::second_clock::local_time() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnStart
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnStart()
{
    if( !CanBeStarted() || ! dialogs::KillRunningProcesses( parentWidget()->parentWidget() ) )
        return;
    const QString exerciseName = exercise_->GetName().c_str();

    const std::string target = ReadTargetApplication( config_.GetExerciseFile( exerciseName.toStdString() ), fileLoader_ );
    if( target == "gaming" )
    {
        const QString session = session_.isEmpty() ? BuildSessionName().c_str() : session_;
        CreateSession( exerciseName, session );
        boost::shared_ptr< frontend::SpawnCommand > simulation( new frontend::StartExercise( config_, exerciseName, session, checkpoint_, true, isLegacy_ ) );
        if ( ! noClient_ )
        {
            boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinExercise( config_, exerciseName, session, profile_.GetLogin(), true ) );
            boost::shared_ptr< CompositeProcessWrapper > process( new CompositeProcessWrapper( *progressPage_, simulation, client ) );
            progressPage_->Attach( process );
            process->Start();
        }
        else
        {
            boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, simulation ) );
            progressPage_->Attach( process );
            process->Start();
        }
        progressPage_->show();
    }
    else if( target == "preparation" )
    {
        boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, exerciseName, true ) );
        boost::shared_ptr< frontend::ProcessWrapper > process( new frontend::ProcessWrapper( *progressPage_, command ) );
        progressPage_->Attach( process );
        process->Start();
        progressPage_->show();
    }
    else if( target == "replayer" )
    {
        const unsigned int port = exercise_->GetPort();
        CreateSession( exerciseName, "default" );
        boost::shared_ptr< frontend::SpawnCommand > replay( new frontend::StartReplay( config_, exerciseName, "default", port, true ) );
        boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinAnalysis( config_, exerciseName, "default", profile_.GetLogin(), true ) );
        boost::shared_ptr< CompositeProcessWrapper >  process( new CompositeProcessWrapper( *progressPage_, replay, client ) );
        progressPage_->Attach( process );
        process->Start();
        progressPage_->show();
    }
    if( target != "gaming" )
    {
        const QStringList resources = GetResources( config_.GetExerciseFile( exerciseName.toStdString() ), fileLoader_ );
        if( ! resources.empty() )
        {
            std::string file = *resources.begin();
            file = ( bfs::path( config_.GetExerciseDir( exerciseName.toStdString() ) ) / file ).string();
            interpreter_.Interprete( MakeLink( file ).c_str() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::CreateSession
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::CreateSession( const QString& exercise, const QString& session )
{
    {
        frontend::CreateSession action( config_, exercise.toStdString(), session.toStdString() );
        action.SetDefaultValues();
        action.Commit();
    }
    {
        BOOST_FOREACH( const T_Plugins::value_type& plugin, plugins_ )
            plugin->Commit( exercise.toStdString(), session.toStdString() );
    }
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
    if( exercise_ )
    {
        const std::string exerciseFile = config_.GetExerciseFile( exercise_->GetName().c_str() );
        if( !IsValid( exerciseFile, fileLoader_ ) )
            return false;
        const std::string target = ReadTargetApplication( exerciseFile, fileLoader_ );
        if( target == "gaming" || target == "replayer" )
            return profile_.IsValid() || noClient_;
        if( target == "preparation" )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSelectCheckpoint
// Created: SBO 2010-04-19
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSelectCheckpoint( const QString& session, const QString& checkpoint )
{
    session_ = session;
    checkpoint_ = checkpoint;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSwordVersionSelected
// Created: RBA 2012-10-17
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnNoClientSelected( bool noClient )
{
    noClient_ = noClient;
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::OnSwordVersionSelected
// Created: SLI 2012-01-27
// -----------------------------------------------------------------------------
void ScenarioLauncherPage::OnSwordVersionSelected( bool isLegacy )
{
    isLegacy_ = isLegacy;
}
