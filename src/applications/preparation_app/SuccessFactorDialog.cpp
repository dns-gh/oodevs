// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorDialog.h"
#include "moc_SuccessFactorDialog.cpp"
#include "SuccessFactorList.h"
#include "preparation/ScoresModel.h"
#include "preparation/SuccessFactorsModel.h"

namespace
{
    class NameValidator : public QValidator
    {
    public:
                 NameValidator( QObject* parent, SuccessFactorsModel& model ) : QValidator( parent ), model_( &model ) {}
        virtual ~NameValidator() {}

        virtual QValidator::State validate( QString& input, int& ) const
        {
            return input.isEmpty() || model_->Find( input ) ? QValidator::Intermediate : QValidator::Acceptable;
        }

    private:
        SuccessFactorsModel* model_;
    };
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog constructor
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
SuccessFactorDialog::SuccessFactorDialog( QWidget* parent, kernel::Controllers& controllers, SuccessFactorsModel& model, gui::ItemFactory_ABC& factory, const SuccessFactorActionTypes& actionTypes, const ScoresModel& scores )
    : QDialog( parent, "SuccessFactorDialog" )
    , model_( model )
    , scores_( scores )
{
    setModal( false );
    setCaption( tr( "Success factors" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 3, 1, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 4 );
    {
        SuccessFactorList* factors = new SuccessFactorList( this, controllers, factory, actionTypes, scores );
        grid->addWidget( factors, 0, 0 );
        connect( factors, SIGNAL( Deleted( const SuccessFactor& ) ), SLOT( OnDelete( const SuccessFactor& ) ) );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Create new factor" ), this );
        new QLabel( tr( "Name: " ), box );
        editor_ = new QLineEdit( box );
        editor_->setValidator( new NameValidator( editor_, model_ ) );
        createButton_ = new QPushButton( tr( "Create" ), box );
        createButton_->setEnabled( false );
        grid->addWidget( box, 1, 0 );
        connect( editor_, SIGNAL( textChanged( const QString& ) ), SLOT( OnCreateTextChanged( const QString& ) ) );
        connect( createButton_, SIGNAL( clicked() ), SLOT( OnCreateButtonClicked() ) );
    }
    {
        QPushButton* ok = new QPushButton( tr( "Ok" ), this );
        grid->addWidget( ok, 2, 0 );
        connect( ok, SIGNAL( clicked() ), SLOT( accept() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog destructor
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
SuccessFactorDialog::~SuccessFactorDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog::OnCreateTextChanged
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void SuccessFactorDialog::OnCreateTextChanged( const QString& /*text*/ )
{
    createButton_->setEnabled( editor_->hasAcceptableInput() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog::OnCreateButtonClicked
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void SuccessFactorDialog::OnCreateButtonClicked()
{
    model_.Create( editor_->text() );
    editor_->clear();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog::sizeHint
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
QSize SuccessFactorDialog::sizeHint() const
{
    return QSize( 400, 400 );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog::OnDelete
// Created: SBO 2010-06-11
// -----------------------------------------------------------------------------
void SuccessFactorDialog::OnDelete( const SuccessFactor& factor )
{
    model_.Delete( factor );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorDialog::showEvent
// Created: SBO 2010-06-11
// -----------------------------------------------------------------------------
void SuccessFactorDialog::showEvent( QShowEvent* e )
{
    QDialog::showEvent( e );
    if( scores_.Count() == 0 )
        QMessageBox::warning( this, tr( "Warning" ), tr( "No score has been defined, success factors cannot be created without scores." ) );
}
