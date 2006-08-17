//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Meteo.inl $
// $Author: Jvt $
// $Modtime: 29/10/04 10:35 $
// $Revision: 2 $
// $Workfile: PHY_Meteo.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::GetEclairement
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
const PHY_Lighting& PHY_Meteo::GetLighting()   const
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
void PHY_Meteo::IncRef( uint nNbrRef )
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
    if ( !--nRefCount_ )
        delete this;
}


//-----------------------------------------------------------------------------
// Name: PHY_Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void PHY_Meteo::DecRef( uint nNbrRef )
{
    assert( nRefCount_ >= nNbrRef );
    if ( !( nRefCount_ -= nNbrRef ) )
        delete this;
}


