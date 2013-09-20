// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PerceptionDistanceComputer_h_
#define __PerceptionDistanceComputer_h_

#include <boost/noncopyable.hpp>

namespace detection
{

// =============================================================================
/** @class  PerceptionDistanceComputer
    @brief  PerceptionDistanceComputer
*/
// Created: MGD 2009-10-05
// =============================================================================
class PerceptionDistanceComputer: boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionDistanceComputer() : factor_( 1 ) {}
    virtual ~PerceptionDistanceComputer() {}
    //@}

    //! @name Operations
    //@{
    void AddModifier( double modifier )
    {
        if( modifier != 0 )
            factor_ *= modifier;
    }

    double GetFactor() const
    {
        return factor_;
    }
    //@}

private:
    //! @name Member data
    //@{
    double factor_;
    //@}
};

} // namespace detection

#endif // __PerceptionDistanceComputer_h_
