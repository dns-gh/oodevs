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

#include "astec_gaming/Types.h"

class Network;

//=============================================================================
/**
*   @class  DisconnectDialog
*   @brief  Disconnect dialog
*/
// Created:  NLD 2002-01-03 
//=============================================================================
class DisconnectDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             DisconnectDialog( QWidget* pParent, Network& network );
    virtual ~DisconnectDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisconnectDialog( const DisconnectDialog& );
    DisconnectDialog& operator=( const DisconnectDialog& );
    //@}

private:
    //! @name Member data
    //@{
    Network& network_;
    //@}
};


#ifdef USE_INLINE
#   include "DisconnectDialog.inl"
#endif

#endif // __DisconnectDialog_h_
