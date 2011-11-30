//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReconnectLoginDialog.h"
#include "gaming/Network.h"
#include "gaming/UserProfile.h"
#include "clients_kernel/Tools.h"
#include "gaming/AgentServerMsgMgr.h"
#include "tools/ExerciseConfig.h"

namespace
{
    class UserItem : public Q3IconViewItem
    {
    public:
        UserItem( Q3IconView* parent, const UserProfile& profile )
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

        const UserProfile& profile_;
    };
}

// -----------------------------------------------------------------------------
// Name: ReconnectLoginDialog constructor
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
ReconnectLoginDialog::ReconnectLoginDialog( QWidget* pParent, const UserProfile& profile, Network& network )
    : LoginDialog( pParent )
    , profile_   ( profile )
    , network_   ( network )
{
    new UserItem( users_, profile );
    Q3IconViewItem* item = users_->firstItem();
    if( item )
        users_->setCurrentItem( item );
    password_->setFocus( Qt::MouseFocusReason );
}

// -----------------------------------------------------------------------------
// Name: ReconnectLoginDialog destructor
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
ReconnectLoginDialog::~ReconnectLoginDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReconnectLoginDialog::OnAccept
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ReconnectLoginDialog::OnAccept()
{
    if( widget_->isShown() && password_->text().isEmpty() )
        return;
    if( UserItem* item = static_cast< UserItem* >( users_->currentItem() ) )
    {
        network_.GetMessageMgr().Reconnect( item->Login().ascii(), password_->text().ascii() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: ReconnectLoginDialog::OnSelectItem
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ReconnectLoginDialog::OnSelectItem( Q3IconViewItem* item )
{
    UserItem* user = static_cast< UserItem* >( item );
    widget_->setShown( user->RequiresPassword() );
    password_->clear();
}
