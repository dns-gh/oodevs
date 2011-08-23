// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyResourceStock_h_
#define __SupplyResourceStock_h_

#include "SupplyResource_ABC.h"

class PHY_DotationStock;

namespace logistic {

// =============================================================================
/** @class  SupplyResourceStock
    @brief  SupplyResourceStock
*/
// Created: NLD 2011-07-26
// =============================================================================
class SupplyResourceStock : public SupplyResource_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResourceStock( PHY_DotationStock& Stock );
    virtual ~SupplyResourceStock();
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
    PHY_DotationStock& stock_;
};

} // end namespace logistic

#endif // __SupplyRequestContainer_ABC_h_
