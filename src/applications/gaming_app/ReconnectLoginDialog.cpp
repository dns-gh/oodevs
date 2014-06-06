//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ReconnectLoginDialog.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/UserProfile_ABC.h"
#include "gaming/Simulation.h"
#include "tools/ExerciseConfig.h"

namespace
{
    class UserItem : public QListWidgetItem
                   , private boost::noncopyable
    {
    public:
        UserItem( QListWidget* parent, const kernel::UserProfile_ABC& profile )
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
        const kernel::UserProfile_ABC& profile_;
    };
}

// -----------------------------------------------------------------------------
// Name: ReconnectLoginDialog constructor
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
ReconnectLoginDialog::ReconnectLoginDialog( QWidget* pParent, const kernel::UserProfile_ABC& profile,
                                            kernel::Controller& controller )
    : LoginDialog( pParent )
    , profile_( profile )
    , controller_( controller )
{
    new UserItem( users_, profile );
    if( users_->count() > 0 )
        users_->setCurrentRow( 0, QItemSelectionModel::ClearAndSelect );
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
    UserItem* item = static_cast< UserItem* >( users_->currentItem() );
    if( !item && users_->count() > 0 )
        item = static_cast< UserItem* >( users_->item( 0 ) );
    if( item )
    {
        controller_.Update( Simulation::Reconnection( item->Login().toStdString(), password_->text().toStdString() ) );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: ReconnectLoginDialog::OnSelectItem
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ReconnectLoginDialog::OnSelectItem()
{
    if( UserItem* user = static_cast< UserItem* >(  users_->currentItem() ) )
    {
        widget_->setVisible( user->RequiresPassword() );
        password_->clear();
    }
}
