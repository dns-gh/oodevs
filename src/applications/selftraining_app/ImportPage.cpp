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
    connect( editModelList, SIGNAL( currentIndexChanged ( const QString & ) ), SLOT( OnModelChanged( const QString & ) ) );
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
    // Either spawn a single process that will spawn Preparation afterwards or handle the steps here:
    // Spawn translation of scenario_ with model_ and if successful ask for a terrain:
    terrain_ = GenerateAndAskForTerrain();
    // Show errors
    ShowErrors();
    // Launch prepa
    if( !terrain_.isEmpty() )
    {
        const QStringList model = QStringList::split( "/", model_ );
        frontend::CreateExercise( config_, outputScenario_.ascii(), terrain_.ascii(), model.front().ascii(), model.back().ascii() );
        boost::shared_ptr< frontend::SpawnCommand > command( new frontend::EditExercise( config_, outputScenario_, true ) );
        boost::shared_ptr< frontend::Process_ABC >  process( new ProcessWrapper( controllers_.controller_, command ) );
        progressPage_->Attach( process );
        progressPage_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: ImportPage::ShowErrors
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
void ImportPage::ShowErrors()
{

}

// -----------------------------------------------------------------------------
// Name: ImportPage::GenerateAndAskForTerrain
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
QString ImportPage::GenerateAndAskForTerrain()
{
    return QString::null;
}
