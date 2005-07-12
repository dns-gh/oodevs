//****************************************************************************
//
// This file is part of MASA DIN library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Math�matiques Appliqu�es SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ConnectDialog.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 8 $
// $Workfile: MOS_ConnectDialog.h $
//
//*****************************************************************************

#ifndef __MOS_ConnectDialog_h_
#define __MOS_ConnectDialog_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"
#include <QDialog.h>

class QComboBox;
class QSpinBox;
class QCheckBox;
class MOS_Value;
class QLineEdit;
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

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void SlotSetModeTest( bool bTest );
    void SlotSetModeReplay( bool bReplay );
    void SlotSetModeRec( bool bRec );
    void SlotSelectOutDir();
    void SlotOutDirChanged( const QString& strName );

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
    bool         bTest_;
    bool         bReplay_;
    bool         bRec_;
    QCheckBox*   pCheckBoxTest_;
    QCheckBox*   pCheckBoxRep_;
    QCheckBox*   pCheckBoxRec_;
    MOS_Value*   pCheckBoxValue_;
    QLineEdit*   pNameOut_;
    QLabel*      pLabelOut_;
    QPushButton* pButtonOutDir_;

};

#ifdef MOS_USE_INLINE
#   include "MOS_ConnectDialog.inl"
#endif

#endif // __MOS_ConnectDialog_h_
