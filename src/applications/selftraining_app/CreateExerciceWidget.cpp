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
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "frontend/ListViewHelper.h"
#include "ScenarioEditPage.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
CreateExerciceWidget::CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : gui::LanguageChangeObserver_ABC< Q3GroupBox >( 3, Qt::Vertical, parent )
    , config_( config )
    , fileLoader_  ( fileLoader )
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
        saveAsGroupBox_ = new Q3GroupBox( 2, Qt::Horizontal, this );
        saveAsGroupBox_->setCheckable( true );
        saveAsGroupBox_->setChecked( false );
        connect( saveAsGroupBox_, SIGNAL( toggled( bool ) ), &page, SLOT( ToggleChanged( bool ) ) );
        {
            copyLabel_ = new QLabel( saveAsGroupBox_ );
            exerciseList_ = new Q3ListBox( saveAsGroupBox_ );
            connect( exerciseList_, SIGNAL( clicked( Q3ListBoxItem* ) ), SLOT( OnSelectionChanged( Q3ListBoxItem* ) ) );
        }
        {
            copyContentLabel_ = new QLabel( saveAsGroupBox_ );
            contentList_ = new Q3ListView( saveAsGroupBox_ );
            contentList_->addColumn( "exercise features" );
            contentList_->setResizeMode( Q3ListView::AllColumns );
            contentList_->header()->hide();
            contentList_->adjustSize();
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
    copyContentLabel_->setText( tools::translate( "CreateExerciceWidget", "Content to copy:" ) );
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
        std::string exercise( item->text().ascii() );
        contentList_->clear();
        contentList_->insertItem( frontend::BuildExerciseFeatures( exercise, config_, contentList_ ) );
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
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
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

        const std::string terrain = editTerrainList_->currentText().ascii();
        const QStringList model = QStringList::split( "/", editModelList_->currentText() );

        if( saveAsGroupBox_->isChecked() )
        {
            Q3ListBoxItem* item = exerciseList_->selectedItem();
            if( item == 0 )
                return;
            frontend::ExerciseCopyParameters params;
            params.from_ = item->text().ascii();
            params.to_ = name.ascii();
            params.terrain_ = terrain;
            params.model_ = model.front().ascii();
            params.physical_ = model.back().ascii();
            params.iterator_ = Q3ListViewItemIterator( contentList_ );
            frontend::CreateExerciseAsCopyOf( config_, params );
        }
        else
            frontend::CreateExercise( config_, name.ascii(), terrain, model.front().ascii(), model.back().ascii() );

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
        && ( !saveAsGroupBox_->isChecked() || saveAsGroupBox_->isChecked() && exerciseList_->selectedItem() != 0 );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnSelectionChanged
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnSelectionChanged( Q3ListBoxItem* item )
{
    if( !item )
        return;
    std::auto_ptr< xml::xistream > xis= fileLoader_.LoadFile( config_.GetExerciseFile( item->text().ascii() ) );
    std::string terrain, physical;
    *xis >> xml::start( "exercise" )
            >> xml::start( "terrain" )
                >> xml::attribute( "name", terrain )
            >> xml::end
            >> xml::start( "model" )
                >> xml::attribute( "physical", physical )
            >> xml::end
         >> xml::end;
    const QStringList terrainList = frontend::commands::ListTerrains( config_ );
    int index = terrainList.findIndex( terrain.c_str() );
    editTerrainList_->setCurrentItem( index + 1 );
    const QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
        int index = physicalModels.findIndex( QString( physical.c_str() ) );
        if( index != -1 )
            editModelList_->setCurrentItem( index + 1 );
    }
    Update();
    page_.UpdateEditButton();
}
