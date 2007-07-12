// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __3A_MessageHandler_ABC_h_
#define __3A_MessageHandler_ABC_h_

#include "dispatcher/MessageHandler_ABC.h"

// =============================================================================
/** @class  MessageHandler_ABC
    @brief  Simulation message handler definition
*/
// Created: AGE 2007-07-05
// =============================================================================
class MessageHandler_ABC : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message ) = 0;
    //@}
};

#endif // __3A_MessageHandler_ABC_h_
