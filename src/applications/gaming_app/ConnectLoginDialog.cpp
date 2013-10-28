//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ConnectLoginDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "gaming/Network.h"
#include "gaming/Profile.h"
#include "tools/ExerciseConfig.h"

namespace
{
    class UserItem : public QListWidgetItem
                   , private boost::noncopyable
    {
    public:
        UserItem( QListWidget* parent, const AvailableProfile& profile )
            : QListWidgetItem( parent )
            , profile_( profile )
        {
            setText( profile_.GetLogin().isEmpty() ? tools::translate( "LoginDialog", "Anonymous" ) : profile_.GetLogin() );
            const QString pixmap = QString( "images/gaming/profile/%1%2.png" ).arg( profile_.IsSupervision() ? "supervisor" : "standard" )
                .arg( profile_.IsPasswordProtected() ? "_password" : "" );
            gui::Image img( tools::ExerciseConfig::BuildResourceChildFile( tools::Path::FromUnicode( pixmap.toStdWString() ) ) );
            setIcon( QPixmap::fromImage( img.scaled( 30, 30 ) ) );
        }

        bool RequiresPassword() const { return profile_.IsPasswordProtected(); }
        QString Login() const { return profile_.GetLogin(); }

    private:
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
    profiles_.insert( profile );
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
    UserItem* item = static_cast< UserItem* >( users_->currentItem() );
    if( !item && users_->count() > 0 )
        item = static_cast< UserItem* >( users_->item( 0 ) );
    if( item )
    {
        profiles_.clear();
        profile_.Login( item->Login().toStdString(), password_->text().toStdString() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::reject
// Created: LGY 2013-08-26
// -----------------------------------------------------------------------------
void ConnectLoginDialog::reject()
{
    profiles_.clear();
    network_.Disconnect();
    LoginDialog::reject();
}

// -----------------------------------------------------------------------------
// Name: ConnectLoginDialog::OnSelectItem
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ConnectLoginDialog::OnSelectItem()
{
    UserItem* user = static_cast< UserItem* >( users_->currentItem() );
    if( user )
    {
        widget_->setVisible( user->RequiresPassword() );
        password_->clear();
    }
}
