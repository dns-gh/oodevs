// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DefaultDotationComputer_h_
#define __DefaultDotationComputer_h_

#include "simulation_kernel/DotationComputer_ABC.h"

namespace dotation
{

// =============================================================================
/** @class  DefaultDotationComputer
    @brief  DefaultDotationComputer
*/
// Created: MGD 2009-09-15
// =============================================================================
class DefaultDotationComputer : public DotationComputer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DefaultDotationComputer();
    virtual ~DefaultDotationComputer();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual void SetDotationContainer( PHY_DotationGroupContainer& container );

    virtual bool HasDotation( const PHY_DotationCategory& category ) const;
    virtual double GetDotationValue( const PHY_DotationCategory& category ) const;
    virtual double GetDotationCapacity( const PHY_DotationCategory& category ) const;
    //@}

private:
    //! @name Attribute
    //@{
    PHY_DotationGroupContainer* pDotationContainer_;
    //@}

};

} // namespace firing

#endif // __DefaultDotationComputer_h_
