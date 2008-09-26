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
#include "ScenarioLauncherPageOptions.h" 
#include "MessageDialog.h" 
#include "MenuButton.h" 
#include "moc_ScenarioLauncherPage.cpp"
#include "ExerciseList.h" 
#include "SideList.h"
#include "Session.h" 
#include "frontend/commands.h"
#include "frontend/StartExercise.h"
#include "frontend/JoinExercise.h"
#include "frontend/CreateSession.h"
#include "frontend/Createexercise.h" 
#include "frontend/EditExercise.h" 
#include "frontend/commands.h" 
#include "clients_gui/Tools.h"
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

namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ScenarioLauncherPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
ScenarioLauncherPage::ScenarioLauncherPage( QWidgetStack* pages, Page_ABC& previous, SessionRunningPage& running, const tools::GeneralConfig& config, boost::shared_ptr< Session > sessionStatus  )
    : ContentPage( pages, tools::translate( "ScenarioLauncherPage", "Scenario" ), previous )
    , config_( config )
    , sessionStatus_( sessionStatus ) 
{
    options_ = new ScenarioLauncherPageOptions( pages, *this, running, config ) ; 

    QVBox* box = new QVBox ( this ) ; 
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setMargin( 5 ) ; 
    exercises_ = new ExerciseList( box , config ) ; 
    QHBox* hbox = new QHBox( box );
    hbox->setBackgroundOrigin( QWidget::WindowOrigin );
    hbox->layout()->setAlignment( Qt::AlignRight );
    connect( exercises_, SIGNAL( Select( const QString& ) ), this, SLOT( OnStartExercise( const QString& ) ) );
    connect( exercises_, SIGNAL( Highlight( const QString& ) ), options_,  SLOT( Update( const QString& ) ) );
    AddContent( box );    
    AddNextButton( tr( "Start" ) , *options_ ) ; 
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



