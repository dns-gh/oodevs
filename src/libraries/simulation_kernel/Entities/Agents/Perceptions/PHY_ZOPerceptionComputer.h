// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ZOPerceptionComputer_h_
#define __PHY_ZOPerceptionComputer_h_

#include "PHY_PerceptionComputer_ABC.h"
// =============================================================================
/** @class  PHY_ZOPerceptionComputer
    @brief  PHY_ZOPerceptionComputer
*/
// Created: SLG 2010-04-29
// =============================================================================
class PHY_ZOPerceptionComputer : public PHY_PerceptionComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_ZOPerceptionComputer( const MIL_Agent_ABC& perceiver );
    virtual ~PHY_ZOPerceptionComputer();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& target ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_ZOPerceptionComputer( const PHY_ZOPerceptionComputer& );            //!< Copy constructor
    PHY_ZOPerceptionComputer& operator=( const PHY_ZOPerceptionComputer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __PHY_ZOPerceptionComputer_h_
