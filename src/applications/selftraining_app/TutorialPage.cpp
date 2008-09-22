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
#include "MessageDialog.h" 
#include "MenuButton.h" 
#include "moc_TutorialPage.cpp"
#include "ExerciseList.h" 
#include "SideList.h"
#include "SessionRunningPage.h" 
#include "SessionStatus.h" 
#include "frontend/commands.h"
#include "frontend/CreateSession.h" 
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include "frontend/commands.h" 
#include "clients_gui/Tools.h"
#include "clients_gui/LinkInterpreter_ABC.h" 
#include "tools/GeneralConfig.h"
#include <qlistbox.h>
#include <qtextedit.h>
#include <qtextbrowser.h> 
#include <qtabwidget.h> 
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qtabbar.h>
#include <qcursor.h>

#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 )
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp> 
#pragma warning( pop )

#include <xeumeuleu/xml.h>

namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;

bool HasODB( const tools::GeneralConfig& config, const QString& exercise ) 
{
    const std::string file = config.GetExerciseFile(exercise.ascii());    
    std::string orbatFile(""); 
    xml::xifstream xis( file );
    xis >> xml::start( "exercise" )
            >> xml::optional() >> xml::start( "orbat" )
                >> xml::attribute( "file", orbatFile)
            >> xml::end()
        >> xml::end(); 
    return ( orbatFile != "" ) ; 
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
    QStringList& stringList_ ; 
};

namespace
{
    std::string MakeLink( const std::string& file )
    {
        const QFileInfo info( file.c_str() );
        const QString protocol = info.extension( false ) == "exe" ? "cmd" : "file";
        return QString( "%1://%2" ).arg( protocol ).arg( info.absFilePath() ).ascii();
    }
}

QStringList GetResources( const tools::GeneralConfig& config, const QString& exercise ) 
{
    const std::string file = config.GetExerciseFile(exercise.ascii());    
    std::string orbatFile(""); 
    xml::xifstream xis( file );
    QStringList result ; 
    ResourcesLoadingWrapper loadingWrapper( result ) ; 
    xis >> xml::start( "exercise" )
            >> xml::optional() >> xml::start( "meta" )
                >> xml::optional() >> xml::start( "resources" )
                    >> xml::list( "resource", loadingWrapper, &ResourcesLoadingWrapper::ReadResource ) 
                >> xml::end()
            >> xml::end()
        >> xml::end(); 
    return result ; 
 }

// -----------------------------------------------------------------------------
// Name: TutorialPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
TutorialPage::TutorialPage( QWidgetStack* pages, Page_ABC& previous, SessionRunningPage& running, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, boost::shared_ptr< SessionStatus > sessionStatus  )
    : ContentPage( pages, tools::translate( "TutorialPage", "Tutorials" ), previous )
    , config_( config )
    , sessionStatus_( sessionStatus ) 
    , interpreter_ ( interpreter ) 
    , running_ ( running ) 
{
    QVBox* box = new QVBox ( this) ; 
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    exercises_ = new ExerciseList( box , config, "tutorials" )  ; ; 
    connect( exercises_, SIGNAL( Select( const QString& ) ), this, SLOT( OnStartExercise( const QString& ) ) ); 
    statusLabel_ = new QLabel( box ) ; 
    statusLabel_->setBackgroundOrigin( QWidget::WindowOrigin );
    QHBox* hbox = new QHBox( box );
    hbox->setBackgroundOrigin( QWidget::WindowOrigin );
    hbox->layout()->setAlignment( Qt::AlignRight );
    AddContent( box );    
    AddNextButton( tr( "Start")  , *this, SLOT( OnStart() ) ); 
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
void TutorialPage::OnStartExercise ( const QString& exercise )
{
     if ( HasODB( config_, exercise ) )
     {
        running_.SetSession( new SessionStatus ( new frontend::StartExercise( config_, exercise, "default" , true ), new frontend::JoinExercise ( config_, exercise, "default", true ) ) );  
        running_.show(); 
     }
     else
     {
        QStringList resources = GetResources( config_, exercise ) ; 
        if ( ! resources.empty() ) 
        {
            std::string file = * resources.begin(); 
            file = ( bfs::path( config_.GetExerciseDir( exercise.ascii() ) , bfs::native ) / file ).native_file_string(); 
            // file = config_.BuildDirectoryFile( exercise.ascii(), file );
            interpreter_.Interprete( MakeLink(file).c_str() ) ; 
        }
     }
}

// -----------------------------------------------------------------------------
// Name: TutorialPage::OnStart
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
void TutorialPage::OnStart( )
{
    QString exercise = exercises_->GetHighlight() ; 
    if ( exercise != "" ) 
        OnStartExercise( exercise ) ; 
}


