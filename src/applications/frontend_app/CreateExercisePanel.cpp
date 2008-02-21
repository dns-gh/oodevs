// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "CreateExercisePanel.h"
#include "moc_CreateExercisePanel.cpp"
#include "InfoBubble.h"
#include "resources.h"
#include "tools/GeneralConfig.h"
#include "frontend/CreateExercise.h"
#include "frontend/commands.h"
#include <qaction.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlistbox.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
CreateExercisePanel::CreateExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC         ( widget, action )
    , config_           ( config )
    , existingExercises_( commands::ListExercises( config ) )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, action.text(), box );

    new QLabel( tr( "New exercise name:" ), group );
    name_ = new QLineEdit( tr( "Enter exercise name" ), group );
    connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( NameChanged( const QString& ) ) );
    {
        new QLabel( tr( "Choose the model data set:" ), group );
        modelList_ = new QListBox( group );
        connect( modelList_, SIGNAL( selectionChanged() ), SLOT( ModelSelected() ) );
    }
    {
        physicalLabel_ = new QLabel( tr( "Choose the physical data set:" ), group );
        physicalList_ = new QListBox( group );
    }
    {
        new QLabel( tr( "Choose the terrain:" ), group );
        terrainList_ = new QListBox( group );
    }

    bubble_ = new InfoBubble( box );
    {
        QHBox* btnBox = new QHBox( box );
        btnBox->layout()->setAlignment( Qt::AlignRight );
        okay_ = new QPushButton( MAKE_PIXMAP( next ), tr( "Create" ), btnBox );
        QFont font( "Arial", 10, QFont::Bold );
        okay_->setFont( font );
    }
    connect( okay_, SIGNAL( pressed() ), SLOT( CreateExercise() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel destructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
CreateExercisePanel::~CreateExercisePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel::showEvent
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void CreateExercisePanel::showEvent( QShowEvent* event )
{
    name_->selectAll();
    name_->setFocus();
    Panel_ABC::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel::CreateExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void CreateExercisePanel::CreateExercise()
{
    if( terrainList_->selectedItem() && modelList_->selectedItem()
        && ( physicalList_->selectedItem() || ! physicalList_->isVisible() ) )
        ::CreateExercise( config_, name_->text().ascii(),
                                 terrainList_->selectedItem()->text().ascii(),
                                 modelList_->selectedItem()->text().ascii(),
                                 physicalList_->selectedItem() ? physicalList_->selectedItem()->text().ascii() :"" );
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel::ModelSelected
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void CreateExercisePanel::ModelSelected()
{
    if( modelList_->selectedItem() )
    {
        QStringList physical = commands::ListPhysicalModels( config_, modelList_->selectedItem()->text().ascii() );
        physicalList_->clear();
        physicalList_->insertStringList( physical );
        physicalLabel_->setShown( !physical.empty() );
        physicalList_->setShown( !physical.empty() );
        physicalList_->setSelected( 0, true );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel::NameChanged
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void CreateExercisePanel::NameChanged( const QString& name )
{
    const bool exists = existingExercises_.contains( name );
    okay_->setDisabled( exists );
    if( exists )
        bubble_->ShowError( tr( "An exercise with this name already exists." ) );
    else
        bubble_->ShowInfo( tr( "The new exercise will be created in:\n%1" ).arg( config_.GetExerciseDir( name.ascii() ).c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void CreateExercisePanel::Update()
{
    modelList_->clear();
    modelList_->insertStringList( commands::ListModels( config_ ) );
    terrainList_->clear();
    terrainList_->insertStringList( commands::ListTerrains( config_ ) );
    modelList_->setSelected( 0, true );
    terrainList_->setSelected( 0, true );
}
