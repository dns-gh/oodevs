// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyResourceDotation_h_
#define __SupplyResourceDotation_h_

#include "SupplyResource_ABC.h"

class PHY_Dotation;

namespace logistic {

// =============================================================================
/** @class  SupplyResourceDotation
    @brief  SupplyResourceDotation
*/
// Created: NLD 2011-07-26
// =============================================================================
class SupplyResourceDotation : public SupplyResource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResourceDotation( PHY_Dotation& dotation );
    virtual ~SupplyResourceDotation();
    //@}

    //! @name Operations
    //@{
    virtual double Supply( double quantity );
    virtual bool   HasReachedSupplyThreshold() const;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_DotationCategory& GetCategory() const;
    //@}

private:
    PHY_Dotation& dotation_;
};

} // end namespace logistic

#endif // __SupplyRequestContainer_ABC_h_
