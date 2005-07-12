// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-02-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_ASN_MsgsMosSimWithContextController.h $
// $Author: Age $
// $Modtime: 5/02/04 17:25 $
// $Revision: 2 $
// $Workfile: NET_ASN_MsgsMosSimWithContextController.h $
//
// *****************************************************************************

#ifndef __NET_ASN_MsgsMosSimWithContextController_h_
#define __NET_ASN_MsgsMosSimWithContextController_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "NET_ASN_MessageController.h"
#include "NET_ASN_Messages.h"

// =============================================================================
/** @class  NET_ASN_MsgsMosSimWithContextController
    @brief  Specializes the template controller class to add context management.
*/
// Created: AGE 2004-02-03
// =============================================================================
class NET_ASN_MsgsMosSimWithContextController : public NET_ASN_MessageController< ASN1T_MsgsMosSimWithContext, ASN1C_MsgsMosSimWithContext >
{
    MT_POOLING_MEMBERS( NET_ASN_MsgsMosSimWithContextController );
public:
    //! @name Constructors/Destructor
    //@{
             NET_ASN_MsgsMosSimWithContextController( uint8* pBuffer, unsigned long nBufferSize );
    virtual ~NET_ASN_MsgsMosSimWithContextController();
    //@}
    int  Decode( MIL_MOSContextID nCtx );
    MIL_MOSContextID GetContext() const;
private:
    MIL_MOSContextID nCtx_;
};

#include "NET_ASN_MsgsMosSimWithContextController.inl"

#endif // __NET_ASN_MsgsMosSimWithContextController_h_
