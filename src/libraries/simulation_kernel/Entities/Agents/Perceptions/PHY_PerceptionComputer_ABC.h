// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_PerceptionComputer_ABC_h_
#define __PHY_PerceptionComputer_ABC_h_

#include <boost/noncopyable.hpp>

class MIL_Agent_ABC;
class PHY_PerceptionLevel;

// =============================================================================
/** @class  PHY_PerceptionComputer_ABC
    @brief  PHY_PerceptionComputer_ABC
*/
// Created: SLG 2010-04-29
// =============================================================================
class PHY_PerceptionComputer_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_PerceptionComputer_ABC( const MIL_Agent_ABC& perceiver )
        : perceiver_( perceiver )
    {
        // NOTHING
    }
    virtual ~PHY_PerceptionComputer_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& target ) const = 0;
    //@}

protected:
    //! @name Member data
    //@{
    const MIL_Agent_ABC& perceiver_;
    //@}
};

#endif // __PHY_PerceptionComputer_ABC_h_
