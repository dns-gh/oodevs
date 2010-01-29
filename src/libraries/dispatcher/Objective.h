// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Objective_h_
#define __Objective_h_

#include "game_asn/Simulation.h"
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
   virtual ~Objective();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_MissionObjective& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    std::string  schedule_;
};

}

#endif // __Objective_h_
