// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NET_Publisher_ABC_h_
#define __NET_Publisher_ABC_h_

#include "game_asn/Simulation.h"

// =============================================================================
/** @class  NET_Publisher_ABC
    @brief  Publisher definition
*/
// Created: AGE 2007-08-10
// =============================================================================
class NET_Publisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NET_Publisher_ABC() {};
    virtual ~NET_Publisher_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( ASN1T_MsgsSimToClient& ) = 0;
    //@}
};

#endif // __NET_Publisher_ABC_h_
