// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::GetEclairement
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
const PHY_Lighting& PHY_Meteo::GetLighting() const
{
    assert( pLighting_ );
    return *pLighting_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::GetPrecipitation
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
const PHY_Precipitation& PHY_Meteo::GetPrecipitation() const
{
    assert( pPrecipitation_ );
    return *pPrecipitation_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetWind
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
inline
const PHY_Meteo::sWindData& PHY_Meteo::GetWind() const
{
    return wind_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetCloudFloor
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
inline
int PHY_Meteo::GetCloudFloor() const
{
    return nPlancherCouvertureNuageuse_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetCloudCeiling
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
inline
int PHY_Meteo::GetCloudCeiling() const
{
    return nPlafondCouvertureNuageuse_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetCloudDensity
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
inline
double PHY_Meteo::GetCloudDensity() const
{
    return rDensiteCouvertureNuageuse_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::GetConversionFactor
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
inline
int PHY_Meteo::GetConversionFactor() const
{
    return conversionFactor_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::IncRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void PHY_Meteo::IncRef()
{
    ++nRefCount_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::IncRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void PHY_Meteo::IncRef( unsigned int nNbrRef )
{
    nRefCount_ += nNbrRef;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void PHY_Meteo::DecRef()
{
    assert( nRefCount_ );
    --nRefCount_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void PHY_Meteo::DecRef( unsigned int nNbrRef )
{
    assert( nRefCount_ >= nNbrRef );
    if ( !( nRefCount_ -= nNbrRef ) )
        delete this;
}
