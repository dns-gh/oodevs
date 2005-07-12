//*****************************************************************************
//
// $Created: JVT 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo.inl $
// $Author: Ape $
// $Modtime: 4/11/04 10:43 $
// $Revision: 3 $
// $Workfile: MOS_Meteo.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_Meteo::Terminate
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::Terminate()
{
    nRefCount_ = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_Meteo::UpdateAllMeteo
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::UpdateAllMeteo( const sEphemeride& ephemeride )
{
    for ( IT_MeteoVector it = vInstanciatedMeteo_.begin(); it != vInstanciatedMeteo_.end(); ++it )
        (*it)->ComputeTimeCategory( ephemeride );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::GetLighting
// Created: JVT 03-08-05
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
E_LightingType MOS_Meteo::GetLighting()   const
{
    return nEclairement_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::GetWeather
// Created: JVT 03-08-05
// Modified: JVT 2004-09-27
//-----------------------------------------------------------------------------
inline
E_WeatherType MOS_Meteo::GetWeather() const
{
    return nPrecipitation_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::IncRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::IncRef()
{
    ++nRefCount_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::IncRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::IncRef( uint nNbrRef )
{
    nRefCount_ += nNbrRef;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::DecRef()
{
    assert( nRefCount_ );
    if ( !--nRefCount_ )
        Destroy();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::DecRef
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::DecRef( uint nNbrRef )
{
    assert( nRefCount_ >= nNbrRef );
    if ( !( nRefCount_ -= nNbrRef ) )
        Destroy();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo::DestroyAllMeteo
// Created: JVT 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo::DestroyAllMeteo()
{
    std::for_each( vInstanciatedMeteo_.begin(), vInstanciatedMeteo_.end(), std::mem_fun( &MOS_Meteo::Destroy ) );
    vInstanciatedMeteo_.clear();
}
