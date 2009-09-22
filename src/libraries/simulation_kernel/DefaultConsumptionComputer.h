// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultConsumptionComputer_h_
#define __DefaultConsumptionComputer_h_

#include "simulation_kernel/ConsumptionComputer_ABC.h"

namespace dotation
{

// =============================================================================
/** @class  DefaultConsumptionComputer
    @brief  DefaultConsumptionComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultConsumptionComputer : public ConsumptionComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultConsumptionComputer();
    virtual ~DefaultConsumptionComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual bool SetConsumptionMode( const PHY_ConsumptionType& mode );
    virtual const PHY_ConsumptionType& Result() const;
    //@}

private:
    //! @name Attribute
    //@{
    const PHY_ConsumptionType* pConsumptionType_;
    //@}

};

} // namespace firing

#endif // __DefaultConsumptionComputer_h_
