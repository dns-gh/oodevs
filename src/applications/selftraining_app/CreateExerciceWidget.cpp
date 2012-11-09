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
#include "clients_gui/Tools.h"
#include "frontend/CheckpointList.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/ListViewHelper.h"
#include "ScenarioEditPage.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/filesystem/path.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
CreateExerciceWidget::CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : gui::LanguageChangeObserver_ABC< QGroupBox >( parent )
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
            connect( exerciseList_, SIGNAL( itemClicked( QListWidgetItem* ) ), SLOT( OnSelectionChanged( QListWidgetItem* ) ) );

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
            connect( sessionList_, SIGNAL( 	currentTextChanged( const QString& ) ), SLOT( OnSessionSelected( const QString& ) ) );

            QVBoxLayout* sessionLayout = new QVBoxLayout();
            sessionLayout->setSpacing( 5 );
            sessionLayout->addWidget( sessionLabel_ );
            sessionLayout->addWidget( sessionList_ );

            //checkpoint panel
            checkpointList_ = new frontend::CheckpointList( config );
            connect( checkpointList_, SIGNAL( Select( const QString& ) ), SLOT( OnCheckpointSelected( const QString& ) ) );

            //checkpoint general selection panel
            checkpointCopyPanel_ = new QWidget();
            QHBoxLayout* checkpointCopyPanelLayout = new QHBoxLayout( checkpointCopyPanel_ );
            checkpointCopyPanelLayout->addLayout( sessionLayout );
            checkpointCopyPanelLayout->addWidget( checkpointList_ );
        }
        copyTab_ = new QTabWidget();
        copyTab_->addTab( contentList_, "Content to copy" );
        copyTab_->addTab( checkpointCopyPanel_, "Checkpoint" );
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
    contentList_->setWindowTitle( tools::translate( "CreateExerciceWidget", "Content to copy" ) );
    checkpointCopyPanel_->setWindowTitle( tools::translate( "CreateExerciceWidget", "Checkpoint" ) );
    sessionLabel_->setText( tools::translate( "CreateExerciceWidget", "Session:" ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::Update
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::Update()
{
    QListWidgetItem* item = exerciseList_->currentItem();
    if( item )
    {
        std::string exercise( item->text().toStdString() );
        contentListModel_.clear();
        frontend::BuildExerciseFeatures( exercise, config_, contentListModel_ );
        contentList_->expandAll();
        sessionList_->clear();
        sessionList_->addItems( frontend::commands::ListSessionsWithCheckpoint( config_, exercise ) );
        sessionList_->setEnabled( sessionList_->count() );
        if( !sessionList_->count() )
            sessionList_->addItem( tools::translate( "CreateExerciceWidget", "No session" ) );
        sessionList_->setCurrentRow( 0 );
    }
    else
        UpdateExercises();
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::UpdateExercises
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::UpdateExercises()
{
    editTerrainList_->clear();
    editTerrainList_->addItem( tools::translate( "CreateExerciceWidget", "Terrain:" ) );
    editTerrainList_->addItems( frontend::commands::ListTerrains( config_ ) );
    editModelList_->clear();
    editModelList_->addItem( tools::translate( "CreateExerciceWidget", "Model:" ) );
    QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toStdString() );
        for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            editModelList_->addItem( QString( "%1/%2" ).arg( *it ).arg( *itP ) );
    }
    if( editModelList_->count() == 2 )
        editModelList_->setCurrentIndex( 1 );
    editModelList_->setVisible( editModelList_->count() > 2 );

    exerciseList_->clear();
    exerciseList_->addItems( frontend::commands::ListExercises( config_ ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::CreateExercise
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::CreateExercise()
{
    if( editTerrainList_->currentIndex() > 0 && editModelList_->currentIndex() > 0 )
    {
        const QString& name = editName_->text();
        if( name.isEmpty() || page_.ExerciceExists( name ) )
            return;

        const std::string terrain = editTerrainList_->currentText().toStdString();
        const QStringList model = editModelList_->currentText().split( "/" );

        if( saveAsGroupBox_->isChecked() )
        {
            QListWidgetItem* item = exerciseList_->currentItem();
            if( item == 0 )
                return;
            frontend::ExerciseCopyParameters params;
            params.to_ = name.toStdString();
            params.terrain_ = terrain;
            params.model_ = model.front().toStdString();
            params.physical_ = model.back().toStdString();
            params.from_ = item->text().toStdString();
            if( contentList_->isVisible() )
            {
                params.itemModel_ = &contentListModel_;
                frontend::CreateExerciseAsCopyOf( config_, params );
            }
            else if( checkpointCopyPanel_->isVisible() && !checkpoint_.empty() )
            {
                bfs::path checkpointPath( config_.GetCheckpointsDir( item->text().toStdString(), session_ ) );
                params.checkpoint_ = ( checkpointPath / checkpoint_ ).string();
                frontend::CreateExerciseAsCopyOfCheckpoint( config_, params );
            }
        }
        else
            frontend::CreateExercise( config_, name.toStdString(), terrain, model.front().toStdString(), model.back().toStdString() );

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
        && !editName_->text().isEmpty() && !page_.ExerciceExists( editName_->text() )
        && ( !saveAsGroupBox_->isChecked()
            || ( exerciseList_->currentItem() != 0 && contentList_->isVisible()
            || ( checkpointCopyPanel_->isVisible() && !checkpoint_.empty() ) ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnSelectionChanged
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnSelectionChanged( QListWidgetItem* item )
{
    if( !item )
        return;
    std::auto_ptr< xml::xistream > xis= fileLoader_.LoadFile( config_.GetExerciseFile( item->text().toStdString() ) );
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
    const QStringList terrainList = frontend::commands::ListTerrains( config_ );
    int index = terrainList.indexOf( terrain.c_str() );
    editTerrainList_->setCurrentIndex( index + 1 );
    const QStringList decisionalModels = frontend::commands::ListModels( config_ );
    int indexModel = 0;
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toStdString() );
        if( std::strcmp( (*it).toStdString().c_str(), dataSet.c_str() ) == 0 )
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
        session_ = session.toStdString();
        checkpointList_->Update( item->text(), session );
        page_.UpdateEditButton();
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnCheckpointSelected
// Created: NPT 2012-09-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnCheckpointSelected( const QString& checkpoint )
{
    checkpoint_ = frontend::commands::CheckpointExists( config_, exerciseList_->currentItem()->text().toStdString(), session_, checkpoint.toStdString() ) ? checkpoint.toStdString() : "";
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
