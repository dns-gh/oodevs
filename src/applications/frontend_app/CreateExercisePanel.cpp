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
#include "ActionsContext.h"
#include "tools/GeneralConfig.h"
#include "frontend/CreateExercise.h"
#include "frontend/commands.h"
#include <Qt3Support/q3action.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel constructor
// Created: SBO 2007-10-04
// -----------------------------------------------------------------------------
CreateExercisePanel::CreateExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : Panel_ABC         ( widget, action, context, "CreateExercisePanel" )
    , config_           ( config )
    , existingExercises_( commands::ListExercises( config ) )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, action.text(), box );

    new QLabel( tr( "New exercise name:" ), group );
    name_ = new QLineEdit( tr( "Enter exercise name" ), group );
    connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( NameChanged( const QString& ) ) );
    {
        new QLabel( tr( "Choose the model data set:" ), group );
        modelList_ = new Q3ListBox( group );
        connect( modelList_, SIGNAL( selectionChanged() ), SLOT( ModelSelected() ) );
    }
    {
        physicalLabel_ = new QLabel( tr( "Choose the physical data set:" ), group );
        physicalList_ = new Q3ListBox( group );
    }
    {
        new QLabel( tr( "Choose the terrain:" ), group );
        terrainList_ = new Q3ListBox( group );
    }

    bubble_ = new InfoBubble( box );
    {
        Q3HBox* btnBox = new Q3HBox( box );
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
    {
        const std::string terrain  = terrainList_->selectedItem()->text().ascii();
        const std::string model    = modelList_->selectedItem()->text().ascii();
        const std::string physical = physicalList_->selectedItem() ? physicalList_->selectedItem()->text().ascii() : "";
        ::CreateExercise( config_, name_->text().ascii(), terrain, model, physical );
        context_.Save( "terrain", terrainList_ );
        context_.Save( "model", modelList_ );
        context_.Save( "physical", physicalList_ );
        context_.Save( "exercise", name_->text() );
    }
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
        context_.Load( "physical", physicalList_ );
    }
}

// -----------------------------------------------------------------------------
// Name: CreateExercisePanel::NameChanged
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void CreateExercisePanel::NameChanged( const QString& name )
{
    const bool exists = existingExercises_.contains( name ) != 0;
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
    context_.Load( "model", modelList_ );
    context_.Load( "terrain", terrainList_ );
}
