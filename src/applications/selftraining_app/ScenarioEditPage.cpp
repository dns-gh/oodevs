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
// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, boost::shared_ptr< Session > sessionStatus )
    : ContentPage( pages, tools::translate( "ScenarioEditPage", "Scenario" ), previous )
    , config_ ( config ) 
    , sessionStatus_ ( sessionStatus ) 
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    
    QLabel* label ; 
    {
        label = new QLabel( tools::translate( "ScenarioEditPage", "New exercise name:" ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        editName_ = new QLineEdit( tools::translate( "ScenarioEditPage", "Enter exercise name" ), box );
        connect( editName_, SIGNAL( textChanged( const QString& ) ), SLOT( EditNameChanged( const QString& ) ) );
    }
    {
        label = new QLabel( tools::translate( "ScenarioEditPage", "Choose the terrain:" ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        editTerrainList_ = new QListBox( box );
    }

    AddContent( box ); 
    AddNextButton( "Create", *this, SLOT( CreateExercise() ) ); 
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
    editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
}


// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::CreateExercise
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::CreateExercise()
{
    if( editTerrainList_->selectedItem() )
    {
        const std::string terrain  = editTerrainList_->selectedItem()->text().ascii();
        frontend::CreateExercise( config_, editName_->text().ascii(), terrain, "ada", "france" );
        sessionStatus_.reset( new Session ( NULL, new frontend::EditExercise( config_, editName_->text(), true )  ) );
        sessionStatus_->Start(); 
        Previous();
    }    
}
