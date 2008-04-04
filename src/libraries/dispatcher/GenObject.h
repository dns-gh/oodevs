// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GenObject_h_
#define __GenObject_h_

#include "game_asn/Simulation.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  GenObject
    @brief  GenObject
*/
// Created: NLD 2007-04-23
// =============================================================================
class GenObject
{

public:
    //! @name Constructors/Destructor
    //@{
    GenObject( const ASN1T_PlannedWork& asn );
    GenObject();
   ~GenObject();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_PlannedWork& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_EnumObjectType      type_;
    Localisation              location_;
    ASN1T_EnumObstacleType    typeObstacle_;
    double                    density_;
    unsigned int              tc2_;
    unsigned int              delaiActiviteMines_;
    //@}
};

}

#endif // __GenObject_h_
