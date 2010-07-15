// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "CreateExerciceWidget.h"
#include "moc_CreateExerciceWidget.cpp"
#include "ScenarioEditPage.h"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
CreateExerciceWidget::CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config )
    : QGroupBox( 1, Qt::Vertical, parent )
    , config_( config )
    , page_( page )
{
    setFrameShape( QFrame::NoFrame );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    QLabel* label = new QLabel( tools::translate( "CreateExerciceWidget", "Create new exercise:" ), this );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    editName_ = new QLineEdit( tools::translate( "CreateExerciceWidget", "Enter exercise name" ), this );
    connect( editName_, SIGNAL( textChanged( const QString& ) ), &page, SLOT( EditNameChanged( const QString& ) ) );
    editTerrainList_ = new QComboBox( this );
    connect( editTerrainList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );
    editModelList_ = new QComboBox( this );
    connect( editModelList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget destructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
CreateExerciceWidget::~CreateExerciceWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::Update
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::Update()
{
    editTerrainList_->clear();
    editTerrainList_->insertItem( tools::translate( "CreateExerciceWidget", "Terrain:" ) );
    editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
    editModelList_->clear();
    editModelList_->insertItem( tools::translate( "CreateExerciceWidget", "Model:" ) );
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
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::CreateExercise
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::CreateExercise()
{
    if( editTerrainList_->currentItem() > 0 && editModelList_->currentItem() > 0 )
    {
        if( editName_->text().isEmpty() || page_.ExerciceExists( editName_->text() ) )
            return;
        const std::string terrain = editTerrainList_->currentText().ascii();
        const QStringList model = QStringList::split( "/", editModelList_->currentText() );
        frontend::CreateExercise( config_, editName_->text().ascii(), terrain, model.front().ascii(), model.back().ascii() );
        page_.Edit( editName_->text() );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::EnableEditButton
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
bool CreateExerciceWidget::EnableEditButton()
{
    return editTerrainList_->currentItem() > 0 && editModelList_->currentItem() > 0
        && !editName_->text().isEmpty() && !page_.ExerciceExists( editName_->text() )
        && editName_->text() != tools::translate( "CreateExerciceWidget", "Enter exercise name" );
}
