// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ProfileDialog.h"
#include "moc_ProfileDialog.cpp"
#include "UserProfileWidget.h"
#include "UserProfileList.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ProfileDialog constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfileDialog::ProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ProfilesModel& model, const kernel::ExtensionTypes& extensions )
    : QDialog( parent, "ProfileDialog" )
    , model_( model )
{
    setCaption( tr( "User profiles" ) );
    QGridLayout* grid = new QGridLayout( this, 3, 2 );
    grid->setColStretch( 0, 1 );
    grid->setColStretch( 1, 3 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 5 );
    grid->setRowStretch( 2, 1 );

    QHBox* box = new QHBox( this );
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
    grid->addMultiCellWidget( box, 0, 0, 0, 1 );

    box = new QVBox( this );
    box->setMargin( 5 );
    pages_ = new UserProfileWidget( box, controllers, factory, icons, extensions );
    pages_->setMargin( 5 );
    grid->addWidget( box, 1, 1 );

    box = new QVBox( this );
    box->setMargin( 5 );
    list_ = new UserProfileList( box, *pages_, controllers, model );
    grid->addWidget( box, 1, 0 );

    box = new QHBox( this );
    box->setMargin( 5 );
    box->setMaximumHeight( 40 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    okBtn->setDefault( true );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    grid->addWidget( box, 2, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfileDialog::~ProfileDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog::sizeHint
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
QSize ProfileDialog::sizeHint() const
{
    return QSize( 650, 550 );
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog::OnAccept
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfileDialog::OnAccept()
{
    list_->Save();
    accept();
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog::OnReject
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void ProfileDialog::OnReject()
{
    list_->Cancel();
    reject();
}
