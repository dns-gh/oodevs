// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "NewProfileDialog.h"
#include "moc_NewProfileDialog.cpp"
#include "ProfilesChecker_ABC.h"
#include "preparation/ProfilesModel.h"

// -----------------------------------------------------------------------------
// Name: NewProfileDialog constructor
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
NewProfileDialog::NewProfileDialog( QWidget* parent, ProfilesModel& model, ProfilesChecker_ABC& checker)
    : QDialog( parent )
    , model_  ( model )
    , checker_( checker )
{
    setCaption( tr( "New profile") );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    QHBoxLayout* editLayout = new QHBoxLayout();
    value_ = new QLineEdit();
    connect( value_, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateText() ) );
    editLayout->addWidget( new QLabel( tr( "Profile name:" ) ) );
    editLayout->addWidget( value_ );
    QHBoxLayout* warningLayout = new QHBoxLayout();
    warningLabel_ = new QLabel();
    warningLayout->addWidget( warningLabel_, Qt::AlignCenter );
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    okButton_ = new QDialogButtonBox( QDialogButtonBox::Ok );
    connect( okButton_, SIGNAL( accepted() ), this, SLOT( Validate() ) );
    okButton_->setFocus();
    QDialogButtonBox* cancelButton = new QDialogButtonBox( QDialogButtonBox::Cancel );
    connect( cancelButton, SIGNAL( rejected() ), this, SLOT( reject() ) );
    buttonLayout->addWidget( okButton_ );
    buttonLayout->addWidget( cancelButton );
    mainLayout->addLayout( editLayout );
    mainLayout->addLayout( warningLayout );
    mainLayout->addLayout( buttonLayout );
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog destructor
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
NewProfileDialog::~NewProfileDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::UpdateText
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void NewProfileDialog::UpdateText()
{
    QString name= value_->text();
    if( name.isEmpty() )
    {
        warningLabel_->setText( "<font color=\"#FF0000\">" + tr( "Profile name is empty." ) + "</font>" );
        okButton_->setEnabled( false );
    }
    else
    {
        if( model_.Exists( name ) || checker_.Exists( name ) )
        {
            warningLabel_->setText( "<font color=\"#FF0000\">" + tr( "Profile '%1' already exists." ).arg( name ) + "</font>" );
            okButton_->setEnabled( false );
        }
        else
        {
            warningLabel_->setText( "" );
            okButton_->setEnabled( true );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::Exec
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void NewProfileDialog::Exec()
{
    value_->setText( BuildUniqueLogin() );
    exec();
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::BuildUniqueLogin
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
QString NewProfileDialog::BuildUniqueLogin() const
{
    static const QString defaultName = tr( "New profile" );
    QString name;
    int i = 0;
    while( model_.Exists( name = defaultName + ( i > 0 ? " (" + locale().toString( i ) + ")" : "" ) ) )
        ++i;
    return name;
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::Validate
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void NewProfileDialog::Validate()
{
     model_.CreateProfile( value_->text() );
     accept();
}
