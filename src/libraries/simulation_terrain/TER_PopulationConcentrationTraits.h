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
    virtual ~TER_PopulationConcentrationTraits() {}
    //! @name Types
    //@{
    typedef TER_PopulationConcentration_ABC* T_Value;
    //@}

    //! @name Operations
    //@{
    int CompareOnX( double rValue, const T_Value& pConcentration ) const;
    int CompareOnY( double rValue, const T_Value& pConcentration ) const;
    //@}
};

#endif // __TER_PopulationConcentrationTraits_h_
