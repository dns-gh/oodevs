//*****************************************************************************
//
// $Created: JVT 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo.h $
// $Author: Nld $
// $Modtime: 27/09/04 17:37 $
// $Revision: 3 $
// $Workfile: MOS_Meteo.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_h_
#define __MOS_Meteo_h_


#include "MOS_Types.h"

struct ASN1T_MeteoAttributs;

enum E_LightingType;
enum E_WeatherType;

#include "MT/MT_Memory/MT_ObjectRecycler.h"

//*****************************************************************************
// Created: JVT 03-08-05
// Last modified: JVT 03-08-08
//*****************************************************************************
class MOS_Meteo
{
    MT_COPYNOTALLOWED( MOS_Meteo )

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
    static MOS_Meteo* Create( MT_InputArchive_ABC& );
    static MOS_Meteo* Create( const ASN1T_MeteoAttributs& );
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
    explicit MOS_Meteo();
             ~MOS_Meteo();

    friend class MOS_RawVisionData;

    void Initialize( MT_InputArchive_ABC& );
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
    
    friend class MT_ObjectRecycler< MOS_Meteo >;
    static MT_ObjectRecycler< MOS_Meteo > recycler_;

    typedef std::vector< MOS_Meteo* > T_MeteoVector;
    typedef T_MeteoVector::iterator   IT_MeteoVector;

    static T_MeteoVector vInstanciatedMeteo_;
    //@}
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo.inl"
#endif


#endif // __MOS_Meteo_h_