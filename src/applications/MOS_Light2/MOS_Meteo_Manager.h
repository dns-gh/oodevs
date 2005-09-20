//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_Manager.h $
// $Author: Age $
// $Modtime: 2/03/05 16:33 $
// $Revision: 7 $
// $Workfile: MOS_Meteo_Manager.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_Manager_h_
#define __MOS_Meteo_Manager_h_

#include "MOS_Meteo_ABC.h"
#include "MOS_ASN_Messages.h"

class MOS_Meteo_Local;
class MT_Ellipse;

// =============================================================================
/** @class  MOS_Meteo_Manager
    @brief  Manages the weather data.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Meteo_Manager
{
    friend class MOS_GLTool;

public:
    typedef std::vector< MOS_Meteo_Local* > T_Meteo_Vector;
    typedef T_Meteo_Vector::iterator        IT_Meteo_Vector;
    typedef T_Meteo_Vector::const_iterator  CIT_Meteo_Vector;

    typedef std::pair< ASN1T_EnumFireEffectType, const MT_Ellipse* > T_AmmoMeteoEffect;
    typedef std::map< uint, T_AmmoMeteoEffect >                      T_AmmoMeteoEffetMap;
    typedef T_AmmoMeteoEffetMap::iterator                           IT_AmmoMeteoEffetMap;
    typedef T_AmmoMeteoEffetMap::const_iterator                    CIT_AmmoMeteoEffetMap;

public:
     MOS_Meteo_Manager();
    ~MOS_Meteo_Manager();

    MOS_Meteo_ABC& GetGlobalWeather();
    T_Meteo_Vector& GetLocalWeatherList();

    MOS_Meteo_Local& CreateLocalWeather();
    void DeleteLocalWeather( MOS_Meteo_Local& weather );

    void RegisterAmmoMeteoEffect( const ASN1T_MsgStartFireEffect& asnMsg );
    void UnregisterAmmoMeteoEffect( uint nID );

private:
    MOS_Meteo_ABC globalWeather_;
    T_Meteo_Vector localWeatherVector_;

    T_AmmoMeteoEffetMap ammoMeteoEffects_;

    static MT_IdentifierManager idManager_;
};

#	include "MOS_Meteo_Manager.inl"

#endif // __MOS_Meteo_Manager_h_