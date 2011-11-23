// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConnectLoginDialog_h_
#define __ConnectLoginDialog_h_

#include "LoginDialog.h"
#include "gaming/AvailableProfile.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class Profile;
class Network;

// =============================================================================
/** @class  ConnectLoginDialog
    @brief  Connect login dialog
*/
// Created: LGY 2011-11-23
// =============================================================================
class ConnectLoginDialog : public LoginDialog
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< AvailableProfile >
{
public:
    //! @name Constructors/Destructor
    //@{
             ConnectLoginDialog( QWidget* pParent, const Profile& profile, Network& network,
                                 kernel::Controllers& controllers );
    virtual ~ConnectLoginDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const AvailableProfile& profile );
    //@}

private:
    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* ev );
    virtual void OnAccept();
    virtual void OnReject();
    virtual void OnSelectItem( Q3IconViewItem* item );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< AvailableProfile > T_Profiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Profile& profile_;
    Network& network_;
    T_Profiles profiles_;
    //@}
};

#endif // __ConnectLoginDialog_h_
