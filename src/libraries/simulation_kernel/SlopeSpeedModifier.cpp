// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SlopeSpeedModifier.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"

// -----------------------------------------------------------------------------
// Name: SlopeSpeedModifier constructor
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
SlopeSpeedModifier::SlopeSpeedModifier()
    : currentSlope_( std::numeric_limits< double >::max() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SlopeSpeedModifier destructor
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
SlopeSpeedModifier::~SlopeSpeedModifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SlopeSpeedModifier::ComputeLocalSlope
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
bool SlopeSpeedModifier::ComputeLocalSlope( const PHY_RawVisionData& data, const MT_Vector2D& from, const MT_Vector2D& to )
{
    const double delta = data.GetAltitude( to ) - data.GetAltitude( from );
    const double distance = from.Distance( to );
    const double slope = distance < 1 ? 0 : delta / distance;
    if( std::abs( slope - currentSlope_ ) > 0.01 )
    {
        currentSlope_ = slope;
        slopes_.push_back( std::make_pair( from, std::max( 0., slope ) ) );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SlopeSpeedModifier::ModifySpeed
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
void SlopeSpeedModifier::ModifySpeed( double& rSpeed, double rSlopeDeceleration, double rMaxSlope, const MT_Vector2D& to ) const
{
    if( slopes_.empty() )
        return;
    double cumulatedTime = 0;
    double cumulatedDist = 0;
    for( auto it = slopes_.begin(); it != slopes_.end(); ++it )
    {
        const MT_Vector2D& localFrom = it->first;
        const double localSlope = it->second;
        const MT_Vector2D& localTo = ( it + 1 == slopes_.end() ? to : ( it + 1 )->first );
        double localSpeed = rSpeed;
        if( localSlope > 0 )
        {
            if( localSlope >= rMaxSlope )
                localSpeed = 0;
            else
                localSpeed *= ( 1 - rSlopeDeceleration * localSlope / rMaxSlope );
            if( localSpeed <= 0. )
            {
                rSpeed = 0;
                return;
            }
        }
        const double distance = localFrom.Distance( localTo );
        cumulatedDist += distance;
        cumulatedTime += distance / localSpeed;
    }
    if( cumulatedTime > 0 )
        rSpeed = cumulatedDist / cumulatedTime;
}

// -----------------------------------------------------------------------------
// Name: SlopeSpeedModifier::GetSlopes
// Created: JSR 2013-11-12
// -----------------------------------------------------------------------------
const SlopeSpeedModifier::T_Slopes& SlopeSpeedModifier::GetSlopes() const
{
    return slopes_;
}
