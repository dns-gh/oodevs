// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageHandler_ABC_h_
#define __MessageHandler_ABC_h_

#include "game_asn/Simulation.h"
#include "game_asn/Aar.h"

namespace dispatcher
{

// =============================================================================
/** @class  MessageHandler_ABC
    @brief  Simulation message handler definition
    // $$$$ SBO 2009-04-29: messages which can be handled by dispatcher plugins
*/
// Created: AGE 2007-07-05
// =============================================================================
class MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MessageHandler_ABC() {};
    virtual ~MessageHandler_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message ) = 0;
    virtual void Receive( const ASN1T_MsgsAarToClient& /*message*/ ) {}
    //@}
};

}

#endif // __MessageHandler_ABC_h_
