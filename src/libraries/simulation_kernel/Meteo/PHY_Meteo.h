//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Meteo.h $
// $Author: Jvt $
// $Modtime: 29/10/04 10:33 $
// $Revision: 2 $
// $Workfile: PHY_Meteo.h $
//
//*****************************************************************************

#ifndef __PHY_Meteo_h_
#define __PHY_Meteo_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class PHY_Ephemeride;
class PHY_Precipitation;
class PHY_Lighting;

struct ASN1T_MeteoAttributes;

//*****************************************************************************
// Created: JVT 03-08-05
// Last modified: JVT 03-08-07
//*****************************************************************************
class PHY_Meteo
{
    MT_COPYNOTALLOWED( PHY_Meteo )

public:
    struct sWindData
    {
        MT_Float    rWindSpeed_;
        MT_Vector2D vWindDirection_;
    };

public:
    PHY_Meteo( xml::xistream& xis, const PHY_Ephemeride& ephemeride );
    PHY_Meteo( const ASN1T_MeteoAttributes& );

    //-------------------------------------------------------------------------
    /** @name Creation / destruction */
    //-------------------------------------------------------------------------
    //@{
    void IncRef();
    void IncRef( uint );
    void DecRef();
    void DecRef( uint );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const PHY_Lighting&      GetLighting     () const;
    const PHY_Precipitation& GetPrecipitation() const;
    const sWindData&         GetWind         () const;       
    //@}

    //! @name Operations
    //@{
    void Update( const PHY_Ephemeride& );
    void Update( const ASN1T_MeteoAttributes& asn );
    //@}

private:
    ~PHY_Meteo();
   
private:
    int                      nPlancherCouvertureNuageuse_;
    int                      nPlafondCouvertureNuageuse_;
    MT_Float                 rDensiteCouvertureNuageuse_;
    sWindData                wind_;
    const PHY_Lighting*      pLighting_;
    const PHY_Precipitation* pPrecipitation_;

    uint nRefCount_;
};

#include "PHY_Meteo.inl"

#endif // __PHY_Meteo_h_