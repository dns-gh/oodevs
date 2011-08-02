// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorEditor.h"
#include "moc_SuccessFactorEditor.cpp"
#include "SuccessFactorActionsEditor.h"
#include "SuccessFactorConditionsEditor.h"
#include "SuccessFactorProfileList.h"
#include "preparation/SuccessFactor.h"

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorEditor::SuccessFactorEditor( QWidget* parent, kernel::Controllers& controllers, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores )
    : QDialog( parent, "SuccessFactorEditor" )
    , current_( 0 )
{
    setCaption( tr( "Success factor editor" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setColStretch( 1, 5 );
    grid->setRowStretch( 1, 5 );
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Profiles" ), this );
        profiles_ = new SuccessFactorProfileList( box, controllers );
        grid->addMultiCellWidget( box, 1, 2, 0, 0 );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Conditions" ), this );
        conditions_ = new SuccessFactorConditionsEditor( box, scores );
        grid->addWidget( box, 1, 1 );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Actions" ), this );
        actions_ = new SuccessFactorActionsEditor( box, actionTypes );
        grid->addWidget( box, 2, 1 );
    }
    {
        Q3HBox* box = new Q3HBox( this );
        ok_ = new QPushButton( tr( "Ok" ), box );
        QPushButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 3, 1, Qt::AlignRight );
        connect( ok_, SIGNAL( clicked() ), SLOT( Commit() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( Cancel() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorEditor::~SuccessFactorEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor::StartEdit
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorEditor::StartEdit( SuccessFactor& factor )
{
    current_ = &factor;
    setCaption( tr( "Success Factor edition - %1 " ).arg( factor.GetName() ) );
    name_->setText( factor.GetName() );
    profiles_->StartEdit( factor.GetProfiles() );
    conditions_->StartEdit( factor.GetConditions() );
    actions_->StartEdit( factor.GetActions() );
    show();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor::Commit
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorEditor::Commit()
{
    if( current_ )
    {
        current_->SetName( name_->text() );
        profiles_->CommitTo( current_->GetProfiles() );
        conditions_->CommitTo( current_->GetConditions() );
        actions_->CommitTo( current_->GetActions() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor::Cancel
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorEditor::Cancel()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor::sizeHint
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QSize SuccessFactorEditor::sizeHint() const
{
    return QSize( 550, 400 );
}
