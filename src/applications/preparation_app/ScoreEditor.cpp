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
#include "ScoreProfilesPage.h"
#include "ScoreVariablesList.h"
#include "ScoreSyntaxHighlighter.h"
#include "indicators/Gauge.h"
#include "indicators/Primitive.h"
#include "indicators/Variables.h"
#include "preparation/Score.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"
#include <xeumeuleu/xml.hpp>

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
            unsigned int findex = charAt( e->pos(), &para );
            unsigned int bindex = findex;
            const QString str = text( para );
            while( bindex > 0 && ! isBoundary( str.at( bindex - 1 ) ) ) --bindex;
            while( findex < str.length() && ! isBoundary( str.at( findex ) ) ) ++findex;
            setSelection( para, bindex, para, findex );
            e->accept();
        }
    };

    class NameValidator : public QValidator
    {
    public:
                 NameValidator( QLineEdit* parent, const ScoresModel& model, Score_ABC* const& score )
                     : QValidator( parent )
                     , model_( &model )
                     , parent_( parent )
                     , score_( score )
                 {}
        virtual ~NameValidator() {}

        virtual QValidator::State validate( QString& input, int& ) const
        {
            const QValidator::State state = input.isEmpty() || model_->Find( input ) && ( !score_ || score_ && score_->GetName() != input ) ? QValidator::Intermediate : QValidator::Acceptable;
            if( state != QValidator::Acceptable )
                parent_->setPaletteBackgroundColor( Qt::red.light( 120 ) );
            else
                parent_->unsetPalette();
            return state;
        }

    private:
        NameValidator& operator=( const NameValidator& );
        const ScoresModel* model_;
        QLineEdit* parent_;
        Score_ABC* const& score_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreEditor::ScoreEditor( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::ParametersLayer& layer, const ScoresModel& model, const StaticModel& staticModel )
    : QDialog( parent, "ScoreEditor" )
    , current_( 0 )
{
    setCaption( tr( "Score editor" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 1, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 1, 4 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        name_->setValidator( new NameValidator( name_, model, current_ ) );
        grid->addWidget( box, 0, 0 );
        connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( AllowCommit() ) );
    }
    {
        QTabWidget* tabs = new QTabWidget( this );
        {
            QWidget* page = new QWidget( tabs );
            QGridLayout* pageLayout = new QGridLayout( page, 6, 2, 0, 5 );
            pageLayout->setRowStretch( 1, 3 );
            pageLayout->setColStretch( 0, 2 );
            pageLayout->setColStretch( 1, 3 );
            {
                QGroupBox* box = new QGroupBox( 2, Qt::Vertical, tr( "Formula" ), page );
                formula_ = new FormulaLineEdit( box );
                new ScoreSyntaxHighlighter( formula_, controllers, staticModel.indicators_ );
                checkResult_ = new QLabel( box );
                checkResult_->setMinimumHeight( 30 );
                pageLayout->addMultiCellWidget( box, 0, 0, 0, 1 );
                connect( formula_, SIGNAL( textChanged() ), SLOT( CheckFormula() ) );
            }
            {
                ScorePrimitivesLibrary* library = new ScorePrimitivesLibrary( page, controllers, factory, staticModel.indicators_ );
                pageLayout->addWidget( library, 1, 0 );
                connect( library, SIGNAL( Selected( const indicators::Primitive& ) ), SLOT( OnSelectPrimitive( const indicators::Primitive& ) ) );
                connect( library, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
            }
            {
                QGroupBox* box = new QHGroupBox( tr( "Variables" ), page );
                variables_ = new ScoreVariablesList( box, factory, controllers, layer, staticModel );
                pageLayout->addWidget( box, 1, 1 );
                connect( variables_, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
                connect( variables_, SIGNAL( Updated() ), SLOT( CheckFormula() ) );
                connect( variables_, SIGNAL( StartEdit() ), SLOT( hide() ) );
                connect( variables_, SIGNAL( EndEdit() ), SLOT( show() ) );
            }
            {
                QGroupBox* box = new QHGroupBox( page );
                box->setMinimumHeight( 80 );
                help_ = new QLabel( box );
                pageLayout->addMultiCellWidget( box, 2, 2, 0, 1 );
            }
            tabs->addTab( page, tr( "Definition" ) );
        }
        {
            gauge_ = new ScoreGaugeConfiguration( tabs, controllers, staticModel.gaugeTypes_ );
            tabs->addTab( gauge_, tr( "Gauge" ) );
        }
        {
            profiles_ = new ScoreProfilesPage( tabs, controllers );
            tabs->addTab( profiles_, tr( "Profiles" ) );
        }
        grid->addWidget( tabs, 1, 0 );
    }
    {
        QHBox* box = new QHBox( this );
        ok_ = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 2, 0, Qt::AlignRight );
        connect( ok_, SIGNAL( clicked() ), SLOT( Commit() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( Cancel() ) );
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
    variables_->StartEdit( score.GetVariables() );
    gauge_->StartEdit( score.GetGauge() );
    profiles_->StartEdit( score.GetProfiles() );
    formula_->setText( score.GetFormula() );
    show();
    emit Show();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::Commit
// Created: SBO 2009-04-23
// -----------------------------------------------------------------------------
void ScoreEditor::Commit()
{
    CommitTo( *current_ );
    accept();
    emit Hide();
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
    concrete.SetProfiles( profiles_->CreateResult() );
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::Cancel
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreEditor::Cancel()
{
    reject();
    emit Hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::closeEvent
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreEditor::closeEvent( QCloseEvent* e )
{
    QDialog::closeEvent( e );
    emit Hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::OnInsert
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreEditor::OnInsert( const QString& text )
{
    formula_->insert( text );
}

namespace
{
    class HelpDisplayer : public kernel::Displayer_ABC
    {
    public:
        explicit HelpDisplayer( QLabel* help ) : help_( help ) {}
        virtual void Hide() {}
        virtual void Clear() {}
        virtual kernel::Displayer_ABC& SubItem( const QString& ) { return *this; }
        virtual void StartDisplay()
        {
            text_ = "";
        }
        virtual void DisplayFormatted( const QString& formatted )
        {
            text_ = formatted;
        }
        virtual void EndDisplay()
        {
            help_->setText( text_ );
        }

    private:
        QLabel* help_;
        QString text_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::OnSelectPrimitive
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScoreEditor::OnSelectPrimitive( const indicators::Primitive& indicator )
{
    HelpDisplayer displayer( help_ );
    indicator.DisplayInTooltip( displayer );
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
        AllowCommit( true );
    }
    catch( std::exception& e )
    {
        checkResult_->setText( QString( "<font color='#ff0000'><b>%1</b></font>" ).arg( e.what() ) );
        AllowCommit( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::AllowCommit
// Created: SBO 2009-07-24
// -----------------------------------------------------------------------------
void ScoreEditor::AllowCommit( bool base /*= true*/ )
{
    ok_->setEnabled( base && name_->hasAcceptableInput() );
}
