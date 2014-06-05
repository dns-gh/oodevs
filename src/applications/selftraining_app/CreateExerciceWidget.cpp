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
#include "clients_kernel/Tools.h"
#include "frontend/CheckpointList.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/ListViewHelper.h"
#include "ScenarioEditPage.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

namespace fc = frontend::commands;

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
CreateExerciceWidget::CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : gui::WidgetLanguageObserver_ABC< QGroupBox >( parent )
    , config_( config )
    , fileLoader_( fileLoader )
    , page_( page )
{
    //name selection label
    createLabel_ = new QLabel();
    editName_ = new QLineEdit();
    connect( editName_, SIGNAL( textChanged( const QString& ) ), &page, SLOT( EditNameChanged( const QString& ) ) );

    //terrain selection combobox
    editTerrainList_ = new QComboBox();
    editTerrainList_->setMinimumWidth( 210 );
    connect( editTerrainList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );

    //model selection combobox
    editModelList_ = new QComboBox();
    editModelList_->setMinimumWidth( 210 );
    connect( editModelList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );

    //exercise selection box
    QVBoxLayout* boxLayout = new QVBoxLayout();
    boxLayout->addWidget( editTerrainList_ );
    boxLayout->addWidget( editModelList_ );

    //save as box
    QHBoxLayout* saveBoxLayout = new QHBoxLayout();
    saveBoxLayout->addWidget( createLabel_ );
    saveBoxLayout->addWidget( editName_ );
    saveBoxLayout->addLayout( boxLayout );
    {
        {
            copyLabel_ = new QLabel();
            exerciseList_ = new QListWidget();
            exerciseList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
            connect( exerciseList_, SIGNAL( itemSelectionChanged() ), SLOT( OnSelectionChanged() ) );

            contentList_ = new QTreeView();
            contentList_->setModel( &contentListModel_ );
            contentList_->setHeaderHidden( true );
            contentList_->setEditTriggers( 0 );
            contentList_->setItemsExpandable( false );
            contentList_->adjustSize();
            contentList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
        }
        {
            //session panel
            sessionLabel_ = new QLabel();
            sessionList_ = new QListWidget();
            sessionList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
            connect( sessionList_, SIGNAL( currentTextChanged( const QString& ) ), SLOT( OnSessionSelected( const QString& ) ) );

            QVBoxLayout* sessionLayout = new QVBoxLayout();
            sessionLayout->setSpacing( 5 );
            sessionLayout->addWidget( sessionLabel_ );
            sessionLayout->addWidget( sessionList_ );

            //checkpoint panel
            checkpointList_ = new frontend::CheckpointList( config );
            connect( checkpointList_, SIGNAL( Select( const tools::Path& ) ), SLOT( OnCheckpointSelected( const tools::Path& ) ) );

            //checkpoint general selection panel
            checkpointCopyPanel_ = new QWidget();
            QHBoxLayout* checkpointCopyPanelLayout = new QHBoxLayout( checkpointCopyPanel_ );
            checkpointCopyPanelLayout->addLayout( sessionLayout );
            checkpointCopyPanelLayout->addWidget( checkpointList_ );
        }
        copyTab_ = new QTabWidget();
        contentListId_ = copyTab_->addTab( contentList_, "Content to copy" );
        checkpointCopyPanelId_ = copyTab_->addTab( checkpointCopyPanel_, "Checkpoint" );
        connect( copyTab_, SIGNAL( currentChanged( int ) ), SLOT( CurrentTabChanged( int ) ) );

        saveAsGroupBox_ = new QGroupBox();
        saveAsGroupBox_->setCheckable( true );
        saveAsGroupBox_->setChecked( false );
        connect( saveAsGroupBox_, SIGNAL( toggled( bool ) ), checkpointList_, SLOT( Toggle( bool ) ) );
        connect( saveAsGroupBox_, SIGNAL( toggled( bool ) ), &page, SLOT( ToggleChanged( bool ) ) );
        QVBoxLayout* saveAslayout = new QVBoxLayout( saveAsGroupBox_ );
        saveAslayout->setMargin( 10 );
        saveAslayout->setSpacing( 10 );
        saveAslayout->setAlignment( Qt::AlignTop );
        saveAslayout->addWidget( copyLabel_ );
        saveAslayout->addWidget( exerciseList_ );
        saveAslayout->addWidget( copyTab_ );
    }
    //global layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 5 );
    layout->addLayout( saveBoxLayout );
    layout->addWidget( saveAsGroupBox_ );

    UpdateExercises();
    UpdateTerrains();
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
// Name: CreateExerciceWidget::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnLanguageChanged()
{
    editName_->setPlaceholderText( tools::translate( "CreateExerciceWidget", "Enter exercise name" ) );
    saveAsGroupBox_->setTitle( tools::translate( "CreateExerciceWidget", "Create as copy of:" ) );
    createLabel_->setText( tools::translate( "CreateExerciceWidget", "Create new exercise:" ) );
    copyLabel_->setText( tools::translate( "CreateExerciceWidget", "Exercise to copy:" ) );
    copyTab_->setTabText( contentListId_, tools::translate( "CreateExerciceWidget", "Content to copy" ) );
    copyTab_->setTabText( checkpointCopyPanelId_, tools::translate( "CreateExerciceWidget", "Checkpoint" ) );
    sessionLabel_->setText( tools::translate( "CreateExerciceWidget", "Session:" ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::Update
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::Update()
{
    QList< QListWidgetItem* > items = exerciseList_->selectedItems();
    if( !items.empty() )
    {
        const tools::Path exercise = tools::Path::FromUnicode( items[ 0 ]->text().toStdWString() );
        contentListModel_.clear();
        frontend::BuildExerciseFeatures( exercise, config_, contentListModel_ );
        contentList_->expandAll();
        sessionList_->clear();
        sessionList_->addItems( fc::PathListToQStringList( fc::ListSessionsWithCheckpoint( config_, exercise ) ) );
        sessionList_->setEnabled( sessionList_->count() > 0 );
        if( !sessionList_->count() )
            sessionList_->addItem( tools::translate( "CreateExerciceWidget", "No session" ) );
        sessionList_->setCurrentRow( 0 );
    }
    if( exerciseList_->count() != static_cast< int >( fc::ListExercises( config_ ).size()) )
        UpdateExercises( items.empty() ? 0 : items[ 0 ]->text() );
    if( editTerrainList_->count() - 1 != static_cast< int >( fc::ListTerrains( config_ ).size()) )
        UpdateTerrains();
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::UpdateExercises
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::UpdateExercises( QString selectedItem )
{
    //models
    editModelList_->clear();
    editModelList_->addItem( tools::translate( "CreateExerciceWidget", "Model:" ) );
    tools::Path::T_Paths decisionalModels = fc::ListModels( config_ );
    for( auto it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const tools::Path::T_Paths physicalModels = fc::ListPhysicalModels( config_, *it );
        for( auto itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            editModelList_->addItem( QString( "%1/%2" ).arg( it->ToUTF8().c_str() ).arg( itP->ToUTF8().c_str() ) );
    }
    if( editModelList_->count() == 2 )
        editModelList_->setCurrentIndex( 1 );
    editModelList_->setVisible( editModelList_->count() > 2 );

    //exercises
    exerciseList_->clear();
    exerciseList_->addItems( fc::PathListToQStringList( fc::ListExercises( config_ ) ) );
    QList< QListWidgetItem* > items = exerciseList_->findItems( selectedItem, Qt::MatchExactly );
    if( !items.isEmpty() )
        exerciseList_->setItemSelected( items[ 0 ], true );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::UpdateTerrains
// Created: NPT 2012-12-17
// -----------------------------------------------------------------------------
void CreateExerciceWidget::UpdateTerrains()
{
    editTerrainList_->clear();
    editTerrainList_->addItem( tools::translate( "CreateExerciceWidget", "Terrain:" ) );
    editTerrainList_->addItems( fc::PathListToQStringList( fc::ListTerrains( config_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::CreateExercise
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::CreateExercise()
{
    if( editTerrainList_->currentIndex() > 0 && editModelList_->currentIndex() > 0 )
    {
        const tools::Path name = tools::Path::FromUnicode( editName_->text().toStdWString() );
        if( name.IsEmpty() || page_.ExerciceExists( name ) )
            return;

        const tools::Path terrain = tools::Path::FromUnicode( editTerrainList_->currentText().toStdWString() );
        const QStringList model = editModelList_->currentText().split( "/" );
        const tools::Path decisionalModel = tools::Path::FromUnicode( model.front().toStdWString() );
        const tools::Path physicalModel = tools::Path::FromUnicode( model.back().toStdWString() );

        if( saveAsGroupBox_->isChecked() )
        {
            QListWidgetItem* item = exerciseList_->currentItem();
            if( item == 0 )
                return;
            const tools::Path exercise = tools::Path::FromUnicode( item->text().toStdWString() );

            frontend::ExerciseCopyParameters params;
            params.to_ = name;
            params.terrain_ = terrain;
            params.model_ = decisionalModel;
            params.physical_ = physicalModel;
            params.from_ = exercise;
            if( contentList_->isVisible() )
            {
                params.itemModel_ = &contentListModel_;
                frontend::CreateExerciseAsCopyOf( config_, params );
            }
            else if( checkpointCopyPanel_->isVisible() && !checkpoint_.IsEmpty() )
            {
                tools::Path checkpointPath = config_.GetCheckpointsDir( exercise, session_ );
                params.checkpoint_ = checkpointPath / checkpoint_;
                frontend::CreateExerciseAsCopyOfCheckpoint( config_, params );
            }
        }
        else
            frontend::CreateExercise( config_, name, terrain, decisionalModel, physicalModel );

        page_.Edit( name );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::EnableEditButton
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
bool CreateExerciceWidget::EnableEditButton()
{
    return editTerrainList_->currentIndex() > 0 && editModelList_->currentIndex() > 0
        && !editName_->text().isEmpty() && !page_.ExerciceExists( tools::Path::FromUnicode( editName_->text().toStdWString() ) )
        && ( !saveAsGroupBox_->isChecked()
            || ( exerciseList_->currentItem() != 0 && contentList_->isVisible()
            || ( checkpointCopyPanel_->isVisible() && !checkpoint_.IsEmpty() ) ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnSelectionChanged
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnSelectionChanged()
{
    QList< QListWidgetItem* > items = exerciseList_->selectedItems();
    if( items.empty() )
        return;
    std::unique_ptr< xml::xistream > xis= fileLoader_.LoadFile( config_.GetExerciseFile( tools::Path::FromUnicode( items[ 0 ]->text().toStdWString() ) ) );
    std::string terrain, physical, dataSet;
    *xis >> xml::start( "exercise" )
            >> xml::start( "terrain" )
                >> xml::attribute( "name", terrain )
            >> xml::end
            >> xml::start( "model" )
                >> xml::attribute( "dataset", dataSet )
                >> xml::attribute( "physical", physical )
            >> xml::end
         >> xml::end;
    const QStringList terrainList = fc::PathListToQStringList( fc::ListTerrains( config_ ) );
    int index = terrainList.indexOf( terrain.c_str() );
    if( index != -1 )
        editTerrainList_->setCurrentIndex( index + 1 );
    const tools::Path::T_Paths decisionalModels = fc::ListModels( config_ );
    int indexModel = 0;
    for( auto it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = fc::PathListToQStringList( fc::ListPhysicalModels( config_, *it ) );
        if( it->ToUTF8() == dataSet )
        {
            int index = physicalModels.indexOf( QString( physical.c_str() ) );
            if( index != -1 )
                editModelList_->setCurrentIndex( indexModel + index + 1 );
        }
        else
            indexModel += physicalModels.size();
    }
    Update();
    page_.UpdateEditButton();
}
// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnSessionSelected
// Created: NPT 2012-09-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnSessionSelected( const QString& session )
{
    if( QListWidgetItem* item = exerciseList_->currentItem() )
    {
        session_ = tools::Path::FromUnicode( session.toStdWString() );
        checkpointList_->Update( tools::Path::FromUnicode( item->text().toStdWString() ), session_ );
        page_.UpdateEditButton();
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnCheckpointSelected
// Created: NPT 2012-09-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnCheckpointSelected( const tools::Path& checkpoint )
{
    checkpoint_ = fc::CheckpointExists( config_, tools::Path::FromUnicode( exerciseList_->currentItem()->text().toStdWString() ), session_, checkpoint ) ? checkpoint : "";
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::CurrentTabChanged
// Created: NPT 2012-09-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::CurrentTabChanged( int )
{
    page_.UpdateEditButton();
}
