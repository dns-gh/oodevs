// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfileDialog.h"
#include "moc_UserProfileDialog.cpp"
#include "UserProfileWidget.h"
#include "UserProfileList.h"
#include "icons.h"
#include "gaming/Profile.h"

// -----------------------------------------------------------------------------
// Name: UserProfileDialog constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileDialog::UserProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons, UserProfileFactory_ABC& factory )
    : QDialog( parent, "UserProfileDialog" )
{
    setCaption( tr( "User profiles" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 3, 2 );
    grid->setColStretch( 0, 1 );
    grid->setColStretch( 1, 3 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 5 );
    grid->setRowStretch( 2, 1 );

    Q3HBox* box = new Q3HBox( this );
    QLabel* title = new QLabel( caption(), box );
    QFont font;
    font.setBold( true );
    font.setPointSize( 16 );
    title->setFont( font );
    title->setMargin( 10 );
    title->setBackgroundColor( Qt::white );
    QLabel* icon = new QLabel( box );
    icon->setPixmap( MAKE_PIXMAP( profiles ) );
    icon->setMaximumWidth( 64 );
    icon->setBackgroundColor( Qt::white );
    box->setMaximumHeight( 64 );
    grid->addMultiCellWidget( box, 0, 0, 0, 1 );

    box = new Q3VBox( this );
    box->setMargin( 5 );
    pages_ = new UserProfileWidget( box, controllers, itemFactory, profile, icons );
    pages_->setMargin( 5 );
    grid->addWidget( box, 1, 1 );

    box = new Q3VBox( this );
    box->setMargin( 5 );
    list_ = new UserProfileList( box, *pages_, controllers, factory );
    grid->addWidget( box, 1, 0 );

    box = new Q3HBox( this );
    box->setMargin( 5 );
    box->setSpacing( 5 );
    box->setMaximumHeight( 40 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    grid->addWidget( box, 2, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileDialog::~UserProfileDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::sizeHint
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
QSize UserProfileDialog::sizeHint() const
{
    return QSize( 650, 550 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::CanBeShown
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
bool UserProfileDialog::CanBeShown( const Profile& profile ) const
{
    return profile.IsSupervision();
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::OnAccept
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileDialog::OnAccept()
{
    pages_->Commit();
    accept();
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::OnReject
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileDialog::OnReject()
{
    pages_->Reset();
    reject();
}
