// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreDialog.h"
#include "ScoreList.h"
#include "moc_ScoreDialog.cpp"
#include "clients_kernel/SimpleLocationDrawer.h"
#include "preparation/ScoresModel.h"

using namespace kernel;

namespace
{
    class NameValidator : public QValidator
    {
    public:
                 NameValidator( QObject* parent, const ScoresModel& model ) : QValidator( parent ), model_( &model ) {}
        virtual ~NameValidator() {}

        virtual QValidator::State validate( QString& input, int& ) const
        {
            return input.isEmpty() || model_->Find( input ) ? QValidator::Intermediate : QValidator::Acceptable;
        }

    private:
        const ScoresModel* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoreDialog::ScoreDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ScoresModel& model,
                          gui::ParametersLayer& layer, const StaticModel& staticModel, const tools::ExerciseConfig& config,
                          const kernel::GlTools_ABC& tools)
    : QDialog( parent, "ScoreDialog" )
    , model_   ( model )
    , tools_   ( tools )
{
    setModal( false );
    setCaption( tr( "Scores" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 4 );
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Scores" ), this );
        scores_ = new ScoreList( box, controllers, factory, layer, model, staticModel, config, tools_ );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
        connect( scores_, SIGNAL( ScoreDeleted( const Score_ABC& ) ), SLOT( OnDeleteScore( const Score_ABC& ) ) );
        connect( scores_, SIGNAL( Show() ), SLOT( show() ) );
        connect( scores_, SIGNAL( Hide() ), SLOT( hide() ) );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Create new score" ), this );
        new QLabel( tr( "Name: " ), box );
        editor_ = new QLineEdit( box );
        editor_->setValidator( new NameValidator( editor_, model_ ) );
        createButton_ = new QPushButton( tr( "Create" ), box );
        createButton_->setEnabled( false );
        grid->addMultiCellWidget( box, 1, 1, 0, 2 );
        connect( editor_, SIGNAL( textChanged( const QString& ) ), SLOT( OnCreateTextChanged( const QString& ) ) );
        connect( createButton_, SIGNAL( clicked() ), SLOT( OnCreateButtonClicked() ) );
    }
    {
        QPushButton* ok = new QPushButton( tr( "Ok" ), this );
        grid->addWidget( ok, 2, 2 );
        connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog destructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoreDialog::~ScoreDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog::OnCreateTextChanged
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoreDialog::OnCreateTextChanged( const QString& )
{
    createButton_->setEnabled( editor_->hasAcceptableInput() );
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog::OnCreateButtonClicked
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
void ScoreDialog::OnCreateButtonClicked()
{
    model_.Create( editor_->text() );
    editor_->clear();
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog::OnDeleteScore
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void ScoreDialog::OnDeleteScore( const Score_ABC& score )
{
    model_.Delete( score );
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog::sizeHint
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QSize ScoreDialog::sizeHint() const
{
    return QSize( 400, 400 );
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog::Load
// Created: ABR 2011-05-27
// -----------------------------------------------------------------------------
void ScoreDialog::Load()
{
    scores_->Load();
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog::Draw
// Created: FPO 2011-07-18
// -----------------------------------------------------------------------------
void ScoreDialog::Draw( kernel::Viewport_ABC& viewport )
{
    scores_->Draw( viewport );
}
