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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_UpdateAttributeDialog.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_UpdateAttributeDialog.h $
//
//*****************************************************************************

#ifndef __MOS_UpdateAttributeDialog_h_
#define __MOS_UpdateAttributeDialog_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_AgentDialog_ABC.h"

class MOS_App;
class MOS_Agent;

class QComboBox;
class QSpinBox;
class QWidgetStack;

//=============================================================================
/**
*   @class  MOS_UpdateAttributeDialog
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_UpdateAttributeDialog : public MOS_AgentDialog_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_UpdateAttributeDialog );

public:
     MOS_UpdateAttributeDialog( QWidget* pParent = 0 );
    ~MOS_UpdateAttributeDialog();

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void SlotValidate       ();
    void SlotCancel         ();
    void SlotAttributeChange( int nIndex );
    //@}

private:
    QComboBox* pAttributesComboBox_;

    QWidgetStack* pAttributeValueWidgetStack_;
    QComboBox* pAttributeValuePosition_;
    QSpinBox*  pAttributeValueCarburant_;
    QSpinBox*  pAttributeValueMunition_;
    QSpinBox*  pAttributeValueCapacityCommunication_;
    QSpinBox*  pAttributeValueCapacityFire_;
    QSpinBox*  pAttributeValueCapacityDetection_;
    QSpinBox*  pAttributeValueCapacityProtection_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_UpdateAttributeDialog.inl"
#endif

#endif // __MOS_UpdateAttributeDialog_h_
