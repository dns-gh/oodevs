// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "EditExercisePanel.h"
#include "moc_EditExercisePanel.cpp"
#include "InfoBubble.h"
#include "resources.h"
#include "ActionsContext.h"
#include "frontend/commands.h"
#include "frontend/EditExercise.h"
#include <Qt3Support/q3action.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3listbox.h>
#include <QtGui/qpushbutton.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: EditExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
EditExercisePanel::EditExercisePanel( Q3WidgetStack* widget, Q3Action& action, const tools::GeneralConfig& config, ActionsContext& context )
    : Panel_ABC( widget, action, context, "EditExercisePanel" )
    , config_( config )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Vertical, action.text(), box );
    new QLabel( tr( "Choose the exercise to edit:" ), group );
    list_ = new Q3ListBox( group );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    Q3HBox* btnBox = new Q3HBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), tr( "Edit exercise" ), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );
    connect( okay, SIGNAL( pressed() ), SLOT( EditExercise() ) );
    connect( list_, SIGNAL( doubleClicked( Q3ListBoxItem* ) ), SLOT( EditExercise() ) );

    Update();
}

// -----------------------------------------------------------------------------
// Name: EditExercisePanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
EditExercisePanel::~EditExercisePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EditExercisePanel::EditExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void EditExercisePanel::EditExercise()
{
    if( list_->selectedItem() )
    {
        Start ( new ::EditExercise( config_, list_->selectedItem()->text() ) );
        context_.Save( "exercise", list_ );
    }
    Update();
    ShowNext();
}

// -----------------------------------------------------------------------------
// Name: EditExercisePanel::Update
// Created: AGE 2007-10-16
// -----------------------------------------------------------------------------
void EditExercisePanel::Update()
{
    list_->clear();
    list_->insertStringList( commands::ListExercises( config_ ) );
    context_.Load( "exercise", list_ );
}
