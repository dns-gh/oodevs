// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Lighting.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_Lighting.h $
//
// *****************************************************************************

#ifndef __weather_PHY_Lighting_h_
#define __weather_PHY_Lighting_h_

#include "ENT/ENT_Enums.h"
#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace weather
{
// =============================================================================
// @class  PHY_Lighting
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Lighting : private boost::noncopyable
{
public:
    static const std::map< std::string, const PHY_Lighting* >& GetLightings();
    static const PHY_Lighting* FindLighting( const std::string& strName );
    static const PHY_Lighting* FindLighting( E_LightingType value );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    E_LightingType GetID() const;
    //@}

    //! @name Operations
    //@{
    const PHY_Lighting& GetDegradedLighting( unsigned int nDegradationFactor ) const;
    //@}

public:
    static const PHY_Lighting jourSansNuage_;
    static const PHY_Lighting jourPeuNuageux_;
    static const PHY_Lighting jourMoyennementNuageux_;
    static const PHY_Lighting jourAssezNuageux_;
    static const PHY_Lighting jourTresNuageux_;
    static const PHY_Lighting nuitPleineLune_;
    static const PHY_Lighting nuitTroisQuartDeLune_;
    static const PHY_Lighting nuitDemiLune_;
    static const PHY_Lighting nuitQuartDeLune_;
    static const PHY_Lighting nuitNouvelleLune_;
    static const PHY_Lighting eclairant_;

private:
     PHY_Lighting( const std::string& strName, E_LightingType nType, const PHY_Lighting* pNextDegradedLighting );
    ~PHY_Lighting();

private:
    const std::string strName_;
    const E_LightingType nType_;
    const PHY_Lighting* pNextDegradedLighting_;
};

}

#endif // __weather_PHY_Lighting_h_
