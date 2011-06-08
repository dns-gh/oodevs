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

#include "ENT/ENT_Enums_Gen.h"
#include "MT_Tools/MT_String.h"
#include "protocol/Protocol.h"
#include <boost/noncopyable.hpp>
#include <map>

namespace weather
{
// =============================================================================
// @class  PHY_Lighting
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Lighting : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Lighting* > T_LightingMap;
    typedef T_LightingMap::const_iterator              CIT_LightingMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_LightingMap& GetLightings();
    static const PHY_Lighting* FindLighting( const std::string& strName );
    static const PHY_Lighting* FindLighting( sword::WeatherAttributes::EnumLightingType nAsnID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    E_LightingType GetID() const;
    sword::WeatherAttributes::EnumLightingType GetAsnID() const;
    //@}

    //! @name Operations
    //@{
    const PHY_Lighting& GetDegradedLighting( unsigned int nDegradationFactor ) const;
    //@}

public:
    static PHY_Lighting jourSansNuage_;
    static PHY_Lighting jourPeuNuageux_;
    static PHY_Lighting jourMoyennementNuageux_;
    static PHY_Lighting jourAssezNuageux_;
    static PHY_Lighting jourTresNuageux_;
    static PHY_Lighting nuitPleineLune_;
    static PHY_Lighting nuitTroisQuartDeLune_;
    static PHY_Lighting nuitDemiLune_;
    static PHY_Lighting nuitQuartDeLune_;
    static PHY_Lighting nuitNouvelleLune_;
    static PHY_Lighting eclairant_;

private:
     PHY_Lighting( const std::string& strName, E_LightingType nType, const PHY_Lighting* pNextDegradedLighting, sword::WeatherAttributes::EnumLightingType nAsnID );
    ~PHY_Lighting();

private:
    const std::string    strName_;
    const E_LightingType     nType_;
    const PHY_Lighting* pNextDegradedLighting_;
    const sword::WeatherAttributes::EnumLightingType nAsnID_;

private:
    static T_LightingMap lightings_;
};

#include "PHY_Lighting.inl"

}

#endif // __weather_PHY_Lighting_h_
