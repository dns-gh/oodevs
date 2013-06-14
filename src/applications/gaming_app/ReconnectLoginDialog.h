// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReconnectLoginDialog_h_
#define __ReconnectLoginDialog_h_

#include "LoginDialog.h"

class Network;
class ProfileFilter;
class UserProfile;

// =============================================================================
/** @class  ReconnectLoginDialog
    @brief  Reconnect login dialog
*/
// Created: LGY 2011-11-23
// =============================================================================
class ReconnectLoginDialog : public LoginDialog
{
public:
    //! @name Constructors/Destructor
    //@{
             ReconnectLoginDialog( QWidget* pParent, const UserProfile& profile, ProfileFilter& filter, Network& network );
    virtual ~ReconnectLoginDialog();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnAccept();
    virtual void OnSelectItem();
    //@}

private:
    //! @name Member data
    //@{
    const UserProfile& profile_;
    ProfileFilter& filter_;
    Network& network_;
    //@}
};

#endif // __ReconnectLoginDialog_h_
