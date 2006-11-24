// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LoginDialog.h"
#include "gaming/Profile.h"
#include "moc_LoginDialog.cpp"

// -----------------------------------------------------------------------------
// Name: LoginDialog constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::LoginDialog( QWidget* pParent, const Profile& profile, Publisher_ABC& publisher )
    : QDialog( pParent )
    , profile_( profile )
    , publisher_( publisher )
{
    setCaption( tr("Profile parameters") );

    QGridLayout* pMainLayout = new QGridLayout( this, 3, 2 );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );

    uint nCurRow = 0;
    
    // Login
    pMainLayout->addWidget( new QLabel( tr("User login"), this ), nCurRow, 0 );
    login_ = new QLineEdit( this );
    pMainLayout->addWidget( login_, nCurRow, 1 );

    // Password
    ++ nCurRow;
    pMainLayout->addWidget( new QLabel( tr("Password"), this ), nCurRow, 0 );
    password_ = new QLineEdit( this );
    password_->setEchoMode( QLineEdit::Password );
    pMainLayout->addWidget( password_ , nCurRow, 1 );

    // Buttons
    ++ nCurRow;
    QPushButton* pOKButton     = new QPushButton( tr( "Ok" )    , this );
    QPushButton* pCancelButton = new QPushButton( tr( "Cancel" ), this );
    pMainLayout->addWidget( pOKButton, nCurRow, 0     );
    pMainLayout->addWidget( pCancelButton, nCurRow, 1 );
    pOKButton->setDefault( TRUE );

    connect( pCancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( pOKButton    , SIGNAL( clicked() ), SLOT( Validate() ) );

    // $$$$ AGE 2006-10-11: read login et password from registry ?
}

// -----------------------------------------------------------------------------
// Name: LoginDialog destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::~LoginDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::Validate
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void LoginDialog::Validate()
{
    profile_.Login( publisher_, login_->text().ascii(), password_->text().ascii() );
    accept();
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::Reject
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void LoginDialog::Reject()
{
    // $$$$ AGE 2006-10-11: what ??
    reject();
}