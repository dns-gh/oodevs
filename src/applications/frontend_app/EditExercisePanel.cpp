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
#include "commands.h"
#include "EditExercise.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>

// -----------------------------------------------------------------------------
// Name: EditExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
EditExercisePanel::EditExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Vertical, action.text(), box );
    new QLabel( tr( "Choose the exercise to edit:" ), group );
    list_ = new QListBox( group );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), tr( "Edit exercise" ), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );
    connect( okay, SIGNAL( pressed() ), SLOT( EditExercise() ) );
    connect( list_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( EditExercise() ) );

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
        new ::EditExercise( this, config_, list_->selectedItem()->text() );
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
    list_->insertStringList( commands::ListExercises( config_) );
    list_->setSelected( 0, true );
}
