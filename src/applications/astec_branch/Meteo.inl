//*****************************************************************************
//
// $Created: JVT 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo.inl $
// $Author: Ape $
// $Modtime: 4/11/04 10:43 $
// $Revision: 3 $
// $Workfile: Meteo.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: Meteo::Terminate
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
void Meteo::Terminate()
{
    nRefCount_ = 0;
}

//-----------------------------------------------------------------------------
// Name: Meteo::UpdateAllMeteo
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo::UpdateAllMeteo( const sEphemeride& ephemeride )
{
    for ( IT_MeteoVector it = vInstanciatedMeteo_.begin(); it != vInstanciatedMeteo_.end(); ++it )
        (*it)->ComputeTimeCategory( ephemeride );
}


//-----------------------------------------------------------------------------
// Name: Meteo::GetLighting
// Created: JVT 03-08-05
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
E_LightingType Meteo::GetLighting()   const
{
    return nEclairement_;
}


//-----------------------------------------------------------------------------
// Name: Meteo::GetWeather
// Created: JVT 03-08-05
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
E_WeatherType Meteo::GetWeather() const
{
    return nPrecipitation_;
}


//-----------------------------------------------------------------------------
// Name: Meteo::IncRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo::IncRef()
{
    ++nRefCount_;
}


//-----------------------------------------------------------------------------
// Name: Meteo::IncRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo::IncRef( uint nNbrRef )
{
    nRefCount_ += nNbrRef;
}


//-----------------------------------------------------------------------------
// Name: Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo::DecRef()
{
    assert( nRefCount_ );
    if ( !--nRefCount_ )
        Destroy();
}


//-----------------------------------------------------------------------------
// Name: Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo::DecRef( uint nNbrRef )
{
    assert( nRefCount_ >= nNbrRef );
    if ( !( nRefCount_ -= nNbrRef ) )
        Destroy();
}


//-----------------------------------------------------------------------------
// Name: Meteo::DestroyAllMeteo
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo::DestroyAllMeteo()
{
    std::for_each( vInstanciatedMeteo_.begin(), vInstanciatedMeteo_.end(), std::mem_fun( &Meteo::Destroy ) );
    vInstanciatedMeteo_.clear();
}
