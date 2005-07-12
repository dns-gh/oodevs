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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RequestAttributeDialog.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_RequestAttributeDialog.h $
//
//*****************************************************************************

#ifndef __MOS_RequestAttributeDialog_h_
#define __MOS_RequestAttributeDialog_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_AgentDialog_ABC.h"

class MOS_Agent;

class QListBox;
class QComboBox;
class QPopupMenu;
class QListBoxItem;
class QCheckBox;

//=============================================================================
/**
*   @class  MOS_RequestAttributeDialog
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_RequestAttributeDialog : public MOS_AgentDialog_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_RequestAttributeDialog );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< QCheckBox* >   T_CheckBoxVector;
    typedef T_CheckBoxVector::iterator  IT_CheckBoxVector;
    //@}

public:
     MOS_RequestAttributeDialog( QWidget* pParent = 0 );
    ~MOS_RequestAttributeDialog();

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void SlotValidate();
    void SlotCancel  ();
    //@}

private:
    T_CheckBoxVector attributeCheckBoxVector_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_RequestAttributeDialog.inl"
#endif

#endif // __MOS_RequestAttributeDialog_h_
