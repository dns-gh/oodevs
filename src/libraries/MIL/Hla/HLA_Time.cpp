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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Time.cpp $
// $Author: Age $
// $Modtime: 5/11/04 11:09 $
// $Revision: 1 $
// $Workfile: HLA_Time.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_Time.h"
#include "HLA_TimeInterval.h"

#include "hla/Serializer.h"
#include <sstream>
#include <limits>

// -----------------------------------------------------------------------------
// Name: HLA_Time constructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time::HLA_Time()
    : rCurrentTime_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Time constructor
// Created: AGE 2004-10-14
// -----------------------------------------------------------------------------
HLA_Time::HLA_Time( double rTime )
    : rCurrentTime_( rTime )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Time destructor
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time::~HLA_Time()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::Clone
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time& HLA_Time::Clone() const
{
    return *new HLA_Time( rCurrentTime_ );
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::SetInitial
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time& HLA_Time::SetInitial()
{
    rCurrentTime_ = 0;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::IsInitial
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsInitial() const
{
    return rCurrentTime_ == 0.;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::SetFinal
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time& HLA_Time::SetFinal()
{
    rCurrentTime_ = std::numeric_limits< double >::infinity();
    return *this;
}
    
// -----------------------------------------------------------------------------
// Name: HLA_Time::IsFinal
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsFinal() const
{
    return rCurrentTime_ == std::numeric_limits< double >::infinity();
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::Set
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time& HLA_Time::Set( Time_ABC const & rhs )
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        rCurrentTime_ = pRhs->rCurrentTime_;
    else
        throw __FUNCTION__;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::IncreaseBy
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time& HLA_Time::IncreaseBy( TimeInterval_ABC const & interval )
{
    const HLA_TimeInterval* pInterval = static_cast< const HLA_TimeInterval* >( &interval );
    if( pInterval )
        rCurrentTime_ += pInterval->GetValue();
    else 
        throw __FUNCTION__;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::DecreaseBy
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
HLA_Time& HLA_Time::DecreaseBy( TimeInterval_ABC const & interval )
{
    const HLA_TimeInterval* pInterval = static_cast< const HLA_TimeInterval* >( &interval );
    if( pInterval )
        rCurrentTime_ -= pInterval->GetValue();
    else 
        throw __FUNCTION__;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::Substract
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
TimeInterval_ABC& HLA_Time::Substract( Time_ABC const & rhs ) const
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        return *new HLA_TimeInterval( rCurrentTime_ - pRhs->rCurrentTime_ );
    throw __FUNCTION__;
}   

// -----------------------------------------------------------------------------
// Name: HLA_Time::IsGreaterThan
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsGreaterThan( Time_ABC const & rhs ) const
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        return rCurrentTime_ > pRhs->rCurrentTime_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::IsLessThan
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsLessThan( Time_ABC const & rhs ) const
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        return rCurrentTime_ < pRhs->rCurrentTime_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::IsEqualTo
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsEqualTo( Time_ABC const & rhs ) const
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        return rCurrentTime_ == pRhs->rCurrentTime_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::IsGreaterThanOrEqualTo
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsGreaterThanOrEqualTo( Time_ABC const & rhs ) const
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        return rCurrentTime_ >= pRhs->rCurrentTime_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::IsLessThanOrEqualTo
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
bool HLA_Time::IsLessThanOrEqualTo( Time_ABC const & rhs ) const
{
    const HLA_Time* pRhs = static_cast< const HLA_Time* >( &rhs );
    if( pRhs )
        return rCurrentTime_ <= pRhs->rCurrentTime_;
    throw __FUNCTION__;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::Encode
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
void HLA_Time::Encode( Serializer& serializer ) const
{
    serializer << rCurrentTime_;
}

// -----------------------------------------------------------------------------
// Name: HLA_Time::ToString
// Created: AGE 2004-10-13
// -----------------------------------------------------------------------------
std::string HLA_Time::ToString() const
{
    std::stringstream message;
    message << rCurrentTime_;
    return message.str();
}
