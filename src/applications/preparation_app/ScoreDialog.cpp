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
#include "actions_gui/InterfaceBuilder.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichGroupBox.h"
#include "preparation/ScoresModel.h"
#include "preparation/StaticModel.h"

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
ScoreDialog::ScoreDialog( const QString& objectName,
                          QWidget* parent,
                          kernel::Controllers& controllers,
                          ScoresModel& model,
                          const std::shared_ptr< gui::ParametersLayer >& layer,
                          const ::StaticModel& staticModel,
                          const tools::ExerciseConfig& config,
                          gui::GLView_ABC& tools)
    : QDialog( parent, "ScoreDialog" )
    , builder_( new actions::gui::InterfaceBuilder( controllers, config, layer, staticModel ) )
    , model_  ( model )
    , tools_  ( tools )
{
    gui::SubObjectName subObject( objectName );
    setModal( false );
    connect( parent, SIGNAL( CloseDialogs() ), this, SLOT( OnCloseDialogs() ) );
    setCaption( tr( "Scores" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 4 );
    {
        //score list
        scores_ = new ScoreList( controllers, model, config, staticModel, tools, *builder_, "score-list" );
        connect( scores_, SIGNAL( ScoreDeleted( const Score_ABC& ) ), SLOT( OnDeleteScore( const Score_ABC& ) ) );
        connect( scores_, SIGNAL( Show() ), SLOT( show() ) );
        connect( scores_, SIGNAL( Hide() ), SLOT( hide() ) );
        connect( this, SIGNAL( CloseDialogs() ), scores_, SIGNAL( CloseDialogs() ) );

        //container
        gui::RichGroupBox* scoreBox = new gui::RichGroupBox( "scoreBox", tr( "Scores" ), this );
        QHBoxLayout* scoreBoxLayout = new QHBoxLayout( scoreBox );
        scoreBoxLayout->addWidget( scores_ );
        grid->addMultiCellWidget( scoreBox, 0, 0, 0, 2 );
    }
    {
        //label
        QLabel* nameLabel = new QLabel( tr( "Name: " ) );
        //RichLineEdit
        editor_ = new gui::RichLineEdit( "editor" );
        editor_->setValidator( new NameValidator( editor_, model_ ) );
        connect( editor_, SIGNAL( textChanged( const QString& ) ), SLOT( OnCreateTextChanged( const QString& ) ) );
        //create button
        createButton_ = new gui::RichPushButton( "create", tr( "Create" ) );
        createButton_->setEnabled( false );
        connect( createButton_, SIGNAL( clicked() ), SLOT( OnCreateButtonClicked() ) );

        //container
        gui::RichGroupBox* createNewScoreBox = new gui::RichGroupBox( "createNewScoreBox", tr( "Create new score" ), this );
        QHBoxLayout* createNewScoreBoxLayout = new QHBoxLayout( createNewScoreBox );
        createNewScoreBoxLayout->addWidget( nameLabel );
        createNewScoreBoxLayout->addWidget( editor_ );
        createNewScoreBoxLayout->addWidget( createButton_ );
        grid->addMultiCellWidget( createNewScoreBox, 1, 1, 0, 2 );
    }
    {
        gui::RichPushButton* ok = new gui::RichPushButton( "ok", tr( "Ok" ), this );
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
    delete builder_;
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
// Name: ScoreDialog::OnCloseDialogs
// Created: LDC 2013-09-06
// -----------------------------------------------------------------------------
void ScoreDialog::OnCloseDialogs()
{
    emit CloseDialogs();
    hide();
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
void ScoreDialog::Draw( gui::Viewport_ABC& viewport )
{
    scores_->Draw( viewport );
}
