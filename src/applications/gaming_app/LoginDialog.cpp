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
#include "clients_kernel/Controllers.h"
#include "gaming/Network.h"
#include "gaming/Profile.h"
#include "gaming/Tools.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    class UserItem : public Q3IconViewItem
    {
    public:
        UserItem( Q3IconView* parent, const AvailableProfile& profile )
            : Q3IconViewItem( parent )
            , profile_( profile )
        {
            setText( profile_.GetLogin().isEmpty() ? tools::translate( "LoginDialog", "Anonymous" ) : profile_.GetLogin() );
            const QString pixmap = QString( "images/gaming/profile/%1%2.png" ).arg( profile_.IsSupervior() ? "supervisor" : "standard" )
                                                                              .arg( profile_.IsPasswordProtected() ? "_password" : "" );
            QImage img( tools::ExerciseConfig::BuildResourceChildFile( pixmap.ascii() ).c_str() );
            img = img.scaled( 30, 30 );
            setPixmap( QPixmap::fromImage( img ) );
        }

        bool RequiresPassword() const { return profile_.IsPasswordProtected(); }
        QString Login() const { return profile_.GetLogin(); }

    private:
        UserItem( const UserItem& );
        UserItem& operator=( const UserItem& );

        const AvailableProfile profile_;
    };
}

// -----------------------------------------------------------------------------
// Name: LoginDialog constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::LoginDialog( QWidget* pParent, const Profile& profile, Network& network, kernel::Controllers& controllers )
    : QDialog( pParent, 0, true, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title )
    , controllers_( controllers )
    , profile_( profile )
    , network_( network )
{
    setCaption( tools::translate( "LoginDialog", "Select user profile" ) );

    Q3GridLayout* grid = new Q3GridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 10 );
    grid->setColStretch( 0, 3 );

    {
        users_ = new Q3IconView( this );
        users_->setGridX( 300 );
        users_->setGridY( 30 );
        users_->setMinimumSize( 345, 200 );
        users_->setWordWrapIconText( true );
        users_->setSorting( true );
        users_->setItemsMovable( false );
        users_->setHScrollBarMode( Q3ScrollView::AlwaysOff );
        users_->setItemTextPos( Q3IconView::Right );
        users_->setResizeMode( Q3IconView::Adjust );
        connect( users_, SIGNAL( selectionChanged( Q3IconViewItem* ) ), SLOT( OnSelectItem( Q3IconViewItem* ) ) );
        connect( users_, SIGNAL( doubleClicked( Q3IconViewItem* ) ), SLOT( OnAccept() ) );
        connect( users_, SIGNAL( returnPressed( Q3IconViewItem* ) ), SLOT( OnAccept() ) );
        grid->addMultiCellWidget( users_, 0, 0, 0, 2 );
    }
    {
        passwordBox_ = new Q3HBox( this );
        QLabel* label = new QLabel( tools::translate( "LoginDialog", "Password: " ), passwordBox_ );
        password_ = new QLineEdit( passwordBox_ );
        password_->setEchoMode( QLineEdit::Password );
        label->setBuddy( password_ );
        grid->addWidget( passwordBox_, 1, 0, Qt::AlignLeft );
        passwordBox_->hide();
    }
    {
        QPushButton* ok     = new QPushButton( tools::translate( "LoginDialog", "Ok" ), this );
        QPushButton* cancel = new QPushButton( tools::translate( "LoginDialog", "Cancel" ), this );
        grid->addWidget( ok, 1, 1 );
        grid->addWidget( cancel, 1, 2 );
        connect( ok, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LoginDialog destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::~LoginDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::NotifyUpdated
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void LoginDialog::NotifyUpdated( const AvailableProfile& profile )
{
    profiles_.push_back( profile );
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::showEvent
// Created: SBO 2009-12-18
// -----------------------------------------------------------------------------
void LoginDialog::showEvent( QShowEvent* /*ev*/ )
{
    users_->clear();
    for( T_Profiles::const_iterator it = profiles_.begin(); it != profiles_.end(); ++it )
        new UserItem( users_, *it );
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnAccept
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnAccept()
{
    if( passwordBox_->isShown() && password_->text().isEmpty() )
        return;
    if( UserItem* item = static_cast< UserItem* >( users_->currentItem() ) )
    {
        profiles_.clear();
        profile_.Login( item->Login().ascii(), password_->text().ascii() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnReject
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnReject()
{
    profiles_.clear();
    network_.Disconnect();
    reject();
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnSelectItem
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnSelectItem( Q3IconViewItem* item )
{
    UserItem* user = static_cast< UserItem* >( item );
    passwordBox_->setShown( user->RequiresPassword() );
    password_->clear();
}
