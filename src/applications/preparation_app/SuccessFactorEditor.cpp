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
#include "preparation/SuccessFactorsModel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichGroupBox.h"

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorEditor::SuccessFactorEditor( QWidget* parent, kernel::Controllers& controllers, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores, SuccessFactorsModel& success )
    : QDialog( parent, "SuccessFactorEditor" )
    , success_( success )
    , current_( 0 )
    , nameChanged_( false )
{
    gui::SubObjectName subObject( "SuccessFactorEditor" );
    setCaption( tr( "Success factor editor" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setColStretch( 1, 5 );
    grid->setRowStretch( 1, 5 );
    {
        gui::SubObjectName subObject( "information" );
        QLabel* nameLabel = new QLabel( tr( "Name:" ) );

        name_ = new gui::RichLineEdit( "name" );
        connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( NameChanged() ) );

        gui::RichGroupBox* informationBox = new gui::RichGroupBox( "information", tr( "Information" ), this );
        QHBoxLayout* informationBoxLayout = new QHBoxLayout( informationBox );
        informationBoxLayout->addWidget( nameLabel );
        informationBoxLayout->addWidget( name_ );
        grid->addWidget( informationBox, 0, 0, 1, 5 );
    }
    {
        gui::SubObjectName subObject( "profiles" );
        profiles_ = new SuccessFactorProfileList( "profilesList", controllers );

        gui::RichGroupBox* profilesBox = new gui::RichGroupBox( "profiles", tr( "Profiles" ), this );
        QHBoxLayout* profilesBoxLayout = new QHBoxLayout( profilesBox );
        profilesBoxLayout->addWidget( profiles_ );
        grid->addWidget( profilesBox, 1, 0, 2, 1 );
    }
    {
        gui::SubObjectName subObject( "conditions" );
        conditions_ = new SuccessFactorConditionsEditor( "conditionsList", scores );

        gui::RichGroupBox* conditionsBox = new gui::RichGroupBox( "conditions", tr( "Conditions" ), this );
        QHBoxLayout* conditionsBoxLayout = new QHBoxLayout( conditionsBox );
        conditionsBoxLayout->addWidget( conditions_ );
        grid->addWidget( conditionsBox, 1, 1, 1, 4 );
    }
    {
        gui::SubObjectName subObject( "actions" );
        actions_ = new SuccessFactorActionsEditor( "actionsEditor", actionTypes );

        gui::RichGroupBox* actionsBox = new gui::RichGroupBox( "actions", tr( "Actions" ), this );
        QHBoxLayout* actionsBoxLayout = new QHBoxLayout( actionsBox );
        actionsBoxLayout->addWidget( actions_ );
        grid->addWidget( actionsBox, 2, 1, 1, 4 );
    }
    {
        Q3HBox* box = new Q3HBox( this );


        ok_ = new gui::RichPushButton( "ok", tr( "Ok" ), box );
        connect( ok_, SIGNAL( clicked() ), SLOT( Commit() ) );

        gui::RichPushButton* cancel = new gui::RichPushButton( "cancel", tr( "Cancel" ), box );
        connect( ok_, SIGNAL( clicked() ), SLOT( Commit() ) );

        connect( cancel, SIGNAL( clicked() ), SLOT( Cancel() ) );

        connect( cancel, SIGNAL( clicked() ), SLOT( Cancel() ) );

        grid->addWidget( box, 3, 1, Qt::AlignRight );
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
        if( nameChanged_ )
        {
            success_.Remove( current_->GetName() );
            current_->SetName( name_->text() );
            success_.Register( current_->GetName(), *current_ );
            nameChanged_ = false;
        }
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
// Name: SuccessFactorEditor::NameChanged
// Created: JSR 2012-10-09
// -----------------------------------------------------------------------------
void SuccessFactorEditor::NameChanged()
{
    nameChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorEditor::sizeHint
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
QSize SuccessFactorEditor::sizeHint() const
{
    return QSize( 550, 400 );
}
