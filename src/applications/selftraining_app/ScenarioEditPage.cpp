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
#include "ProgressPage.h"
#include "ProcessWrapper.h"
#include "frontend/commands.h" 
#include "frontend/CreateExercise.h" 
#include "frontend/EditExercise.h" 
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include <qcombobox.h>

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage constructor
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
ScenarioEditPage::ScenarioEditPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers )
    : ContentPage( pages, tools::translate( "ScenarioEditPage", "Scenario" ), previous )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ScenarioEditPage", "Editing exercise" ), controllers ) )
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
        editModelList_ = new QComboBox( hbox );
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
    editTerrainList_->insertItem( tools::translate( "ScenarioEditPage", "Terrain:" ) );
    editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
    editModelList_->clear();
    editModelList_->insertItem( tools::translate( "ScenarioEditPage", "Model:" ) );
    QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
        for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            editModelList_->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ) );
    }
    if( editModelList_->count() == 2 )
        editModelList_->setCurrentItem( 1 );
    editModelList_->setShown( editModelList_->count() > 2 );
    editExerciseList_->clear();
    editExerciseList_->insertStringList( frontend::commands::ListExercises( config_ ) );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::CreateExercise
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::CreateExercise()
{
    if( editTerrainList_->currentItem() > 0 && editModelList_->currentItem() > 0 )
    {
        if( editName_->text().isEmpty() || editExerciseList_->findItem( editName_->text(), Qt::ExactMatch ) )
            return;
        const std::string terrain = editTerrainList_->currentText().ascii();
        const QStringList model = QStringList::split( "/", editModelList_->currentText() );
        frontend::CreateExercise( config_, editName_->text().ascii(), terrain, model.front().ascii(), model.back().ascii() );
        Edit( editName_->text() );
    }    
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::EditExercise
// Created: SBO 2008-10-07
// -----------------------------------------------------------------------------
void ScenarioEditPage::EditExercise()
{
    if( editExerciseList_->selectedItem() )
        Edit( editExerciseList_->selectedItem()->text() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::Edit
// Created: SBO 2008-10-16
// -----------------------------------------------------------------------------
void ScenarioEditPage::Edit( const QString& exercise )
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, exercise, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}
