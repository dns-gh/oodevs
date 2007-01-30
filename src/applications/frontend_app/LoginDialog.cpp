// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "LoginDialog.h"
#include "moc_LoginDialog.cpp"
#include "frontend/Profile.h"

// -----------------------------------------------------------------------------
// Name: LoginDialog constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
LoginDialog::LoginDialog( QWidget* parent, const frontend::Profile& profile, frontend::Publisher_ABC& publisher )
    : QDialog( parent )
    , profile_( profile )
    , publisher_( publisher )
{
    setCaption( tr( "Authentication" ) );

    QVBoxLayout* layout = new QVBoxLayout( this );
    QGroupBox* box = new QGroupBox( 2, Qt::Vertical, this );
    layout->addWidget( box );

    new QLabel( tr( "Login" ), box );
    login_ = new QLineEdit( box );

    new QLabel( tr( "Password" ), box );
    password_ = new QLineEdit( box );

    QPushButton* ok     = new QPushButton( tr( "Ok" )    , box );
    QPushButton* cancel = new QPushButton( tr( "Cancel" ), box );
    ok->setDefault( true );

    connect( ok    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( Reject() ) );
}

// -----------------------------------------------------------------------------
// Name: LoginDialog destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
LoginDialog::~LoginDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::Validate
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void LoginDialog::Validate()
{
    profile_.Login( publisher_, login_->text(), password_->text() );
    accept();
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::Reject
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
void LoginDialog::Reject()
{
    // $$$$ SBO 2007-01-29: Kill frontend !
    reject();
}
