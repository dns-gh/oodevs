//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Tools.h $
// $Author: Age $
// $Modtime: 31/03/05 17:16 $
// $Revision: 13 $
// $Workfile: MOS_Tools.h $
//
//*****************************************************************************

#ifndef __MOS_Tools_h_
#define __MOS_Tools_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
#include "MOS_Order_Def.h"
#include "MOS_Attr_Def.h"
#include "MOS_Perception_Def.h"
#include "MOS_Meteo_ABC.h"
#include "MOS_RawVisionData.h"
#include "MOS_Team.h"

class QListView;


//=============================================================================
// Created: NLD 2003-05-31
//=============================================================================
class MOS_Tools
{
    MT_COPYNOTALLOWED( MOS_Tools );

public:
    //$$$$ QString -> const char*
    static QString ToString( E_UnitPosture nPosture );
    static QString ToString( E_TroopHealthState nState );
    static QString ToString( E_FuncLimaType nType );

    static QString ToString( E_NatureAtlasType nType );
    static QString ToString( E_PerceptionResult nResult );


    static QString ToString( ASN1T_EnumTypeLocalisation nType );
    static QString ToString( ASN1T_EnumMissionGenUrgence nUrgency );
    static QString ToString( ASN1T_EnumMissionGenSousTypeObstacle nType );
    static QString ToString( ASN1T_EnumMissionGenPriorite nObject );

    static QString ToString( MOS_Meteo_ABC::E_WeatherType nType );

    static const char* ToString( const ASN1T_EnumInfoContextErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumCtrlErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumOrderErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumUnitAttrErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumObjectErrorCode& nCode );
    static const char* ToString( const ASN1T_EnumPopulationAttrErrorCode& nCode );


    //$$$$$ bof bof - N'IMPORTE QUOI !
    static const char* ToSymbol( const std::string& strUnitType );
    static void FromString( const char* szString, ASN1T_EnumTypeLocalisation& nId );

    static bool PointNearLine( const MT_Vector2D& point, const T_PointVector& line, MT_Float rMinDist, bool bLineLoop = false );




public:

    static std::string ConvertWeatherType ( E_WeatherType nWeatherType );
    static std::string ConvertLightingType( E_LightingType nLightingType );

    static std::string ConvertNatureAtlas          ( E_NatureAtlasType );
    static std::string ConvertPerceptionLevel      ( E_PerceptionResult );
    static std::string ConvertEnvironementType     ( MOS_RawVisionData::E_VisionObject );

    static std::string ConvertDiplomaty            ( ASN1T_EnumDiplomatie );

    static ASN1T_EnumDiplomatie GetDiplomatyFromName ( const std::string& );
    static E_LightingType GetLightingFromName         ( const std::string& );

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
    static converter< E_NatureAtlasType >                 natureAtlasConverter_[];
    static converter< E_PerceptionResult >                perceptionLevelConverter_[];
    static converter< E_WeatherType >                     weatherConverter_[];
    static converter< E_LightingType >                    lightingConverter_[];
    static converter< MOS_RawVisionData::E_VisionObject > environnementConverter_[];
    static converter< ASN1T_EnumDiplomatie >              diplomatyConverter_[];
};

#endif // __MOS_Tools_h_
