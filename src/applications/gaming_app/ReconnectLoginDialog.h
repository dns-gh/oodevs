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

class UserProfile;
class Network;

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
             ReconnectLoginDialog( QWidget* pParent, const UserProfile& profile, Network& network );
    virtual ~ReconnectLoginDialog();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnAccept();
    virtual void OnSelectItem( Q3IconViewItem* item );
    //@}

private:
    //! @name Member data
    //@{
    const UserProfile& profile_;
    Network& network_;
    //@}
};

#endif // __ReconnectLoginDialog_h_
