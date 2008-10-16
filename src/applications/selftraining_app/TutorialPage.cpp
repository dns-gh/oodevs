// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "TutorialPage.h"
#include "moc_TutorialPage.cpp"
#include "ExerciseList.h"
#include "ProgressPage.h"
#include "ProcessWrapper.h"
#include "CompositeProcessWrapper.h"
#include "frontend/commands.h"
#include "frontend/CreateSession.h" 
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include "frontend/EditExercise.h"
#include "frontend/StartReplay.h"
#include "frontend/JoinAnalysis.h"
#include "frontend/CommandLineTools.h"
#include "clients_gui/Tools.h"
#include "clients_gui/LinkInterpreter_ABC.h" 
#include "clients_kernel/Controllers.h" 
#include "tools/GeneralConfig.h"
#include <xeumeuleu/xml.h>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp> 
#pragma warning( pop )

namespace bfs = boost::filesystem;

namespace
{
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
        xml::xifstream xis( file );
        xis >> xml::start( "exercise" )
                >> xml::optional() >> xml::start( "meta" )
                    >> xml::optional() >> xml::start( "tutorial" )
                        >> xml::attribute( "target", target )
                >> xml::end()
            >> xml::end();
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
        xml::xifstream xis( config.GetExerciseFile( exercise.ascii() ) );
        QStringList result;
        ResourcesLoadingWrapper loadingWrapper( result );
        xis >> xml::start( "exercise" )
                >> xml::optional() >> xml::start( "meta" )
                    >> xml::optional() >> xml::start( "resources" )
                        >> xml::list( "resource", loadingWrapper, &ResourcesLoadingWrapper::ReadResource )
                    >> xml::end()
                >> xml::end()
            >> xml::end(); 
        return result;
     }
}

// -----------------------------------------------------------------------------
// Name: TutorialPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
TutorialPage::TutorialPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, gui::LinkInterpreter_ABC& interpreter )
    : ContentPage( pages, tools::translate( "TutorialPage", "Tutorials" ), previous )
    , config_( config )
    , controllers_ ( controllers )
    , interpreter_ ( interpreter )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "TutorialPage", "Starting tutorial" ), controllers ) )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    exercises_ = new ExerciseList( box , config, "tutorials" );
    connect( exercises_, SIGNAL( Select( const QString& ) ), this, SLOT( OnStartExercise( const QString& ) ) );
    AddContent( box );
    AddNextButton( tools::translate( "TutorialPage", "Start" ), *this, SLOT( OnStart() ) );
}

// -----------------------------------------------------------------------------
// Name: TutorialPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
TutorialPage::~TutorialPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::Update
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void TutorialPage::Update()
{
    exercises_->Update();
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStartExercise
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void TutorialPage::OnStartExercise( const QString& exercise )
{
    const std::string target = ReadTargetApplication( config_, exercise );
    if( target == "gaming" )
    {
        boost::shared_ptr< frontend::SpawnCommand > command1( new frontend::StartExercise( config_, exercise, "default", true ) );
        boost::shared_ptr< frontend::SpawnCommand > command2( new frontend::JoinExercise( config_, exercise, "default", true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new CompositeProcessWrapper( controllers_.controller_, command1, command2 ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
    else if( target == "preparation" )
    {
        boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, exercise, true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
    else if( target == "replayer" )
    {
        const unsigned int port = frontend::DispatcherPort( 1 ); // $$$$ SBO 2008-10-16: hard coded port
        boost::shared_ptr< frontend::SpawnCommand > command1( new frontend::StartReplay( config_, exercise, "default", port, true ) );
        boost::shared_ptr< frontend::SpawnCommand > command2( new frontend::JoinAnalysis( config_, exercise, port, true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new CompositeProcessWrapper( controllers_.controller_, command1, command2 ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }

    if( target != "gaming" )
    {
        const QStringList resources = GetResources( config_, exercise );
        if( ! resources.empty() )
        {
            std::string file = *resources.begin();
            file = ( bfs::path( config_.GetExerciseDir( exercise.ascii() ), bfs::native ) / file ).native_file_string();
            // file = config_.BuildDirectoryFile( exercise.ascii(), file );
            interpreter_.Interprete( MakeLink( file ).c_str() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStart
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void TutorialPage::OnStart()
{
    const QString exercise = exercises_->GetHighlight();
    if ( exercise != "" )
        OnStartExercise( exercise );
}
