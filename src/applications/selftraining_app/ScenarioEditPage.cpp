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
#include "ExerciseList.h"
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
    : ContentPage( pages, tools::translate( "ScenarioEditPage", "Scenario" ), previous, eButtonBack | eButtonEdit )
    , config_( config )
    , controllers_( controllers )
    , lister_( config_, "" )
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
        connect( editTerrainList_, SIGNAL( activated( int ) ), SLOT( ComboChanged( int ) ) );
        editModelList_ = new QComboBox( hbox );
        connect( editModelList_, SIGNAL( activated( int ) ), SLOT( ComboChanged( int ) ) );
        createButton_ = new QPushButton( tools::translate( "ScenarioEditPage", "Create" ), hbox );
        createButton_->setEnabled( false );
        QPalette p( palette() );
        p.setColor( QPalette::Active, QColorGroup::ButtonText, QColor( 240, 240, 240 ) );
        p.setColor( QPalette::Disabled, QColorGroup::Text, QColor( 140, 140, 150 ) );
        createButton_->setPalette( p );
        connect( createButton_, SIGNAL( clicked() ), this, SLOT( CreateExercise() ) );
    }
    {
        exercises_ = new ExerciseList( box, config_, lister_, "", true, false );
        connect( exercises_, SIGNAL( Select( const QString&, const Profile& ) ), this, SLOT( OnSelect( const QString&, const Profile& ) ) );
    }
    EnableButton( eButtonEdit, false );
    AddContent( box );
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
    exercises_->Update();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::CreateExercise
// Created: RDS 2008-09-09
// -----------------------------------------------------------------------------
void ScenarioEditPage::CreateExercise()
{
    if( editTerrainList_->currentItem() > 0 && editModelList_->currentItem() > 0 )
    {
        if( editName_->text().isEmpty() || exercises_->Exists( editName_->text() ) )
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
void ScenarioEditPage::OnEdit()
{
    if( !exercises_->Exists( editName_->text() ) && !exercise_.isEmpty() )
    {
        exercises_->ChangeExerciceParameters( exercise_.ascii() );
        Edit( exercise_ );
    }
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

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::OnSelect
// Created: SBO 2010-04-15
// -----------------------------------------------------------------------------
void ScenarioEditPage::OnSelect( const QString& exercise, const Profile& )
{
    exercise_ = exercise;
    EnableButton( eButtonEdit, !exercise_.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::EditNameChanged
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void ScenarioEditPage::EditNameChanged( const QString& )
{
    UpdateCreateButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::ComboChanged
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void ScenarioEditPage::ComboChanged( int )
{
    UpdateCreateButton();
}

// -----------------------------------------------------------------------------
// Name: ScenarioEditPage::UpdateCreateButton
// Created: JSR 2010-06-11
// -----------------------------------------------------------------------------
void ScenarioEditPage::UpdateCreateButton()
{
    bool enable = editTerrainList_->currentItem() > 0 && editModelList_->currentItem() > 0
        && !editName_->text().isEmpty() && !exercises_->Exists( editName_->text() )
        && editName_->text() != tools::translate( "ScenarioEditPage", "Enter exercise name" );
    createButton_->setEnabled( enable );
}
