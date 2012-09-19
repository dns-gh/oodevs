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
#include "clients_gui/ValuedListItem.h"
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
    : gui::LanguageChangeObserver_ABC< Q3GroupBox >( 3, Qt::Vertical, parent )
    , config_( config )
    , fileLoader_( fileLoader )
    , page_( page )
{
    setFrameShape( Q3GroupBox::DummyFrame::NoFrame );
    setMargin( 5 );
    {
        Q3HBox* saveBox = new Q3HBox( this );
        createLabel_ = new QLabel( saveBox );
        editName_ = new QLineEdit( saveBox );
        connect( editName_, SIGNAL( textChanged( const QString& ) ), &page, SLOT( EditNameChanged( const QString& ) ) );
        {
            Q3VBox* box = new Q3VBox( saveBox );
            editTerrainList_ = new QComboBox( box );
            connect( editTerrainList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );
            editModelList_ = new QComboBox( box );
            connect( editModelList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );
            editTerrainList_->setMinimumWidth( 210 );
            editModelList_->setMinimumWidth( 210 );
        }
    }
    {
        saveAsGroupBox_ = new Q3GroupBox( this );
        saveAsGroupBox_->setCheckable( true );
        saveAsGroupBox_->setChecked( false );
        connect( saveAsGroupBox_, SIGNAL( toggled( bool ) ), &page, SLOT( ToggleChanged( bool ) ) );
        {
            copyLabel_ = new QLabel();
            exerciseList_ = new Q3ListBox();
            connect( exerciseList_, SIGNAL( clicked( Q3ListBoxItem* ) ), SLOT( OnSelectionChanged( Q3ListBoxItem* ) ) );
        }
        {
            contentList_ = new Q3ListView();
            contentList_->addColumn( "exercise features" );
            contentList_->setResizeMode( Q3ListView::AllColumns );
            contentList_->header()->hide();
            contentList_->adjustSize();

            checkpointCopyPanel_ = new Q3HBox();
            {
                Q3VBox* box = new Q3VBox( checkpointCopyPanel_ );
                sessionLabel_ = new QLabel( box );
                box->setSpacing( 5 );
                sessionList_ = new Q3ListBox( box );
                connect( sessionList_, SIGNAL( highlighted( const QString& ) ), SLOT( OnSessionSelected( const QString& ) ) );
            }
            {
                Q3VBox* box = new Q3VBox( checkpointCopyPanel_ );
                checkpointList_ = new frontend::CheckpointList( box, config );
                connect( saveAsGroupBox_, SIGNAL( toggled( bool ) ), checkpointList_, SLOT( Toggle( bool ) ) );
                connect( checkpointList_, SIGNAL( Select( const QString& ) ), SLOT( OnCheckpointSelected( const QString& ) ) );
            }

            copyTab_ = new QTabWidget();
            copyTab_->addTab( contentList_, "Content to copy" );
            copyTab_->addTab( checkpointCopyPanel_, "Checkpoint" );
            connect( copyTab_, SIGNAL( currentChanged( int ) ), SLOT( CurrentTabChanged( int ) ) );

            QVBoxLayout* layout = new QVBoxLayout( saveAsGroupBox_ );
            layout->setMargin( 10 );
            layout->setSpacing( 10 );
            layout->setAlignment( Qt::AlignTop );
            layout->addWidget( copyLabel_ );
            layout->addWidget( exerciseList_ );
            layout->addWidget( copyTab_ );
        }
    }
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
    Q3ListBoxItem* item = exerciseList_->selectedItem();
    if( item )
    {
        std::string exercise( item->text().toAscii().constData() );
        contentList_->clear();
        contentList_->insertItem( frontend::BuildExerciseFeatures( exercise, config_, contentList_ ) );

        sessionList_->clear();
        sessionList_->insertStringList( frontend::commands::ListSessionsWithCheckpoint( config_, exercise ) );
        sessionList_->setEnabled( sessionList_->count() );
        if( !sessionList_->count() )
            sessionList_->insertItem( tools::translate( "CreateExerciceWidget", "No session" ) );
        sessionList_->setSelected( 0, true );
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
    editTerrainList_->insertItem( tools::translate( "CreateExerciceWidget", "Terrain:" ) );
    editTerrainList_->insertStringList( frontend::commands::ListTerrains( config_ ) );
    editModelList_->clear();
    editModelList_->insertItem( tools::translate( "CreateExerciceWidget", "Model:" ) );
    QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toAscii().constData() );
        for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            editModelList_->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ) );
    }
    if( editModelList_->count() == 2 )
        editModelList_->setCurrentItem( 1 );
    editModelList_->setShown( editModelList_->count() > 2 );

    exerciseList_->clear();
    exerciseList_->insertStringList( frontend::commands::ListExercises( config_ ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::CreateExercise
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void CreateExerciceWidget::CreateExercise()
{
    if( editTerrainList_->currentItem() > 0 && editModelList_->currentItem() > 0 )
    {
        const QString& name = editName_->text();
        if( name.isEmpty() || page_.ExerciceExists( name ) )
            return;

        const std::string terrain = editTerrainList_->currentText().toAscii().constData();
        const QStringList model = QStringList::split( "/", editModelList_->currentText() );

        if( saveAsGroupBox_->isChecked() )
        {
            Q3ListBoxItem* item = exerciseList_->selectedItem();
            if( item == 0 )
                return;
            frontend::ExerciseCopyParameters params;
            params.to_ = name.toAscii().constData();
            params.terrain_ = terrain;
            params.model_ = model.front().toAscii().constData();
            params.physical_ = model.back().toAscii().constData();
            params.from_ = item->text().toAscii().constData();
            if( contentList_->isVisible() )
            {
                params.iterator_ = Q3ListViewItemIterator( contentList_ );
                frontend::CreateExerciseAsCopyOf( config_, params );
            }
            else if( checkpointCopyPanel_->isVisible() && !checkpoint_.empty() )
            {
                bfs::path checkpointPath( config_.GetCheckpointsDir( item->text().toAscii().constData(), session_ ) );
                params.checkpoint_ = ( checkpointPath / checkpoint_ ).string();
                frontend::CreateExerciseAsCopyOfCheckpoint( config_, params );
            }
        }
        else
            frontend::CreateExercise( config_, name.toAscii().constData(), terrain, model.front().toAscii().constData(), model.back().toAscii().constData() );

        page_.Edit( name );
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
        && ( !saveAsGroupBox_->isChecked() 
            || ( exerciseList_->selectedItem() != 0 && contentList_->isVisible()
            || ( checkpointCopyPanel_->isVisible() && !checkpoint_.empty() ) ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnSelectionChanged
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnSelectionChanged( Q3ListBoxItem* item )
{
    if( !item )
        return;
    std::auto_ptr< xml::xistream > xis= fileLoader_.LoadFile( config_.GetExerciseFile( item->text().toAscii().constData() ) );
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
    int index = terrainList.findIndex( terrain.c_str() );
    editTerrainList_->setCurrentItem( index + 1 );
    const QStringList decisionalModels = frontend::commands::ListModels( config_ );
    int indexModel = 0;
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        if( std::strcmp( (*it).toAscii().constData(), dataSet.c_str() ) == 0 )
        {
            const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).toAscii().constData() );
            int index = physicalModels.findIndex( QString( physical.c_str() ) );
            if( index != -1 )
                editModelList_->setCurrentItem( indexModel + index + 1 );
            else
                indexModel += physicalModels.size();
        }
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
    if( Q3ListBoxItem* item = exerciseList_->selectedItem() )
    {
        session_ = session.toAscii().constData();
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
    checkpoint_ = frontend::commands::CheckpointExists( config_, exerciseList_->selectedItem()->text().toAscii().constData(), session_, checkpoint.toAscii().constData() ) ? checkpoint : "";
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
