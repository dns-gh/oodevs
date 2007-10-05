// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "StartExercisePanel.h"
#include "moc_StartExercisePanel.cpp"
#include "commands.h"
#include "StartExercise.h"
#include "InfoBubble.h"
#include "resources.h"
#include <qaction.h>
#include <qlistbox.h>
#include <qpushbutton.h>

// -----------------------------------------------------------------------------
// Name: StartExercisePanel constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercisePanel::StartExercisePanel( QWidgetStack* widget, QAction& action, const tools::GeneralConfig& config )
    : Panel_ABC( widget, action )
    , config_( config )
{
    QVBox* box = new QVBox( this );
    box->setMargin( 10 );
    box->setSpacing( 10 );

    QGroupBox* group = new QGroupBox( 2, Qt::Vertical, action.text(), box );
    new QLabel( tr( "Choose the exercise to start:" ), group );
    list_ = new QListBox( group );
    list_->insertStringList( commands::ListExercises( config ) );
    list_->setSelected( 0, true );

    bubble_ = new InfoBubble( box ); // $$$$ SBO 2007-10-05: TODO
    QHBox* btnBox = new QHBox( box );
    btnBox->layout()->setAlignment( Qt::AlignRight );
    QPushButton* okay = new QPushButton( MAKE_PIXMAP( next ), tr( "Start exercise" ), btnBox );
    QFont font( "Arial", 10, QFont::Bold );
    okay->setFont( font );
    connect( okay, SIGNAL( pressed() ), SLOT( StartExercise() ) );
    connect( list_, SIGNAL( doubleClicked( QListBoxItem* ) ), SLOT( StartExercise() ) );
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercisePanel::~StartExercisePanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartExercisePanel::StartExercise
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
void StartExercisePanel::StartExercise()
{
    if( list_->selectedItem() )
        new ::StartExercise( this, config_, list_->selectedItem()->text() );
}

