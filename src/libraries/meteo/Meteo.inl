// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: Meteo::GetLighting
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
const PHY_Lighting& Meteo::GetLighting() const
{
    assert( pLighting_ );
    return *pLighting_;
}

//-----------------------------------------------------------------------------
// Name: Meteo::GetPrecipitation
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
const PHY_Precipitation& Meteo::GetPrecipitation() const
{
    assert( pPrecipitation_ );
    return *pPrecipitation_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetWind
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
const Meteo::sWindData& Meteo::GetWind() const
{
    return wind_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetCloud
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
const Meteo::sCloudData& Meteo::GetCloud() const
{
    return cloud_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetPrecipitation
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
void Meteo::SetPrecipitation( const PHY_Precipitation& precipitation )
{
    if( pPrecipitation_ != &precipitation )
        modified_ = true;
    pPrecipitation_ = &precipitation;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetLighting
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
void Meteo::SetLighting( const PHY_Lighting& light )
{
    if( pLighting_ != &light )
        modified_ = true;
    pLighting_ = &light;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetWind
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
void Meteo::SetWind( const sWindData& wind )
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
void Meteo::SetCloud( const sCloudData& cloud )
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
double Meteo::GetConversionFactor() const
{
    return conversionFactor_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsModified
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
inline
bool Meteo::IsModified() const
{
    return modified_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::SetModified
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
inline
void Meteo::SetModified( bool modified )
{
    modified_ = modified;
}

// -----------------------------------------------------------------------------
// Name: Meteo::GetId
// Created: ABR 2011-06-01
// -----------------------------------------------------------------------------
inline
unsigned long Meteo::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Meteo::IsInside
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
inline
bool Meteo::IsInside( const geometry::Point2f& /*point*/ ) const
{
    return true;
}
