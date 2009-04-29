// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreVariableCreationWizard.h"
#include "moc_ScoreVariableCreationWizard.cpp"
#include "indicators/Variable.h"

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard constructor
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
ScoreVariableCreationWizard::ScoreVariableCreationWizard( QWidget* parent )
    : QDialog( parent )
{
    setCaption( tr( "Create variable" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    {
        QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, tr( "Information" ), this );
        new QLabel( tr( "Name: " ), box );
        name_ = new QLineEdit( box );
        new QLabel( tr( "Type: " ), box );
        type_ = new QComboBox( box );
        type_->insertItem( "float" );
        type_->insertItem( "string" );
        type_->insertItem( "unit" );
        type_->insertItem( "unit list" );
        type_->insertItem( "position" );
        type_->insertItem( "area" );
        new QLabel( tr( "Value: " ), box );
        value_ = new QLineEdit( box );
        grid->addWidget( box, 0, 0 );
    }
    {
        QHBox* box = new QHBox( this );
        QButton* ok = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 1, 0 );
        connect( ok, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    }
    hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard destructor
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
ScoreVariableCreationWizard::~ScoreVariableCreationWizard()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::Create
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::Create()
{
    name_->clear();
    type_->setCurrentItem( 0 );
    value_->clear();
    show();
}

// -----------------------------------------------------------------------------
// Name: ScoreVariableCreationWizard::OnAccept
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariableCreationWizard::OnAccept()
{
    if( name_->text().isEmpty() || value_->text().isEmpty() )
        return;
    // $$$$ SBO 2009-04-21: TODO: use variable factory, value definition helpers and validators...
    indicators::Variable* var = new indicators::Variable( name_->text().ascii(), type_->currentText().ascii(), value_->text().ascii() );
    emit VariableCreated( *var );
    accept();
}
