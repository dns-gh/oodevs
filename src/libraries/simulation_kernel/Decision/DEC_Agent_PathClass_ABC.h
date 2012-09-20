// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Agent_PathClass_ABC_h_
#define __DEC_Agent_PathClass_ABC_h_

// =============================================================================
/** @class  DEC_Agent_PathClass_ABC
    @brief  DEC Agent path class base
*/
// Created: MCO 2012-09-20
// =============================================================================
class DEC_Agent_PathClass_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Agent_PathClass_ABC() {}
    virtual ~DEC_Agent_PathClass_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual double GetPopulationSecurityRange() const = 0;
    virtual double GetCostOutsideOfPopulation() const = 0;
    virtual double GetPopulationAttitudeCost( unsigned int attitudeID ) const = 0;
    //@}
};

#endif // __DEC_Agent_PathClass_ABC_h_
