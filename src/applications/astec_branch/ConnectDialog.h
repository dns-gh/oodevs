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
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ConnectDialog.h $
// $Author: Ape $
// $Modtime: 21/09/04 15:56 $
// $Revision: 3 $
// $Workfile: ConnectDialog.h $
//
//*****************************************************************************

#ifndef __ConnectDialog_h_
#define __ConnectDialog_h_

#include "Types.h"

class QComboBox;
class QSpinBox;
class QCheckBox;
class QLabel;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
    explicit ConnectDialog( QWidget* pParent = 0 );
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
    QComboBox*   pHostNameComboBox_;
    QSpinBox*    pPortSpinBox_;
    //@}
};

#endif // __ConnectDialog_h_
