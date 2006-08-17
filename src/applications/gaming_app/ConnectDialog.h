//****************************************************************************
//
// This file is part of MASA DIN library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************

#ifndef __ConnectDialog_h_
#define __ConnectDialog_h_

#include "astec_gui/Types.h"

class QComboBox;
class QSpinBox;
class QCheckBox;
class QLabel;
class Network;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             ConnectDialog( QWidget* pParent, Network& network );
    virtual ~ConnectDialog();
    //@}

private slots:
    //! @name Main methods
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConnectDialog( const ConnectDialog& );
    ConnectDialog& operator=( const ConnectDialog& );
    //@}

private:
    //! @name Config management
    //@{
    void LoadDefaultConfig();
    //@}

private:
    //! @name Member data
    //@{
    Network&     network_;
    QComboBox*   pHostNameComboBox_;
    QSpinBox*    pPortSpinBox_;
    //@}
};

#endif // __ConnectDialog_h_
