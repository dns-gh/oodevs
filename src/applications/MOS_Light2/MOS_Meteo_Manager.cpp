//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_Manager.cpp $
// $Author: Age $
// $Modtime: 2/03/05 16:34 $
// $Revision: 7 $
// $Workfile: MOS_Meteo_Manager.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"
#include "MOS_Meteo_Manager.h"

#include "MOS_Meteo_Local.h"
#include "MOS_World.h"

MT_IdentifierManager MOS_Meteo_Manager::idManager_;

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_Manager.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_Manager::MOS_Meteo_Manager()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
MOS_Meteo_Manager::~MOS_Meteo_Manager()
{
    MT_DELETEOWNED( localWeatherVector_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::CreateLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_Local& MOS_Meteo_Manager::CreateLocalWeather()
{
    MOS_Meteo_Local* pNew = new MOS_Meteo_Local( idManager_.GetFreeIdentifier() );
    localWeatherVector_.push_back( pNew );
    return *pNew;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::DeleteLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_Manager::DeleteLocalWeather( MOS_Meteo_Local& localWeather )
{
   idManager_.ReleaseIdentifier( localWeather.GetId() );
    IT_Meteo_Vector found = std::find( localWeatherVector_.begin(), localWeatherVector_.end(), & localWeather );
    if( found != localWeatherVector_.end() )
        localWeatherVector_.erase( found );
}


// -----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::RegisterAmmoMeteoEffect
/** @param  asnMsg 
*/
// Created: APE 2004-09-23
// -----------------------------------------------------------------------------
void MOS_Meteo_Manager::RegisterAmmoMeteoEffect( const ASN1T_MsgStartFireEffect& asnMsg )
{
    MT_Vector2D point1, point2, point3;

    assert( asnMsg.localisation.vecteur_point.n == 3 );

    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[0].data, point1 );
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[1].data, point2 );
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[2].data, point3 );

    MT_Ellipse* pEllipse = new MT_Ellipse( point1, point2, point3 );
    ammoMeteoEffects_[ asnMsg.oid_effet ] = std::make_pair( asnMsg.type, pEllipse );
}


// -----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::UnregisterAmmoMeteoEffect
/** @param  nID 
*/
// Created: APE 2004-09-23
// -----------------------------------------------------------------------------
void MOS_Meteo_Manager::UnregisterAmmoMeteoEffect( uint nID )
{
    IT_AmmoMeteoEffetMap it = ammoMeteoEffects_.find( nID );

    if ( it != ammoMeteoEffects_.end() )
    {
        delete it->second.second;
        ammoMeteoEffects_.erase( it );
    }
}
