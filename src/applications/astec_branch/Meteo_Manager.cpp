//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_Manager.cpp $
// $Author: Age $
// $Modtime: 2/03/05 16:34 $
// $Revision: 7 $
// $Workfile: Meteo_Manager.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Meteo_Manager.h"

#include "Meteo_Local.h"
#include "World.h"

MT_IdentifierManager Meteo_Manager::idManager_;

//-----------------------------------------------------------------------------
// Name: Meteo_Manager constructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
Meteo_Manager::Meteo_Manager()
{
}


//-----------------------------------------------------------------------------
// Name: Meteo_Manager destructor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
Meteo_Manager::~Meteo_Manager()
{
    MT_DELETEOWNED( localWeatherVector_ );
}


//-----------------------------------------------------------------------------
// Name: Meteo_Manager::CreateLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
Meteo_Local& Meteo_Manager::CreateLocalWeather()
{
    Meteo_Local* pNew = new Meteo_Local( idManager_.GetFreeIdentifier() );
    localWeatherVector_.push_back( pNew );
    return *pNew;
}


//-----------------------------------------------------------------------------
// Name: Meteo_Manager::DeleteLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void Meteo_Manager::DeleteLocalWeather( Meteo_Local& localWeather )
{
   idManager_.ReleaseIdentifier( localWeather.GetId() );
    IT_Meteo_Vector found = std::find( localWeatherVector_.begin(), localWeatherVector_.end(), & localWeather );
    if( found != localWeatherVector_.end() )
        localWeatherVector_.erase( found );
}


// -----------------------------------------------------------------------------
// Name: Meteo_Manager::RegisterAmmoMeteoEffect
/** @param  asnMsg 
*/
// Created: APE 2004-09-23
// -----------------------------------------------------------------------------
void Meteo_Manager::RegisterAmmoMeteoEffect( const ASN1T_MsgStartFireEffect& asnMsg )
{
    MT_Vector2D point1, point2, point3;

    assert( asnMsg.localisation.vecteur_point.n == 3 );

    App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[0].data, point1 );
    App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[1].data, point2 );
    App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[2].data, point3 );

    MT_Ellipse* pEllipse = new MT_Ellipse( point1, point2, point3 );
    ammoMeteoEffects_[ asnMsg.oid_effet ] = std::make_pair( asnMsg.type, pEllipse );
}


// -----------------------------------------------------------------------------
// Name: Meteo_Manager::UnregisterAmmoMeteoEffect
/** @param  nID 
*/
// Created: APE 2004-09-23
// -----------------------------------------------------------------------------
void Meteo_Manager::UnregisterAmmoMeteoEffect( uint nID )
{
    IT_AmmoMeteoEffetMap it = ammoMeteoEffects_.find( nID );

    if ( it != ammoMeteoEffects_.end() )
    {
        delete it->second.second;
        ammoMeteoEffects_.erase( it );
    }
}
