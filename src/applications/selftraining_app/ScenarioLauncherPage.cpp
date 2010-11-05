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
#include "ProcessWrapper.h"
#include "frontend/AdvancedConfigPanel.h"
#include "frontend/CheckpointConfigPanel.h"
#include "frontend/CreateSession.h"
#include "frontend/CrossbowPluginConfigPanel.h"
#include "frontend/EdxlHavePluginConfigPanel.h"
#include "frontend/TimelinePluginConfigPanel.h"
#include "frontend/DisPluginConfigPanel.h"
#include "frontend/EditExercise.h"
#include "frontend/HlaPluginConfigPanel.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/JoinExercise.h"
#include "frontend/RandomPluginConfigPanel.h"
#include "frontend/SessionConfigPanel.h"
#include "frontend/StartExercise.h"
#include "frontend/StartReplay.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/LinkInterpreter_ABC.h"
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include <boost/foreach.hpp>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <xeumeuleu/xml.hpp>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#pragma warning( pop )

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
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, const QString& title /*= ""*/ )
    : ContentPage( pages, title.isEmpty() ? tools::translate( "ScenarioLauncherPage", "Scenario" ) : title, previous, eButtonBack | eButtonStart )
    , config_( config )
    , controllers_( controllers )
    , interpreter_( interpreter )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioLauncherPage", "Starting %1" ).arg( title.isEmpty() ? tools::translate( "ScenarioLauncherPage", "Scenario" ) : title ), controllers ) )
    , lister_( config, "" )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 );
    {
        TabWidget* tabs = new TabWidget( box );
        {
            exercises_ = new ExerciseList( tabs, config_, lister_, "", true, true, true, false );
            exercises_->setBackgroundOrigin( QWidget::WindowOrigin );
            connect( exercises_, SIGNAL( Select( const QString&, const Profile& ) ), this, SLOT( OnSelect( const QString&, const Profile& ) ) );
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
                connect( exercises_, SIGNAL( Select( const QString&, const Profile& ) ), panel, SLOT( Select( const QString& ) ) );
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
    exercises_->Update();
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
    const std::string target = ReadTargetApplication( config_, exercise_ );
    if( target == "gaming" )
    {
        const QString session = session_.isEmpty() ? BuildSessionName().c_str() : session_;
        CreateSession( exercise_, session );
        boost::shared_ptr< frontend::SpawnCommand > simulation( new frontend::StartExercise( config_, exercise_, session, checkpoint_, true ) );
        boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinExercise( config_, exercise_, session, profile_.GetLogin(), true ) );
        boost::shared_ptr< frontend::Process_ABC > process( new CompositeProcessWrapper( controllers_.controller_, simulation, client ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
    else if( target == "preparation" )
    {
        boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, exercise_, true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
    else if( target == "replayer" )
    {
        const unsigned int port = lister_.GetPort( exercise_ );
        CreateSession( exercise_, "default" );
        boost::shared_ptr< frontend::SpawnCommand > replay( new frontend::StartReplay( config_, exercise_, "default", port, true ) );
        boost::shared_ptr< frontend::SpawnCommand > client( new frontend::JoinAnalysis( config_, exercise_, "default", profile_.GetLogin(), port, true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new CompositeProcessWrapper( controllers_.controller_, replay, client ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
    if( target != "gaming" )
    {
        const QStringList resources = GetResources( config_, exercise_ );
        if( ! resources.empty() )
        {
            std::string file = *resources.begin();
            file = ( bfs::path( config_.GetExerciseDir( exercise_.ascii() ), bfs::native ) / file ).native_file_string();
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
        // force the networklogger to be used
        action.SetOption( "session/config/simulation/debug/@networklogger", true );
        action.SetOption( "session/config/simulation/debug/@networkloggerport", 20000 );
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
void ScenarioLauncherPage::OnSelect( const QString& exercise, const Profile& profile )
{
    exercise_ = exercise;
    profile_ = profile;
    EnableButton( eButtonStart, CanBeStarted() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage::CanBeStarted
// Created: SBO 2010-08-04
// -----------------------------------------------------------------------------
bool ScenarioLauncherPage::CanBeStarted() const
{
    const std::string target = ReadTargetApplication( config_, exercise_ );
    if( target == "gaming" || target == "replayer" )
        return !exercise_.isEmpty() && profile_.IsValid();
    if( target == "preparation" )
        return !exercise_.isEmpty();
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
