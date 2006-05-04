//*****************************************************************************
//
// $Created: NLD 2003-05-31 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Tools.cpp $
// $Author: Age $
// $Modtime: 27/04/05 18:51 $
// $Revision: 21 $
// $Workfile: Tools.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "Tools.h"

//#include "Meteo_ABC.h"
//#include "RawVisionData.h"
//#include "MT_Tools/MT_Tools_Types.h"

// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
QString Tools::ToString( E_UnitPosture nPosture )
{
    QObject a;
    switch( nPosture )
    {
        case eUnitPosture_PostureMouvement:         return a.tr( "Mouvement" );
        case eUnitPosture_PostureMouvementDiscret:  return a.tr( "MouvementDiscret" );
        case eUnitPosture_PostureArret:             return a.tr( "Arret" );
        case eUnitPosture_PosturePosteReflexe:      return a.tr( "PosteReflexe" );
        case eUnitPosture_PosturePoste:             return a.tr( "Poste" );
        case eUnitPosture_PosturePosteAmenage:      return a.tr( "PosteAmenage" );
        case eUnitPosture_PosturePostePrepareGenie: return a.tr( "PostePrepareGenie" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nState 
    @return 
*/
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
QString Tools::ToString( E_TroopHealthState nState )
{
    QObject a;
    switch( nState )
    {
        case eTroopHealthStateTotal:              return a.tr( "Total" );
        case eTroopHealthStateOperational:        return a.tr( "Opérationnels" );
        case eTroopHealthStateDead:               return a.tr( "Morts" );
        case eTroopHealthStateWounded:            return a.tr( "Blessés" );
        case eTroopHealthStateMentalWounds:       return a.tr( "Blessés mentalement" );
        case eTroopHealthStateContaminated:       return a.tr( "Contaminés" );
        case eTroopHealthStateInTreatment:        return a.tr( "En traitement" );
        case eTroopHealthStateUsedForMaintenance: return a.tr( "Utilisés pour maintenance" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
QString Tools::ToString( E_FuncLimaType nType )
{
    QObject a;
    switch( nType )
    {
        case eLimaFuncLD:     return a.tr( "LD" );
        case eLimaFuncLCA:    return a.tr( "LCA" );
        case eLimaFuncLC:     return a.tr( "LC" );
        case eLimaFuncLI:     return a.tr( "LI" );
        case eLimaFuncLO:     return a.tr( "LO" );
        case eLimaFuncLCAR:   return a.tr( "LCAR" );
        case eLimaFuncLR:     return a.tr( "LR" );
        case eLimaFuncLDM:    return a.tr( "LDM" );
        case eLimaFuncLFM:    return a.tr( "LFM" );
        case eLimaFuncLIA:    return a.tr( "LIA" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
QString Tools::ToString( E_NatureAtlasType nType )
{
    QObject a;
    switch( nType )
    {
        case eNatureAtlas_Armored:          return a.tr( "Blinde" );
        case eNatureAtlas_ASS:              return a.tr( "ASS" );
        case eNatureAtlas_ASA:              return a.tr( "ASA" );
        case eNatureAtlas_PC:               return a.tr( "PC" );
        case eNatureAtlas_Log:              return a.tr( "Log" );
        case eNatureAtlas_Mortar:           return a.tr( "Mortier" );
        case eNatureAtlas_RocketLauncher:   return a.tr( "Lance roquette" );
        case eNatureAtlas_Vehicule:         return a.tr( "Vehicule" );
        case eNatureAtlas_PersonnelAPied:   return a.tr( "Personnel a pied" );
        case eNatureAtlas_Helicoptere:      return a.tr( "Helicoptere" );
        case eNatureAtlas_Indefini:         return a.tr( "Indefini" );
        case eNatureAtlas_None:             return a.tr( "Aucun" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nResult 
    @return 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
QString Tools::ToString( E_PerceptionResult nResult )
{
    QObject a;
    switch( nResult )
    {
        case eIdentification:  return a.tr( "Identification" );
        case eRecognition:     return a.tr( "Reconnaissance" );
        case eDetection:       return a.tr( "Detection" );
        case eNotSeen:         return a.tr( "Non vu" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
QString Tools::ToString( ASN1T_EnumTypeLocalisation nType )
{
    QObject a;
    switch( nType )
    {
        case EnumTypeLocalisation::circle:   return a.tr( "cercle" );
        case EnumTypeLocalisation::ellipse:  return a.tr( "ellipse" );
        case EnumTypeLocalisation::line:     return a.tr( "ligne" );
        case EnumTypeLocalisation::polygon:  return a.tr( "polygone" );
        case EnumTypeLocalisation::point:    return a.tr( "point" );
        case EnumTypeLocalisation::sector:   return a.tr( "secteur" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nUrgency 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString Tools::ToString( ASN1T_EnumMissionGenUrgence nUrgency )
{
    QObject a;
    switch( nUrgency )
    {
        case EnumMissionGenUrgence::serie_minimum:          return a.tr( "Urgence" );
        case EnumMissionGenUrgence::serie_complementaire:   return a.tr( "Complémentaire" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString Tools::ToString( ASN1T_EnumMissionGenSousTypeObstacle nType )
{
    QObject a;
    switch( nType )
    {
        case EnumMissionGenSousTypeObstacle::preliminaire:   return a.tr( "Préliminaire" );
        case EnumMissionGenSousTypeObstacle::de_manoeuvre:   return a.tr( "De manoeuvre" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
/** @param  nObject 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString Tools::ToString( ASN1T_EnumMissionGenPriorite nObject )
{
    QObject a;
    switch( nObject )
    {
        case EnumMissionGenPriorite::prioritaire:     return a.tr( "Prioritaire" );
        case EnumMissionGenPriorite::normal:          return a.tr( "Normal" );
        case EnumMissionGenPriorite::peu_prioritaire: return a.tr( "Peu prioritaire" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
//QString Tools::ToString( Meteo_ABC::E_WeatherType nType )
//{
//    QObject a;
//    switch( nType )
//    {
//        case Meteo_ABC::eWeather_Clear:     return a.tr( "Dégagé" );
//        case Meteo_ABC::eWeather_Storm:     return a.tr( "Tempête" );
//        case Meteo_ABC::eWeather_Fog:       return a.tr( "Brouillard" );
//        case Meteo_ABC::eWeather_Drizzle:   return a.tr( "Crachin" );
//        case Meteo_ABC::eWeather_Rain:      return a.tr( "Pluie" );
//        case Meteo_ABC::eWeather_Snow:      return a.tr( "Neige" );
//        default:
//            assert( false );
//            return "Unknown";
//    }
//}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* Tools::ToString( const ASN1T_EnumInfoContextErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumInfoContextErrorCode::no_error                     : return "Aucune erreur";
        case EnumInfoContextErrorCode::error_invalid_id             : return "ID non valide";
        case EnumInfoContextErrorCode::error_invalid_geometry       : return "Géométrie non valide" ;
        case EnumInfoContextErrorCode::error_invalid_lima_function  : return "Lima non valide" ;
        case EnumInfoContextErrorCode::error_invalid_limit_or_lima_used : return "Lima ou limite non valide";
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* Tools::ToString( const ASN1T_EnumCtrlErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumCtrlErrorCode::no_error                 : return "No error";
        case EnumCtrlErrorCode::error_already_started    : return "Already started";
        case EnumCtrlErrorCode::error_not_started        : return "Not started";
        case EnumCtrlErrorCode::error_not_paused         : return "Not paused";
        case EnumCtrlErrorCode::error_already_paused     : return "Already paused";
        case EnumCtrlErrorCode::error_invalid_time_factor: return "Invalid time factor";

        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* Tools::ToString( const ASN1T_EnumOrderErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumOrderErrorCode::no_error                        : return "No error";
        case EnumOrderErrorCode::error_invalid_unit              : return "Invalid unit";
        case EnumOrderErrorCode::error_invalid_limit             : return "Invalid limit";
        case EnumOrderErrorCode::error_invalid_lima              : return "Invalid lima";
        case EnumOrderErrorCode::error_invalid_mission           : return "Invalid mission";
        case EnumOrderErrorCode::error_invalid_mission_parameters: return "Invalid mission parameters";
        case EnumOrderErrorCode::error_unit_cannot_receive_order : return "Unit cannot receive this order";
        case EnumOrderErrorCode::error_invalid_order_conduite    : return "Invalid ordre de conduite";
        case EnumOrderErrorCode::error_invalid_order_mission     : return "Invalid ordre de mission";
        case EnumOrderErrorCode::error_invalid_order_initial     : return "Invalid ordre initial";
        case EnumOrderErrorCode::error_invalid_order_conduite_parameters: return "Invalid ordre de conduite parameters";
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* Tools::ToString( const ASN1T_EnumUnitAttrErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumUnitAttrErrorCode::no_error              : return "No error";
        case EnumUnitAttrErrorCode::error_invalid_unit    : return "Invalid unit";
        case EnumUnitAttrErrorCode::error_automate_embraye: return "Error : automate embraye";

        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
const char* Tools::ToString( const ASN1T_EnumPopulationAttrErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumPopulationAttrErrorCode::no_error               : return "No error";
        case EnumPopulationAttrErrorCode::error_invalid_unit     : return "Invalid population";
        case EnumPopulationAttrErrorCode::error_invalid_attribute: return "Invalid attribute";

        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* Tools::ToString( const ASN1T_EnumObjectErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumObjectErrorCode::no_error                  : return "No error";
        case EnumObjectErrorCode::error_invalid_object      : return "Invalid object";
        case EnumObjectErrorCode::error_invalid_id          : return "Invalid id";
        case EnumObjectErrorCode::error_invalid_camp        : return "Invalid camp";
        case EnumObjectErrorCode::error_invalid_localisation: return "Invalid localisation";
        case EnumObjectErrorCode::error_missing_specific_attributes: return "Missing specific parameters";
        case EnumObjectErrorCode::error_invalid_specific_attributes: return "Invalid specific parameters";

        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: Tools::FromString
/** @param  szString 
    @param  nId 
    */
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void Tools::FromString( const char* szString, ASN1T_EnumTypeLocalisation& nId )
{
    std::string strType( szString );
    nId = (ASN1T_EnumTypeLocalisation)-1;

    if( strType == "cercle" )       nId = EnumTypeLocalisation::circle;
    if( strType == "ellipse" )      nId = EnumTypeLocalisation::ellipse;
    if( strType == "ligne" )        nId = EnumTypeLocalisation::line;
    if( strType == "polygone" )     nId = EnumTypeLocalisation::polygon;
    if( strType == "point" )        nId = EnumTypeLocalisation::point;
    if( strType == "secteur" )      nId = EnumTypeLocalisation::sector;

    if( nId == (ASN1T_EnumTypeLocalisation)-1 )
    {
        assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: Tools::PointNearLine
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
//bool Tools::PointNearLine( const MT_Vector2D& point, const T_PointVector& line, MT_Float rMinDist, bool bLineLoop )
//{
//    assert( ! line.empty() );
//
//    if( line.size() == 1 )
//        return point.SquareDistance( line.front() ) < (rMinDist * rMinDist);
//
//    for( uint n = 0; n < line.size() - 1; ++n )
//    {
//        MT_Line tmpLine( line[n], line[n+1] );
//        if( tmpLine.IsInside( point, rMinDist ) )
//            return true;
//    }
//
//    if( bLineLoop )
//    {
//        MT_Line tmpLine( line[line.size()-1], line[0] );
//        if( tmpLine.IsInside( point, rMinDist ) )
//            return true;
//    }
//
//    return false;
//}

Tools::converter< E_NatureAtlasType > Tools::natureAtlasConverter_[] = 
{
    converter< E_NatureAtlasType >( "Blinde"          , eNatureAtlas_Armored         ),
    converter< E_NatureAtlasType >( "ASS"             , eNatureAtlas_ASS             ),
    converter< E_NatureAtlasType >( "ASA"             , eNatureAtlas_ASA             ),
    converter< E_NatureAtlasType >( "PC"              , eNatureAtlas_PC              ),
    converter< E_NatureAtlasType >( "Log"             , eNatureAtlas_Log             ),
    converter< E_NatureAtlasType >( "Mortier"         , eNatureAtlas_Mortar          ),
    converter< E_NatureAtlasType >( "Lance roquette"  , eNatureAtlas_RocketLauncher  ),
    converter< E_NatureAtlasType >( "Vehicule"        , eNatureAtlas_Vehicule        ),
    converter< E_NatureAtlasType >( "Personnel a pied", eNatureAtlas_PersonnelAPied  ),
    converter< E_NatureAtlasType >( "Helicoptere"     , eNatureAtlas_Helicoptere     ),
    converter< E_NatureAtlasType >( "Indefini"        , eNatureAtlas_Indefini        ),
    converter< E_NatureAtlasType >( "Aucun"           , eNatureAtlas_None            ),
    converter< E_NatureAtlasType >( ""                , (E_NatureAtlasType)-1        )
};

Tools::converter< E_PerceptionResult > Tools::perceptionLevelConverter_[] = 
{
    converter< E_PerceptionResult >( "Identification", eIdentification       ),
        converter< E_PerceptionResult >( "Reconnaissance", eRecognition          ),
        converter< E_PerceptionResult >( "Detection"     , eDetection            ),
        converter< E_PerceptionResult >( "Non vu"        , eNotSeen              ),
        converter< E_PerceptionResult >( ""              , (E_PerceptionResult)-1 )
};

Tools::converter< E_WeatherType > Tools::weatherConverter_[] =
{
    converter< E_WeatherType >( "PasDePrecipitation", eWeatherTypeNone            ),
        converter< E_WeatherType >( "TempeteDeSable"    , eWeatherTypeRainOrSnowStorm ),
        converter< E_WeatherType >( "Brouillard"        , eWeatherTypeFog             ),
        converter< E_WeatherType >( "Crachin"           , eWeatherTypeDrizzle         ),
        converter< E_WeatherType >( "Pluie"             , eWeatherTypeRain            ),
        converter< E_WeatherType >( "Neige"             , eWeatherTypeSnow            ),
        converter< E_WeatherType >( "Fumigene"          , eWeatherTypeSmoke           ),
        converter< E_WeatherType >( ""                  , (E_WeatherType)-1 )
};

Tools::converter< E_LightingType > Tools::lightingConverter_[] =
{
    converter< E_LightingType >( "JourSansNuage"         , eLightingJourSansNuage          ),
        converter< E_LightingType >( "JourPeuNuageux"        , eLightingJourPeuNuageux         ),
        converter< E_LightingType >( "JourMoyennementNuageux", eLightingJourMoyennementNuageux ),
        converter< E_LightingType >( "JourAssezNuageux"      , eLightingJourAssezNuageux       ),
        converter< E_LightingType >( "JourTresNuageux"       , eLightingJourTresNuageux        ),
        converter< E_LightingType >( "NuitPleineLune"        , eLightingNuitPleineLune         ),
        converter< E_LightingType >( "NuitTroisQuartDeLune"  , eLightingNuitTroisQuartDeLune   ),
        converter< E_LightingType >( "NuitDemiLune"          , eLightingNuitDemiLune           ),
        converter< E_LightingType >( "NuitQuartDeLune"       , eLightingNuitQuartDeLune        ),
        converter< E_LightingType >( "NuitNouvelleLune"      , eLightingNuitNouvelleLune       ),
        converter< E_LightingType >( "Eclairant"             , eLightingEclairant              ),
        converter< E_LightingType >( ""                      , (E_LightingType)-1 )
};

//Tools::converter< RawVisionData::E_VisionObject > Tools::environnementConverter_[] =
//{
//    converter< RawVisionData::E_VisionObject >( "Sol"   , RawVisionData::eVisionGround ),
//    converter< RawVisionData::E_VisionObject >( "Vide"  , RawVisionData::eVisionEmpty  ),
//    converter< RawVisionData::E_VisionObject >( "Foret" , RawVisionData::eVisionForest ),
//    converter< RawVisionData::E_VisionObject >( "Urbain", RawVisionData::eVisionUrban  ),
//    converter< RawVisionData::E_VisionObject >( ""      , (RawVisionData::E_VisionObject)-1 )
//};

Tools::converter< ASN1T_EnumDiplomatie > Tools::diplomatyConverter_[] =
{
    converter< ASN1T_EnumDiplomatie >( "inconnu", EnumDiplomatie::inconnu ),
    converter< ASN1T_EnumDiplomatie >( "ami"    , EnumDiplomatie::ami  ),
    converter< ASN1T_EnumDiplomatie >( "neutre" , EnumDiplomatie::neutre ),
    converter< ASN1T_EnumDiplomatie >( "ennemi" , EnumDiplomatie::ennemi  ),
    converter< ASN1T_EnumDiplomatie >( ""       , (ASN1T_EnumDiplomatie)-1 )
};

// -----------------------------------------------------------------------------
// Name: Tools::ConvertEnvironementType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
//std::string Tools::ConvertEnvironementType( RawVisionData::E_VisionObject nObject )
//{
//    return InverseFindInConverter( environnementConverter_, nObject );
//}

// -----------------------------------------------------------------------------
// Name: Tools::ConvertWeatherType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string Tools::ConvertWeatherType( E_WeatherType nWeatherType )
{
    return InverseFindInConverter( weatherConverter_, nWeatherType );
}

// -----------------------------------------------------------------------------
// Name: Tools::ConvertLightingType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string Tools::ConvertLightingType( E_LightingType nLightingType )
{
    return InverseFindInConverter( lightingConverter_, nLightingType );
}

// -----------------------------------------------------------------------------
// Name: Tools::GetLightingFromName
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
E_LightingType Tools::GetLightingFromName( const std::string& sName )
{
    return FindInConverter( lightingConverter_, sName );
}

// -----------------------------------------------------------------------------
// Name: Tools::ConvertNatureAtlas
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string Tools::ConvertNatureAtlas( E_NatureAtlasType n  )
{
    return InverseFindInConverter( natureAtlasConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: Tools::ConvertPerceptionLevel
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string Tools::ConvertPerceptionLevel( E_PerceptionResult n )
{
    return InverseFindInConverter( perceptionLevelConverter_, n );     
}

// -----------------------------------------------------------------------------
// Name: Tools::ConvertDiplomaty
/** @return .
*/
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
std::string Tools::ConvertDiplomaty( ASN1T_EnumDiplomatie n )
{
    return InverseFindInConverter( diplomatyConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: Tools::GetDiplomatyFromName
/** @return 
*/
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
ASN1T_EnumDiplomatie Tools::GetDiplomatyFromName( const std::string& sName )
{
    return FindInConverter( diplomatyConverter_, sName );
}
