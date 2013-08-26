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
#include "moc_LoginDialog.cpp"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LoginDialog constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::LoginDialog( QWidget* pParent )
    : QDialog( pParent, 0, true, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title )
{
    setCaption( tools::translate( "LoginDialog", "Select user profile" ) );
    QVBoxLayout* mainLayout = new QVBoxLayout( this );

    users_ = new QListWidget( this );
    users_->setGridSize( QSize( 300, 30 ) );
    users_->setUniformItemSizes( true );
    users_->setMinimumSize( 345, 200 );
    users_->setWordWrap( true );
    connect( users_, SIGNAL( itemSelectionChanged() ), SLOT( OnSelectItem() ) );
    connect( users_, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), SLOT( OnAccept() ) );
    mainLayout->addWidget( users_ );

    widget_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( widget_ );
    QHBoxLayout* informationLayout = new QHBoxLayout();
    QLabel* information = new QLabel( tools::translate( "LoginDialog", "Please enter the password..." ) );
    QFont font;
    font.setBold( true );
    information->setFont( font );
    information->setAlignment( Qt::AlignHCenter );
    informationLayout->addWidget( information );
    QHBoxLayout* passwordLayout = new QHBoxLayout();
    QLabel* label = new QLabel( tools::translate( "LoginDialog", "Password: " ) );
    password_ = new QLineEdit();
    password_->setEchoMode( QLineEdit::Password );
    label->setBuddy( password_ );
    passwordLayout->addWidget( label );
    passwordLayout->addWidget( password_ );
    widget_->hide();
    layout->addLayout( informationLayout );
    layout->addLayout( passwordLayout );
    mainLayout->addWidget( widget_ );

    QHBoxLayout* buttonBox = new QHBoxLayout();
    QPushButton* ok = new QPushButton( tools::translate( "LoginDialog", "Ok" ) );
    QPushButton* cancel = new QPushButton( tools::translate( "LoginDialog", "Cancel" ) );
    connect( ok, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancel, SIGNAL( clicked() ), SLOT( reject() ) );
    buttonBox->addWidget( ok );
    buttonBox->addWidget( cancel );
    mainLayout->addLayout( buttonBox );
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
// Name: LoginDialog::OnAccept
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnAccept()
{
    accept();
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnSelectItem
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnSelectItem()
{
    // NOTHING
}
