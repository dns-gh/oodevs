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
#include "CompositeProcessWrapper.h"
#include "ExerciseList.h"
#include "ProcessDialogs.h"
#include "ProgressPage.h"
#include "frontend/AdvancedConfigPanel.h"
#include "frontend/CheckpointConfigPanel.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/EdxlHavePluginConfigPanel.h"
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
#include "frontend/TabWidget.h"
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
        return QString( "%1://%2" ).arg( protocol ).arg( info.absFilePath() ).ascii();
    }

    std::string ReadTargetApplication( const std::string& fileName, const tools::Loader_ABC& fileLoader )
    {
        std::string target = "gaming";
        try
        {
            std::auto_ptr< xml::xistream > xis = fileLoader.LoadFile( fileName );
            *xis >> xml::start( "exercise" )
                    >> xml::optional() >> xml::start( "meta" )
                        >> xml::optional() >> xml::start( "tutorial" )
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
                    >> xml::optional() >> xml::start( "meta" )
                        >> xml::optional() >> xml::start( "resources" )
                            >> xml::list( "resource", loadingWrapper, &ResourcesLoadingWrapper::ReadResource )
                        >> xml::end()
                    >> xml::end()
                >> xml::end();
        }
        catch( ... )
        {
            // NOTHING
        }
        return result;
    }

    QString MakeTitle( const QString& title )
    {
        return title.isEmpty() ? tools::translate( "ScenarioLauncherPage", "Scenario" ) : title;
    }
    QString LocalizedTitle( const QString& title )
    {
        return tools::translate( "ScenarioLauncherPage", "Starting %1" ).arg( MakeTitle( title ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( Q3WidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const frontend::Config& config, const tools::Loader_ABC& fileLoader, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter, const QString& title /* = ""*/ )
    : LauncherClientPage( pages, MakeTitle( title ), previous, eButtonBack | eButtonStart, launcher )
    , config_( config )
    , fileLoader_( fileLoader )
    , controllers_( controllers )
    , interpreter_( interpreter )
    , progressPage_( new ProgressPage( pages, *this, LocalizedTitle( title ) ) )
    , exercise_( 0 )
{
    Q3VBox* box = new Q3VBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        frontend::TabWidget* tabs = new frontend::TabWidget( box );
        {
            exercises_ = new ExerciseList( tabs, config_, fileLoader_, controllers, true, true, true, false );
            exercises_->setBackgroundOrigin( QWidget::WindowOrigin );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC&, const frontend::Profile& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
            tabs->addTab( exercises_, tools::translate( "ScenarioLauncherPage", "General" ) );
        }
        {
            Q3GroupBox* configBox = new Q3GroupBox( 1, Qt::Vertical, tabs );
            configBox->setMargin( 5 );
            configBox->setFrameShape( Q3GroupBox::NoFrame );
            configBox->setBackgroundOrigin( QWidget::WindowOrigin );
            frontend::TabWidget* config = new frontend::TabWidget( configBox );
            tabs->addTab( configBox, tools::translate( "ScenarioLauncherPage", "Settings" ) );
            {
                frontend::CheckpointConfigPanel* panel = AddPlugin< frontend::CheckpointConfigPanel >( config );
                connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const frontend::Profile& ) ), panel, SLOT( Select( const frontend::Exercise_ABC& ) ) );
                connect( exercises_, SIGNAL( ClearSelection() ), panel, SLOT( ClearSelection() ) );
                connect( panel, SIGNAL( CheckpointSelected( const QString&, const QString& ) ), SLOT( OnSelectCheckpoint( const QString&, const QString& ) ) );
                AddPlugin< frontend::SessionConfigPanel >( config );
                AddPlugin< frontend::RandomPluginConfigPanel >( config );
                AddPlugin< frontend::AdvancedConfigPanel >( config );
            }
        }
        {
            frontend::PluginConfigBuilder builder( config_, tabs );
            plugins_.push_back( builder.BuildFromXml()
                                       .Build< frontend::EdxlHavePluginConfigPanel >()
                                       .Finalize() );
        }
    }
    EnableButton( eButtonStart, false );
    AddContent( box );
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
    if( !CanBeStarted() || ! dialogs::KillRunningProcesses( this ) )
        return;
    const QString exerciseName = exercise_->GetName().c_str();

    const std::string target = ReadTargetApplication( config_.GetExerciseFile( exerciseName.ascii() ), fileLoader_ );
    if( target == "gaming" )
    {
        const QString session = session_.isEmpty() ? BuildSessionName().c_str() : session_;
        CreateSession( exerciseName, session );
        boost::shared_ptr< frontend::SpawnCommand > simulation( new frontend::StartExercise( config_, exerciseName, session, checkpoint_, true ) );
        boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinExercise( config_, exerciseName, session, profile_.GetLogin(), true ) );
        boost::shared_ptr< CompositeProcessWrapper > process( new CompositeProcessWrapper( *progressPage_, simulation, client ) );
        progressPage_->Attach( process );
        process->Start();
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
        boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinAnalysis( config_, exerciseName, "default", profile_.GetLogin(), port, true ) );
        boost::shared_ptr< CompositeProcessWrapper >  process( new CompositeProcessWrapper( *progressPage_, replay, client ) );
        progressPage_->Attach( process );
        process->Start();
        progressPage_->show();
    }
    if( target != "gaming" )
    {
        const QStringList resources = GetResources( config_.GetExerciseFile( exerciseName.ascii() ), fileLoader_ );
        if( ! resources.empty() )
        {
            std::string file = *resources.begin();
            file = ( bfs::path( config_.GetExerciseDir( exerciseName.ascii() ), bfs::native ) / file ).native_file_string();
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
        frontend::CreateSession action( config_, exercise.ascii(), session.ascii() );
        action.SetDefaultValues();
        action.Commit();
    }
    {
        BOOST_FOREACH( const T_Plugins::value_type& plugin, plugins_ )
            plugin->Commit( exercise.ascii(), session.ascii() );
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
        const std::string target = ReadTargetApplication( config_.GetExerciseFile( exercise_->GetName().c_str() ), fileLoader_ );
        if( target == "gaming" || target == "replayer" )
            return profile_.IsValid();
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
// Name: ScenarioLauncherPage::AddPlugin
// Created: SBO 2009-12-09
// -----------------------------------------------------------------------------
template< typename T >
T* ScenarioLauncherPage::AddPlugin( QTabWidget* tabs )
{
    std::auto_ptr< T > plugin( new T( tabs, config_ ) );
    if( plugin.get() && plugin->IsAvailable() )
    {
        tabs->addTab( plugin.get(), plugin->GetName() );
        plugins_.push_back( plugin.get() );
        return plugin.release();
    }
    return 0;
}
