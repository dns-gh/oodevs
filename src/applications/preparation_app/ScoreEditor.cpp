// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreEditor.h"
#include "moc_ScoreEditor.cpp"
#include "ScorePrimitivesLibrary.h"
#include "ScoreVariablesEditor.h"
#include "preparation/Score_ABC.h"

// -----------------------------------------------------------------------------
// Name: ScoreEditor constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreEditor::ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Resolver_ABC< IndicatorPrimitive, QString >& primitives )
    : QDialog( parent, "ScoreEditor" )
    , current_( controllers )
{
    setCaption( tr( "Score editor" ) );
    QGridLayout* grid = new QGridLayout( this, 4, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 2, 3 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Formula" ), this );
        formula_ = new QLineEdit( box );
        checkFormula_ = new QPushButton( tr( "Check" ), box ); // $$$$ SBO 2009-04-20: Check performance to see if manual check is needed
        checkFormula_->setEnabled( false );
        grid->addMultiCellWidget( box, 1, 1, 0, 2 );
        connect( formula_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFormulaChanged( const QString& ) ) );
    }
    {
        ScorePrimitivesLibrary* library = new ScorePrimitivesLibrary( this, controllers, factory, primitives );
        grid->addWidget( library, 2, 0 );
        connect( library, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Where: " ), this );
        variables_ = new ScoreVariablesEditor( box, factory );
        grid->addWidget( box, 2, 1 );
        connect( variables_, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
    }
    {
        QHBox* box = new QHBox( this );
        QButton* ok = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 3, 1 );
        connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreEditor::~ScoreEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::StartEdit
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreEditor::StartEdit( Score_ABC& score )
{
    current_ = &score;
    setCaption( tr( "Score edition - %1 " ).arg( current_->GetName() ) );
    name_->setText( current_->GetName() );
    formula_->setText( current_->GetFormula() );
    variables_->StartEdit( score );
    show();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::OnInsert
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreEditor::OnInsert( const QString& text )
{
    formula_->insert( text );
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::OnFormulaChanged
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreEditor::OnFormulaChanged( const QString& text )
{
    checkFormula_->setEnabled( !text.isEmpty() );
}
