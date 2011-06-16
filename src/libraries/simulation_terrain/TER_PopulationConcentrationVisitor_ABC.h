// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationConcentrationVisitor_ABC_h_
#define __TER_PopulationConcentrationVisitor_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  TER_PopulationConcentrationVisitor_ABC
    @brief  TER PopulationConcentration Visitor
*/
// Created: MMC 2011-06-15
// =============================================================================
class TER_PopulationConcentrationVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PopulationConcentrationVisitor_ABC() {}
    virtual ~TER_PopulationConcentrationVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( TER_PopulationConcentration_ABC& object ) = 0;
    //@}
};

#endif // __TER_PopulationConcentrationVisitor_ABC_h_
