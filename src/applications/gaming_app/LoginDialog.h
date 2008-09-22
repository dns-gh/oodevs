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

class Profile;
class Network;

// =============================================================================
/** @class  LoginDialog
    @brief  LoginDialog
*/
// Created: AGE 2006-10-11
// =============================================================================
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LoginDialog( QWidget* pParent, const Profile& profile, Network& network );
    virtual ~LoginDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoginDialog( const LoginDialog& );            //!< Copy constructor
    LoginDialog& operator=( const LoginDialog& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Profile& profile_;
    Network& network_;
    QLineEdit* login_;
    QLineEdit* password_;
    //@}
};

#endif // __LoginDialog_h_
