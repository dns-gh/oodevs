// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PopulationFlowVisitor_ABC_h_
#define __TER_PopulationFlowVisitor_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  TER_PopulationFlowVisitor_ABC
    @brief  TER PopulationFlow Visitor
*/
// Created: MMC 2011-06-15
// =============================================================================
class TER_PopulationFlowVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PopulationFlowVisitor_ABC() {}
    virtual ~TER_PopulationFlowVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( TER_PopulationFlow_ABC& object ) = 0;
    //@}
};

#endif // __TER_PopulationFlowVisitor_ABC_h_
