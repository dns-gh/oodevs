//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_ABC.h $
// $Author: Ape $
// $Modtime: 20/07/04 17:41 $
// $Revision: 3 $
// $Workfile: MOS_Meteo_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_ABC_h_
#define __MOS_Meteo_ABC_h_

struct ASN1T_MeteoAttributs;


// =============================================================================
/** @class  MOS_Meteo_ABC
    @brief  Global weather data. Acts as a base class for local weather data.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Meteo_ABC
{
public:
    enum E_WeatherType
    {
        eWeather_Clear,
        eWeather_Storm,
        eWeather_Fog,
        eWeather_Drizzle,
        eWeather_Rain,
        eWeather_Snow,
        eWeatherNbr
    };

public:
    MOS_Meteo_ABC();
    virtual ~MOS_Meteo_ABC();

    void SetTemperature( int nTemperature );
    void SetWindSpeed( int nWindSpeed );
    void SetWindDirection( int nWindDirection );
    void SetCloudCoverFloor( int nFloor );
    void SetCloudCoverCeiling( int nCeiling );
    void SetCloudDensity( int nDensity );
    void SetWeatherType( E_WeatherType nType );

    int GetTemperature() const;
    int GetWindSpeed() const;
    int GetWindDirection() const;
    int GetCloudCoverFloor() const;
    int GetCloudCoverCeiling() const;
    int GetCloudDensity() const;
    E_WeatherType GetWeatherType() const;

    virtual void SendUpdateMsg();

protected:
    void BuildAsnMsg( ASN1T_MeteoAttributs& asnMsg );

protected:
    int nTemperature_;
    int nWindSpeed_;
    int nWindDirection_;
    int nCloudCoverFloor_;
    int nCloudCoverCeiling_;
    int nCloudCoverAverageDensity_;
    E_WeatherType nWeatherType_;
};

#	include "MOS_Meteo_ABC.inl"

#endif // __MOS_Meteo_ABC_h_