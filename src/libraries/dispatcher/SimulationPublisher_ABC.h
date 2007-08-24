// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimulationPublisher_ABC_h_
#define __SimulationPublisher_ABC_h_

#include "game_asn/Asn.h"

namespace dispatcher
{

// =============================================================================
/** @class  SimulationPublisher_ABC
    @brief  SimulationPublisher_ABC
*/
// Created: AGE 2007-08-24
// =============================================================================
class SimulationPublisher_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SimulationPublisher_ABC();
    virtual ~SimulationPublisher_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const ASN1T_MsgsClientToSim& msg );
    virtual void Send( const ASN1T_MsgsMiddleToSim& msg );
    //@}
};

}

#endif // __SimulationPublisher_ABC_h_
