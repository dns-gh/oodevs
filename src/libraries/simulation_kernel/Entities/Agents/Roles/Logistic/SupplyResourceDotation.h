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
#include <boost/serialization/export.hpp>

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
             SupplyResourceDotation();
    virtual ~SupplyResourceDotation();
    //@}

    //! @name Operations
    //@{
    virtual double Supply( double quantity );
    virtual bool   HasReachedLowThreshold() const;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_DotationCategory& GetCategory() const;
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

private:
    PHY_Dotation* dotation_;
};

} // end namespace logistic

BOOST_CLASS_EXPORT_KEY( logistic::SupplyResourceDotation )

#endif // __SupplyResourceDotation_h_
