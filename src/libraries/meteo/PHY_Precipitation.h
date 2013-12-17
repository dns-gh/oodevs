// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/PHY_Precipitation.h $
// $Author: Nld $
// $Modtime: 25/11/04 18:23 $
// $Revision: 2 $
// $Workfile: PHY_Precipitation.h $
//
// *****************************************************************************

#ifndef __weather_PHY_Precipitation_h_
#define __weather_PHY_Precipitation_h_

#include "ENT/ENT_Enums.h"
#include <boost/noncopyable.hpp>
#include <map>
#include <string>

namespace weather
{

// =============================================================================
// @class  PHY_Precipitation
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Precipitation : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Precipitation* > T_PrecipitationMap;
    typedef T_PrecipitationMap::const_iterator CIT_PrecipitationMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_PrecipitationMap& GetPrecipitations();
    static const PHY_Precipitation* FindPrecipitation( const std::string& strName );
    static const PHY_Precipitation* FindPrecipitation( E_WeatherType type );
    //@}

    //! @name Accessors
    //@{
    const std::string&            GetName () const;
    E_WeatherType                 GetID   () const;
    //@}

public:
    static PHY_Precipitation none_;
    static PHY_Precipitation sandStorm_;
    static PHY_Precipitation fog_;
    static PHY_Precipitation drizzle_;
    static PHY_Precipitation rain_;
    static PHY_Precipitation snow_;
    static PHY_Precipitation smoke_;

private:
    //! @name Constructors/Destructor
    //@{
     PHY_Precipitation( const std::string& strName, E_WeatherType nType );
    ~PHY_Precipitation();
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const E_WeatherType nType_;
    //@}

    //! @name Static data
    //@{
    static T_PrecipitationMap precipitations_;
    //@}
};

}

#endif // __weather_PHY_Precipitation_h_
