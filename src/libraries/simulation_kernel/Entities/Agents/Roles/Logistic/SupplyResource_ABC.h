// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyResource_ABC_h_
#define __SupplyResource_ABC_h_

class PHY_DotationCategory;

namespace logistic {

// =============================================================================
/** @class  SupplyResource_ABC
    @brief  SupplyResource_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyResource_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyResource_ABC() {}
    virtual ~SupplyResource_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double Supply                   ( double quantity ) = 0;
    virtual bool   HasReachedLowThreshold() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_DotationCategory& GetCategory() const = 0;
    //@}
        
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

} // end namespace logistic

#endif // __SupplyRequestContainer_ABC_h_
