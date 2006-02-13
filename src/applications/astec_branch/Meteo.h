//*****************************************************************************
//
// $Created: JVT 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo.h $
// $Author: Ape $
// $Modtime: 27/10/04 10:43 $
// $Revision: 3 $
// $Workfile: Meteo.h $
//
//*****************************************************************************

#ifndef __Meteo_h_
#define __Meteo_h_


#include "Types.h"

struct ASN1T_MeteoAttributs;

//enum E_LightingType;
//enum E_WeatherType;

#include "MT/MT_Memory/MT_ObjectRecycler.h"

//*****************************************************************************
// Created: JVT 03-08-05
// Last modified: JVT 03-08-08
//*****************************************************************************
class Meteo
{
    MT_COPYNOTALLOWED( Meteo )

public:
    class sEphemeride
    {
        MT_COPYNOTALLOWED( sEphemeride );
    
    public:
        sEphemeride(){}
        void           Initialize( int, int, int, int, E_LightingType );
        bool           UpdateNight();
        E_LightingType GetCurrentTimeBase() const;

    private:
        bool           bIsNight_;
        std::pair< int, int > sunriseTime_;
        std::pair< int, int > sunsetTime_;
        E_LightingType nNightBase_;
    };

public:
    //-------------------------------------------------------------------------
    /** @name Creation / destruction */
    //-------------------------------------------------------------------------
    //@{
    static Meteo* Create( InputArchive& );
    static Meteo* Create( const ASN1T_MeteoAttributs& );
    bool              Destroy();

    void IncRef();
    void IncRef( uint );
    void DecRef();
    void DecRef( uint );
    //@}

    //-------------------------------------------------------------------------
    /** @name Memory Management */
    //-------------------------------------------------------------------------
    //@{
    static void DestroyAllMeteo();
    static void UpdateAllMeteo( const sEphemeride& );
    //@}


    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    E_LightingType GetLighting() const;
    E_WeatherType  GetWeather()  const;
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    explicit Meteo();
             ~Meteo();

    friend class RawVisionData;

    void Initialize( InputArchive& );
    bool Initialize( const ASN1T_MeteoAttributs& );
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void ComputeTimeCategory( const sEphemeride& );
    //@}
    
private:
//    uint16         nTemperature_;
//    uint16         nGradianTemperature_;

//    uint           nVitesseVent_;
//    MT_Vector2D    vDirectionVent_;

    int            nPlancherCouvertureNuageuse_;
    int            nPlafondCouvertureNuageuse_;
    MT_Float       rDensiteCouvertureNuageuse_;
    E_LightingType nEclairement_;
    E_WeatherType  nPrecipitation_;

    //-------------------------------------------------------------------------
    /** @name Memory management */
    //-------------------------------------------------------------------------
    //@{
    uint nRefCount_;
    
    friend class MT_ObjectRecycler< Meteo >;
    static MT_ObjectRecycler< Meteo > recycler_;

    typedef std::vector< Meteo* > T_MeteoVector;
    typedef T_MeteoVector::iterator   IT_MeteoVector;

    static T_MeteoVector vInstanciatedMeteo_;
    //@}
};

#	include "Meteo.inl"



#endif // __Meteo_h_