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

#ifndef __PHY_Precipitation_h_
#define __PHY_Precipitation_h_

#include "MIL.h"

#include "game_asn/Simulation.h"

// =============================================================================
// @class  PHY_Precipitation
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Precipitation : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Precipitation*, sCaseInsensitiveLess > T_PrecipitationMap;
    typedef T_PrecipitationMap::const_iterator                                       CIT_PrecipitationMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_PrecipitationMap& GetPrecipitations();
    static const PHY_Precipitation* FindPrecipitation( const std::string& strName );
    static const PHY_Precipitation* FindPrecipitation( ASN1T_EnumPrecipitationType nAsnID );
    //@}

    //! @name Accessors
    //@{
    const std::string&          GetName () const;
    uint                        GetID   () const;
    ASN1T_EnumPrecipitationType GetAsnID() const;
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
    //! @name Types
    //@{
    enum E_Precipitation
    {
        eNone     ,
        eSandStorm,
        eFog      ,
        eDrizzle  ,
        eRain     ,
        eSnow     ,
        eSmoke    
    };
    //@}

private:
     PHY_Precipitation( const std::string& strName, E_Precipitation nType, ASN1T_EnumPrecipitationType nAsnID );
    ~PHY_Precipitation();

private:
    const std::string                 strName_;
    const E_Precipitation             nType_;
    const ASN1T_EnumPrecipitationType nAsnID_;

private:
    static T_PrecipitationMap precipitations_;
};

#include "PHY_Precipitation.inl"

#endif // __PHY_Precipitation_h_
