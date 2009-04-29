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
#include "ScoreVariablesList.h"
#include "indicators/IndicatorVariables.h"
#include "indicators/IndicatorPrimitive.h"
#include "indicators/IndicatorPrimitives.h"
#include "preparation/Score.h"
#include <xeumeuleu/xml.h>

namespace
{
    class FormulaLineEdit : public QLineEdit
    {
    public:
        explicit FormulaLineEdit( QWidget* parent ) : QLineEdit( parent ) {}
        virtual ~FormulaLineEdit() {}

        virtual void focusOutEvent( QFocusEvent* e )
        {
            const int start = selectionStart();
            const unsigned int length = selectedText().length();
            QLineEdit::focusOutEvent( e );
            setSelection( start, length );
        }

        virtual void mouseDoubleClickEvent( QMouseEvent* e )
        {
            QLineEdit::mouseDoubleClickEvent( e );
            const QString selected = selectedText();
            if( selected.endsWith( "," ) )
                setSelection( selectionStart(), selected.length() - 1 );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreEditor::ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const IndicatorPrimitives& indicators )
    : QDialog( parent, "ScoreEditor" )
    , indicators_( indicators )
    , current_( 0 )
{
    setCaption( tr( "Score editor" ) );
    QGridLayout* grid = new QGridLayout( this, 5, 3, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 2, 3 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    {
        QGroupBox* box = new QGroupBox( 2, Qt::Vertical, tr( "Formula" ), this );
        formula_ = new FormulaLineEdit( box );
        checkResult_ = new QLabel( box );
        checkResult_->setMinimumHeight( 30 );
        grid->addMultiCellWidget( box, 1, 1, 0, 1 );
        connect( formula_, SIGNAL( textChanged( const QString& ) ), SLOT( OnFormulaChanged( const QString& ) ) );
    }
    {
        ScorePrimitivesLibrary* library = new ScorePrimitivesLibrary( this, controllers, factory, indicators_ );
        grid->addWidget( library, 2, 0 );
        connect( library, SIGNAL( Selected( const IndicatorPrimitive& ) ), SLOT( OnSelectPrimitive( const IndicatorPrimitive& ) ) );
        connect( library, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Variables: " ), this );
        variables_ = new ScoreVariablesList( box, factory );
        grid->addWidget( box, 2, 1 );
        connect( variables_, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
        connect( variables_, SIGNAL( Updated() ), SLOT( CheckFormula() ) );
    }
    {
        QGroupBox* box = new QHGroupBox( this );
        box->setMinimumHeight( 80 );
        help_ = new QLabel( box );
        grid->addMultiCellWidget( box, 3, 3, 0, 1 );
    }
    {
        QHBox* box = new QHBox( this );
        ok_ = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 4, 1 );
        connect( ok_, SIGNAL( clicked() ), SLOT( Commit() ) );
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
    setCaption( tr( "Score edition - %1 " ).arg( score.GetName() ) );
    name_->setText( score.GetName() );
    formula_->setText( score.GetFormula() );
    variables_->StartEdit( score );
    show();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::Commit
// Created: SBO 2009-04-23
// -----------------------------------------------------------------------------
void ScoreEditor::Commit()
{
    {
        IndicatorVariables variables;
        variables_->CommitTo( variables );
        Score copy( name_->text(), formula_->text(), variables, indicators_ );
        *static_cast< Score* >( current_ ) = copy; // $$$$ SBO 2009-04-24: sucks
    }
    accept();
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
// Name: ScoreEditor::OnSelectPrimitive
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScoreEditor::OnSelectPrimitive( const IndicatorPrimitive& indicator )
{
    help_->setText( QString( "<b>%1</b><br><i>%2</i><br>%3" )
                            .arg( indicator.GetName() )
                            .arg( indicator.GetPrototype() )
                            .arg( indicator.GetComment() ) );
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::OnFormulaChanged
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreEditor::OnFormulaChanged( const QString& /*text*/ )
{
    CheckFormula();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::CheckFormula
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScoreEditor::CheckFormula()
{
    try
    {
        if( formula_->text().isEmpty() )
            throw std::exception( tr( "Formula is empty." ) );
        IndicatorVariables variables;
        variables_->CommitTo( variables );
        Score score( name_->text(), formula_->text(), variables, indicators_ );
        score.CheckValidity();
        checkResult_->setText( "" );
        ok_->setEnabled( true );
    }
    catch( std::exception& e )
    {
        checkResult_->setText( QString( "<font color='#ff0000'><b>%1</b></font>" ).arg( e.what() ) );
        ok_->setEnabled( false );
    }
}
