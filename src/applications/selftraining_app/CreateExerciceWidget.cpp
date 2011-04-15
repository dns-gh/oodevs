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
//#include "frontend/Exercise_ABC.h"
#include "clients_gui/ValuedListItem.h"
#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <QCheckBox.h>

namespace
{
    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
CreateExerciceWidget::CreateExerciceWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : QGroupBox( 3, Qt::Vertical, parent )
    , config_( config )
    , fileLoader_  ( fileLoader )
    , page_( page )
{
    setFrameShape( QFrame::NoFrame );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );

    {
        QHBox* saveBox = Style( new QHBox( this ) );

        Style( new QLabel( tools::translate( "CreateExerciceWidget", "Create new exercise:" ), saveBox ) );
        editName_ = Style( new QLineEdit( tools::translate( "CreateExerciceWidget", "Enter exercise name" ), saveBox ) );
        connect( editName_, SIGNAL( textChanged( const QString& ) ), &page, SLOT( EditNameChanged( const QString& ) ) );
        editTerrainList_ = Style( new QComboBox( saveBox ) );
        connect( editTerrainList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );
        editModelList_ = Style( new QComboBox( saveBox ) );
        connect( editModelList_, SIGNAL( activated( int ) ), &page, SLOT( ComboChanged( int ) ) );
    }
    {
        saveAsGroupBox_ = Style( new QGroupBox( 2, Qt::Horizontal, tools::translate( "CreateExerciceWidget", "Create as copy of:" ), this ) );
        saveAsGroupBox_->setCheckable( true );
        saveAsGroupBox_->setChecked( false );
        connect( saveAsGroupBox_, SIGNAL( toggled( bool ) ), &page, SLOT( ToggleChanged( bool ) ) );
        {
            QVBox* box = Style( new QVBox( saveAsGroupBox_ ) );
            Style( new QLabel(  tools::translate( "CreateExerciceWidget", "Exercise to copy:" ), box ) );
            exerciseList_ = Style( new QListBox( box ) );
            connect( exerciseList_, SIGNAL( clicked( QListBoxItem* ) ), SLOT( OnSelectionChanged( QListBoxItem* ) ) );
        }
        {
            QVBox* box = Style( new QVBox( saveAsGroupBox_ ) );
            scriptCheckBox_ = Style( new QCheckBox( tools::translate( "CreateExerciceWidget", "Copy scripts" ), box ) );
            scoresCheckBox_ = Style( new QCheckBox( tools::translate( "CreateExerciceWidget", "Copy scores" ), box ) );
            metaDataCheckBox_ = Style( new QCheckBox( tools::translate( "CreateExerciceWidget", "Copy meta data" ), box ) );
            drawingsCheckBox_ = Style( new QCheckBox( tools::translate( "CreateExerciceWidget", "Copy drawings" ), box ) );
            successFactorsCheckBox_ = Style( new QCheckBox( tools::translate( "CreateExerciceWidget", "Copy success factors" ), box ) );
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
    UpdateExercises();
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
            QListBoxItem* item = exerciseList_->selectedItem();
            if( item == 0 )
                return;
            frontend::ExerciseCopyParameters params;
            params.from_ = item->text().ascii();
            params.to_ = name.ascii();
            params.terrain_ = terrain;
            params.model_ = model.front().ascii();
            params.physical_ = model.back().ascii();
            params.copyScript_ = scriptCheckBox_->isChecked();
            params.copyScores_ = scoresCheckBox_->isChecked();
            params.copyDrawing_ = drawingsCheckBox_->isChecked();
            params.copyMetaData_ = metaDataCheckBox_->isChecked();
            params.copySuccessFactors_ = successFactorsCheckBox_->isChecked();
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
        && editName_->text() != tools::translate( "CreateExerciceWidget", "Enter exercise name" )
        && ( !saveAsGroupBox_->isChecked() || saveAsGroupBox_->isChecked() && exerciseList_->selectedItem() != 0 );
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::OnSelectionChanged
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::OnSelectionChanged( QListBoxItem* item )
{
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
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: CreateExerciceWidget::UpdateExercises
// Created: ABR 2011-04-14
// -----------------------------------------------------------------------------
void CreateExerciceWidget::UpdateExercises()
{
    exerciseList_->clear();
    exerciseList_->insertStringList( frontend::commands::ListExercises( config_ ) );
}

