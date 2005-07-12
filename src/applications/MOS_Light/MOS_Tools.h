//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Tools.h $
// $Author: Jvt $
// $Modtime: 9/12/04 12:02 $
// $Revision: 9 $
// $Workfile: MOS_Tools.h $
//
//*****************************************************************************

#ifndef __MOS_Tools_h_
#define __MOS_Tools_h_

#include "MOS_Types.h"
#include "MOS_Order_Def.h"

#include "MOS_Attr_Def.h"
#include "MOS_Perception_Def.h"
#include "MOS_RawVisionData.h"


enum E_WeatherType;
enum E_LightingType;

//=============================================================================
// Created: NLD 2003-05-31
//=============================================================================
class MOS_Tools
{
    MT_COPYNOTALLOWED( MOS_Tools );

public:
    static E_MissionID ConvertMissionType( const std::string& strMissionName );
    static std::string GetMissionDisplay( E_MissionID nMission );


    static std::string ConvertPostureType ( E_PostureType nPostureType );
    static std::string ConvertWeatherType ( E_WeatherType nWeatherType );
    static std::string ConvertLightingType( E_LightingType nLightingType );
    static std::string GetOrderDisplay    ( E_OrderConduiteID nOrder );

    static std::string ConvertNatureLevel          ( E_NatureLevelType );
    static std::string ConvertNatureWeapon         ( E_NatureWeaponType );
    static std::string ConvertNatureSpecialization ( E_NatureSpecializationType );
    static std::string ConvertNatureQualifier      ( E_NatureQualifierType );
    static std::string ConvertNatureCategory       ( E_NatureCategoryType );
    static std::string ConvertNatureMobility       ( E_NatureMobilityType );
    static std::string ConvertNatureAtlas          ( E_NatureAtlasType );
    static std::string ConvertMissionCapacity      ( E_NatureCapaciteMissionType );
    static std::string ConvertPerceptionLevel      ( E_PerceptionResult );
    static std::string ConvertEnvironementType     ( MOS_RawVisionData::E_VisionObject );

    static E_LightingType GetLightingFromName      ( const std::string& );

private:
    enum E_ConverterType
    {
        eConverterSim,
        eConverterApp
    };

    //-----------------------------------------------------------------------------
    // Name: struct converter
    // Created: JVT 03-01-08
    //-----------------------------------------------------------------------------
    template < typename type > struct converter
    {
        const type        type_;
        const std::string simname_;
        const std::string appname_;

        converter( const char* simname, const char* appname, type t ) : simname_( simname ), appname_(appname), type_( t ) {};
        converter( const char* simname, type t, const char* appname ) : simname_( simname ), appname_(appname), type_( t ) {};
        converter( const char* simname, type t ) : simname_( simname ), appname_(simname), type_( t ) {};
        converter& operator = ( const converter& rhs ) { simname_ = rhs.name_; appname_(appname); type_ = rhs.type_; return *this;};
    };

    //-----------------------------------------------------------------------------
    // Name: FindInConverter
    // Created: JVT 03-01-08
    // Last modified: JVT 03-04-29
    //-----------------------------------------------------------------------------
    template < typename T > static const T& FindInConverter( const converter<T> conv[], const std::string& str )
    {
        static T badId = (T)-1;

        for ( uint i = 0; ; ++i )
        {
            const std::string& sCurrentName = conv[i].simname_;

            if ( sCurrentName.empty() || sCurrentName == str )
                return conv[i].type_;
        }
        return badId;
    }

    static const std::string& FindInConverter( const converter<std::string> conv[], const std::string& str )
    {
        static std::string badString( "" );

        for ( int i = 0; ;  ++i )
        {
            const std::string& sCurrentName = conv[i].simname_;

            if ( sCurrentName.empty() || sCurrentName == str )
                return conv[i].type_;
        }
        return badString;
    }

    //-----------------------------------------------------------------------------
    // Name: InverseFindInConverter
    // Created: JVT 03-04-29
    //-----------------------------------------------------------------------------
    template < typename T > static const std::string& InverseFindInConverter( const converter<T> conv[], T val ,E_ConverterType e=eConverterSim)
    {
        static std::string badString( "" );

        for ( uint i = 0; !conv[i].simname_.empty(); ++i )
            if ( conv[i].type_ == val )
                switch (e)
                {
                    case eConverterSim:
                        return conv[i].simname_;
                    case eConverterApp:
                        return conv[i].appname_;
                }
        return badString;
    }    
private:
    typedef MOS_Tools::converter< E_MissionID > T_MissionConverter;
    typedef MOS_Tools::converter< E_OrderConduiteID > T_OrderConverter;

    static T_MissionConverter missionConverter_[];
    static T_OrderConverter orderConverter_[];

    static converter< E_NatureLevelType >                 natureLevelConverter_[];
    static converter< E_NatureWeaponType >                natureWeaponConverter_[];
    static converter< E_NatureSpecializationType >        natureSpecializationConverter_[];
    static converter< E_NatureQualifierType >             natureQualifierConverter_[];
    static converter< E_NatureCategoryType >              natureCategoryConverter_[];
    static converter< E_NatureMobilityType >              natureMobilityConverter_[];
    static converter< E_NatureAtlasType >                 natureAtlasConverter_[];
    static converter< E_NatureCapaciteMissionType >       natureCapaciteMissionConverter_[];
    static converter< E_PerceptionResult >                perceptionLevelConverter_[];
    static converter< E_PostureType     >                 postureConverter_[];
    static converter< E_WeatherType >                     weatherConverter_[];
    static converter< E_LightingType >                    lightingConverter_[];
    static converter< MOS_RawVisionData::E_VisionObject > environnementConverter_[];
};

#ifdef MOS_USE_INLINE
#   include "MOS_Tools.inl"
#endif

#endif // __MOS_Tools_h_
