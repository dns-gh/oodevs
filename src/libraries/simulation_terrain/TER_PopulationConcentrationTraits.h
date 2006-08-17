// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationConcentrationTraits_h_
#define __TER_PopulationConcentrationTraits_h_

class TER_PopulationConcentration_ABC;

// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PopulationConcentrationTraits
{
public:
    //! @name Types
    //@{
    typedef TER_PopulationConcentration_ABC* T_Value;
    //@}
public:
    //! @name Operations
    //@{
    int CompareOnX( MT_Float rValue, const T_Value& pConcentration ) const;
    int CompareOnY( MT_Float rValue, const T_Value& pConcentration ) const;
    //@}
};

#endif // __TER_PopulationConcentrationTraits_h_
