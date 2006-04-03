// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationFlowTraits_h_
#define __TER_PopulationFlowTraits_h_

class TER_PopulationFlow_ABC;

// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PopulationFlowTraits
{
public:
    //! @name Types
    //@{
    typedef TER_PopulationFlow_ABC* T_Value;
    //@}
public:
    //! @name Operations
    //@{
    int CompareOnX( MT_Float rValue, const T_Value& pFlow ) const;
    int CompareOnY( MT_Float rValue, const T_Value& pFlow ) const;
    //@}
};

#endif // __TER_PopulationFlowTraits_h_
