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
#include "gaming/UserProfile.h"
#include "clients_kernel/Controllers.h"

Q_DECLARE_METATYPE( const UserProfile* )
// -----------------------------------------------------------------------------
// Name: UserProfileDialog constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileDialog::UserProfileDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& itemFactory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons, UserProfileFactory_ABC& factory )
    : QDialog( parent, "UserProfileDialog" )
    , controllers_ ( controllers )
    , controlTimeProfile_( 0 )
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
    connect( timeControlCombo_, SIGNAL( currentIndexChanged( const QString& ) ), this, SLOT( OnTimeControlChanged( const QString& ) ) );
    box->setStretchFactor( timeControlLabel, 1 );
    box->setStretchFactor( timeControlCombo_, 2 );
    box->setMargin( 5 );
    grid->addWidget( box, 2, 1 );

    box = new Q3VBox( this );
    box->setMargin( 5 );
    pages_ = new UserProfileWidget( box, controllers, itemFactory, profile, icons );
    pages_->setMargin( 5 );
    grid->addWidget( box, 1, 1 );

    box = new Q3VBox( this );
    box->setMargin( 5 );
    list_ = new UserProfileList( box, *pages_, controllers, factory );
    connect( list_, SIGNAL( ProfileChanged( const UserProfile*, const UserProfile* ) ), this, SLOT( OnProfileChanged( const UserProfile*, const UserProfile* ) ) );
    grid->addWidget( box, 1, 0, 2, 1 );

    box = new Q3HBox( this );
    box->setMargin( 5 );
    box->setSpacing( 5 );
    box->setMaximumHeight( 40 );
    QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
    okBtn->setDefault( true );
    grid->addWidget( box, 3, 1, Qt::AlignRight );

    connect( okBtn, SIGNAL( clicked() ), SLOT( OnAccept() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileDialog::~UserProfileDialog()
{
    controllers_.Unregister( *this );
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
    list_->Save( controlTimeProfile_ );
    accept();
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::OnReject
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileDialog::OnReject()
{
    pages_->Reset();
    list_->Cancel();
    reject();
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::OnProfileChanged
// Created: NPT 2013-05-23
// -----------------------------------------------------------------------------
void UserProfileDialog::OnProfileChanged( const UserProfile* profile, const UserProfile* editor )
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
    if( editor->IsSupervisor() && timeControlCombo_->findText( editor->GetLogin() ) == -1 )
        timeControlCombo_->addItem( editor->GetLogin(), QVariant::fromValue( profile ) );
    else if( !editor->IsSupervisor() && timeControlCombo_->findText( editor->GetLogin() ) != -1 )
        timeControlCombo_->removeItem( timeControlCombo_->findText( editor->GetLogin() ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::OnTimeControlChanged
// Created: NPT 2013-05-24
// -----------------------------------------------------------------------------
void UserProfileDialog::OnTimeControlChanged( const QString& text )
{
    int index = timeControlCombo_->findText( text );
    if( index != -1 )
        controlTimeProfile_ = timeControlCombo_->itemData( index ).value< const UserProfile* >();
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::NotifyCreated
// Created: NPT 2013-05-23
// -----------------------------------------------------------------------------
void UserProfileDialog::NotifyCreated( const UserProfile& profile )
{
    if( profile.IsSupervisor() )
    {
        if( timeControlCombo_->findText( profile.GetLogin() ) != -1 )
            timeControlCombo_->addItem( profile.GetLogin(), QVariant::fromValue( &profile ) );
        if( profile.CanControlTime() )
            timeControlCombo_->setCurrentIndex( timeControlCombo_->findText( profile.GetLogin() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::NotifyUpdated
// Created: NPT 2013-05-29
// -----------------------------------------------------------------------------
void UserProfileDialog::NotifyUpdated( const UserProfile& profile )
{
    if( profile.IsSupervisor() && timeControlCombo_->findText( profile.GetLogin() ) == -1 )
        timeControlCombo_->addItem( profile.GetLogin(), QVariant::fromValue( &profile ) );
    if( profile.CanControlTime() )
        timeControlCombo_->setCurrentIndex( timeControlCombo_->findText( profile.GetLogin() ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileDialog::NotifyDeleted
// Created: NPT 2013-05-23
// -----------------------------------------------------------------------------
void UserProfileDialog::NotifyDeleted( const UserProfile& profile )
{
    int index = timeControlCombo_->findText( profile.GetLogin() );
    if( index != -1 )
        timeControlCombo_->removeItem( index );
}
