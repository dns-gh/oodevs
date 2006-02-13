//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_Manager.h $
// $Author: Age $
// $Modtime: 2/03/05 16:33 $
// $Revision: 7 $
// $Workfile: Meteo_Manager.h $
//
//*****************************************************************************

#ifndef __Meteo_Manager_h_
#define __Meteo_Manager_h_

#include "Meteo_ABC.h"
#include "ASN_Messages.h"

class Meteo_Local;
class MT_Ellipse;

// =============================================================================
/** @class  Meteo_Manager
    @brief  Manages the weather data.
*/
// Created: APE 2004-07-19
// =============================================================================
class Meteo_Manager
{
    friend class GLTool;

public:
    typedef std::vector< Meteo_Local* > T_Meteo_Vector;
    typedef T_Meteo_Vector::iterator        IT_Meteo_Vector;
    typedef T_Meteo_Vector::const_iterator  CIT_Meteo_Vector;

    typedef std::pair< ASN1T_EnumFireEffectType, const MT_Ellipse* > T_AmmoMeteoEffect;
    typedef std::map< uint, T_AmmoMeteoEffect >                      T_AmmoMeteoEffetMap;
    typedef T_AmmoMeteoEffetMap::iterator                           IT_AmmoMeteoEffetMap;
    typedef T_AmmoMeteoEffetMap::const_iterator                    CIT_AmmoMeteoEffetMap;

public:
     Meteo_Manager();
    ~Meteo_Manager();

    Meteo_ABC& GetGlobalWeather();
    T_Meteo_Vector& GetLocalWeatherList();

    Meteo_Local& CreateLocalWeather();
    void DeleteLocalWeather( Meteo_Local& weather );

    void RegisterAmmoMeteoEffect( const ASN1T_MsgStartFireEffect& asnMsg );
    void UnregisterAmmoMeteoEffect( uint nID );

private:
    Meteo_ABC globalWeather_;
    T_Meteo_Vector localWeatherVector_;

    T_AmmoMeteoEffetMap ammoMeteoEffects_;

    static MT_IdentifierManager idManager_;
};

#	include "Meteo_Manager.inl"

#endif // __Meteo_Manager_h_