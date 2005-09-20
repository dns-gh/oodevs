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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ConnectDialog.h $
// $Author: Ape $
// $Modtime: 21/09/04 15:56 $
// $Revision: 3 $
// $Workfile: MOS_ConnectDialog.h $
//
//*****************************************************************************

#ifndef __MOS_ConnectDialog_h_
#define __MOS_ConnectDialog_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"

class QComboBox;
class QSpinBox;
class QCheckBox;
class QLabel;

//=============================================================================
/**
*   @class  MOS_ConnectDialog
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_ConnectDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ConnectDialog );

public:
     MOS_ConnectDialog( QWidget* pParent = 0 );
    ~MOS_ConnectDialog();

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Config management */
    //-------------------------------------------------------------------------
    //@{
    void LoadDefaultConfig();
    //@}

private:
    QComboBox*   pHostNameComboBox_;
    QSpinBox*    pPortSpinBox_;
};

#   include "MOS_ConnectDialog.inl"

#endif // __MOS_ConnectDialog_h_
