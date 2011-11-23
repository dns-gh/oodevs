// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LoginDialog_h_
#define __LoginDialog_h_

#include "gaming/AvailableProfile.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class Profile;
class Network;

// =============================================================================
/** @class  LoginDialog
    @brief  LoginDialog
*/
// Created: AGE 2006-10-11
// =============================================================================
class LoginDialog : public QDialog
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< AvailableProfile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LoginDialog( QWidget* pParent, const Profile& profile, Network& network, kernel::Controllers& controllers );
    virtual ~LoginDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const AvailableProfile& profile );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnSelectItem( Q3IconViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoginDialog( const LoginDialog& );            //!< Copy constructor
    LoginDialog& operator=( const LoginDialog& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* ev );
    //@}

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
    Q3IconView* users_;
    QWidget* widget_;
    QLineEdit* password_;
    T_Profiles profiles_;
    //@}
};

#endif // __LoginDialog_h_
