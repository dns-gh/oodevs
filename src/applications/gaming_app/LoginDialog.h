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

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
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
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LoginDialog( QWidget* pParent, const Profile& profile, Network& network, const tools::ExerciseConfig& config );
    virtual ~LoginDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnSelectItem( QIconViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LoginDialog( const LoginDialog& );            //!< Copy constructor
    LoginDialog& operator=( const LoginDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void FillUsers( const tools::ExerciseConfig& config );
    void ReadUser( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const Profile& profile_;
    Network& network_;
    QIconView* users_;
    QHBox* passwordBox_;
    QLineEdit* password_;
    //@}
};

#endif // __LoginDialog_h_
