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

class MIL_Agent_ABC;
class PHY_PerceptionLevel;
// =============================================================================
/** @class  PHY_PerceptionComputer_ABC
    @brief  PHY_PerceptionComputer_ABC
*/
// Created: SLG 2010-04-29
// =============================================================================
class PHY_PerceptionComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_PerceptionComputer_ABC( const MIL_Agent_ABC& perceiver ) : perceiver_( perceiver ){}
    virtual ~PHY_PerceptionComputer_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& target ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_PerceptionComputer_ABC( const PHY_PerceptionComputer_ABC& );            //!< Copy constructor
    PHY_PerceptionComputer_ABC& operator=( const PHY_PerceptionComputer_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

protected:
    //! @name Member data
    //@{
    const MIL_Agent_ABC& perceiver_;
    //@}
};

#endif // __PHY_PerceptionComputer_ABC_h_
