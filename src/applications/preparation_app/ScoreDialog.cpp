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
#include "preparation/ScoresModel.h"

namespace
{
    class NameValidator : public QValidator
    {
    public:
                 NameValidator( QObject* parent, ScoresModel& model ) : QValidator( parent ), model_( &model ) {}
        virtual ~NameValidator() {}

        virtual QValidator::State validate( QString& input, int& ) const
        {
            return input.isEmpty() || model_->Find( input ) ? QValidator::Intermediate : QValidator::Acceptable;
        }

    private:
        ScoresModel* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: ScoreDialog constructor
// Created: SBO 2009-04-16
// -----------------------------------------------------------------------------
ScoreDialog::ScoreDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ScoresModel& model, gui::ParametersLayer& layer, const StaticModel& staticModel )
    : QDialog( parent, "ScoreDialog" )
    , model_( model )
{
    setModal( false );
    setCaption( tr( "Scores" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 4 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Scores" ), this );
        ScoreList* scores = new ScoreList( box, controllers, factory, layer, staticModel );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
        connect( scores, SIGNAL( ScoreDeleted( const Score_ABC& ) ), SLOT( OnDeleteScore( const Score_ABC& ) ) );
        connect( scores, SIGNAL( Show() ), SLOT( show() ) );
        connect( scores, SIGNAL( Hide() ), SLOT( hide() ) );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Create new score" ), this );
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
        QButton* ok = new QPushButton( tr( "Ok" ), this );
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
