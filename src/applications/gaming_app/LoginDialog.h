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

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  LoginDialog
    @brief  Login dialog
*/
// Created: AGE 2006-10-11
// =============================================================================
class LoginDialog : public QDialog
                  , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit LoginDialog( QWidget* pParent );
    virtual ~LoginDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnAccept();
    virtual void OnReject();
    virtual void OnSelectItem( Q3IconViewItem* item );
    //@}

protected:
    //! @name Member data
    //@{
    Q3IconView* users_;
    QWidget* widget_;
    QLineEdit* password_;
    //@}
};

#endif // __LoginDialog_h_
