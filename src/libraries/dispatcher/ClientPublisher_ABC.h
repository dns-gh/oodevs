// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientPublisher_ABC_h_
#define __ClientPublisher_ABC_h_

#include "game_asn/Asn.h"

namespace dispatcher
{

// =============================================================================
/** @class  ClientPublisher_ABC
    @brief  ClientPublisher_ABC
*/
// Created: AGE 2007-08-24
// =============================================================================
class ClientPublisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ClientPublisher_ABC() {};
    virtual ~ClientPublisher_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsSimToClient&            msg ) = 0;
    virtual void Send( const ASN1T_MsgsAuthenticationToClient& msg ) = 0;
    virtual void Send( const ASN1T_MsgsReplayToClient&         msg ) = 0;
    //@}
};

}

#endif // __ClientPublisher_ABC_h_
