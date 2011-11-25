//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ConnectLoginDialog.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Network.h"
#include "gaming/Profile.h"
#include "gaming/Tools.h"
#include "tools/ExerciseConfig.h"

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
            const QString pixmap = QString( "images/gaming/profile/%1%2.png" ).arg( profile_.IsSupervision() ? "supervisor" : "standard" )
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
// Name: ConnectLoginDialog constructor
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
ConnectLoginDialog::ConnectLoginDialog( QWidget* pParent, const Profile& profile, Network& network, kernel::Controllers& controllers )
    : LoginDialog( pParent )
    , controllers_( controllers )
    , profile_    ( profile )
    , network_    ( network )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog destructor
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
ConnectLoginDialog::~ConnectLoginDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::NotifyUpdated
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ConnectLoginDialog::NotifyUpdated( const AvailableProfile& profile )
{
    profiles_.push_back( profile );
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::showEvent
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ConnectLoginDialog::showEvent( QShowEvent* /*ev*/ )
{
    users_->clear();
    for( T_Profiles::const_iterator it = profiles_.begin(); it != profiles_.end(); ++it )
        new UserItem( users_, *it );
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::OnAccept
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ConnectLoginDialog::OnAccept()
{
    if( widget_->isShown() && password_->text().isEmpty() )
        return;
    if( UserItem* item = static_cast< UserItem* >( users_->currentItem() ) )
    {
        profiles_.clear();
        profile_.Login( item->Login().ascii(), password_->text().ascii() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::OnReject
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ConnectLoginDialog::OnReject()
{
    profiles_.clear();
    network_.Disconnect();
    reject();
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::OnSelectItem
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ConnectLoginDialog::OnSelectItem( Q3IconViewItem* item )
{
    UserItem* user = static_cast< UserItem* >( item );
    widget_->setShown( user->RequiresPassword() );
    password_->clear();
}
