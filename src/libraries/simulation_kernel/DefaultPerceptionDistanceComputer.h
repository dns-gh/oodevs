// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultPerceptionDistanceComputer_h_
#define __DefaultPerceptionDistanceComputer_h_

#include "simulation_kernel/PerceptionDistanceComputer_ABC.h"

namespace detection
{

// =============================================================================
/** @class  DefaultPerceptionDistanceComputer
    @brief  DefaultPerceptionDistanceComputer
*/
// Created: MGD 2009-10-05
// =============================================================================
class DefaultPerceptionDistanceComputer : public PerceptionDistanceComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultPerceptionDistanceComputer();
    virtual ~DefaultPerceptionDistanceComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual void AddModifier( double modifier );
    virtual double GetFactor() const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< double > modifiers_;
    //@}
};

} // namespace detection

#endif // __DefaultPerceptionDistanceComputer_h_
