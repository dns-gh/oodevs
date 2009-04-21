// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreVariablesEditor.h"
#include "moc_ScoreVariablesEditor.cpp"
#include "preparation/IndicatorElement_ABC.h"
#include "preparation/IndicatorType.h"
#include "preparation/IndicatorVariables.h"
#include "preparation/Score_ABC.h"

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreVariablesEditor::ScoreVariablesEditor( QWidget* parent, gui::ItemFactory_ABC& factory )
    : QVBox( parent )
    , factory_( factory )
    , list_( new gui::ListDisplayer< ScoreVariablesEditor >( this, *this, factory ) )
{
    {
        list_->AddColumn( tr( "Name" ) );
        list_->AddColumn( tr( "Type" ) );
        list_->AddColumn( tr( "Value" ) );
    }
    {
        QHBox* box = new QHBox( this );
        QButton* add = new QPushButton( tr( "Add" ), box );
        QButton* del = new QPushButton( tr( "Del" ), box );
        QButton* paste = new QPushButton( tr( "Paste" ), box );
        connect( add, SIGNAL( clicked() ), SLOT( OnAdd() ) );
        connect( del, SIGNAL( clicked() ), SLOT( OnDelete() ) );
        connect( paste, SIGNAL( clicked() ), SLOT( OnPaste() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreVariablesEditor::~ScoreVariablesEditor()
{
    // NOTHING
}

namespace
{
    class VariablesCollector : public IndicatorVariablesVisitor_ABC
    {
    public:
        explicit VariablesCollector( ScoreVariablesEditor& editor ) : editor_( &editor ) {}
        virtual ~VariablesCollector() {}

        virtual void Visit( IndicatorElement_ABC& variable )
        {
            editor_->AddVariable( variable );
        }
    private:
        ScoreVariablesEditor* editor_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor::StartEdit
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariablesEditor::StartEdit( Score_ABC& score )
{
    list_->clear();
    VariablesCollector collector( *this );
    score.Accept( collector );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor::AddVariable
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariablesEditor::AddVariable( const IndicatorElement_ABC& variable )
{
    gui::ValuedListItem* item = factory_.CreateItem( list_ );
    Display( variable, item );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor::Display
// Created: SBO 2009-04-21
// -----------------------------------------------------------------------------
void ScoreVariablesEditor::Display( const IndicatorElement_ABC& variable, gui::ValuedListItem* item )
{
    item->Set( &variable, variable.GetInput().c_str(), variable.GetType().ToString().c_str() );
    item->setText( 2, variable.GetValue().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor::OnAdd
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesEditor::OnAdd()
{
    
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor::OnDelete
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesEditor::OnDelete()
{
    
}

// -----------------------------------------------------------------------------
// Name: ScoreVariablesEditor::OnPaste
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreVariablesEditor::OnPaste()
{
    // $$$$ SBO 2009-04-20: emit Insert( "$variable" );
}
