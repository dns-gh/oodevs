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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DisconnectDialog.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: MOS_DisconnectDialog.h $
//
//*****************************************************************************

#ifndef __MOS_DisconnectDialog_h_
#define __MOS_DisconnectDialog_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"


//=============================================================================
/**
*   @class  MOS_DisconnectDialog
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created:  NLD 2002-01-03 
//=============================================================================
class MOS_DisconnectDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_DisconnectDialog );

public:
     MOS_DisconnectDialog( QWidget* pParent = 0 );
    ~MOS_DisconnectDialog();

private slots:
    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Reject();
    //@}
};


#ifdef USE_INLINE
#   include "MOS_DisconnectDialog.inl"
#endif

#endif // __MOS_DisconnectDialog_h_
