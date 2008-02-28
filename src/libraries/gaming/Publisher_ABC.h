// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Publisher_ABC_h_
#define __Publisher_ABC_h_

#include "game_asn/Simulation.h"
#include "game_asn/Authentication.h"
#include "game_asn/Replay.h"
#include "game_asn/Aar.h"

// =============================================================================
/** @class  Publisher_ABC
    @brief  Publisher abstract base class
*/
// Created: SBO 2006-07-06
// =============================================================================
class Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Publisher_ABC() {}
    virtual ~Publisher_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( ASN1T_MsgsClientToSim& message ) = 0;
    virtual void Send( ASN1T_MsgsClientToAuthentication& message ) = 0;
    virtual void Send( ASN1T_MsgsClientToReplay& message ) = 0;
    virtual void Send( ASN1T_MsgsClientToAar& message ) = 0;
    //@}
};

#endif // __Publisher_ABC_h_
