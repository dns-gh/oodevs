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

#include <boost/noncopyable.hpp>

class PHY_DotationGroupContainer;
class PHY_DotationCategory;

namespace dotation
{

// =============================================================================
/** @class  DotationComputer_ABC
    @brief  DotationComputer_ABC
*/
// Created: MGD 2009-09-30
// =============================================================================
class DotationComputer_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DotationComputer_ABC() {}
    virtual ~DotationComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void SetDotationContainer( PHY_DotationGroupContainer& container ) = 0;
    virtual void SetForbiddenAmmunition( const std::vector< const PHY_DotationCategory* >* container ) = 0;

    virtual bool HasDotation( const PHY_DotationCategory& category ) const = 0;
    virtual double GetDotationValue( const PHY_DotationCategory& category ) const = 0;
    virtual double GetDotationCapacity( const PHY_DotationCategory& category ) const = 0;
    //@}
};

}

#endif // __DotationComputer_ABC_h_
