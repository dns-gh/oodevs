// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Objective_h_
#define __Objective_h_

#include "game_asn/Asn.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  Objective
    @brief  Objective
*/
// Created: NLD 2007-04-23
// =============================================================================
class Objective
{
public:
    //! @name Constructors/Destructor
    //@{
    Objective( const ASN1T_MissionObjective& asn );
    Objective();
   ~Objective();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_MissionObjective& asn ) const;

    static void AsnDelete( ASN1T_MissionObjective& asn );
    //@}

private:
    //! @name Member data
    //@{
    Localisation  location_;
    unsigned int  schedule_;
};

}

#endif // __Objective_h_