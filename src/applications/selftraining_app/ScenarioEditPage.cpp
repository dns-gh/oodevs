// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ScenarioEditPage.h"
#include "moc_ScenarioEditPage.cpp"
#include "MenuButton.h" 
#include "Session.h" 
#include "clients_gui/Tools.h"
#include "frontend/commands.h" 
#include "frontend/CreateExercise.h" 
#include "frontend/EditExercise.h" 
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include <QComboBox.h>

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, boost::shared_ptr< Session > sessionStatus )
    : ContentPage( pages, tools::translate( "ScenarioEditPage", "Scenario" ), previous )
    , controllers_ ( controllers ) 
    , config_ ( config ) 
    , sessionStatus_ ( sessionStatus ) 
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    
    {
        QGroupBox* hbox = new QGroupBox( 1, Qt::Vertical, box );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "ScenarioEditPage", "Create new exercise:" ), hbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        editName_ = new QLineEdit( tools::translate( "ScenarioEditPage", "Enter exercise name" ), hbox );
        connect( editName_, SIGNAL( textChanged( const QString& ) ), SLOT( EditNameChanged( const QString& ) ) );
        editTerrainList_ = new QComboBox( hbox );
        QPushButton* button = new QPushButton( tools::translate( "ScenarioEditPage", "Create" ), hbox );
        connect( button, SIGNAL( clicked() ), this, SLOT( CreateExercise() ) );
    }
    {
        QGroupBox* vbox = new QGroupBox( 1, Qt::Horizontal, box );
        vbox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "ScenarioEditPage", "Select exercise to edit:" ), vbox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        editExerciseList_ = new QListBox( vbox );
    }
    AddContent( box ); 
    AddNextButton( tools::translate( "ScenarioEditPage", "Edit" ), *this, SLOT( EditExercise() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage destructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::~ScenarioEditPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::Update
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::Update()
{
    editTerrainList_->clear();
    editTerrainList_->insertItem( tools::translate( "ScenarioEditPage", "Select the terrain" ) );
    editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
    editExerciseList_->clear();
    editExerciseList_->insertStringList( frontend::commands::ListExercises( config_ ) );
}


// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::CreateExercise
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::CreateExercise()
{
    if( editTerrainList_->currentItem() > 0 )
    {
        const std::string terrain  = editTerrainList_->currentText().ascii();
        frontend::CreateExercise( config_, editName_->text().ascii(), terrain, "ada", "france" );
        sessionStatus_.reset( new Session ( controllers_.controller_, 0, new frontend::EditExercise( config_, editName_->text(), true )  ) );
        sessionStatus_->Start(); 
        Previous();
    }    
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::EditExercise
// Created: SBO 2008-10-07
// -----------------------------------------------------------------------------
void ScenarioEditPage::EditExercise()
{
    if( editExerciseList_->selectedItem() )
    {
        const std::string exercise = editExerciseList_->selectedItem()->text().ascii();
        sessionStatus_.reset( new Session( controllers_.controller_, 0, new frontend::EditExercise( config_, exercise.c_str(), true )  ) );
        sessionStatus_->Start(); 
        Previous();
    } 
}
