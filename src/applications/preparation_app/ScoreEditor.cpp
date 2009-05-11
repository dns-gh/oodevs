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
#include "ScoreGaugeConfiguration.h"
#include "ScorePrimitivesLibrary.h"
#include "ScoreVariablesList.h"
#include "ScoreSyntaxHighlighter.h"
#include "indicators/Gauge.h"
#include "indicators/Primitive.h"
#include "indicators/Variables.h"
#include "preparation/Score.h"
#include <xeumeuleu/xml.h>

namespace
{
    class FormulaLineEdit : public QTextEdit
    {
    public:
        explicit FormulaLineEdit( QWidget* parent )
            : QTextEdit( parent )
        {
            setMaximumHeight( 50 );
        }
        virtual ~FormulaLineEdit() {}

        virtual void focusOutEvent( QFocusEvent* e )
        {
            int paraFrom = 0, indexFrom = 0, paraTo = 0, indexTo = 0;
            getSelection( &paraFrom, &indexFrom, &paraTo, &indexTo );
            QTextEdit::focusOutEvent( e );
            setSelection( paraFrom, indexFrom, paraTo, indexTo );
        }

        virtual void contentsMousePressEvent( QMouseEvent* e )
        {
            QTextEdit::contentsMousePressEvent( e );
            contentsMouseDoubleClickEvent( e );
        }

        bool isBoundary( const QChar c ) const
        {
            return c.isSpace() || ( c.isPunct() && c != '-' );
        }

        virtual void contentsMouseDoubleClickEvent( QMouseEvent* e )
        {
            if( e->button() != Qt::LeftButton )
            {
	            e->ignore();
	            return;
            }
            int para = 0;
            int findex = charAt( e->pos(), &para );
            int bindex = findex;
            const QString str = text( para );
            while( bindex > 0 && ! isBoundary( str.at( bindex - 1 ) ) ) --bindex;
            while( findex < str.length() && ! isBoundary( str.at( findex ) ) ) ++findex;
            setSelection( para, bindex, para, findex );
            e->accept();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreEditor::ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const indicators::Primitives& indicators, const indicators::GaugeTypes& gauges )
    : QDialog( parent, "ScoreEditor" )
    , current_( 0 )
{
    setCaption( tr( "Score editor" ) );
    QGridLayout* grid = new QGridLayout( this, 6, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 2, 3 );
    grid->setColStretch( 0, 2 );
    grid->setColStretch( 1, 3 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    {
        QGroupBox* box = new QGroupBox( 2, Qt::Vertical, tr( "Formula" ), this );
        formula_ = new FormulaLineEdit( box );
        new ScoreSyntaxHighlighter( formula_, controllers, indicators );
        checkResult_ = new QLabel( box );
        checkResult_->setMinimumHeight( 30 );
        grid->addMultiCellWidget( box, 1, 1, 0, 1 );
        connect( formula_, SIGNAL( textChanged() ), SLOT( CheckFormula() ) );
    }
    {
        ScorePrimitivesLibrary* library = new ScorePrimitivesLibrary( this, controllers, factory, indicators );
        grid->addWidget( library, 2, 0 );
        connect( library, SIGNAL( Selected( const indicators::Primitive& ) ), SLOT( OnSelectPrimitive( const indicators::Primitive& ) ) );
        connect( library, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Variables" ), this );
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
        gauge_ = new ScoreGaugeConfiguration( this, controllers, gauges );
        grid->addMultiCellWidget( gauge_, 4, 4, 0, 1 );
    }
    {
        QHBox* box = new QHBox( this );
        ok_ = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 5, 1 );
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
    variables_->StartEdit( score );
    gauge_->StartEdit( score.GetGauge() );
    formula_->setText( score.GetFormula() );
    show();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::Commit
// Created: SBO 2009-04-23
// -----------------------------------------------------------------------------
void ScoreEditor::Commit()
{
    CommitTo( *current_ );
    accept();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::CommitTo
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
void ScoreEditor::CommitTo( Score_ABC& score )
{
    Score& concrete = static_cast< Score& >( score ); // $$$$ SBO 2009-05-07: use a command pattern or something to apply modifications
    concrete.SetName( name_->text() );
    concrete.SetFormula( formula_->text() );
    concrete.SetVariables( variables_->GetValue() );
    concrete.SetGauge( gauge_->GetValue() );
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
void ScoreEditor::OnSelectPrimitive( const indicators::Primitive& indicator )
{
    help_->setText( QString( "<b>%1</b><br><i>%2</i><br>%3" )
                            .arg( indicator.GetName() )
                            .arg( indicator.GetPrototype() )
                            .arg( indicator.GetComment() ) );
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
        Score score( static_cast< Score& >( *current_ ) ); // $$$$ SBO 2009-05-07: 
        CommitTo( score );
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
