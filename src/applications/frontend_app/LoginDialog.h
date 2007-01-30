// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LoginDialog_h_
#define __LoginDialog_h_

namespace frontend
{
    class Profile;
    class Publisher_ABC;
}

// =============================================================================
/** @class  LoginDialog
    @brief  LoginDialog
*/
// Created: SBO 2007-01-29
// =============================================================================
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LoginDialog( QWidget* parent, const frontend::Profile& profile, frontend::Publisher_ABC& publisher );
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
    LoginDialog& operator=( const LoginDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const frontend::Profile& profile_;
    frontend::Publisher_ABC& publisher_;
    QLineEdit* login_;
    QLineEdit* password_;
    //@}
};

#endif // __LoginDialog_h_
