// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ProfileDialog.h"
#include "moc_ProfileDialog.cpp"
#include "UserProfileWidget.h"
#include "UserProfileList.h"
#include "ControlsChecker.h"
#include "icons.h"
#include "preparation/Model.h"
#include "preparation/UserProfile.h"
#include "clients_kernel/Controllers.h"

Q_DECLARE_METATYPE( const UserProfile* )

// -----------------------------------------------------------------------------
// Name: ProfileDialog constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfileDialog::ProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const gui::EntitySymbols& icons,
                              Model& model, const kernel::ExtensionTypes& extensions )
    : ModalDialog( parent, "ProfileDialog" )
    , controllers_ ( controllers )
    , pChecher_( new ControlsChecker( controllers, model ) )
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

    box = new Q3HBox( this );
    QLabel* timeControlLabel = new QLabel( tr( "Time Control Profile" ), box );
    timeControlCombo_ = new QComboBox( box );
    box->setStretchFactor( timeControlLabel, 1 );
    box->setStretchFactor( timeControlCombo_, 2 );
    box->setMargin( 5 );
    grid->addWidget( box, 2, 1 );

    box = new Q3VBox( this );
    box->setMargin( 5 );
    pages_ = new UserProfileWidget( box, controllers, factory, icons, extensions, *pChecher_, model );
    pages_->setMargin( 5 );
    grid->addWidget( box, 1, 1 );

    box = new Q3VBox( this );
    box->setMargin( 5 );
    list_ = new UserProfileList( box, *pages_, controllers, model.profiles_, extensions, *pChecher_ );
    connect( list_, SIGNAL( ProfileChanged( const UserProfile*, const UserProfile* ) ), this, SLOT( OnProfileChanged( const UserProfile*, const UserProfile* ) ) );
    connect( list_, SIGNAL( DoConsistencyCheck() ), parent, SIGNAL( CheckConsistency() ) );
    grid->addWidget( box, 1, 0, 2, 1 );


    box = new Q3HBox( this );
    box->setMargin( 5 );
    box->setSpacing( 5 );
    box->setMaximumHeight( 40 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    okBtn->setDefault( true );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    grid->addWidget( box, 3, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfileDialog::~ProfileDialog()
{
    controllers_.Unregister( *this );
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
    const UserProfile* profile = 0;
    int index = timeControlCombo_->currentIndex();
    if( index != -1 )
        profile = timeControlCombo_->itemData( index ).value< const UserProfile* >();
    list_->Save( profile );
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

// -----------------------------------------------------------------------------
// Name: ProfileDialog::OnProfileChanged
// Created: NPT 2013-05-22
// -----------------------------------------------------------------------------
void ProfileDialog::OnProfileChanged( const UserProfile* profile, const UserProfile* editor )
{
    if( !profile )
        return;

    for( int i = 0; i< timeControlCombo_->count(); ++i )
    {
        if( const UserProfile* comboProfile = timeControlCombo_->itemData( i ).value< const UserProfile* >() )
        {
            if( profile == comboProfile )
                timeControlCombo_->setItemText( i, editor->GetLogin() );
        }
    }
    if( editor->GetRole() == "supervisor" && timeControlCombo_->findText( editor->GetLogin() ) == -1 )
        timeControlCombo_->addItem( editor->GetLogin(), QVariant::fromValue( profile ) );
    else if( editor->GetRole() != "supervisor" && timeControlCombo_->findText( editor->GetLogin() ) != -1 )
        timeControlCombo_->removeItem( timeControlCombo_->findText( editor->GetLogin() ) );
}


// -----------------------------------------------------------------------------
// Name: ProfileDialog::showEvent
// Created: LGY 2011-09-28
// -----------------------------------------------------------------------------
void ProfileDialog::showEvent( QShowEvent* /*pEvent*/ )
{
    pages_->Show();
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog::hideEvent
// Created: LGY 2011-12-15
// -----------------------------------------------------------------------------
void ProfileDialog::hideEvent( QHideEvent* )
{
    pages_->Hide();
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog::NotifyCreated
// Created: NPT 2013-05-21
// -----------------------------------------------------------------------------
void ProfileDialog::NotifyCreated( const UserProfile& profile )
{
    if( profile.GetRole() == "supervisor" )
    {
        timeControlCombo_->addItem( profile.GetLogin(), QVariant::fromValue( &profile ) );
        if( profile.CanControlTime() )
            timeControlCombo_->setCurrentIndex( timeControlCombo_->findText( profile.GetLogin() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfileDialog::NotifyDeleted
// Created: NPT 2013-05-21
// -----------------------------------------------------------------------------
void ProfileDialog::NotifyDeleted( const UserProfile& profile )
{
    int index = timeControlCombo_->findText( profile.GetLogin() );
    if( index != -1 )
        timeControlCombo_->removeItem( index );
}
