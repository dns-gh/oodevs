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
// $Archive: /MVW_v10/Build/SDK/Light2/src/DisconnectDialog.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: DisconnectDialog.h $
//
//*****************************************************************************

#ifndef __DisconnectDialog_h_
#define __DisconnectDialog_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "Types.h"


//=============================================================================
/**
*   @class  DisconnectDialog
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created:  NLD 2002-01-03 
//=============================================================================
class DisconnectDialog : public QDialog
{
    Q_OBJECT
    MT_COPYNOTALLOWED( DisconnectDialog );

public:
     DisconnectDialog( QWidget* pParent = 0 );
    ~DisconnectDialog();

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
#   include "DisconnectDialog.inl"
#endif

#endif // __DisconnectDialog_h_
