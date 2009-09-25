// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DotationComputer_ABC_h_
#define __DotationComputer_ABC_h_

class PHY_ComposantePion;

namespace dotation
{
    class ConsumptionOperator_ABC;
// =============================================================================
/** @class  Dotationcomputer_ABC
    @brief  Dotationcomputer_ABC
*/
// Created: MGD 2009-09-15
// =============================================================================
class DotationComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationComputer_ABC();
    virtual ~DotationComputer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset( ConsumptionOperator_ABC& consumptionOperator ) = 0;
    virtual void ApplyOnComponent( const PHY_ComposantePion& component ) = 0;
    //@}
};

} // namespace dotation

#endif // __DotationComputer_ABC_h_
