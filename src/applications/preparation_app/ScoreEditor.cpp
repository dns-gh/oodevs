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

#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichWidget.h"
#include "clients_kernel/Displayer_ABC.h"
#include "indicators/Gauge.h"
#include "indicators/Primitive.h"
#include "indicators/Variables.h"
#include "preparation/Score.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"
#include "preparation/ProfileSelection.h"

#include <xeumeuleu/xml.hpp>

using namespace kernel;

namespace
{
    class FormulaLineEdit : public gui::RichWidget< QTextEdit >
    {
    public:
        explicit FormulaLineEdit()
            : gui::RichWidget< QTextEdit >( "FormulaLineEdit" )
        {
            setMaximumHeight( 50 );
        }
        virtual ~FormulaLineEdit() {}

        virtual void focusOutEvent( QFocusEvent* e )
        {
            if( textCursor().hasSelection() )
            {
                QTextCursor cursor = textCursor();
                int selectionStart = cursor.selectionStart();
                int selectionEnd = cursor.selectionEnd();
                gui::RichWidget< QTextEdit >::focusOutEvent( e );
                cursor.setPosition( selectionStart );
                cursor.setPosition( selectionEnd, QTextCursor::KeepAnchor );
                setTextCursor( cursor );
            }
            else
                gui::RichWidget< QTextEdit >::focusOutEvent( e );
        }

        virtual void mousePressEvent( QMouseEvent* e )
        {
            gui::RichWidget< QTextEdit >::mousePressEvent( e );
            mouseDoubleClickEvent( e );
        }

        bool isBoundary( const QChar c ) const
        {
            return c.isSpace() || ( c.isPunct() && c != '-' );
        }

        virtual void mouseDoubleClickEvent( QMouseEvent* e )
        {
            if( e->button() != Qt::LeftButton )
            {
                e->ignore();
                return;
            }
            int start = cursorForPosition( e->pos() ).position();
            int end = start;
            const QString str = text();
            while( start > 0 && !isBoundary( str.at( start - 1 ) ) ) --start;
            while( end < str.length() && !isBoundary( str.at( end ) ) ) ++end;
            QTextCursor cursor = textCursor();
            cursor.setPosition( start );
            cursor.setPosition( end, QTextCursor::KeepAnchor );
            setTextCursor( cursor );
            e->accept();
        }
    };

    class NameValidator : public QValidator
    {
    public:
                 NameValidator( gui::RichLineEdit* parent, const ScoresModel& model, Score_ABC* const& score )
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
                parent_->setPaletteBackgroundColor( QColor( Qt::red ).light( 120 ) );
            else
                parent_->unsetPalette();
            return state;
        }

    private:
        NameValidator& operator=( const NameValidator& );
        const ScoresModel* model_;
        gui::RichLineEdit* parent_;
        Score_ABC* const& score_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScoreEditor::ScoreEditor( QWidget* parent, kernel::Controllers& controllers, ScoresModel& model,
                          const ::StaticModel& staticModel, gui::GLView_ABC& tools, actions::gui::InterfaceBuilder_ABC& builder )
    : QDialog( parent, "ScoreEditor" )
    , model_( model )
    , current_( 0 )
    , tools_( tools )
    , nameChanged_( false )
{
    gui::SubObjectName subObject( "ScoreEditor" );
    setCaption( tr( "Score editor" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 3, 1, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 1, 4 );
    {
        QLabel* nameLabel = new QLabel( tr( "Name:" ) );
        name_ = new gui::RichLineEdit( "name" );
        name_->setValidator( new NameValidator( name_, model, current_ ) );

        gui::RichGroupBox* informationBox = new gui::RichGroupBox( tr( "Information" ), this );
        gui::SubObjectName subObject( "informationBox" );
        QHBoxLayout* informationBoxLayout = new QHBoxLayout( informationBox );
        informationBoxLayout->addWidget( nameLabel );
        informationBoxLayout->addWidget( name_ );
        grid->addWidget( informationBox, 0, 0 );
    }
    {
        gui::RichWidget< QTabWidget >* tabs = new gui::RichWidget< QTabWidget >( "tabs", this );
        {
            gui::SubObjectName subObject( "tabs" );
            QWidget* page = new QWidget( tabs );
            Q3GridLayout* pageLayout = new Q3GridLayout( page, 6, 2, 0, 5 );
            pageLayout->setRowStretch( 1, 3 );
            pageLayout->setColStretch( 0, 2 );
            pageLayout->setColStretch( 1, 3 );
            {
                gui::SubObjectName subObject( "FormulaBox" );
                formula_ = new FormulaLineEdit();
                connect( formula_, SIGNAL( textChanged() ), SLOT( CheckFormula() ) );
                new ScoreSyntaxHighlighter( formula_, controllers, staticModel.indicators_ );

                checkResult_ = new QLabel();
                checkResult_->setMinimumHeight( 30 );

                gui::RichGroupBox* formulaBox = new gui::RichGroupBox( "FormulaBox", tr( "Formula" ), page );
                QVBoxLayout* informationBoxLayout = new QVBoxLayout( formulaBox );
                informationBoxLayout->addWidget( formula_ );
                informationBoxLayout->addWidget( checkResult_ );

                pageLayout->addMultiCellWidget( formulaBox, 0, 0, 0, 1 );
            }
            {
                ScorePrimitivesLibrary* library = new ScorePrimitivesLibrary( page, controllers, staticModel.indicators_ );
                pageLayout->addWidget( library, 1, 0 );
                connect( library, SIGNAL( Selected( const indicators::Primitive& ) ), SLOT( OnSelectPrimitive( const indicators::Primitive& ) ) );
                connect( library, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
            }
            {
                variables_ = new ScoreVariablesList( controllers, staticModel, tools, builder );
                connect( variables_, SIGNAL( Insert( const QString& ) ), SLOT( OnInsert( const QString& ) ) );
                connect( variables_, SIGNAL( Updated() ), SLOT( CheckFormula() ) );
                connect( variables_, SIGNAL( StartEdit() ), SLOT( hide() ) );
                connect( variables_, SIGNAL( EndEdit() ), SLOT( show() ) );
                connect( parent, SIGNAL( CloseDialogs() ), this, SLOT( OnCloseDialogs() ) );
                connect( this, SIGNAL( CloseDialogs() ), variables_, SLOT( OnCloseDialogs() ) );

                gui::SubObjectName subObject( "VariablesBox" );
                gui::RichGroupBox* variablesBox = new gui::RichGroupBox( "variablesBox", tr( "Variables" ), page );
                QVBoxLayout* variablesBoxLayout = new QVBoxLayout( variablesBox );
                variablesBoxLayout->addWidget( variables_ );
                pageLayout->addWidget( variablesBox, 1, 1 );
            }
            {
                gui::SubObjectName subObject( "helpBox" );

                help_ = new QLabel();

                gui::RichGroupBox* helpBox = new gui::RichGroupBox( "helpBox", page );
                QHBoxLayout* helpBoxLayout = new QHBoxLayout( helpBox );
                helpBoxLayout->addWidget( help_ );

                helpBox->setMinimumHeight( 80 );
                pageLayout->addMultiCellWidget( helpBox, 2, 2, 0, 1 );
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
        Q3HBox* box = new Q3HBox( this );
        ok_ = new gui::RichPushButton( "ok", tr( "Ok" ), box );
        gui::RichPushButton* cancel = new gui::RichPushButton( "cancel", tr( "Cancel" ), box );
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
    connect( name_, SIGNAL( textChanged( const QString& ) ), SLOT( NameChanged() ) );
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
    if( &score == current_ && nameChanged_ )
    {
        model_.Remove( score.GetName() );
        model_.Register( name_->text(), score );
        nameChanged_ = false;
    }
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
// Name: ScoreEditor::OnCloseDialogs
// Created: LDC 2013-09-05
// -----------------------------------------------------------------------------
void ScoreEditor::OnCloseDialogs()
{
    emit CloseDialogs();
    reject();
    hide();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::closeEvent
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void ScoreEditor::closeEvent( QCloseEvent* e )
{
    disconnect( name_, SIGNAL( textChanged( const QString& ) ) );
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
            throw MASA_EXCEPTION( tr( "Formula is empty." ).toStdString() );
        Score score( static_cast< Score& >( *current_ ) ); // $$$$ SBO 2009-05-07:
        CommitTo( score );
        score.CheckValidity();
        checkResult_->setText( "" );
        AllowCommit( true );
    }
    catch( const std::exception& e )
    {
        checkResult_->setText( QString( "<font color='#ff0000'><b>%1</b></font>" ).arg( tools::GetExceptionMsg( e ).c_str() ) );
        AllowCommit( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::AllowCommit
// Created: SBO 2009-07-24
// -----------------------------------------------------------------------------
void ScoreEditor::AllowCommit( bool base /* = true*/ )
{
    ok_->setEnabled( base && name_->hasAcceptableInput() );
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::NameChanged
// Created: JSR 2012-10-08
// -----------------------------------------------------------------------------
void ScoreEditor::NameChanged()
{
    nameChanged_ = true;
    AllowCommit();
}

// -----------------------------------------------------------------------------
// Name: ScoreEditor::Draw
// Created: FPO 2011-07-18
// -----------------------------------------------------------------------------
void ScoreEditor::Draw( gui::Viewport_ABC& viewport )
{
    variables_->Draw( viewport );
}
