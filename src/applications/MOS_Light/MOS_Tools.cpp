//*****************************************************************************
//
// $Created: NLD 2003-05-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Tools.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 9:59 $
// $Revision: 54 $
// $Workfile: MOS_Tools.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_Tools.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Tools.inl"
#endif

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_UnitPosture nPosture )
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

MOS_Tools::converter< E_NatureAtlasType > MOS_Tools::natureAtlasConverter_[] = 
{
    converter< E_NatureAtlasType >( "Blinde"          , eNatureAtlas_Blinde          ),
    converter< E_NatureAtlasType >( "ASS"             , eNatureAtlas_ASS             ),
    converter< E_NatureAtlasType >( "PC"              , eNatureAtlas_PC              ),
    converter< E_NatureAtlasType >( "Log"             , eNatureAtlas_Log             ),
    converter< E_NatureAtlasType >( "Mortier"         , eNatureAtlas_Mortier         ),
    converter< E_NatureAtlasType >( "Lance roquette"  , eNatureAtlas_LanceRoquette   ),
    converter< E_NatureAtlasType >( "Vehicule"        , eNatureAtlas_Vehicule        ),
    converter< E_NatureAtlasType >( "Personnel a pied", eNatureAtlas_PersonnelAPied  ),
    converter< E_NatureAtlasType >( "Helicoptere"     , eNatureAtlas_Helicoptere     ),
    converter< E_NatureAtlasType >( "Indefini"        , eNatureAtlas_Indefini        ),
    converter< E_NatureAtlasType >( "None"            , eNatureAtlas_None            ),
    converter< E_NatureAtlasType >( ""                , (E_NatureAtlasType)-1        )
};

MOS_Tools::converter< E_PerceptionResult > MOS_Tools::perceptionLevelConverter_[] = 
{
    converter< E_PerceptionResult >( "Identification", eIdentification       ),
    converter< E_PerceptionResult >( "Reconnaissance", eRecognition          ),
    converter< E_PerceptionResult >( "Detection"     , eDetection            ),
    converter< E_PerceptionResult >( "Non vu"        , eNotSeen              ),
    converter< E_PerceptionResult >( ""              , (E_PerceptionResult)-1 )
};

MOS_Tools::converter< E_WeatherType > MOS_Tools::weatherConverter_[] =
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

MOS_Tools::converter< E_LightingType > MOS_Tools::lightingConverter_[] =
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

MOS_Tools::converter< MOS_RawVisionData::E_VisionObject > MOS_Tools::environnementConverter_[] =
{
    converter< MOS_RawVisionData::E_VisionObject >( "Sol"   , MOS_RawVisionData::eVisionGround ),
    converter< MOS_RawVisionData::E_VisionObject >( "Vide"  , MOS_RawVisionData::eVisionEmpty  ),
    converter< MOS_RawVisionData::E_VisionObject >( "Foret" , MOS_RawVisionData::eVisionForest ),
    converter< MOS_RawVisionData::E_VisionObject >( "Urbain", MOS_RawVisionData::eVisionUrban  ),
    converter< MOS_RawVisionData::E_VisionObject >( ""      , (MOS_RawVisionData::E_VisionObject)-1 )
};

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertEnvironementType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertEnvironementType( MOS_RawVisionData::E_VisionObject nObject )
{
    return InverseFindInConverter( environnementConverter_, nObject );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertWeatherType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertWeatherType( E_WeatherType nWeatherType )
{
    return InverseFindInConverter( weatherConverter_, nWeatherType );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertLightingType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertLightingType( E_LightingType nLightingType )
{
    return InverseFindInConverter( lightingConverter_, nLightingType );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::GetLightingFromName
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
E_LightingType MOS_Tools::GetLightingFromName( const std::string& sName )
{
    return FindInConverter( lightingConverter_, sName );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureAtlas
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureAtlas( E_NatureAtlasType n  )
{
    return InverseFindInConverter( natureAtlasConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertPerceptionLevel
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertPerceptionLevel( E_PerceptionResult n )
{
    return InverseFindInConverter( perceptionLevelConverter_, n );     
}

