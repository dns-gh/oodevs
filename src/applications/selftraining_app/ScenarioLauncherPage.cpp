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
#include "frontend/CrossbowPluginConfigPanel.h"
#include "frontend/EdxlHavePluginConfigPanel.h"
#include "frontend/TimelinePluginConfigPanel.h"
#include "frontend/DisPluginConfigPanel.h"
#include "frontend/EditExercise.h"
#include "frontend/Exercise_ABC.h"
#include "frontend/HlaPluginConfigPanel.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/JoinExercise.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/RandomPluginConfigPanel.h"
#include "frontend/SessionConfigPanel.h"
#include "frontend/StartExercise.h"
#include "frontend/StartReplay.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/LinkInterpreter_ABC.h"
#include "clients_gui/Tools.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <qfileinfo.h>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;
namespace bpt = boost::posix_time;

namespace
{
    class TabWidget : public QTabWidget
    {
    public:
        explicit TabWidget( QWidget* parent ) : QTabWidget( parent )
        {
            setBackgroundOrigin( QWidget::WindowOrigin );
            tabBar()->setBackgroundOrigin( QWidget::WindowOrigin );
            setMargin( 0 );
        }
    };

    std::string MakeLink( const std::string& file )
    {
        const QFileInfo info( file.c_str() );
        const QString protocol = info.extension( false ) == "exe" ? "cmd" : "file";
        return QString( "%1://%2" ).arg( protocol ).arg( info.absFilePath() ).ascii();
    }

    std::string ReadTargetApplication( const tools::GeneralConfig& config, const QString& exercise )
    {
        const std::string file = config.GetExerciseFile( exercise.ascii() );
        std::string target = "gaming";
        try
        {
            xml::xifstream xis( file );
            xis >> xml::start( "exercise" )
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

    QStringList GetResources( const tools::GeneralConfig& config, const QString& exercise )
    {
        QStringList result;
        try
        {
            xml::xifstream xis( config.GetExerciseFile( exercise.ascii() ) );
            ResourcesLoadingWrapper loadingWrapper( result );
            xis >> xml::start( "exercise" )
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
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const frontend::Config& config, frontend::LauncherClient& launcher, gui::LinkInterpreter_ABC& interpreter, const QString& title /*= ""*/ )
    : LauncherClientPage( pages, MakeTitle( title ), previous, eButtonBack | eButtonStart, launcher )
    , config_( config )
    , controllers_( controllers )
    , interpreter_( interpreter )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioLauncherPage", "Starting %1" ).arg( MakeTitle( title ) ) ) )
    , exercise_( 0 )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        TabWidget* tabs = new TabWidget( box );
        {
            exercises_ = new ExerciseList( tabs, config_, controllers, true, true, true, false );
            exercises_->setBackgroundOrigin( QWidget::WindowOrigin );
            connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const Profile& ) ), SLOT( OnSelect( const frontend::Exercise_ABC&, const Profile& ) ) );
            connect( exercises_, SIGNAL( ClearSelection() ), SLOT( ClearSelection() ) );
            tabs->addTab( exercises_, tools::translate( "ScenarioLauncherPage", "General" ) );
        }
        {
            QGroupBox* configBox = new QGroupBox( 1, Qt::Vertical, tabs );
            configBox->setMargin( 5 );
            configBox->setFrameShape( QFrame::NoFrame );
            configBox->setBackgroundOrigin( QWidget::WindowOrigin );
            TabWidget* config = new TabWidget( configBox );
            tabs->addTab( configBox, tools::translate( "ScenarioLauncherPage", "Settings" ) );
            {
                frontend::CheckpointConfigPanel* panel = AddPlugin< frontend::CheckpointConfigPanel >( config, tools::translate( "ScenarioLauncherPage", "Checkpoints" ) );
                connect( exercises_, SIGNAL( Select( const frontend::Exercise_ABC&, const Profile& ) ), panel, SLOT( Select( const frontend::Exercise_ABC& ) ) );
                connect( exercises_, SIGNAL( ClearSelection() ), panel, SLOT( ClearSelection() ) );
                connect( panel, SIGNAL( CheckpointSelected( const QString&, const QString& ) ), SLOT( OnSelectCheckpoint( const QString&, const QString& ) ) );
                AddPlugin< frontend::SessionConfigPanel >( config, tools::translate( "ScenarioLauncherPage", "Session" ) );
                AddPlugin< frontend::RandomPluginConfigPanel >( config, tools::translate( "ScenarioLauncherPage", "Random" ) );
                AddPlugin< frontend::AdvancedConfigPanel >( config, tools::translate( "ScenarioLauncherPage", "Advanced" ) );
            }
        }
        {
            QGroupBox* pluginsBox = new QGroupBox( 1, Qt::Vertical, tabs );
            pluginsBox->setMargin( 5 );
            pluginsBox->setFrameShape( QFrame::NoFrame );
            pluginsBox->setBackgroundOrigin( QWidget::WindowOrigin );
            TabWidget* plugins = new TabWidget( pluginsBox );
            tabs->addTab( pluginsBox, tools::translate( "ScenarioLauncherPage", "Plugins" ) );
            {
                AddPlugin< frontend::DisPluginConfigPanel >( plugins, tools::translate( "ScenarioLauncherPage", "DIS" ) );
                AddPlugin< frontend::HlaPluginConfigPanel >( plugins, tools::translate( "ScenarioLauncherPage", "HLA" ) );
                AddPlugin< frontend::CrossbowPluginConfigPanel >( plugins, tools::translate( "ScenarioLauncherPage", "Crossbow" ) );
                AddPlugin< frontend::EdxlHavePluginConfigPanel >( plugins, tools::translate( "ScenarioLauncherPage", "EDXL-HAVE" ) );
                AddPlugin< frontend::TimelinePluginConfigPanel >( plugins, tools::translate( "ScenarioLauncherPage", "ERP/Timeline" ) );
            }
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
    const std::string target = ReadTargetApplication( config_, exerciseName );
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
        const QStringList resources = GetResources( config_, exerciseName );
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
void ScenarioLauncherPage::OnSelect( const frontend::Exercise_ABC& exercise, const Profile& profile )
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
    profile_ = Profile::Invalid;
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
        const std::string target = ReadTargetApplication( config_, exercise_->GetName().c_str() );
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
T* ScenarioLauncherPage::AddPlugin( QTabWidget* tabs, const QString& name )
{
    T* plugin = new T( tabs, config_ );
    tabs->addTab( plugin, name );
    plugins_.push_back( plugin );
    return plugin;
}
