// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ENT_Tr.h"
#include "ENT_Private.h"

#include "protocol/Simulation.h"

#include <tools/Helpers.h>
#include <boost/static_assert.hpp>

namespace
{
// Typedefs
typedef ENT_Tr::Converter< E_LocationType > T_ConverterLocationType;
typedef ENT_Tr::Converter< E_AmmunitionType > T_ConverterAmmunitionType;
typedef ENT_Tr::Converter< E_NatureLevel > T_ConverterNatureLevel;
typedef ENT_Tr::Converter< E_Diplomacy > T_ConverterDiplomacy;
typedef ENT_Tr::Converter< E_ForceRatioStatus > T_ConverterForceRatioStatus;
typedef ENT_Tr::Converter< E_MeetingEngagementStatus > T_ConverterMeetingEngagementStatus;
typedef ENT_Tr::Converter< E_OperationalStatus > T_ConverterOperationalStatus;
typedef ENT_Tr::Converter< E_Roe > T_ConverterRoe;
typedef ENT_Tr::Converter< E_UnitPosture > T_ConverterUnitPosture;
typedef ENT_Tr::Converter< E_FireAvailability > T_ConverterFireAvailability;
typedef ENT_Tr::Converter< E_PopulationRoe > T_ConverterPopulationRoe;
typedef ENT_Tr::Converter< E_UnitTiredness > T_ConverterUnitTiredness;
typedef ENT_Tr::Converter< E_UnitMorale > T_ConverterUnitMorale;
typedef ENT_Tr::Converter< E_UnitExperience > T_ConverterUnitExperience;
typedef ENT_Tr::Converter< E_UnitStress > T_ConverterUnitStress;
typedef ENT_Tr::Converter< E_LightingType > T_ConverterLightingType;
typedef ENT_Tr::Converter< E_WeatherType > T_ConverterWeatherType;
typedef ENT_Tr::Converter< E_ObstacleActivation > T_ConverterObstacleActivation;
typedef ENT_Tr::Converter< E_PopulationAttitude > T_ConverterPopulationAttitude;
typedef ENT_Tr::Converter< E_Location > T_ConverterLocation;
typedef ENT_Tr::Converter< E_CrossingType > T_ConverterCrossingType;
typedef ENT_Tr::Converter< E_HumanWound > T_ConverterHumanWound;
typedef ENT_Tr::Converter< E_HumanRank > T_ConverterHumanRank;
typedef ENT_Tr::Converter< E_HumanState > T_ConverterHumanState;
typedef ENT_Tr::Converter< E_HumanLocation > T_ConverterHumanLocation;
typedef ENT_Tr::Converter< E_EquipmentState > T_ConverterEquipmentState;
typedef ENT_Tr::Converter< E_InjuriesSeriousness > T_ConverterInjuriesSeriousness;
typedef ENT_Tr::Converter< E_BreakdownType > T_ConverterBreakdownType;
typedef ENT_Tr::Converter< E_BreakdownNTI > T_ConverterBreakdownNTI;
typedef ENT_Tr::Converter< E_GhostType > T_ConverterGhostType;
typedef ENT_Tr::Converter< E_NbcState > T_ConverterNbcState;
typedef ENT_Tr::Converter< E_Modes > T_ConverterModes;
typedef ENT_Tr::Converter< E_AgentNbcSuit > T_ConverterAgentNbcSuit;
typedef ENT_Tr::Converter< E_AviationRange > T_ConverterAviationRange;
typedef ENT_Tr::Converter< E_LayerTypes > T_ConverterLayerTypes;
typedef ENT_Tr::Converter< E_EventTypes > T_ConverterEventTypes;
typedef ENT_Tr::Converter< E_MissionType > T_ConverterMissionType;
typedef ENT_Tr::Converter< E_MissionType > T_ConverterMissionType;
typedef ENT_Tr::Converter< E_EventDockModes > T_ConverterEventDockModes;
typedef ENT_Tr::Converter< E_CoordinateSystem > T_ConverterCoordinateSystem;

typedef ENT_Tr::Converter< sword::MagicAction::Type > T_ConverterMagicActionType;
typedef ENT_Tr::Converter< sword::UnitMagicAction::Type > T_ConverterUnitMagicActionType;
typedef ENT_Tr::Converter< sword::KnowledgeMagicAction::Type > T_ConverterKnowledgeMagicActionType;
typedef ENT_Tr::Converter< sword::ObjectMagicAction::Type > T_ConverterObjectMagicActionType;
typedef ENT_Tr::Converter< sword::EnumLogisticLevel > T_ConverterLogisticLevel;
typedef ENT_Tr::Converter< sword::LogFuneralHandlingUpdate_EnumLogFuneralHandlingStatus > T_ConverterLogFuneralHandlingStatus;
typedef ENT_Tr::Converter< sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus > T_ConverterLogMaintenanceHandlingStatus;
typedef ENT_Tr::Converter< sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus > T_ConverterLogMedicalHandlingStatus;
typedef ENT_Tr::Converter< sword::LogSupplyHandlingUpdate_EnumLogSupplyHandlingStatus > T_ConverterLogSupplyHandlingStatus;
typedef ENT_Tr::Converter< sword::DotationType > T_ConverterDotationType;

T_ConverterLocationType LocationTypeConverter_[] =
{
    T_ConverterLocationType( "circle", QT_TRANSLATE_NOOP( "ENT_Tr", "circle" ), eLocationType_Circle ),
    T_ConverterLocationType( "ellipse", QT_TRANSLATE_NOOP( "ENT_Tr", "ellipse" ), eLocationType_Ellipse ),
    T_ConverterLocationType( "line", QT_TRANSLATE_NOOP( "ENT_Tr", "line" ), eLocationType_Line ),
    T_ConverterLocationType( "rectangle", QT_TRANSLATE_NOOP( "ENT_Tr", "rectangle" ), eLocationType_Rectangle ),
    T_ConverterLocationType( "polygon", QT_TRANSLATE_NOOP( "ENT_Tr", "polygon" ), eLocationType_Polygon ),
    T_ConverterLocationType( "point", QT_TRANSLATE_NOOP( "ENT_Tr", "point" ), eLocationType_Point ),
    T_ConverterLocationType( "sector", QT_TRANSLATE_NOOP( "ENT_Tr", "sector" ), eLocationType_Sector ),
    T_ConverterLocationType( "", "", (E_LocationType)-1 )
};

T_ConverterAmmunitionType AmmunitionTypeConverter_[] =
{
    T_ConverterAmmunitionType( "Obus",       QT_TRANSLATE_NOOP( "ENT_Tr", "Shell" ),       eAmmunitionType_Obus ),
    T_ConverterAmmunitionType( "MissileSol", QT_TRANSLATE_NOOP( "ENT_Tr", "Missile" ),     eAmmunitionType_MissileSol ),
    T_ConverterAmmunitionType( "MissileAir", QT_TRANSLATE_NOOP( "ENT_Tr", "Air missile" ), eAmmunitionType_MissileAir ),
    T_ConverterAmmunitionType( "Mitraille",  QT_TRANSLATE_NOOP( "ENT_Tr", "Bullet" ),      eAmmunitionType_Mitraille ),
    T_ConverterAmmunitionType( "ALR",        QT_TRANSLATE_NOOP( "ENT_Tr", "ALR" ),         eAmmunitionType_ALR ),
    T_ConverterAmmunitionType( "", "", (E_AmmunitionType)-1 )
};

T_ConverterNatureLevel NatureLevelConverter_[] =
{
    T_ConverterNatureLevel( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eNatureLevel_None ),
    T_ConverterNatureLevel( "b", QT_TRANSLATE_NOOP( "ENT_Tr", "b" ), eNatureLevel_b ),
    T_ConverterNatureLevel( "o", QT_TRANSLATE_NOOP( "ENT_Tr", "o" ), eNatureLevel_o ),
    T_ConverterNatureLevel( "c", QT_TRANSLATE_NOOP( "ENT_Tr", "c" ), eNatureLevel_c ),
    T_ConverterNatureLevel( "oo", QT_TRANSLATE_NOOP( "ENT_Tr", "oo" ), eNatureLevel_oo ),
    T_ConverterNatureLevel( "ooo", QT_TRANSLATE_NOOP( "ENT_Tr", "ooo" ), eNatureLevel_ooo ),
    T_ConverterNatureLevel( "i", QT_TRANSLATE_NOOP( "ENT_Tr", "i" ), eNatureLevel_i ),
    T_ConverterNatureLevel( "ii", QT_TRANSLATE_NOOP( "ENT_Tr", "ii" ), eNatureLevel_ii ),
    T_ConverterNatureLevel( "iii", QT_TRANSLATE_NOOP( "ENT_Tr", "iii" ), eNatureLevel_iii ),
    T_ConverterNatureLevel( "x", QT_TRANSLATE_NOOP( "ENT_Tr", "x" ), eNatureLevel_x ),
    T_ConverterNatureLevel( "xx", QT_TRANSLATE_NOOP( "ENT_Tr", "xx" ), eNatureLevel_xx ),
    T_ConverterNatureLevel( "xxx", QT_TRANSLATE_NOOP( "ENT_Tr", "xxx" ), eNatureLevel_xxx ),
    T_ConverterNatureLevel( "xxxx", QT_TRANSLATE_NOOP( "ENT_Tr", "xxxx" ), eNatureLevel_xxxx ),
    T_ConverterNatureLevel( "xxxxx", QT_TRANSLATE_NOOP( "ENT_Tr", "xxxxx" ), eNatureLevel_xxxxx ),
    T_ConverterNatureLevel( "", "", (E_NatureLevel)-1 )
};

T_ConverterDiplomacy DiplomacyConverter_[] =
{
    T_ConverterDiplomacy( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "unknown" ), eDiplomacy_Inconnu ),
    T_ConverterDiplomacy( "ami", QT_TRANSLATE_NOOP( "ENT_Tr", "friendly" ), eDiplomacy_Ami ),
    T_ConverterDiplomacy( "ennemi", QT_TRANSLATE_NOOP( "ENT_Tr", "enemy" ), eDiplomacy_Ennemi ),
    T_ConverterDiplomacy( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutral" ), eDiplomacy_Neutre ),
    T_ConverterDiplomacy( "", "", (E_Diplomacy)-1 )
};

T_ConverterForceRatioStatus ForceRatioStatusConverter_[] =
{
    T_ConverterForceRatioStatus( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eForceRatioStatus_None ),
    T_ConverterForceRatioStatus( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "even" ), eForceRatioStatus_Neutre ),
    T_ConverterForceRatioStatus( "favorable", QT_TRANSLATE_NOOP( "ENT_Tr", "high" ), eForceRatioStatus_Favorable ),
    T_ConverterForceRatioStatus( "defavorable", QT_TRANSLATE_NOOP( "ENT_Tr", "low" ), eForceRatioStatus_Defavorable ),
    T_ConverterForceRatioStatus( "", "", (E_ForceRatioStatus)-1 )
};

T_ConverterMeetingEngagementStatus MeetingEngagementStatusConverter_[] =
{
    T_ConverterMeetingEngagementStatus( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eMeetingEngagementStatus_None ),
    T_ConverterMeetingEngagementStatus( "etat esquive", QT_TRANSLATE_NOOP( "ENT_Tr", "withdrawing" ), eMeetingEngagementStatus_EtatEsquive ),
    T_ConverterMeetingEngagementStatus( "etat fixe", QT_TRANSLATE_NOOP( "ENT_Tr", "fixed" ), eMeetingEngagementStatus_EtatFixe ),
    T_ConverterMeetingEngagementStatus( "etat poursuite mission", QT_TRANSLATE_NOOP( "ENT_Tr", "carrying out mission" ), eMeetingEngagementStatus_EtatPoursuiteMission ),
    T_ConverterMeetingEngagementStatus( "", "", (E_MeetingEngagementStatus)-1 )
};

T_ConverterOperationalStatus OperationalStatusConverter_[] =
{
    T_ConverterOperationalStatus( "detruit totalement", QT_TRANSLATE_NOOP( "ENT_Tr", "totally destroyed" ), eOperationalStatus_DetruitTotalement ),
    T_ConverterOperationalStatus( "detruit tactiquement", QT_TRANSLATE_NOOP( "ENT_Tr", "tactically destroyed" ), eOperationalStatus_DetruitTactiquement ),
    T_ConverterOperationalStatus( "operationnel", QT_TRANSLATE_NOOP( "ENT_Tr", "operational" ), eOperationalStatus_Operationnel ),
    T_ConverterOperationalStatus( "", "", (E_OperationalStatus)-1 )
};

T_ConverterRoe RoeConverter_[] =
{
    T_ConverterRoe( "tir libre", QT_TRANSLATE_NOOP( "ENT_Tr", "free fire" ), eRoe_TirLibre ),
    T_ConverterRoe( "tir sur riposte", QT_TRANSLATE_NOOP( "ENT_Tr", "retaliation only" ), eRoe_TirSurRiposte ),
    T_ConverterRoe( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "forbidden fire" ), eRoe_TirInterdit ),
    T_ConverterRoe( "", "", (E_Roe)-1 )
};

T_ConverterUnitPosture UnitPostureConverter_[] =
{
    T_ConverterUnitPosture( "posture mouvement", QT_TRANSLATE_NOOP( "ENT_Tr", "moving" ), eUnitPosture_PostureMouvement ),
    T_ConverterUnitPosture( "posture mouvement discret", QT_TRANSLATE_NOOP( "ENT_Tr", "infiltrating" ), eUnitPosture_PostureMouvementDiscret ),
    T_ConverterUnitPosture( "posture arret", QT_TRANSLATE_NOOP( "ENT_Tr", "stopping" ), eUnitPosture_PostureArret ),
    T_ConverterUnitPosture( "posture poste reflexe", QT_TRANSLATE_NOOP( "ENT_Tr", "short stopping" ), eUnitPosture_PosturePosteReflexe ),
    T_ConverterUnitPosture( "posture poste", QT_TRANSLATE_NOOP( "ENT_Tr", "parked" ), eUnitPosture_PosturePoste ),
    T_ConverterUnitPosture( "posture poste amenage", QT_TRANSLATE_NOOP( "ENT_Tr", "parked on self-prepared area" ), eUnitPosture_PosturePosteAmenage ),
    T_ConverterUnitPosture( "posture poste prepare genie", QT_TRANSLATE_NOOP( "ENT_Tr", "parked on engineer-prepared area" ), eUnitPosture_PosturePostePrepareGenie ),
    T_ConverterUnitPosture( "", "", (E_UnitPosture)-1 )
};

T_ConverterFireAvailability FireAvailabilityConverter_[] =
{
    T_ConverterFireAvailability( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eFireAvailability_None ),
    T_ConverterFireAvailability( "pret au tir", QT_TRANSLATE_NOOP( "ENT_Tr", "ready to fire" ), eFireAvailability_PretAuTir ),
    T_ConverterFireAvailability( "indisponible", QT_TRANSLATE_NOOP( "ENT_Tr", "unavailable" ), eFireAvailability_Indisponible ),
    T_ConverterFireAvailability( "", "", (E_FireAvailability)-1 )
};

T_ConverterPopulationRoe PopulationRoeConverter_[] =
{
    T_ConverterPopulationRoe( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), ePopulationRoe_None ),
    T_ConverterPopulationRoe( "emploi force interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "force shall not be used" ), ePopulationRoe_EmploiForceInterdit ),
    T_ConverterPopulationRoe( "maintien a distance par moyens non letaux", QT_TRANSLATE_NOOP( "ENT_Tr", "non lethal weapons use allowed" ), ePopulationRoe_MaintienADistanceParMoyensNonLetaux ),
    T_ConverterPopulationRoe( "armes letales autorisees", QT_TRANSLATE_NOOP( "ENT_Tr", "lethal weapon use allowed" ), ePopulationRoe_ArmesLetalesAutorisees ),
    T_ConverterPopulationRoe( "", "", (E_PopulationRoe)-1 )
};

T_ConverterUnitTiredness UnitTirednessConverter_[] =
{
    T_ConverterUnitTiredness( "normal", QT_TRANSLATE_NOOP( "ENT_Tr", "not tired" ), eUnitTiredness_Normal ),
    T_ConverterUnitTiredness( "fatigue", QT_TRANSLATE_NOOP( "ENT_Tr", "tired" ), eUnitTiredness_Fatigue ),
    T_ConverterUnitTiredness( "epuise", QT_TRANSLATE_NOOP( "ENT_Tr", "exhausted" ), eUnitTiredness_Epuise ),
    T_ConverterUnitTiredness( "", "", (E_UnitTiredness)-1 )
};

T_ConverterUnitMorale UnitMoraleConverter_[] =
{
    T_ConverterUnitMorale( "fanatique", QT_TRANSLATE_NOOP( "ENT_Tr", "fanatical" ), eUnitMorale_Fanatique ),
    T_ConverterUnitMorale( "bon", QT_TRANSLATE_NOOP( "ENT_Tr", "high" ), eUnitMorale_Bon ),
    T_ConverterUnitMorale( "moyen", QT_TRANSLATE_NOOP( "ENT_Tr", "standard" ), eUnitMorale_Moyen ),
    T_ConverterUnitMorale( "mauvais", QT_TRANSLATE_NOOP( "ENT_Tr", "low" ), eUnitMorale_Mauvais ),
    T_ConverterUnitMorale( "", "", (E_UnitMorale)-1 )
};

T_ConverterUnitExperience UnitExperienceConverter_[] =
{
    T_ConverterUnitExperience( "veteran", QT_TRANSLATE_NOOP( "ENT_Tr", "veteran" ), eUnitExperience_Veteran ),
    T_ConverterUnitExperience( "experimente", QT_TRANSLATE_NOOP( "ENT_Tr", "experienced" ), eUnitExperience_Experimente ),
    T_ConverterUnitExperience( "conscrit", QT_TRANSLATE_NOOP( "ENT_Tr", "newbie" ), eUnitExperience_Conscrit ),
    T_ConverterUnitExperience( "", "", (E_UnitExperience)-1 )
};

T_ConverterUnitStress UnitStressConverter_[] =
{
    T_ConverterUnitStress( "calm", QT_TRANSLATE_NOOP( "ENT_Tr", "calm" ), eUnitStress_Calm ),
    T_ConverterUnitStress( "worried", QT_TRANSLATE_NOOP( "ENT_Tr", "worried" ), eUnitStress_Worried ),
    T_ConverterUnitStress( "stressed", QT_TRANSLATE_NOOP( "ENT_Tr", "stressed" ), eUnitStress_Stressed ),
    T_ConverterUnitStress( "", "", (E_UnitStress)-1 )
};

T_ConverterLightingType LightingTypeConverter_[] =
{
    T_ConverterLightingType( "JourSansNuage"         , QT_TRANSLATE_NOOP( "ENT_Tr", "Very sunny day" ),      eLightingType_JourSansNuage ),
    T_ConverterLightingType( "JourPeuNuageux"        , QT_TRANSLATE_NOOP( "ENT_Tr", "Sunny day" ),           eLightingType_JourPeuNuageux ),
    T_ConverterLightingType( "JourMoyennementNuageux", QT_TRANSLATE_NOOP( "ENT_Tr", "Partly cloudy day" ),   eLightingType_JourMoyennementNuageux ),
    T_ConverterLightingType( "JourAssezNuageux"      , QT_TRANSLATE_NOOP( "ENT_Tr", "Cloudy day" ),          eLightingType_JourAssezNuageux ),
    T_ConverterLightingType( "JourTresNuageux"       , QT_TRANSLATE_NOOP( "ENT_Tr", "Very cloudy day" ),     eLightingType_JourTresNuageux ),
    T_ConverterLightingType( "NuitPleineLune"        , QT_TRANSLATE_NOOP( "ENT_Tr", "Full moon night" ),     eLightingType_NuitPleineLune ),
    T_ConverterLightingType( "NuitTroisQuartDeLune"  , QT_TRANSLATE_NOOP( "ENT_Tr", "Waning moon night" ),   eLightingType_NuitTroisQuartDeLune ),
    T_ConverterLightingType( "NuitDemiLune"          , QT_TRANSLATE_NOOP( "ENT_Tr", "Quarter moon night" ),  eLightingType_NuitDemiLune ),
    T_ConverterLightingType( "NuitQuartDeLune"       , QT_TRANSLATE_NOOP( "ENT_Tr", "Crescent moon night" ), eLightingType_NuitQuartDeLune ),
    T_ConverterLightingType( "NuitNouvelleLune"      , QT_TRANSLATE_NOOP( "ENT_Tr", "New moon night" ),      eLightingType_NuitNouvelleLune ),
    T_ConverterLightingType( "Eclairant"             , QT_TRANSLATE_NOOP( "ENT_Tr", "Illumination shell" ),  eLightingType_Eclairant ),
    T_ConverterLightingType( "", "", (E_LightingType)-1 )
};

T_ConverterWeatherType WeatherTypeConverter_[] =
{
    T_ConverterWeatherType( "pas de precipitation", QT_TRANSLATE_NOOP( "ENT_Tr", "No rainfall" ), eWeatherType_None ),
    T_ConverterWeatherType( "tempete sable", QT_TRANSLATE_NOOP( "ENT_Tr", "Sand storm" ), eWeatherType_SandStorm ),
    T_ConverterWeatherType( "brouillard", QT_TRANSLATE_NOOP( "ENT_Tr", "Fog" ), eWeatherType_Fog ),
    T_ConverterWeatherType( "crachin", QT_TRANSLATE_NOOP( "ENT_Tr", "Drizzle" ), eWeatherType_Drizzle ),
    T_ConverterWeatherType( "pluie", QT_TRANSLATE_NOOP( "ENT_Tr", "Rain" ), eWeatherType_Rain ),
    T_ConverterWeatherType( "neige", QT_TRANSLATE_NOOP( "ENT_Tr", "Snow" ), eWeatherType_Snow ),
    T_ConverterWeatherType( "fumigene", QT_TRANSLATE_NOOP( "ENT_Tr", "Smoke" ), eWeatherType_Smoke ),
    T_ConverterWeatherType( "", "", (E_WeatherType)-1 )
};

T_ConverterObstacleActivation ObstacleActivationConverter_[] =
{
    T_ConverterObstacleActivation( "activated", QT_TRANSLATE_NOOP( "ENT_Tr", "Activated" ), eObstacleActivation_Activated ),
    T_ConverterObstacleActivation( "deactivated", QT_TRANSLATE_NOOP( "ENT_Tr", "Deactivated" ), eObstacleActivation_Deactivated ),
    T_ConverterObstacleActivation( "", "", (E_ObstacleActivation)-1 )
};

T_ConverterPopulationAttitude PopulationAttitudeConverter_[] =
{
    T_ConverterPopulationAttitude( "calme", QT_TRANSLATE_NOOP( "ENT_Tr", "quiet" ), ePopulationAttitude_Calme ),
    T_ConverterPopulationAttitude( "agitee", QT_TRANSLATE_NOOP( "ENT_Tr", "agitated" ), ePopulationAttitude_Agitee ),
    T_ConverterPopulationAttitude( "excitee", QT_TRANSLATE_NOOP( "ENT_Tr", "nervous" ), ePopulationAttitude_Excitee ),
    T_ConverterPopulationAttitude( "agressive", QT_TRANSLATE_NOOP( "ENT_Tr", "agressive" ), ePopulationAttitude_Agressive ),
    T_ConverterPopulationAttitude( "", "", (E_PopulationAttitude)-1 )
};

T_ConverterLocation LocationConverter_[] =
{
    T_ConverterLocation( "unknown",        QT_TRANSLATE_NOOP( "ENT_Tr", "unknown" ),        eLocation_Unknown            ),
    T_ConverterLocation( "forest",         QT_TRANSLATE_NOOP( "ENT_Tr", "forest" ),         eLocation_Forest             ),
    T_ConverterLocation( "orchard",        QT_TRANSLATE_NOOP( "ENT_Tr", "orchard" ),        eLocation_Plantation         ),
    T_ConverterLocation( "swamp",          QT_TRANSLATE_NOOP( "ENT_Tr", "swamp" ),          eLocation_Swamp              ),
    T_ConverterLocation( "dune",           QT_TRANSLATE_NOOP( "ENT_Tr", "dune" ),           eLocation_Dune               ),
    T_ConverterLocation( "urban area",     QT_TRANSLATE_NOOP( "ENT_Tr", "urban area" ),     eLocation_Urban              ),
    T_ConverterLocation( "lake",           QT_TRANSLATE_NOOP( "ENT_Tr", "lake" ),           eLocation_Water              ),
    T_ConverterLocation( "glacier",        QT_TRANSLATE_NOOP( "ENT_Tr", "glacier" ),        eLocation_Ice                ),
    T_ConverterLocation( "forest edge",    QT_TRANSLATE_NOOP( "ENT_Tr", "forest edge" ),    eLocation_ForestBorder       ),
    T_ConverterLocation( "suburb",         QT_TRANSLATE_NOOP( "ENT_Tr", "suburb" ),         eLocation_UrbanBorder        ),
    T_ConverterLocation( "waterfront",     QT_TRANSLATE_NOOP( "ENT_Tr", "waterfront" ),     eLocation_WaterBorder        ),
    T_ConverterLocation( "orchard edge",   QT_TRANSLATE_NOOP( "ENT_Tr", "orchard edge" ),   eLocation_PlantationBorder   ),
    T_ConverterLocation( "swamp edge",     QT_TRANSLATE_NOOP( "ENT_Tr", "swamp edge" ),     eLocation_SwampBorder        ),
    T_ConverterLocation( "dune edge",      QT_TRANSLATE_NOOP( "ENT_Tr", "dune edge" ),      eLocation_DuneBorder         ),
    T_ConverterLocation( "glacier edge",   QT_TRANSLATE_NOOP( "ENT_Tr", "glacier edge" ),   eLocation_IceBorder          ),
    T_ConverterLocation( "country road",   QT_TRANSLATE_NOOP( "ENT_Tr", "country road" ),   eLocation_SmallRoad          ),
    T_ConverterLocation( "highway",        QT_TRANSLATE_NOOP( "ENT_Tr", "highway" ),        eLocation_Motorway           ),
    T_ConverterLocation( "main road",      QT_TRANSLATE_NOOP( "ENT_Tr", "main road" ),      eLocation_LargeRoad          ),
    T_ConverterLocation( "secondary road", QT_TRANSLATE_NOOP( "ENT_Tr", "secondary road" ), eLocation_MediumRoad         ),
    T_ConverterLocation( "stream",         QT_TRANSLATE_NOOP( "ENT_Tr", "stream" ),         eLocation_SmallRiver         ),
    T_ConverterLocation( "river",          QT_TRANSLATE_NOOP( "ENT_Tr", "river" ),          eLocation_MediumRiver        ),
    T_ConverterLocation( "main river",     QT_TRANSLATE_NOOP( "ENT_Tr", "main river" ),     eLocation_LargeRiver         ),
    T_ConverterLocation( "bridge",         QT_TRANSLATE_NOOP( "ENT_Tr", "bridge" ),         eLocation_Bridge             ),
    T_ConverterLocation( "cliff",          QT_TRANSLATE_NOOP( "ENT_Tr", "cliff" ),          eLocation_Cliff              ),
    T_ConverterLocation( "railroad",       QT_TRANSLATE_NOOP( "ENT_Tr", "railroad" ),       eLocation_Railroad           ),
    T_ConverterLocation( "crossroad",      QT_TRANSLATE_NOOP( "ENT_Tr", "crossroad" ),      eLocation_Crossroad          ),
    T_ConverterLocation( "mountain"  ,     QT_TRANSLATE_NOOP( "ENT_Tr", "mountain" )  ,     eLocation_Mountain           ),
    T_ConverterLocation( "mountain edge",  QT_TRANSLATE_NOOP( "ENT_Tr", "mountain edge" ),  eLocation_MountainBorder     ),
    T_ConverterLocation( "", "", (E_Location)-1 )
};

T_ConverterCrossingType CrossingTypeConverter_[] =
{
    T_ConverterCrossingType( "bridge"        , QT_TRANSLATE_NOOP( "ENT_Tr", "bridge" )        , eBridge ),
    T_ConverterCrossingType( "highway"       , QT_TRANSLATE_NOOP( "ENT_Tr", "highway" )       , eHighway ),
    T_ConverterCrossingType( "main road"     , QT_TRANSLATE_NOOP( "ENT_Tr", "main road" )     , eMainRoad ),
    T_ConverterCrossingType( "secondary road", QT_TRANSLATE_NOOP( "ENT_Tr", "secondary road" ), eSecondaryRoad ),
    T_ConverterCrossingType( "country road"  , QT_TRANSLATE_NOOP( "ENT_Tr", "country road" )  , eCountryRoad ),
    T_ConverterCrossingType( ""       , QT_TRANSLATE_NOOP( "ENT_Tr", "" )              , eNothing ),
    T_ConverterCrossingType( "", "", ( E_CrossingType ) - 1 )
};

T_ConverterHumanWound HumanWoundConverter_[] =
{
    T_ConverterHumanWound( "non blesse", QT_TRANSLATE_NOOP( "ENT_Tr", "unwounded" ), eHumanWound_NonBlesse ),
    T_ConverterHumanWound( "mort", QT_TRANSLATE_NOOP( "ENT_Tr", "dead" ), eHumanWound_Mort ),
    T_ConverterHumanWound( "blesse urgence 1", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded seriousness level 1" ), eHumanWound_BlesseUrgence1 ),
    T_ConverterHumanWound( "blesse urgence 2", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded seriousness level 2" ), eHumanWound_BlesseUrgence2 ),
    T_ConverterHumanWound( "blesse urgence 3", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded seriousness level 3" ), eHumanWound_BlesseUrgence3 ),
    T_ConverterHumanWound( "blesse urgence extreme", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded extreme seriousness" ), eHumanWound_BlesseUrgenceExtreme ),
    T_ConverterHumanWound( "", "", (E_HumanWound)-1 )
};

T_ConverterHumanRank HumanRankConverter_[] =
{
    T_ConverterHumanRank( "Officier", QT_TRANSLATE_NOOP( "ENT_Tr", "Officer" ), eHumanRank_Officier ),
    T_ConverterHumanRank( "SousOfficier", QT_TRANSLATE_NOOP( "ENT_Tr", "Warrant officer" ), eHumanRank_SousOfficer ),
    T_ConverterHumanRank( "MilitaireDuRang", QT_TRANSLATE_NOOP( "ENT_Tr", "Trooper" ), eHumanRank_Mdr ),
    T_ConverterHumanRank( "", "", (E_HumanRank)-1 )
};

T_ConverterHumanState HumanStateConverter_ [] =
{
    T_ConverterHumanState( "healthy", QT_TRANSLATE_NOOP( "ENT_Tr", "Healthy" ), eHumanState_Healthy ),
    T_ConverterHumanState( "injured", QT_TRANSLATE_NOOP( "ENT_Tr", "Injured" ), eHumanState_Injured ),
    T_ConverterHumanState( "dead",    QT_TRANSLATE_NOOP( "ENT_Tr", "Dead" ), eHumanState_Dead ),
    T_ConverterHumanState( "", "", ( E_HumanState ) - 1 )
};

T_ConverterHumanLocation HumanLocationConverter_ [] =
{
    T_ConverterHumanLocation( "battlefield", QT_TRANSLATE_NOOP( "ENT_Tr", "On battlefield" ), eHumanLocation_Battlefield ),
    T_ConverterHumanLocation( "maintenance", QT_TRANSLATE_NOOP( "ENT_Tr", "Assigned to maintenance" ), eHumanLocation_Maintenance ),
    T_ConverterHumanLocation( "medical",     QT_TRANSLATE_NOOP( "ENT_Tr", "In medical treatment" ), eHumanLocation_Medical ),
    T_ConverterHumanLocation( "funeral",     QT_TRANSLATE_NOOP( "ENT_Tr", "In funeral system" ), eHumanLocation_Funeral ),
    T_ConverterHumanLocation( "", "", ( E_HumanLocation ) - 1 )
};

T_ConverterEquipmentState EquipmentStateConverter_ [] =
{
    T_ConverterEquipmentState( "Disponible",              QT_TRANSLATE_NOOP( "ENT_Tr", "Available" ), eEquipmentState_Available ),
    T_ConverterEquipmentState( "Detruit",                 QT_TRANSLATE_NOOP( "ENT_Tr", "Destroyed" ), eEquipmentState_Destroyed ),
    T_ConverterEquipmentState( "ReparableAvecEvacuation", QT_TRANSLATE_NOOP( "ENT_Tr", "Maintenance support needed" ), eEquipmentState_RepairableWithEvacuation ),
    T_ConverterEquipmentState( "ReparableSurPlace",       QT_TRANSLATE_NOOP( "ENT_Tr", "On site fixable" ), eEquipmentState_OnSiteFixable ),
    T_ConverterEquipmentState( "EnMaintenance",           QT_TRANSLATE_NOOP( "ENT_Tr", "In maintenance" ), eEquipmentState_InMaintenance ),
    T_ConverterEquipmentState( "Prisonnier",              QT_TRANSLATE_NOOP( "ENT_Tr", "Prisoner" ), eEquipmentState_Prisonner ),
    T_ConverterEquipmentState( "", "", ( E_EquipmentState ) - 1 )
};

T_ConverterInjuriesSeriousness InjuriesSeriousnessConverter_ [] =
{
    T_ConverterInjuriesSeriousness( "U1", QT_TRANSLATE_NOOP( "ENT_Tr", "U1" ), eInjuriesSeriousness_U1 ),
    T_ConverterInjuriesSeriousness( "U2", QT_TRANSLATE_NOOP( "ENT_Tr", "U2" ), eInjuriesSeriousness_U2 ),
    T_ConverterInjuriesSeriousness( "U3", QT_TRANSLATE_NOOP( "ENT_Tr", "U3" ), eInjuriesSeriousness_U3 ),
    T_ConverterInjuriesSeriousness( "UE", QT_TRANSLATE_NOOP( "ENT_Tr", "UE" ), eInjuriesSeriousness_UE ),
    T_ConverterInjuriesSeriousness( "", "", ( E_InjuriesSeriousness ) - 1 )
};

T_ConverterBreakdownType BreakdownTypeConverter_[] =
{
    T_ConverterBreakdownType( "EA", QT_TRANSLATE_NOOP( "ENT_Tr", "Electronic" ), eBreakdownType_EA ),
    T_ConverterBreakdownType( "M",  QT_TRANSLATE_NOOP( "ENT_Tr", "Mobility" ),   eBreakdownType_M ),
    T_ConverterBreakdownType( "", "", (E_BreakdownType)-1 )
};

T_ConverterBreakdownNTI BreakdownNTIConverter_[] =
{
    T_ConverterBreakdownNTI( "NTI1", QT_TRANSLATE_NOOP( "ENT_Tr", "Seriousness level 1" ), eBreakdownNTI_NTI1 ),
    T_ConverterBreakdownNTI( "NTI2", QT_TRANSLATE_NOOP( "ENT_Tr", "Seriousness level 2" ), eBreakdownNTI_NTI2 ),
    T_ConverterBreakdownNTI( "NTI3", QT_TRANSLATE_NOOP( "ENT_Tr", "Seriousness level 3" ), eBreakdownNTI_NTI3 ),
    T_ConverterBreakdownNTI( "", "", (E_BreakdownNTI)-1 )
};

T_ConverterGhostType GhostTypeConverter_ [] =
{
    T_ConverterGhostType( "agent",   QT_TRANSLATE_NOOP( "ENT_Tr", "Agent" ),   eGhostType_Agent ),
    T_ConverterGhostType( "automat", QT_TRANSLATE_NOOP( "ENT_Tr", "Automat" ), eGhostType_Automat ),
    T_ConverterGhostType( "invalid", QT_TRANSLATE_NOOP( "ENT_Tr", "Invalid" ), eGhostType_Invalid ),
    T_ConverterGhostType( "", "", ( E_GhostType ) - 1 )
};

T_ConverterNbcState NbcStateConverter_ [] =
{
    T_ConverterNbcState( "liquid",  QT_TRANSLATE_NOOP( "ENT_Tr", "Liquid" ),  eNbcState_Liquid ),
    T_ConverterNbcState( "gaseous", QT_TRANSLATE_NOOP( "ENT_Tr", "Gaseous" ), eNbcState_Gaseous ),
    T_ConverterNbcState( "", "", ( E_NbcState ) - 1 )
};

T_ConverterModes ModesConverter_ [] =
{
    T_ConverterModes( "none",        QT_TRANSLATE_NOOP( "ENT_Tr", "None" ),        eModes_None          ),
    T_ConverterModes( "default",     QT_TRANSLATE_NOOP( "ENT_Tr", "Default" ),     eModes_Default       ),
    T_ConverterModes( "prepare",     QT_TRANSLATE_NOOP( "ENT_Tr", "Prepare" ),     eModes_Prepare       ),
    T_ConverterModes( "terrain",     QT_TRANSLATE_NOOP( "ENT_Tr", "Terrain" ),     eModes_Terrain       ),
    T_ConverterModes( "living_area", QT_TRANSLATE_NOOP( "ENT_Tr", "Living Area" ), eModes_LivingArea    ),
    T_ConverterModes( "preparation", QT_TRANSLATE_NOOP( "ENT_Tr", "Preparation" ), eModes_Preparation   ),
    T_ConverterModes( "game",        QT_TRANSLATE_NOOP( "ENT_Tr", "Game" ),        eModes_Gaming        ),
    T_ConverterModes( "planning",    QT_TRANSLATE_NOOP( "ENT_Tr", "Planning" ),    eModes_Planning      ),
    T_ConverterModes( "replay",      QT_TRANSLATE_NOOP( "ENT_Tr", "Replay" ),      eModes_Replay        ),
    T_ConverterModes( "itinerary",   QT_TRANSLATE_NOOP( "ENT_Tr", "Itinerary" ),   eModes_Itinerary     ),
    T_ConverterModes( "all_gaming",  QT_TRANSLATE_NOOP( "ENT_Tr", "All Gaming" ),  eModes_AllGaming     ),
    T_ConverterModes( "all",         QT_TRANSLATE_NOOP( "ENT_Tr", "All" ),         eModes_All           ),
    T_ConverterModes( "", "", ( E_Modes ) - 1 )
};

T_ConverterAgentNbcSuit AgentNbcSuitConverter_[] =
{
    T_ConverterAgentNbcSuit( "none",   QT_TRANSLATE_NOOP( "ENT_Tr", "None" ),    eAgentNone ),
    T_ConverterAgentNbcSuit( "level1", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 1" ), eAgentNbcSuitLevel1 ),
    T_ConverterAgentNbcSuit( "level2", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 2" ), eAgentNbcSuitLevel2 ),
    T_ConverterAgentNbcSuit( "level3", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 3" ), eAgentNbcSuitLevel3 ),
    T_ConverterAgentNbcSuit( "level4", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 4" ), eAgentNbcSuitLevel4 ),
    T_ConverterAgentNbcSuit( "level5", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 5" ), eAgentNbcSuitLevel5 ),
    T_ConverterAgentNbcSuit( "", "", (E_AgentNbcSuit)-1 )
};

T_ConverterAviationRange AviationRangeConverter_[] =
{
    T_ConverterAviationRange( "short range",   QT_TRANSLATE_NOOP( "ENT_Tr", "Short range" ),  eAviationRange_ShortRange ),
    T_ConverterAviationRange( "medium range",  QT_TRANSLATE_NOOP( "ENT_Tr", "Medium range" ), eAviationRange_MediumRange ),
    T_ConverterAviationRange( "long range",    QT_TRANSLATE_NOOP( "ENT_Tr", "Long range" ),   eAviationRange_LongRange ),
    T_ConverterAviationRange( "no ammunition", QT_TRANSLATE_NOOP( "ENT_Tr", "No ammunition" ),   eAviationRange_NoAmmunition ),
    T_ConverterAviationRange( "", "", (E_AviationRange)-1 )
};

T_ConverterLayerTypes LayerTypesConverter_[] =
{
    T_ConverterLayerTypes( "units",             QT_TRANSLATE_NOOP( "ENT_Tr", "Units" ),             eLayerTypes_Agent ),
    T_ConverterLayerTypes( "objects",           QT_TRANSLATE_NOOP( "ENT_Tr", "Objects" ),           eLayerTypes_Objects ),
    T_ConverterLayerTypes( "automata",          QT_TRANSLATE_NOOP( "ENT_Tr", "Automata" ),          eLayerTypes_Automats ),
    T_ConverterLayerTypes( "formations",        QT_TRANSLATE_NOOP( "ENT_Tr", "Formations" ),        eLayerTypes_Formations ),
    T_ConverterLayerTypes( "crowds",            QT_TRANSLATE_NOOP( "ENT_Tr", "Crowds" ),            eLayerTypes_Populations ),
    T_ConverterLayerTypes( "urban_blocks",      QT_TRANSLATE_NOOP( "ENT_Tr", "Urban blocks" ),      eLayerTypes_UrbanObjects ),
    T_ConverterLayerTypes( "tactical_lines",    QT_TRANSLATE_NOOP( "ENT_Tr", "Tactical lines" ),    eLayerTypes_TacticalLines ),
    T_ConverterLayerTypes( "ghosts",            QT_TRANSLATE_NOOP( "ENT_Tr", "Ghosts" ),            eLayerTypes_Ghosts ),
    T_ConverterLayerTypes( "fog",               QT_TRANSLATE_NOOP( "ENT_Tr", "Fog" ),               eLayerTypes_Fog ),
    T_ConverterLayerTypes( "parties",           QT_TRANSLATE_NOOP( "ENT_Tr", "Parties" ),           eLayerTypes_Parties ),
    T_ConverterLayerTypes( "drawings",          QT_TRANSLATE_NOOP( "ENT_Tr", "Drawings" ),          eLayerTypes_Drawers ),
    T_ConverterLayerTypes( "knowledges",        QT_TRANSLATE_NOOP( "ENT_Tr", "Knowledges" ),        eLayerTypes_Knowledges ),
    T_ConverterLayerTypes( "inhabitants",       QT_TRANSLATE_NOOP( "ENT_Tr", "Inhabitants" ),       eLayerTypes_Inhabitants ),
    T_ConverterLayerTypes( "resource_networks", QT_TRANSLATE_NOOP( "ENT_Tr", "Resource networks" ), eLayerTypes_ResourceNetworks ),
    T_ConverterLayerTypes( "pathfinds",         QT_TRANSLATE_NOOP( "ENT_Tr", "Itineraries" ),       eLayerTypes_Pathfinds ),
    T_ConverterLayerTypes( "", "", (E_LayerTypes)-1 )
};

T_ConverterEventTypes EventTypesConverter_[] =
{
    T_ConverterEventTypes( "order",             QT_TRANSLATE_NOOP( "ENT_Tr", "Order" ),             eEventTypes_Order ),
    T_ConverterEventTypes( "magic",             QT_TRANSLATE_NOOP( "ENT_Tr", "Supervisor action" ), eEventTypes_Magic ),
    T_ConverterEventTypes( "report",            QT_TRANSLATE_NOOP( "ENT_Tr", "Report" ),            eEventTypes_Report ),
    T_ConverterEventTypes( "task",              QT_TRANSLATE_NOOP( "ENT_Tr", "Task" ),              eEventTypes_Task ),
    T_ConverterEventTypes( "multimedia",        QT_TRANSLATE_NOOP( "ENT_Tr", "Multimedia" ),        eEventTypes_Multimedia ),
    T_ConverterEventTypes( "", "", (E_EventTypes)-1 )
};

T_ConverterMissionType MissionTypeConverter_[] =
{
    T_ConverterMissionType( "unit",      QT_TRANSLATE_NOOP( "ENT_Tr", "Unit missions" ),      eMissionType_Pawn ),
    T_ConverterMissionType( "automat",   QT_TRANSLATE_NOOP( "ENT_Tr", "Automat missions" ),   eMissionType_Automat ),
    T_ConverterMissionType( "crowd",     QT_TRANSLATE_NOOP( "ENT_Tr", "Crowd missions" ),     eMissionType_Population ),
    T_ConverterMissionType( "frag-order", QT_TRANSLATE_NOOP( "ENT_Tr", "Fragmentary Orders" ), eMissionType_FragOrder ),
    T_ConverterMissionType( "", "", (E_MissionType)-1 )
};

T_ConverterEventDockModes EventDockModesConverter_[] =
{
    T_ConverterEventDockModes( "none",              QT_TRANSLATE_NOOP( "ENT_Tr", "Default" ),           eEventDockModes_None ),
    T_ConverterEventDockModes( "display-triggered", QT_TRANSLATE_NOOP( "ENT_Tr", "Display triggered" ), eEventDockModes_DisplayTriggered ),
    T_ConverterEventDockModes( "edit-triggered",    QT_TRANSLATE_NOOP( "ENT_Tr", "Edit triggered" ),    eEventDockModes_EditTriggered ),
    T_ConverterEventDockModes( "edit-planned",      QT_TRANSLATE_NOOP( "ENT_Tr", "Edit planned" ),      eEventDockModes_EditPlanned ),
    T_ConverterEventDockModes( "create",            QT_TRANSLATE_NOOP( "ENT_Tr", "Create new" ),        eEventDockModes_Create ),
    T_ConverterEventDockModes( "", "", (E_EventDockModes)-1 )
};

T_ConverterCoordinateSystem CoordinateSystemConverter_[] =
{
    T_ConverterCoordinateSystem( "mgrs",     QT_TRANSLATE_NOOP( "CoordinateSystem", "UTM MGRS" ),          eCoordinateSystem_Mgrs ),
    T_ConverterCoordinateSystem( "sanc",     QT_TRANSLATE_NOOP( "CoordinateSystem", "UTM SAD69 Brazil" ),  eCoordinateSystem_SanC ),
    T_ConverterCoordinateSystem( "wgs84dd",  QT_TRANSLATE_NOOP( "CoordinateSystem", "LatLong WGS84 DD" ),  eCoordinateSystem_Wgs84Dd ),
    T_ConverterCoordinateSystem( "wgs84dms", QT_TRANSLATE_NOOP( "CoordinateSystem", "LatLong WGS84 DMS" ), eCoordinateSystem_Wgs84Dms ),
    T_ConverterCoordinateSystem( "local",    QT_TRANSLATE_NOOP( "CoordinateSystem", "Local coordinates" ), eCoordinateSystem_Local ),
    T_ConverterCoordinateSystem( "", "", (E_CoordinateSystem)-1 )
};

T_ConverterMagicActionType MagicActionTypeConverter_[] =
{
    T_ConverterMagicActionType( "change_diplomacy",               QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Change diplomacy" ), sword::MagicAction::change_diplomacy ),
    T_ConverterMagicActionType( "change_resource_links",          QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Change resource links" ), sword::MagicAction::change_resource_network_properties ),
    T_ConverterMagicActionType( "create_knowledge_group",         QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Create Knowledge Group" ), sword::MagicAction::create_knowledge_group ),
    T_ConverterMagicActionType( "debug_internal",                 QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Debug internal" ), sword::MagicAction::debug_internal),
    T_ConverterMagicActionType( "fire_order_on_location",         QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Strike order on location" ), sword::MagicAction::create_fire_order_on_location ),
    T_ConverterMagicActionType( "global_weather",                 QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Change Global Weather" ), sword::MagicAction::global_weather ),
    T_ConverterMagicActionType( "local_weather",                  QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Change Local Weather" ), sword::MagicAction::local_weather ),
    T_ConverterMagicActionType( "local_weather_destruction",      QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Local Weather Destruction" ), sword::MagicAction::local_weather_destruction ),
    T_ConverterMagicActionType( "pathfind_creation",              QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Create pathfind" ), sword::MagicAction::pathfind_creation ),
    T_ConverterMagicActionType( "pathfind_destruction",           QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Destroy pathfind" ), sword::MagicAction::pathfind_destruction ),
    T_ConverterMagicActionType( "select_diagnosis_team",          QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Select Diagnosis Team" ), sword::MagicAction::select_diagnosis_team ),
    T_ConverterMagicActionType( "select_maintenance_transporter", QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Select Maintenance Transporter" ), sword::MagicAction::select_maintenance_transporter ),
    T_ConverterMagicActionType( "select_new_logistic_state",      QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Select logistic automated resolution" ), sword::MagicAction::select_new_logistic_state ),
    T_ConverterMagicActionType( "select_repair_team",             QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Select Repair Team" ), sword::MagicAction::select_repair_team ),
    T_ConverterMagicActionType( "transfer_to_logistic_superior",  QT_TRANSLATE_NOOP( "sword::MagicAction::Type", "Transfer To Logistic Superior" ), sword::MagicAction::transfer_to_logistic_superior ),
    T_ConverterMagicActionType( "", "", sword::MagicAction::Type_MAX )
};

T_ConverterUnitMagicActionType UnitMagicActionTypeConverter_[] =
{
    T_ConverterUnitMagicActionType( "automat_and_units_creation",       QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Automat and units creation" ), sword::UnitMagicAction::automat_and_units_creation ),
    T_ConverterUnitMagicActionType( "automat_creation",                 QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Automat Creation" ), sword::UnitMagicAction::automat_creation ),
    T_ConverterUnitMagicActionType( "cancel_surrender",                 QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Cancel - Surrender" ), sword::UnitMagicAction::cancel_surrender ),
    T_ConverterUnitMagicActionType( "change_automat_superior",          "" /* doesn't seems to work, but change_formation_superior does */, sword::UnitMagicAction::change_automat_superior ),
    T_ConverterUnitMagicActionType( "change_brain_debug",               QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change brain debug" ), sword::UnitMagicAction::change_brain_debug ),
    T_ConverterUnitMagicActionType( "change_critical_intelligence",     QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change critical intelligence" ), sword::UnitMagicAction::change_critical_intelligence ),
    T_ConverterUnitMagicActionType( "change_dotation",                  QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change dotations" ), sword::UnitMagicAction::change_dotation ),
    T_ConverterUnitMagicActionType( "change_equipment_human_size",      "" /* deprecated */, sword::UnitMagicAction::change_equipment_human_size ),
    T_ConverterUnitMagicActionType( "change_equipment_state",           QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change equipment state" ), sword::UnitMagicAction::change_equipment_state ),
    T_ConverterUnitMagicActionType( "change_extension",                 QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change extension" ), sword::UnitMagicAction::change_extension ),
    T_ConverterUnitMagicActionType( "change_formation_superior",        QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change superior" ), sword::UnitMagicAction::change_formation_superior ),
    T_ConverterUnitMagicActionType( "change_human_factors",             QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change human factors" ), sword::UnitMagicAction::change_human_factors ),
    T_ConverterUnitMagicActionType( "change_human_state",               QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change human state" ), sword::UnitMagicAction::change_human_state ),
    T_ConverterUnitMagicActionType( "change_knowledge_group",           QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change superior" ), sword::UnitMagicAction::change_knowledge_group ),
    T_ConverterUnitMagicActionType( "change_logistic_links",            QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change Logistic Links" ), sword::UnitMagicAction::change_logistic_links ),
    T_ConverterUnitMagicActionType( "change_posture",                   QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change posture" ), sword::UnitMagicAction::change_posture ),
    T_ConverterUnitMagicActionType( "create_basic_load_supply_request", QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Create basic load supply request" ), sword::UnitMagicAction::create_basic_load_supply_request ),
    T_ConverterUnitMagicActionType( "create_breakdowns",                "" /* deprecated */, sword::UnitMagicAction::create_breakdowns ),
    T_ConverterUnitMagicActionType( "create_direct_fire_order",         QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Direct strike order" ) /* hla */, sword::UnitMagicAction::create_direct_fire_order ),
    T_ConverterUnitMagicActionType( "create_stock_supply_request",      QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Create stock supply request" ), sword::UnitMagicAction::create_stock_supply_request ),
    T_ConverterUnitMagicActionType( "create_wound",                     "" /* deprecated */, sword::UnitMagicAction::create_wound ),
    T_ConverterUnitMagicActionType( "create_wounds",                    "" /* deprecated */, sword::UnitMagicAction::create_wounds ),
    T_ConverterUnitMagicActionType( "crowd_change_affinities",          QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change affinities" ), sword::UnitMagicAction::crowd_change_affinities ),
    T_ConverterUnitMagicActionType( "crowd_change_armed_individuals",   QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Crowd Change Armed Individuals" ), sword::UnitMagicAction::crowd_change_armed_individuals ),
    T_ConverterUnitMagicActionType( "crowd_change_attitude",            QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Crowd Change Attitude" ), sword::UnitMagicAction::crowd_change_attitude ),
    T_ConverterUnitMagicActionType( "crowd_change_health_state",        QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Crowd Change Health State" ), sword::UnitMagicAction::crowd_change_health_state ),
    T_ConverterUnitMagicActionType( "crowd_creation",                   QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Crowd Creation" ), sword::UnitMagicAction::crowd_creation ),
    T_ConverterUnitMagicActionType( "crowd_total_destruction",          QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Crowd Total Destruction" ), sword::UnitMagicAction::crowd_total_destruction ),
    T_ConverterUnitMagicActionType( "delete_unit",                      QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Delete unit" ), sword::UnitMagicAction::delete_unit ),
    T_ConverterUnitMagicActionType( "destroy_all",                      QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Destroy - All" ), sword::UnitMagicAction::destroy_all ),
    T_ConverterUnitMagicActionType( "destroy_component",                QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Destroy Component" ), sword::UnitMagicAction::destroy_component ),
    T_ConverterUnitMagicActionType( "exec_script",                      QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Exec script" ) /*  */, sword::UnitMagicAction::exec_script ),
    T_ConverterUnitMagicActionType( "fire_order",                       QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Strike order" ), sword::UnitMagicAction::create_fire_order ),
    T_ConverterUnitMagicActionType( "formation_creation",               QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Formation Creation" ), sword::UnitMagicAction::formation_creation ),
    T_ConverterUnitMagicActionType( "inhabitant_change_affinities",     QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change affinities" ), sword::UnitMagicAction::inhabitant_change_affinities ),
    T_ConverterUnitMagicActionType( "inhabitant_change_alerted_state",  QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Population Change Alerted State" ), sword::UnitMagicAction::inhabitant_change_alerted_state ),
    T_ConverterUnitMagicActionType( "inhabitant_change_confined_state", QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Population Change Confined State" ), sword::UnitMagicAction::inhabitant_change_confined_state ),
    T_ConverterUnitMagicActionType( "inhabitant_change_health_state",   QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Population Change Health State" ), sword::UnitMagicAction::inhabitant_change_health_state ),
    T_ConverterUnitMagicActionType( "knowledge_group_update",           "" /* deprecated */, sword::UnitMagicAction::knowledge_group_update ),
    T_ConverterUnitMagicActionType( "load_unit",                        QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Load unit" ) /* hla */, sword::UnitMagicAction::load_unit ),
    T_ConverterUnitMagicActionType( "log_finish_handlings",             QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Finish logistic handlings" ), sword::UnitMagicAction::log_finish_handlings ),
    T_ConverterUnitMagicActionType( "log_maintenance_set_manual",       QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Switch Log Maintenance Mode" ), sword::UnitMagicAction::log_maintenance_set_manual ),
    T_ConverterUnitMagicActionType( "log_supply_change_quotas",         QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Log Supply Change Quotas" ), sword::UnitMagicAction::log_supply_change_quotas ),
    T_ConverterUnitMagicActionType( "log_supply_pull_flow",             QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Log Supply Pull Flow" ), sword::UnitMagicAction::log_supply_pull_flow ),
    T_ConverterUnitMagicActionType( "log_supply_push_flow",             QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Log Supply Push Flow" ), sword::UnitMagicAction::log_supply_push_flow ),
    T_ConverterUnitMagicActionType( "log_supply_set_manual",            QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Switch Log Supply Mode" ), sword::UnitMagicAction::log_supply_set_manual ),
    T_ConverterUnitMagicActionType( "partial_recovery",                 QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Partial Recovery" ), sword::UnitMagicAction::partial_recovery ),
    T_ConverterUnitMagicActionType( "recover_all",                      QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - All" ), sword::UnitMagicAction::recover_all ),
    T_ConverterUnitMagicActionType( "recover_all_except_log",           QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - All" ), sword::UnitMagicAction::recover_all_except_log ),
    T_ConverterUnitMagicActionType( "recover_equipments",               QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - Equipments" ), sword::UnitMagicAction::recover_equipments ),
    T_ConverterUnitMagicActionType( "recover_equipments_except_log",    QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - Equipments" ), sword::UnitMagicAction::recover_equipments_except_log ),
    T_ConverterUnitMagicActionType( "recover_resources",                QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - Resources" ), sword::UnitMagicAction::recover_resources ),
    T_ConverterUnitMagicActionType( "recover_resources_except_log",     QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - Resources" ), sword::UnitMagicAction::recover_resources_except_log ),
    T_ConverterUnitMagicActionType( "recover_transporters",             QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover Transporters" ), sword::UnitMagicAction::recover_transporters ),
    T_ConverterUnitMagicActionType( "recover_troops",                   QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - Troops" ), sword::UnitMagicAction::recover_troops ),
    T_ConverterUnitMagicActionType( "recover_troops_except_log",        QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Recover - Troops" ), sword::UnitMagicAction::recover_troops_except_log ),
    T_ConverterUnitMagicActionType( "reload_brain",                     QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Reload brain" ), sword::UnitMagicAction::reload_brain ),
    T_ConverterUnitMagicActionType( "surrender",                        QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Surrender" ), sword::UnitMagicAction::surrender_to ),
    T_ConverterUnitMagicActionType( "teleport",                         QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Teleport" ), sword::UnitMagicAction::move_to ),
    T_ConverterUnitMagicActionType( "transfer_equipment",               QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Equipment transfer" ), sword::UnitMagicAction::transfer_equipment ),
    T_ConverterUnitMagicActionType( "unit_change_affinities",           QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Change affinities" ), sword::UnitMagicAction::unit_change_affinities ),
    T_ConverterUnitMagicActionType( "unit_change_superior",             QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Unit Change Superior" ), sword::UnitMagicAction::unit_change_superior ),
    T_ConverterUnitMagicActionType( "unit_creation",                    QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Unit Creation" ), sword::UnitMagicAction::unit_creation ),
    T_ConverterUnitMagicActionType( "unload_unit",                      QT_TRANSLATE_NOOP( "sword::UnitMagicAction::Type", "Unload unit" ) /* hla */, sword::UnitMagicAction::unload_unit ),
    T_ConverterUnitMagicActionType( "", "", sword::UnitMagicAction::Type_MAX )
};

T_ConverterKnowledgeMagicActionType KnowledgeMagicActionTypeConverter_[] =
{
    T_ConverterKnowledgeMagicActionType( "knowledge_group_add_knowledge",      QT_TRANSLATE_NOOP( "sword::KnowledgeMagicAction::Type", "Knowledge Group Add knowledge" ), sword::KnowledgeMagicAction::add_knowledge ),
    T_ConverterKnowledgeMagicActionType( "knowledge_group_enable",             QT_TRANSLATE_NOOP( "sword::KnowledgeMagicAction::Type", "Knowledge Group Activation Change" ), sword::KnowledgeMagicAction::enable ),
    T_ConverterKnowledgeMagicActionType( "knowledge_group_update_side",        QT_TRANSLATE_NOOP( "sword::KnowledgeMagicAction::Type", "Change superior" ), sword::KnowledgeMagicAction::update_party ),
    T_ConverterKnowledgeMagicActionType( "knowledge_group_update_side_parent", QT_TRANSLATE_NOOP( "sword::KnowledgeMagicAction::Type", "Change superior" ), sword::KnowledgeMagicAction::update_party_parent ),
    T_ConverterKnowledgeMagicActionType( "knowledge_group_update_type",        QT_TRANSLATE_NOOP( "sword::KnowledgeMagicAction::Type", "Knowledge Group Change Type" ), sword::KnowledgeMagicAction::update_type ),
    T_ConverterKnowledgeMagicActionType( "", "", sword::KnowledgeMagicAction::Type_MAX )
};

T_ConverterObjectMagicActionType ObjectMagicActionTypeConverter_[] =
{
    T_ConverterObjectMagicActionType( "create_object",  QT_TRANSLATE_NOOP( "sword::ObjectMagicAction::Type", "Object Creation" ),    sword::ObjectMagicAction::create ),
    T_ConverterObjectMagicActionType( "destroy_object", QT_TRANSLATE_NOOP( "sword::ObjectMagicAction::Type", "Object Destruction" ), sword::ObjectMagicAction::destroy ),
    T_ConverterObjectMagicActionType( "update_object",  QT_TRANSLATE_NOOP( "sword::ObjectMagicAction::Type", "Object Update" ),      sword::ObjectMagicAction::update ),
    T_ConverterObjectMagicActionType( "", "", sword::ObjectMagicAction::Type_MAX )
};

T_ConverterLogisticLevel LogisticLevelConverter_[] =
{
    T_ConverterLogisticLevel( "none",          QT_TRANSLATE_NOOP( "sword::EnumLogisticLevel", "None" ),          sword::none ),
    T_ConverterLogisticLevel( "logistic-base", QT_TRANSLATE_NOOP( "sword::EnumLogisticLevel", "Logistic base" ), sword::logistic_base ),
    T_ConverterLogisticLevel( "", "", sword::EnumLogisticLevel_MAX )
};

T_ConverterLogMaintenanceHandlingStatus LogMaintenanceHandlingStatusConverter_[] =
{
    T_ConverterLogMaintenanceHandlingStatus( "diagnosing",                           QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "diagnosis" ),                             sword::LogMaintenanceHandlingUpdate::diagnosing ),
    T_ConverterLogMaintenanceHandlingStatus( "finished",                             QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "finished" ),                              sword::LogMaintenanceHandlingUpdate::finished ),
    T_ConverterLogMaintenanceHandlingStatus( "moving_back",                          QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "unit returning" ),                        sword::LogMaintenanceHandlingUpdate::moving_back ),
    T_ConverterLogMaintenanceHandlingStatus( "moving_to_supply",                     QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "moving toward logistic system" ),         sword::LogMaintenanceHandlingUpdate::moving_to_supply ),
    T_ConverterLogMaintenanceHandlingStatus( "repairing",                            QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "repairing" ),                             sword::LogMaintenanceHandlingUpdate::repairing ),
    T_ConverterLogMaintenanceHandlingStatus( "searching_upper_levels",               QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting to be handled by higher level" ), sword::LogMaintenanceHandlingUpdate::searching_upper_levels ),
    T_ConverterLogMaintenanceHandlingStatus( "transporter_loading",                  QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "tow-truck loading" ),                     sword::LogMaintenanceHandlingUpdate::transporter_loading ),
    T_ConverterLogMaintenanceHandlingStatus( "transporter_moving_back",              QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "tow-truck moving from target" ),          sword::LogMaintenanceHandlingUpdate::transporter_moving_back ),
    T_ConverterLogMaintenanceHandlingStatus( "transporter_moving_to_supply",         QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "tow-truck moving to target" ),            sword::LogMaintenanceHandlingUpdate::transporter_moving_to_supply ),
    T_ConverterLogMaintenanceHandlingStatus( "transporter_unloading",                QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "tow-truck unloading" ),                   sword::LogMaintenanceHandlingUpdate::transporter_unloading ),
    T_ConverterLogMaintenanceHandlingStatus( "waiting_for_diagnosis_team_selection", QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting for diagnosis team selection" ),  sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection ),
    T_ConverterLogMaintenanceHandlingStatus( "waiting_for_parts",                    QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting for available parts" ),           sword::LogMaintenanceHandlingUpdate::waiting_for_parts ),
    T_ConverterLogMaintenanceHandlingStatus( "waiting_for_repair_team_selection",    QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting for repair team selection" ),     sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection ),
    T_ConverterLogMaintenanceHandlingStatus( "waiting_for_repairer",                 QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting for available repairer" ),        sword::LogMaintenanceHandlingUpdate::waiting_for_repairer ),
    T_ConverterLogMaintenanceHandlingStatus( "waiting_for_transporter",              QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting for available tow-truck" ),       sword::LogMaintenanceHandlingUpdate::waiting_for_transporter ),
    T_ConverterLogMaintenanceHandlingStatus( "waiting_for_transporter_selection",    QT_TRANSLATE_NOOP( "sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus", "waiting for transporter selection" ),     sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection ),
    T_ConverterLogMaintenanceHandlingStatus( "", "", sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus_MAX )
};

T_ConverterLogFuneralHandlingStatus LogFuneralHandlingStatusConverter_[] =
{
    T_ConverterLogFuneralHandlingStatus( "waiting_for_handling",    QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "waiting for handling" ),    sword::LogFuneralHandlingUpdate::waiting_for_handling ),
    T_ConverterLogFuneralHandlingStatus( "transporting_unpackaged", QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "transporting unpackaged" ), sword::LogFuneralHandlingUpdate::transporting_unpackaged ),
    T_ConverterLogFuneralHandlingStatus( "waiting_for_packaging",   QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "waiting for packaging" ),   sword::LogFuneralHandlingUpdate::waiting_for_packaging ),
    T_ConverterLogFuneralHandlingStatus( "packaging",               QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "packaging" ),               sword::LogFuneralHandlingUpdate::packaging ),
    T_ConverterLogFuneralHandlingStatus( "waiting_for_transporter", QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "waiting for transporter" ), sword::LogFuneralHandlingUpdate::waiting_for_transporter ),
    T_ConverterLogFuneralHandlingStatus( "transporting_packaged",   QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "transporting packaged" ),   sword::LogFuneralHandlingUpdate::transporting_packaged ),
    T_ConverterLogFuneralHandlingStatus( "finished",                QT_TRANSLATE_NOOP( "sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus", "finished" ),                sword::LogFuneralHandlingUpdate::finished ),
    T_ConverterLogFuneralHandlingStatus( "", "", sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus_MAX )
};

T_ConverterLogMedicalHandlingStatus LogMedicalHandlingStatusConverter_[] =
{
    T_ConverterLogMedicalHandlingStatus( "waiting_for_evacuation",                      QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for evacuation ambulance" ),         sword::LogMedicalHandlingUpdate::waiting_for_evacuation ),
    T_ConverterLogMedicalHandlingStatus( "evacuation_ambulance_moving_in",              QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "evacuation ambulance coming" ),              sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_in ),
    T_ConverterLogMedicalHandlingStatus( "evacuation_ambulance_loading",                QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "evacuation ambulance loading" ),             sword::LogMedicalHandlingUpdate::evacuation_ambulance_loading ),
    T_ConverterLogMedicalHandlingStatus( "waiting_for_evacuation_loading_completion",   QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for evacuation ambulance loading" ), sword::LogMedicalHandlingUpdate::waiting_for_evacuation_loading_completion ),
    T_ConverterLogMedicalHandlingStatus( "evacuation_ambulance_moving_out",             QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "evacuation ambulance going back" ),          sword::LogMedicalHandlingUpdate::evacuation_ambulance_moving_out ),
    T_ConverterLogMedicalHandlingStatus( "evacuation_ambulance_unloading",              QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "evacuation ambulance unloading" ),           sword::LogMedicalHandlingUpdate::evacuation_ambulance_unloading ),
    T_ConverterLogMedicalHandlingStatus( "waiting_for_diagnostic",                      QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for doctor" ),                       sword::LogMedicalHandlingUpdate::waiting_for_diagnostic ),
    T_ConverterLogMedicalHandlingStatus( "diagnosing",                                  QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "diagnosis" ),                                sword::LogMedicalHandlingUpdate::diagnosing ),
    T_ConverterLogMedicalHandlingStatus( "looking_for_triage",                          QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "looking for triage area" ),                  sword::LogMedicalHandlingUpdate::looking_for_triage ),
    T_ConverterLogMedicalHandlingStatus( "waiting_for_triage",                          QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for triage" ),                       sword::LogMedicalHandlingUpdate::waiting_for_triage ),
    T_ConverterLogMedicalHandlingStatus( "triaging",                                    QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "triage" ),                                   sword::LogMedicalHandlingUpdate::triaging ),
    T_ConverterLogMedicalHandlingStatus( "looking_for_medical_attention",               QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "looking for treatment area" ),               sword::LogMedicalHandlingUpdate::looking_for_medical_attention ),
    T_ConverterLogMedicalHandlingStatus( "waiting_for_medical_attention",               QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for doctor (treatment)" ),           sword::LogMedicalHandlingUpdate::waiting_for_medical_attention ),
    T_ConverterLogMedicalHandlingStatus( "receiving_medical_attention",                 QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "treatment" ),                                sword::LogMedicalHandlingUpdate::receiving_medical_attention ),
    T_ConverterLogMedicalHandlingStatus( "resting",                                     QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "hospitalization" ),                          sword::LogMedicalHandlingUpdate::resting ),
    T_ConverterLogMedicalHandlingStatus( "waiting_for_collection",                      QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for collection ambulance" ),         sword::LogMedicalHandlingUpdate::waiting_for_collection ),
    T_ConverterLogMedicalHandlingStatus( "collection_ambulance_loading",                QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "collection ambulance loading" ),             sword::LogMedicalHandlingUpdate::collection_ambulance_loading ),
    T_ConverterLogMedicalHandlingStatus( "waiting_for_collection_loading_completion",   QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "waiting for collection ambulance loading" ), sword::LogMedicalHandlingUpdate::waiting_for_collection_loading_completion ),
    T_ConverterLogMedicalHandlingStatus( "collection_ambulance_moving_in",              QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "collection ambulance coming" ),              sword::LogMedicalHandlingUpdate::collection_ambulance_moving_in ),
    T_ConverterLogMedicalHandlingStatus( "collection_ambulance_unloading",              QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "collection ambulance unloading" ),           sword::LogMedicalHandlingUpdate::collection_ambulance_unloading ),
    T_ConverterLogMedicalHandlingStatus( "finished",                                    QT_TRANSLATE_NOOP( "sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus", "finished" ),                                 sword::LogMedicalHandlingUpdate::finished ),
    T_ConverterLogMedicalHandlingStatus( "", "", sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus_MAX )
};

T_ConverterLogSupplyHandlingStatus LogSupplyHandlingStatusConverter_[] =
{
    T_ConverterLogSupplyHandlingStatus( "convoy_waiting_for_transporters",     QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy waiting for trucks" ),        sword::LogSupplyHandlingUpdate::convoy_waiting_for_transporters ),
    T_ConverterLogSupplyHandlingStatus( "convoy_setup",                        QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy setting up" ),                sword::LogSupplyHandlingUpdate::convoy_setup ),
    T_ConverterLogSupplyHandlingStatus( "convoy_moving_to_loading_point",      QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy moving to loading point" ),   sword::LogSupplyHandlingUpdate::convoy_moving_to_loading_point ),
    T_ConverterLogSupplyHandlingStatus( "convoy_loading",                      QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy loading" ),                   sword::LogSupplyHandlingUpdate::convoy_loading ),
    T_ConverterLogSupplyHandlingStatus( "convoy_moving_to_unloading_point",    QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy moving to unloading point" ), sword::LogSupplyHandlingUpdate::convoy_moving_to_unloading_point ),
    T_ConverterLogSupplyHandlingStatus( "convoy_unloading",                    QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy unloading" ),                 sword::LogSupplyHandlingUpdate::convoy_unloading ),
    T_ConverterLogSupplyHandlingStatus( "convoy_moving_back_to_loading_point", QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "convoy moving back" ),               sword::LogSupplyHandlingUpdate::convoy_moving_back_to_loading_point ),
    T_ConverterLogSupplyHandlingStatus( "convoy_finished",                     QT_TRANSLATE_NOOP( "sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus", "finished" ),                         sword::LogSupplyHandlingUpdate::convoy_finished ),
    T_ConverterLogSupplyHandlingStatus( "", "", sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus_MAX )
};

T_ConverterDotationType DotationTypeConverter_[] =
{
    T_ConverterDotationType( "munition",         QT_TRANSLATE_NOOP( "sword::DotationType", "Ammunition" ),         sword::dotation_type_ammunition ),
    T_ConverterDotationType( "carburant",        QT_TRANSLATE_NOOP( "sword::DotationType", "Fuel" ),               sword::dotation_type_fuel ),
    T_ConverterDotationType( "explosif",         QT_TRANSLATE_NOOP( "sword::DotationType", "Explosive" ),          sword::dotation_type_explosive ),
    T_ConverterDotationType( "mine",             QT_TRANSLATE_NOOP( "sword::DotationType", "Mine" ),               sword::dotation_type_mine ),
    T_ConverterDotationType( "barbele",          QT_TRANSLATE_NOOP( "sword::DotationType", "Barbed wire" ),        sword::dotation_type_barbed_wire ),
    T_ConverterDotationType( "piece",            QT_TRANSLATE_NOOP( "sword::DotationType", "Parts" ),              sword::dotation_type_parts ),
    T_ConverterDotationType( "ration",           QT_TRANSLATE_NOOP( "sword::DotationType", "Food/water" ),         sword::dotation_type_food_and_water ),
    T_ConverterDotationType( "agent extincteur", QT_TRANSLATE_NOOP( "sword::DotationType", "Extinguisher agent" ), sword::dotation_type_extinguisher_agent ),
    T_ConverterDotationType( "energie",          QT_TRANSLATE_NOOP( "sword::DotationType", "Resources" ),          sword::dotation_type_energy ),
    T_ConverterDotationType( "funeraire",        QT_TRANSLATE_NOOP( "sword::DotationType", "Funeral" ),            sword::dotation_type_funeral ),
    T_ConverterDotationType( "", "", sword::DotationType_MAX )
};

}  // namespace

#define INIT_TRANSLATION( NAME, CONTEXT )\
    BOOST_STATIC_ASSERT( COUNT_OF( NAME ## Converter_ ) == ( eNbr ## NAME ) + 1 );\
    InitTr( ( NAME ## Converter_ ), CONTEXT );

#define INIT_TR( NAME ) INIT_TRANSLATION( NAME, "ENT_Tr" )

#define INIT_TR_WITH_CONTEXT( NAME ) INIT_TRANSLATION( NAME, #NAME )

#define INIT_PROTO_TR( CLASS, ALIAS )\
    BOOST_STATIC_ASSERT( COUNT_OF( ALIAS ## Converter_ ) == ( sword:: ## CLASS ## _ARRAYSIZE ) + 1 );\
    InitTr( ( ALIAS ## Converter_ ), "sword::" # CLASS );

#define INIT_SUB_PROTO_TR( CLASS, NAME, ALIAS )\
    BOOST_STATIC_ASSERT( COUNT_OF( ALIAS ## Converter_ ) == ( sword:: ## CLASS ## :: ## NAME ## _ARRAYSIZE ) + 1 );\
    InitTr( ( ALIAS ## Converter_ ), "sword::" # CLASS "::" # NAME );

#define IMPLEMENT_CONVERT_METHODS( NAME )                                               \
const std::string& ENT_Tr::ConvertFrom ## NAME ## ( E_ ## NAME value,                   \
                                                    ENT_Tr::E_Conversion conversion )   \
{                                                                                       \
    return InverseFindInConverter( NAME ## Converter_, value, conversion );             \
}                                                                                       \
E_ ## NAME ENT_Tr::ConvertTo ## NAME ##( const std::string& text,                       \
                                         E_Conversion conversion )                      \
{                                                                                       \
    return FindInConverter( NAME ## Converter_, text, conversion );                     \
}                                                                                       \

#define IMPLEMENT_CONVERT_METHODS_PROTO( CLASS, ALIAS )                                 \
const std::string& ENT_Tr::ConvertFrom ## ALIAS ## ( sword:: ## CLASS value,            \
                                                     ENT_Tr::E_Conversion conversion )  \
{                                                                                       \
    return InverseFindInConverter( ALIAS ## Converter_, value, conversion );            \
}                                                                                       \
sword:: ## CLASS ENT_Tr::ConvertTo ## ALIAS ##( const std::string& text,                \
                                                E_Conversion conversion )               \
{                                                                                       \
    return FindInConverter( ALIAS ## Converter_, text, conversion );                    \
}                                                                                       \

#define IMPLEMENT_CONVERT_METHODS_SUB_PROTO( CLASS, NAME, ALIAS )                           \
const std::string& ENT_Tr::ConvertFrom ## ALIAS ## ( sword:: ## CLASS ## :: ## NAME value,  \
                                                     ENT_Tr::E_Conversion conversion )      \
{                                                                                           \
    return InverseFindInConverter( ALIAS ## Converter_, value, conversion );                \
}                                                                                           \
sword:: ## CLASS ## :: ## NAME ENT_Tr::ConvertTo ## ALIAS ##( const std::string& text,      \
                                                              E_Conversion conversion )     \
{                                                                                           \
    return FindInConverter( ALIAS ## Converter_, text, conversion );                        \
}                                                                                           \

//-----------------------------------------------------------------------------
// Name: InitTranslations
// Created: AGR
//-----------------------------------------------------------------------------
void ENT_Tr::InitTranslations()
{
    INIT_TR( AgentNbcSuit );
    INIT_TR( AmmunitionType );
    INIT_TR( AviationRange );
    INIT_TR( BreakdownNTI );
    INIT_TR( BreakdownType );
    INIT_TR_WITH_CONTEXT( CoordinateSystem );
    INIT_TR( CrossingType );
    INIT_TR( Diplomacy );
    INIT_TR( EquipmentState );
    INIT_TR( EventDockModes );
    INIT_TR( EventTypes );
    INIT_TR( FireAvailability );
    INIT_TR( ForceRatioStatus );
    INIT_TR( GhostType );
    INIT_TR( HumanLocation );
    INIT_TR( HumanRank );
    INIT_TR( HumanState );
    INIT_TR( HumanWound );
    INIT_TR( InjuriesSeriousness );
    INIT_TR( LayerTypes );
    INIT_TR( LightingType );
    INIT_TR( Location );
    INIT_TR( LocationType );
    INIT_TR( MeetingEngagementStatus );
    INIT_TR( MissionType );
    INIT_TR( Modes );
    INIT_TR( NatureLevel );
    INIT_TR( NbcState );
    INIT_TR( ObstacleActivation );
    INIT_TR( OperationalStatus );
    INIT_TR( PopulationAttitude );
    INIT_TR( PopulationRoe );
    INIT_TR( Roe );
    INIT_TR( UnitExperience );
    INIT_TR( UnitMorale );
    INIT_TR( UnitPosture );
    INIT_TR( UnitStress );
    INIT_TR( UnitTiredness );
    INIT_TR( WeatherType );

    INIT_PROTO_TR( DotationType, DotationType );
    INIT_PROTO_TR( EnumLogisticLevel, LogisticLevel );
    INIT_SUB_PROTO_TR( KnowledgeMagicAction, Type, KnowledgeMagicActionType );
    INIT_SUB_PROTO_TR( LogFuneralHandlingUpdate, EnumLogFuneralHandlingStatus, LogFuneralHandlingStatus );
    INIT_SUB_PROTO_TR( LogMaintenanceHandlingUpdate, EnumLogMaintenanceHandlingStatus, LogMaintenanceHandlingStatus );
    INIT_SUB_PROTO_TR( LogMedicalHandlingUpdate, EnumLogMedicalHandlingStatus, LogMedicalHandlingStatus );
    INIT_SUB_PROTO_TR( LogSupplyHandlingUpdate, EnumLogSupplyHandlingStatus, LogSupplyHandlingStatus );
    INIT_SUB_PROTO_TR( MagicAction, Type, MagicActionType );
    INIT_SUB_PROTO_TR( ObjectMagicAction, Type, ObjectMagicActionType );
    INIT_SUB_PROTO_TR( UnitMagicAction, Type, UnitMagicActionType );
}

IMPLEMENT_CONVERT_METHODS( AgentNbcSuit );
IMPLEMENT_CONVERT_METHODS( AmmunitionType )
IMPLEMENT_CONVERT_METHODS( AviationRange );
IMPLEMENT_CONVERT_METHODS( BreakdownNTI );
IMPLEMENT_CONVERT_METHODS( BreakdownType );
IMPLEMENT_CONVERT_METHODS( CoordinateSystem );
IMPLEMENT_CONVERT_METHODS( CrossingType );
IMPLEMENT_CONVERT_METHODS( Diplomacy );
IMPLEMENT_CONVERT_METHODS( EquipmentState );
IMPLEMENT_CONVERT_METHODS( EventDockModes );
IMPLEMENT_CONVERT_METHODS( EventTypes );
IMPLEMENT_CONVERT_METHODS( FireAvailability );
IMPLEMENT_CONVERT_METHODS( ForceRatioStatus );
IMPLEMENT_CONVERT_METHODS( GhostType );
IMPLEMENT_CONVERT_METHODS( HumanLocation );
IMPLEMENT_CONVERT_METHODS( HumanRank );
IMPLEMENT_CONVERT_METHODS( HumanState );
IMPLEMENT_CONVERT_METHODS( HumanWound );
IMPLEMENT_CONVERT_METHODS( InjuriesSeriousness );
IMPLEMENT_CONVERT_METHODS( LayerTypes );
IMPLEMENT_CONVERT_METHODS( LightingType );
IMPLEMENT_CONVERT_METHODS( Location );
IMPLEMENT_CONVERT_METHODS( LocationType );
IMPLEMENT_CONVERT_METHODS( MeetingEngagementStatus );
IMPLEMENT_CONVERT_METHODS( MissionType );
IMPLEMENT_CONVERT_METHODS( Modes );
IMPLEMENT_CONVERT_METHODS( NatureLevel );
IMPLEMENT_CONVERT_METHODS( NbcState );
IMPLEMENT_CONVERT_METHODS( ObstacleActivation );
IMPLEMENT_CONVERT_METHODS( OperationalStatus );
IMPLEMENT_CONVERT_METHODS( PopulationAttitude );
IMPLEMENT_CONVERT_METHODS( PopulationRoe );
IMPLEMENT_CONVERT_METHODS( Roe );
IMPLEMENT_CONVERT_METHODS( UnitExperience );
IMPLEMENT_CONVERT_METHODS( UnitMorale );
IMPLEMENT_CONVERT_METHODS( UnitPosture );
IMPLEMENT_CONVERT_METHODS( UnitStress );
IMPLEMENT_CONVERT_METHODS( UnitTiredness );
IMPLEMENT_CONVERT_METHODS( WeatherType );

IMPLEMENT_CONVERT_METHODS_PROTO( DotationType, DotationType );
IMPLEMENT_CONVERT_METHODS_PROTO( EnumLogisticLevel, LogisticLevel );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( KnowledgeMagicAction, Type, KnowledgeMagicActionType );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( LogFuneralHandlingUpdate, EnumLogFuneralHandlingStatus, LogFuneralHandlingStatus );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( LogMaintenanceHandlingUpdate, EnumLogMaintenanceHandlingStatus, LogMaintenanceHandlingStatus );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( LogMedicalHandlingUpdate, EnumLogMedicalHandlingStatus, LogMedicalHandlingStatus );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( LogSupplyHandlingUpdate, EnumLogSupplyHandlingStatus, LogSupplyHandlingStatus );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( MagicAction, Type, MagicActionType );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( ObjectMagicAction, Type, ObjectMagicActionType );
IMPLEMENT_CONVERT_METHODS_SUB_PROTO( UnitMagicAction, Type, UnitMagicActionType );
