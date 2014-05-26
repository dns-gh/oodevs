 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotation_h_
#define __Dotation_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class DotationStock;
    class ResourceDotations_ResourceDotation;
}

namespace dispatcher
{

// =============================================================================
/** @class  Dotation
    @brief  Dotation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Dotation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Dotation( const sword::ResourceDotations_ResourceDotation& asnMsg );
    explicit Dotation( const sword::DotationStock& asnMsg );
    virtual ~Dotation();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::ResourceDotations_ResourceDotation& asnMsg );
    void Update( const sword::DotationStock& asnMsg );
    void Send( sword::ResourceDotations_ResourceDotation& asnMsg ) const ;
    void Send( sword::DotationStock& asnMsg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
    unsigned int       nNbr_;
    float              rLowThresholdPercentage_;
    float              rHighThresholdPercentage_;
};

}

#endif // __Dotation_h_
