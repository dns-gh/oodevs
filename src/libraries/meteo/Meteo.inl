// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Meteo::GetID
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
inline
unsigned long weather::Meteo::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetName
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
inline
const std::string& weather::Meteo::GetName() const
{
    return name_;
}

//-----------------------------------------------------------------------------
// Name: Meteo::GetPrecipitation
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
const weather::PHY_Precipitation& weather::Meteo::GetPrecipitation() const
{
    assert( pPrecipitation_ );
    return *pPrecipitation_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetWind
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
const weather::Meteo::sWindData& weather::Meteo::GetWind() const
{
    return wind_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetCloud
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
const weather::Meteo::sCloudData& weather::Meteo::GetCloud() const
{
    return cloud_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetPrecipitation
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
void weather::Meteo::SetPrecipitation( const weather::PHY_Precipitation& precipitation )
{
    if( pPrecipitation_ != &precipitation )
        modified_ = true;
    pPrecipitation_ = &precipitation;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetWind
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
void weather::Meteo::SetWind( const sWindData& wind )
{
    if( wind_.eAngle_ != wind.eAngle_ || wind_.rSpeed_ != wind.rSpeed_ )
        modified_ = true;
    wind_ = wind;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetCloud
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
void weather::Meteo::SetCloud( const sCloudData& cloud )
{
    if( cloud_.nCeiling_ != cloud.nCeiling_ || cloud_.nFloor_ != cloud.nFloor_ || cloud_.nDensityPercentage_ != cloud.nDensityPercentage_ || cloud_.rDensity_ != cloud.rDensity_ )
        modified_ = true;
    cloud_ = cloud;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetConversionFactor
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
inline
double weather::Meteo::GetConversionFactor() const
{
    return conversionFactor_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsModified
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
inline
bool weather::Meteo::IsModified() const
{
    return modified_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetModified
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
inline
void weather::Meteo::SetModified( bool modified )
{
    modified_ = modified;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsInside
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
inline
bool weather::Meteo::IsInside( const geometry::Point2f& /*point*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsPatched
// Created: ABR 2012-03-21
// -----------------------------------------------------------------------------
inline
bool weather::Meteo::IsPatched() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsYounger
// Created: ABR 2012-03-23
// -----------------------------------------------------------------------------
inline
bool weather::Meteo::IsOlder( const weather::Meteo& /*other*/ ) const
{
    return true;
}

