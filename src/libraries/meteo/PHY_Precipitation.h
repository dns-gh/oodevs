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
    static const std::map< std::string, const PHY_Precipitation* >& GetPrecipitations();
    static const PHY_Precipitation* FindPrecipitation( const std::string& strName );
    static const PHY_Precipitation* FindPrecipitation( E_WeatherType type );
    //@}

    //! @name Accessors
    //@{
    const std::string&            GetName () const;
    E_WeatherType                 GetID   () const;
    //@}

public:
    static const PHY_Precipitation none_;
    static const PHY_Precipitation sandStorm_;
    static const PHY_Precipitation fog_;
    static const PHY_Precipitation drizzle_;
    static const PHY_Precipitation rain_;
    static const PHY_Precipitation snow_;
    static const PHY_Precipitation smoke_;

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
};

}

#endif // __weather_PHY_Precipitation_h_
