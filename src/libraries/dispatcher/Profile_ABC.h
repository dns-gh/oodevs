// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_ABC_h_
#define __Profile_ABC_h_

#include "game_asn/Simulation.h"
#include "game_asn/Authentication.h"
#include "game_asn/Replay.h"

namespace dispatcher
{

// =============================================================================
/** @class  Profile_ABC
    @brief  Profile definition
*/
// Created: AGE 2007-08-24
// =============================================================================
class Profile_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Profile_ABC() {};
    virtual ~Profile_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool CheckRights( const ASN1T_MsgsClientToSim& msg ) const = 0;
    virtual bool CheckRights( const ASN1T_MsgsClientToAuthentication& msg ) const = 0;
    virtual bool CheckRights( const ASN1T_MsgsClientToReplay& msg ) const = 0;
    //@}
};

}

#endif // __Profile_ABC_h_
