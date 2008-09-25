// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-10-13 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TimeInterval.cpp $
// $Author: Age $
// $Modtime: 17/11/04 11:28 $
// $Revision: 2 $
// $Workfile: HLA_TimeInterval.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_TimeInterval.h"
#include <hla/Serializer.h>
#include <sstream>

using namespace hla;

const double HLA_TimeInterval::rEpsilon_ = 0.000000001;

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval constructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval::HLA_TimeInterval()
    : rInterval_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval constructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval::HLA_TimeInterval( double rInterval )
    : rInterval_( rInterval )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval destructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval::~HLA_TimeInterval()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::Clone
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval& HLA_TimeInterval::Clone() const
{
    return *new HLA_TimeInterval( rInterval_ );
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsZero
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsZero() const
{
    return rInterval_ == 0.;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::SetZero
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval& HLA_TimeInterval::SetZero()
{
    rInterval_ = 0;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsEpsilon
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsEpsilon() const
{
    return rInterval_ == rEpsilon_;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::SetEpsilon
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval& HLA_TimeInterval::SetEpsilon()
{
    rInterval_ = rEpsilon_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::Set
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_TimeInterval& HLA_TimeInterval::Set( TimeInterval_ABC const & rhs )
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        rInterval_ = pRhs->rInterval_;
    else
        throw __FUNCTION__;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IncreaseBy
// Created: AGE 2004-11-17
// -----------------------------------------------------------------------------
HLA_TimeInterval& HLA_TimeInterval::IncreaseBy( TimeInterval_ABC const & rhs )
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        rInterval_ += pRhs->rInterval_;
    else
        throw __FUNCTION__;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::DecreaseBy
// Created: AGE 2004-11-17
// -----------------------------------------------------------------------------
HLA_TimeInterval& HLA_TimeInterval::DecreaseBy( TimeInterval_ABC const & rhs )
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        rInterval_ -= pRhs->rInterval_;
    else
        throw __FUNCTION__;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsGreaterThan
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsGreaterThan( TimeInterval_ABC const & rhs ) const
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        return rInterval_ > pRhs->rInterval_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsLessThan
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsLessThan( TimeInterval_ABC const & rhs ) const
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        return rInterval_ < pRhs->rInterval_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsEqualTo
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsEqualTo( TimeInterval_ABC const & rhs ) const
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        return rInterval_ == pRhs->rInterval_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsGreaterThanOrEqualTo
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsGreaterThanOrEqualTo( TimeInterval_ABC const & rhs ) const
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        return rInterval_ >= pRhs->rInterval_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::IsLessThanOrEqualTo
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_TimeInterval::IsLessThanOrEqualTo( TimeInterval_ABC const & rhs ) const
{
    const HLA_TimeInterval* pRhs = static_cast< const HLA_TimeInterval* >( &rhs );
    if( pRhs )
        return rInterval_ <= pRhs->rInterval_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::Encode
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
void HLA_TimeInterval::Encode( Serializer& serializer ) const
{
    serializer << rInterval_;
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::ToString
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
std::string HLA_TimeInterval::ToString() const
{
    std::ostringstream message;
    message << rInterval_;
    return message.str();
}

// -----------------------------------------------------------------------------
// Name: HLA_TimeInterval::GetValue
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
double HLA_TimeInterval::GetValue() const
{
    return rInterval_;
}
