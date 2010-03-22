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

#include <boost/noncopyable.hpp>
#include "MT_Tools/MT_Tools_Types.h"
#include "MT_Tools/MT_Vector2D.h"


namespace Common
{
    class MsgMeteoAttributes;
}

namespace xml
{
    class xistream;
}


class PHY_Precipitation;
class PHY_Lighting;
class MeteoManager_ABC;
class PHY_RawVisionData_ABC;

//*****************************************************************************
// Created: JVT 03-08-05
// Last modified: JVT 03-08-07
//*****************************************************************************
class PHY_Meteo : private boost::noncopyable
{

public:
    struct sWindData
    {
        MT_Float    rWindSpeed_;
        MT_Vector2D vWindDirection_;
    };

public:
             PHY_Meteo( xml::xistream& xis, const PHY_Lighting& light, int conversionFactor );
    explicit PHY_Meteo( const Common::MsgMeteoAttributes& );

    //-------------------------------------------------------------------------
    /** @name Creation / destruction */
    //-------------------------------------------------------------------------
    //@{
    void IncRef();
    void IncRef( unsigned int );
    void DecRef();
    void DecRef( unsigned int );
    void SetListener( MeteoManager_ABC* );
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const PHY_Precipitation& GetPrecipitation() const;
    const PHY_Lighting&      GetLighting     () const;
    const sWindData&         GetWind         () const;       
    //@}

    //! @name Operations
    //@{
    void Update( const Common::MsgMeteoAttributes& asn );
    void Update( const PHY_Lighting& /*PHY_Ephemeride&*/ );
    virtual void UpdateMeteoPatch( int date, PHY_RawVisionData_ABC& dataVision_ );
     //@}

public:
    ~PHY_Meteo();

   
private:
    int                      nPlancherCouvertureNuageuse_;
    int                      nPlafondCouvertureNuageuse_;
    MT_Float                 rDensiteCouvertureNuageuse_;
    sWindData                wind_;
    const PHY_Lighting*      pLighting_;
    const PHY_Precipitation* pPrecipitation_;

    unsigned int nRefCount_;
    int conversionFactor_;
    MeteoManager_ABC* listener_;
};

#include "PHY_Meteo.inl"

#endif // __PHY_Meteo_h_