// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// LTO
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ImportPage.h"
#include "moc_ImportPage.cpp"
#include "ProcessWrapper.h"
#include "ProgressPage.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/EditExercise.h"
#include "frontend/ImportExercise.h"
#include "tools/GeneralConfig.h"
#include <qcombobox.h>
#include <QFileDialog.h>

// -----------------------------------------------------------------------------
// Name: ImportPage constructor
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ImportPage::ImportPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers )
    : ContentPage( pages, tools::translate( "ImportPage", "Scenario" ), previous, eButtonBack | eButtonEdit )
    , config_( config )
    , controllers_( controllers )
    , progressPage_( new ProgressPage( pages, *this, tools::translate( "ImportPage", "Editing exercise" ), controllers ) )
{
    QVBox* mainBox = new QVBox( this );
    QGroupBox* box = new QGroupBox( 1, Qt::Horizontal, mainBox );
    box->setFlat( true );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setInsideSpacing( 10 );
    AddModelChoice( box );
    AddScenarioOutput( box );
    AddScenarioInput( box );
    AddTerrainChoice( box );
    AddContent( mainBox );
}

// -----------------------------------------------------------------------------
// Name: ImportPage::AddModelChoice
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::AddModelChoice( QGroupBox* box )
{
    QGroupBox* hbox = new QGroupBox( 1, Qt::Vertical, box );
    QLabel* label = new QLabel( tools::translate( "ImportPage", "Model: " ), hbox );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QComboBox* editModelList = new QComboBox( hbox );
    connect( editModelList, SIGNAL( activated ( const QString & ) ), SLOT( OnModelChanged( const QString & ) ) );
    editModelList->insertItem( tools::translate( "ImportPage", "Model:" ) );
    QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
        for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            editModelList->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ) );
    }
    if( editModelList->count() == 2 )
        editModelList->setCurrentItem( 1 );
    editModelList->setShown( editModelList->count() > 2 );
}

// -----------------------------------------------------------------------------
// Name: ImportPage::AddScenarioOutput
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::AddScenarioOutput( QGroupBox* box )
{
    QHBox* scenarioBox = new QHBox( box );
    QLabel* label = new QLabel( tools::translate( "ImportPage", "Output Scenario Name" ), scenarioBox );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QLineEdit* editName = new QLineEdit( tools::translate( "ImportPage", "Scenario Name" ), scenarioBox );
    editName->setBackgroundOrigin( QWidget::WindowOrigin );
    connect( editName, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnOutputName( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ImportPage::AddScenarioInput
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::AddScenarioInput( QGroupBox* box )
{
    QHBox* hbox = new QHBox( box );
    hbox->setBackgroundOrigin( QWidget::WindowOrigin );
    QLabel* label = new QLabel( tools::translate( "ImportPage", "Input Scenario" ), hbox );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    browse_ = new QPushButton( tools::translate( "ImportPage", "..." ), hbox );
    connect( browse_, SIGNAL( clicked() ), SLOT( OnChangeScenario() ) );
}

// -----------------------------------------------------------------------------
// Name: ImportPage::AddTerrainChoice
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void ImportPage::AddTerrainChoice( QGroupBox* box )
{
    QComboBox* editTerrainList = new QComboBox( box );
    connect( editTerrainList, SIGNAL( activated( const QString& ) ), SLOT( OnTerrainChanged( const QString& ) ) );
    editTerrainList->clear();
    editTerrainList->insertItem( tools::translate( "ScenarioEditPage", "Terrain:" ) );
    editTerrainList->insertStringList( frontend::commands::ListTerrains( config_ ) );
}

// -----------------------------------------------------------------------------
// Name: ImportPage destructor
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ImportPage::~ImportPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ImportPage::OnModelChanged
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::OnModelChanged( const QString & model )
{
    model_ = model;
}

// -----------------------------------------------------------------------------
// Name: ImportPage::OnTerrainChanged
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void ImportPage::OnTerrainChanged( const QString& terrain )
{
    terrain_ = terrain;
}

// -----------------------------------------------------------------------------
// Name: ImportPage::OnOutputName
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void ImportPage::OnOutputName( const QString& scenario )
{
    outputScenario_ = scenario;
}

// -----------------------------------------------------------------------------
// Name: ImportPage::OnChangeScenario
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::OnChangeScenario()
{
    inputScenario_ = QFileDialog::getOpenFileName( QString::null, "*.xml", this );
    browse_->setText( inputScenario_ );
}

// -----------------------------------------------------------------------------
// Name: ImportPage::OnEdit
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::OnEdit()
{
    if( !terrain_.isEmpty() && !outputScenario_.isEmpty() )
    {
        const QStringList model = QStringList::split( "/", model_ );
        frontend::CreateExercise( config_, outputScenario_.ascii(), terrain_.ascii(), model.front().ascii(), model.back().ascii() );
        LaunchImport();
        // $$$$ Wait for import to complete, and then launch prepa or have import app launch prepa itself.
        //LaunchPreparation();
    }
}
    
// -----------------------------------------------------------------------------
// Name: ImportPage::LaunchImport
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void ImportPage::LaunchImport()
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::ImportExercise( config_, inputScenario_, outputScenario_, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();        
}
    
// -----------------------------------------------------------------------------
// Name: ImportPage::LaunchPreparation
// Created: LDC 2010-07-06
// -----------------------------------------------------------------------------
void ImportPage::LaunchPreparation()
{
    boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, outputScenario_, true ) );
    boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
    progressPage_->Attach( process );
    progressPage_->show();
}

