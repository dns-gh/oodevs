// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConsumptionComputer_ABC_h_
#define __ConsumptionComputer_ABC_h_

class PHY_ConsumptionType;

namespace dotation
{

// =============================================================================
/** @class  ConsumptionComputer_ABC
    @brief  ConsumptionComputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class ConsumptionComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConsumptionComputer_ABC();
    virtual ~ConsumptionComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset() = 0;
    virtual bool SetConsumptionMode( const PHY_ConsumptionType& mode ) = 0;
    virtual const PHY_ConsumptionType& Result() const = 0;
    //@}
};

} // namespace firing

#endif // __ConsumptionComputer_ABC_h_
