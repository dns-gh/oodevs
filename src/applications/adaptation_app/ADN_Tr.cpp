// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Tr.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 10:15 $
// $Revision: 12 $
// $Workfile: ADN_Tr.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Tr.h"

#include "ENT/ENT_Tr.h"
#include "ENT/ENT_Private.h"

#include <tools/Helpers.h>

#include <boost/static_assert.hpp>

using namespace ENT_Tr;

namespace
{
typedef Converter< E_Location >                 T_ConverterLocation;
typedef Converter< E_ConsumptionType >          T_ConverterConsumptionType;
typedef Converter< E_VisionObject >             T_ConverterVisionObject;
typedef Converter< E_KeyPoint >                 T_ConverterKeyPoint;
typedef Converter< E_AgentTypePion >            T_ConverterAgentTypePion;
typedef Converter< E_AgentTypeAutomate >        T_ConverterAgentTypeAutomate;
typedef Converter< E_NatureAtlasType >          T_ConverterNatureAtlasType;
typedef Converter< E_SpeedImpact >              T_ConverterSpeedImpact;
typedef Converter< E_CrossingHeight >           T_ConverterCrossingHeight;
typedef Converter< E_SensorWeatherModifiers >   T_ConverterSensorWeatherModifiers;
typedef Converter< E_DoctorSkills >             T_ConverterDoctorSkills;
typedef Converter< E_ProtectionType >           T_ConverterProtectionType;
typedef Converter< E_RadarType >                T_ConverterRadarType;
typedef Converter< E_EquipmentState_ADN >       T_ConverterEquipmentState;
typedef Converter< E_TypeMunitionTirIndirect >  T_ConverterTypeMunitionTirIndirect;
typedef Converter< E_MissionParameterType >     T_ConverterMissionParameterType;
typedef Converter< E_SupplyConvoyType >         T_ConverterSupplyConvoyType;
typedef Converter< E_ConstructorType >          T_ConverterConstructorType;
typedef Converter< E_PropagationModel >         T_ConverterPropagationModel;
typedef Converter< E_WorkspaceElements >        T_ConverterWorkspaceElements;
typedef Converter< E_Days >                     T_ConverterDays;
typedef Converter< E_ContextParameters >        T_ConverterContextParameters;
typedef Converter< E_EntityType >               T_ConverterEntityType;

// -----------------------------------------------------------------------------
// Name: ADN_Tr::InitTranslations
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
T_ConverterTypeMunitionTirIndirect TypeMunitionTirIndirectConverter_[] =
{
    T_ConverterTypeMunitionTirIndirect( "explosif",  QT_TRANSLATE_NOOP( "ADN_Tr", "explosive" ),          eTypeMunitionTirIndirect_Explosif ),
    T_ConverterTypeMunitionTirIndirect( "fumigene",  QT_TRANSLATE_NOOP( "ADN_Tr", "smoke" ),              eTypeMunitionTirIndirect_Fumigene ),
    T_ConverterTypeMunitionTirIndirect( "eclairant", QT_TRANSLATE_NOOP( "ADN_Tr", "illumination shell" ), eTypeMunitionTirIndirect_Eclairant ),
    T_ConverterTypeMunitionTirIndirect( "mine",      QT_TRANSLATE_NOOP( "ADN_Tr", "mine" ),               eTypeMunitionTirIndirect_Mine ),
    T_ConverterTypeMunitionTirIndirect( "effect",    QT_TRANSLATE_NOOP( "ADN_Tr", "effect" ),             eTypeMunitionTirIndirect_Effect ),
    T_ConverterTypeMunitionTirIndirect( "", "", (E_TypeMunitionTirIndirect)-1 )
};

T_ConverterConsumptionType ConsumptionTypeConverter_[] =
{
    T_ConverterConsumptionType( "ArretMoteurEteint" , QT_TRANSLATE_NOOP( "ADN_Tr" , "Parked, engine stopped" ), eEngineStopped ) ,
    T_ConverterConsumptionType( "ArretMoteurAllume" , QT_TRANSLATE_NOOP( "ADN_Tr" , "Parked, engine running" ), eEngineStarted ) ,
    T_ConverterConsumptionType( "Deplacement" ,       QT_TRANSLATE_NOOP( "ADN_Tr" , "Moving" ),                 eMoving ) ,
    T_ConverterConsumptionType( "EnTravaux" ,         QT_TRANSLATE_NOOP( "ADN_Tr" , "Working" ),                eWorking ) ,
    T_ConverterConsumptionType( "" , "" , (E_ConsumptionType)-1 )
};

T_ConverterVisionObject VisionObjectConverter_[] =
{
    T_ConverterVisionObject( "Sol"   , QT_TRANSLATE_NOOP("ADN_Tr" ,"Ground"),     eVisionGround ),
    T_ConverterVisionObject( "Vide"  , QT_TRANSLATE_NOOP("ADN_Tr" ,"Empty"),      eVisionEmpty  ),
    T_ConverterVisionObject( "Foret" , QT_TRANSLATE_NOOP("ADN_Tr" ,"Forest"),     eVisionForest ),
    T_ConverterVisionObject( "Urbain", QT_TRANSLATE_NOOP("ADN_Tr" ,"Urban area"), eVisionUrban  ),
    T_ConverterVisionObject( "", "", (E_VisionObject)-1 )
};

T_ConverterKeyPoint KeyPointConverter_[] =
{
    T_ConverterKeyPoint( "forest",     QT_TRANSLATE_NOOP( "ADN_Tr" , "forest" ),     eKeyPointForest ),
    T_ConverterKeyPoint( "urban area", QT_TRANSLATE_NOOP( "ADN_Tr" , "urban area" ), eKeyPointUrban ),
    T_ConverterKeyPoint( "crossroad",  QT_TRANSLATE_NOOP( "ADN_Tr" , "crossroad" ),  eKeyPointCrossroads ),
    T_ConverterKeyPoint( "bridge",     QT_TRANSLATE_NOOP( "ADN_Tr" , "bridge" ),     eKeyPointBridge ),
    T_ConverterKeyPoint( "lima",       QT_TRANSLATE_NOOP( "ADN_Tr" , "lima" ),       eKeyPointLima ),
    T_ConverterKeyPoint( "" , "" , (E_KeyPoint)-1 )
};

T_ConverterAgentTypePion AgentTypePionConverter_[] =
{
    T_ConverterAgentTypePion( "Pion INF"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Infantry" ),                           eAgentTypePionINF ),
    T_ConverterAgentTypePion( "Pion ASA"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Anti air" ),                           eAgentTypePionASA ),
    T_ConverterAgentTypePion( "Pion ALAT"                       ,QT_TRANSLATE_NOOP( "ADN_Tr", "Army aviation" ),                      eAgentTypePionALAT ),
    T_ConverterAgentTypePion( "Pion RENS"                       ,QT_TRANSLATE_NOOP( "ADN_Tr", "Intelligence" ),                       eAgentTypePionRENS ),
    T_ConverterAgentTypePion( "Pion NBC"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "NBC" ),                                eAgentTypePionNBC ),
    T_ConverterAgentTypePion( "Pion ABC"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Armor" ),                              eAgentTypePionABC ),
    T_ConverterAgentTypePion( "Pion GEN"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Engineer" ),                           eAgentTypePionGEN ),
    T_ConverterAgentTypePion( "Pion ASS"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Ground artillery" ),                   eAgentTypePionASS ),
    T_ConverterAgentTypePion( "Pion TRANS"                      ,QT_TRANSLATE_NOOP( "ADN_Tr", "Communication" ),                      eAgentTypePionTRANS ),
    T_ConverterAgentTypePion( "Pion LOG TC2"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Logistic close support" ),             eAgentTypePionLOGTC2 ),
    T_ConverterAgentTypePion( "Pion LOG BLD Sante"              ,QT_TRANSLATE_NOOP( "ADN_Tr", "Medical system, brigade level" ),      eAgentTypePionLOGBLDSante ),
    T_ConverterAgentTypePion( "Pion LOG BLD Maintenance"        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Maintenance system, brigade level" ),  eAgentTypePionLOGBLDMaintenance ),
    T_ConverterAgentTypePion( "Pion LOG BLD Ravitaillement"     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Supply system, brigade level" ),       eAgentTypePionLOGBLDRavitaillement ),
    T_ConverterAgentTypePion( "Pion LOG BLT Sante"              ,QT_TRANSLATE_NOOP( "ADN_Tr", "Medical system, division level" ),     eAgentTypePionLOGBLTSante ),
    T_ConverterAgentTypePion( "Pion LOG BLT Maintenance"        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Maintenance system, division level" ), eAgentTypePionLOGBLTMaintenance ),
    T_ConverterAgentTypePion( "Pion LOG BLT Ravitaillement"     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Supply system, division level" ),      eAgentTypePionLOGBLTRavitaillement ),
    T_ConverterAgentTypePion( "Pion LOG Convoi"                 ,QT_TRANSLATE_NOOP( "ADN_Tr", "Convoy" ),                             eAgentTypePionLOGConvoi ),
    T_ConverterAgentTypePion( "Pion CIRCULATION"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Movement support" ),                   eAgentTypePionCIRCULATION ),
    T_ConverterAgentTypePion( "Pion REFUGIE"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Refugee" ),                            eAgentTypePionREFUGIE ),
    T_ConverterAgentTypePion( "Pion MILICE"                     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Militia" ),                            eAgentTypePionMILICE ),
    T_ConverterAgentTypePion( "Pion JOINT"                      ,QT_TRANSLATE_NOOP( "ADN_Tr", "Joint" ),                              eAgentTypePionJOINT ),
    T_ConverterAgentTypePion( "Pion ASY"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Asymetric threat" ),                   eAgentTypePionASY ),
    T_ConverterAgentTypePion( "Pion Emergency"                  ,QT_TRANSLATE_NOOP( "ADN_Tr", "Emergency management" ),               eAgentTypePionEmergency ),
    T_ConverterAgentTypePion( "Pion Organization"               ,QT_TRANSLATE_NOOP( "ADN_Tr", "Organization" ),                       eAgentTypePionOrganization ),
    T_ConverterAgentTypePion( "Pion Notable"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Notable" ),                            eAgentTypePionNotable ),
    T_ConverterAgentTypePion( "Pion Journalist"                 ,QT_TRANSLATE_NOOP( "ADN_Tr", "Journalist" ),                         eAgentTypePionJournalist ),
    T_ConverterAgentTypePion( "Pion Civilian"                   ,QT_TRANSLATE_NOOP( "ADN_Tr", "Civilian" ),                           eAgentTypePionCivilian ),
    T_ConverterAgentTypePion( "Pion Remote"                     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Remote" ),                             eAgentTypePionRemote ),
    T_ConverterAgentTypePion( "", "", (E_AgentTypePion)-1 )
};

T_ConverterAgentTypeAutomate AgentTypeAutomateConverter_[] =
{
    T_ConverterAgentTypeAutomate( "Automate SGTIA"                  ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Combined Arms" ),                      eAgentTypeAutomateSGTIA ),
    T_ConverterAgentTypeAutomate( "Automate INF"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Infantry" ),                           eAgentTypeAutomateINF ),
    T_ConverterAgentTypeAutomate( "Automate ASA"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Anti air" ),                           eAgentTypeAutomateASA ),
    T_ConverterAgentTypeAutomate( "Automate ALAT"                   ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Army aviation" ),                      eAgentTypeAutomateALAT ),
    T_ConverterAgentTypeAutomate( "Automate RENS"                   ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Intelligence" ),                       eAgentTypeAutomateRENS ),
    T_ConverterAgentTypeAutomate( "Automate NBC"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, NBC" ),                                eAgentTypeAutomateNBC ),
    T_ConverterAgentTypeAutomate( "Automate ABC"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Armor" ),                              eAgentTypeAutomateABC ),
    T_ConverterAgentTypeAutomate( "Automate GEN"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Engineer" ),                           eAgentTypeAutomateGEN ),
    T_ConverterAgentTypeAutomate( "Automate ASS"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Ground artillery" ),                   eAgentTypeAutomateASS ),
    T_ConverterAgentTypeAutomate( "Automate TRANS"                  ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Communication" ),                      eAgentTypeAutomateTRANS ),
    T_ConverterAgentTypeAutomate( "Automate LOG TC2"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Logistic close support" ),             eAgentTypeAutomateLOGTC2 ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLD Sante"          ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Medical system, brigade level" ),      eAgentTypeAutomateLOGBLDSante ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLD Maintenance"    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Maintenance system, brigade level" ),  eAgentTypeAutomateLOGBLDMaintenance ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLD Ravitaillement" ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Supply system, brigade level" ),       eAgentTypeAutomateLOGBLDRavitaillement ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLT Sante"          ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Medical system, division level" ),     eAgentTypeAutomateLOGBLTSante ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLT Maintenance"    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Maintenance system, division level" ), eAgentTypeAutomateLOGBLTMaintenance ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLT Ravitaillement" ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Supply system, division level" ),      eAgentTypeAutomateLOGBLTRavitaillement ),
    T_ConverterAgentTypeAutomate( "Automate CIRCULATION"            ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Movement support" ),                   eAgentTypeAutomateCIRCULATION ),
    T_ConverterAgentTypeAutomate( "Automate REFUGIE"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Refugee" ),                            eAgentTypeAutomateREFUGIE ),
    T_ConverterAgentTypeAutomate( "Automate MILICE"                 ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Militia" ),                            eAgentTypeAutomateMILICE ),
    T_ConverterAgentTypeAutomate( "Automate JOINT"                  ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Joint" ),                              eAgentTypeAutomateJOINT ),
    T_ConverterAgentTypeAutomate( "Automate ASY"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Asymetric threat" ),                   eAgentTypeAutomateASY ),
    T_ConverterAgentTypeAutomate( "Automate Emergency"              ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Emergency management" ),               eAgentTypeAutomateEmergency ),
    T_ConverterAgentTypeAutomate( "Automate Organization"           ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Organization" ),                       eAgentTypeAutomateOrganization ),
    T_ConverterAgentTypeAutomate( "Automate Notable"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Notable" ),                            eAgentTypeAutomateNotable ),
    T_ConverterAgentTypeAutomate( "Automate Journalist"             ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Journalist" ),                         eAgentTypeAutomateJournalist ),
    T_ConverterAgentTypeAutomate( "Automate Civilian"               ,QT_TRANSLATE_NOOP( "ADN_Tr", "Commander, Civilian" ),                           eAgentTypeAutomateCivilian ),
    T_ConverterAgentTypeAutomate( "", "", (E_AgentTypeAutomate)-1 )
};

T_ConverterNatureAtlasType NatureAtlasTypeConverter_[] =
{
    T_ConverterNatureAtlasType( "PC"               ,QT_TRANSLATE_NOOP("ADN_Tr" ,"CP"),                  eNatureAtlas_PC             ),
    T_ConverterNatureAtlasType( "ASS"              ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Ground artillery"),    eNatureAtlas_ASS            ),
    T_ConverterNatureAtlasType( "Log"              ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Logistic"),            eNatureAtlas_Log            ),
    T_ConverterNatureAtlasType( "None"             ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Other"),               eNatureAtlas_None           ),
    T_ConverterNatureAtlasType( "Blinde"           ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Armor"),               eNatureAtlas_Armored        ),
    T_ConverterNatureAtlasType( "Mortier"          ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Mortar"),              eNatureAtlas_Mortar         ),
    T_ConverterNatureAtlasType( "Lance roquette"   ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Rocket launcher"),     eNatureAtlas_RocketLauncher ),
    T_ConverterNatureAtlasType( "Vehicule"         ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Vehicle"),             eNatureAtlas_Vehicule       ),
    T_ConverterNatureAtlasType( "Personnel a pied" ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Dismounted infantry"), eNatureAtlas_PersonnelAPied ),
    T_ConverterNatureAtlasType( "Helicoptere"      ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Helicopter"),          eNatureAtlas_Helicoptere    ),
    T_ConverterNatureAtlasType( "ASA"              ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Anti air artillery"),  eNatureAtlas_ASA            ),
    T_ConverterNatureAtlasType( "Indefini"         ,QT_TRANSLATE_NOOP("ADN_Tr" ,"Undefined"),           eNatureAtlas_Indefini       ),
    T_ConverterNatureAtlasType( "", "", (E_NatureAtlasType)-1 )
};

T_ConverterSpeedImpact SpeedImpactConverter_[] =
{
    T_ConverterSpeedImpact( "AuPlusLent",        QT_TRANSLATE_NOOP( "ADN_Tr", "Slowest" ),           eSpeedImpact_AuPlusLent ),
    T_ConverterSpeedImpact( "VitesseMaxObjet",   QT_TRANSLATE_NOOP( "ADN_Tr", "Object, Max speed" ), eSpeedImpact_VitesseMaxObjet ),
    T_ConverterSpeedImpact( "VitesseMaxAgent",   QT_TRANSLATE_NOOP( "ADN_Tr", "Unit, Max speed" ),   eSpeedImpact_VitesseMaxAgent ),
    T_ConverterSpeedImpact( "VitesseBlocUrbain", QT_TRANSLATE_NOOP( "ADN_Tr", "Urban block speed" ), eSpeedImpact_VitesseBlocUrbain ),
    T_ConverterSpeedImpact( "", "", (E_SpeedImpact)-1 )
};

T_ConverterCrossingHeight CrossingHeightConverter_[] =
{
    T_ConverterCrossingHeight( "0m",  QT_TRANSLATE_NOOP( "ADN_Tr", "0m" ),  eCrossingHeight_Never ),
    T_ConverterCrossingHeight( "1m",  QT_TRANSLATE_NOOP( "ADN_Tr", "1m" ),  eCrossingHeight_LowAreas ),
    T_ConverterCrossingHeight( ">1m", QT_TRANSLATE_NOOP( "ADN_Tr", ">1m" ), eCrossingHeight_Always ),
    T_ConverterCrossingHeight( "", "", (E_CrossingHeight)-1 )
};

T_ConverterSensorWeatherModifiers SensorWeatherModifiersConverter_[] =
{
    T_ConverterSensorWeatherModifiers( "PasDePrecipitation", QT_TRANSLATE_NOOP( "ADN_Tr", "No rainfall" ), eSensorWeatherModifiers_PasDePrecipitation ),
    T_ConverterSensorWeatherModifiers( "TempeteDeSable",     QT_TRANSLATE_NOOP( "ADN_Tr", "Sand storm" ),  eSensorWeatherModifiers_TempeteSable ),
    T_ConverterSensorWeatherModifiers( "Brouillard",         QT_TRANSLATE_NOOP( "ADN_Tr", "Fog" ),         eSensorWeatherModifiers_Brouillard ),
    T_ConverterSensorWeatherModifiers( "Crachin",            QT_TRANSLATE_NOOP( "ADN_Tr", "Drizzle" ),     eSensorWeatherModifiers_Crachin ),
    T_ConverterSensorWeatherModifiers( "Pluie",              QT_TRANSLATE_NOOP( "ADN_Tr", "Rain" ),        eSensorWeatherModifiers_Pluie ),
    T_ConverterSensorWeatherModifiers( "Neige",              QT_TRANSLATE_NOOP( "ADN_Tr", "Snow" ),        eSensorWeatherModifiers_Neige ),
    T_ConverterSensorWeatherModifiers( "Fumigene",           QT_TRANSLATE_NOOP( "ADN_Tr", "Smoke" ),       eSensorWeatherModifiers_Fumigene ),
    T_ConverterSensorWeatherModifiers( "", "", (E_SensorWeatherModifiers)-1 )
};

T_ConverterDoctorSkills DoctorSkillsConverter_[] =
{
    T_ConverterDoctorSkills( "U1", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded seriousness level 1" ), eDoctorSkills_U1 ),
    T_ConverterDoctorSkills( "U2", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded seriousness level 2" ), eDoctorSkills_U2 ),
    T_ConverterDoctorSkills( "U3", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded seriousness level 3" ), eDoctorSkills_U3 ),
    T_ConverterDoctorSkills( "UE", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded extreme seriousness" ), eDoctorSkills_UE ),
    T_ConverterDoctorSkills( "", "", (E_DoctorSkills)-1 )
};

T_ConverterProtectionType ProtectionTypeConverter_[] =
{
    T_ConverterProtectionType( "materiel", QT_TRANSLATE_NOOP( "ADN_Tr", "Equipment" ), eProtectionType_Material ),
    T_ConverterProtectionType( "humain",   QT_TRANSLATE_NOOP( "ADN_Tr", "Human" ),     eProtectionType_Human ),
    T_ConverterProtectionType( "foule",    QT_TRANSLATE_NOOP( "ADN_Tr", "crowd" ),     eProtectionType_Crowd ),
    T_ConverterProtectionType( "", "", (E_ProtectionType)-1 )
};

T_ConverterSupplyConvoyType SupplyConvoyTypeConverter_[] =
{
    T_ConverterSupplyConvoyType( "real", QT_TRANSLATE_NOOP( "ADN_Tr", "Real" ), eSupplyConvoy_Real ),
    T_ConverterSupplyConvoyType( "virtual", QT_TRANSLATE_NOOP( "ADN_Tr", "Virtual" ), eSupplyConvoy_Virtual ),
    T_ConverterSupplyConvoyType( "", "", (E_SupplyConvoyType)-1 )
};

T_ConverterRadarType RadarTypeConverter_[] =
{
    T_ConverterRadarType( "radar",          QT_TRANSLATE_NOOP( "ADN_Tr", "radar" ),           eRadarType_Radar ),
    T_ConverterRadarType( "ecoute",         QT_TRANSLATE_NOOP( "ADN_Tr", "listening" ),       eRadarType_Ecoute ),
    T_ConverterRadarType( "ecoute radar",   QT_TRANSLATE_NOOP( "ADN_Tr", "radar listening" ), eRadarType_EcouteRadar ),
    T_ConverterRadarType( "", "", (E_RadarType)-1 )
};

T_ConverterEquipmentState EquipmentState_ADNConverter_[] =
{
    T_ConverterEquipmentState( "ReparableAvecEvacuation", QT_TRANSLATE_NOOP("ADN_Tr", "Maintenance support needed" ), eEquipmentState_ADN_FixableWithEvac ),
    T_ConverterEquipmentState( "ReparableSurPlace"      , QT_TRANSLATE_NOOP("ADN_Tr", "On site fixable"),             eEquipmentState_ADN_FixableInPlace ),
    T_ConverterEquipmentState( "Detruit"                , QT_TRANSLATE_NOOP("ADN_Tr", "Destroyed"),                   eEquipmentState_ADN_Destroyed ),
    T_ConverterEquipmentState( "", "", (E_EquipmentState_ADN)-1 )
};

T_ConverterMissionParameterType MissionParameterTypeConverter_[] =
{
    T_ConverterMissionParameterType( "Agent",                 QT_TRANSLATE_NOOP("ADN_Tr", "Agent" ),                  eMissionParameterTypeAgent ),
    T_ConverterMissionParameterType( "Automat",               QT_TRANSLATE_NOOP("ADN_Tr", "Automat" ),                eMissionParameterTypeAutomate ),
    T_ConverterMissionParameterType( "Boolean",               QT_TRANSLATE_NOOP("ADN_Tr", "Boolean" ),                eMissionParameterTypeBool ),
    T_ConverterMissionParameterType( "Integer",               QT_TRANSLATE_NOOP("ADN_Tr", "Integer" ),                eMissionParameterTypeNumeric ),
    T_ConverterMissionParameterType( "String",                QT_TRANSLATE_NOOP("ADN_Tr", "String" ),                 eMissionParameterTypeString ),
    T_ConverterMissionParameterType( "Heading",               QT_TRANSLATE_NOOP("ADN_Tr", "Heading" ),                eMissionParameterTypeDirection ),
    T_ConverterMissionParameterType( "DateTime",              QT_TRANSLATE_NOOP("ADN_Tr", "Date Time" ),              eMissionParameterTypeGDH ),
    T_ConverterMissionParameterType( "Enumeration",           QT_TRANSLATE_NOOP("ADN_Tr", "Enumeration" ),            eMissionParameterTypeEnumeration ),
    T_ConverterMissionParameterType( "NatureAtlas",           QT_TRANSLATE_NOOP("ADN_Tr", "Nature Atlas" ),           eMissionParameterTypeNatureAtlas ),
    T_ConverterMissionParameterType( "AgentKnowledge",        QT_TRANSLATE_NOOP("ADN_Tr", "Agent knowledge" ),        eMissionParameterTypeAgentKnowledge ),
    T_ConverterMissionParameterType( "ObjectKnowledge",       QT_TRANSLATE_NOOP("ADN_Tr", "Object knowledge" ),       eMissionParameterTypeObjectKnowledge ),
    T_ConverterMissionParameterType( "CrowdKnowledge",        QT_TRANSLATE_NOOP("ADN_Tr", "Crowd knowledge" ),        eMissionParameterTypePopulationKnowledge ),
    T_ConverterMissionParameterType( "PlannedWork",           QT_TRANSLATE_NOOP("ADN_Tr", "Planned work" ),           eMissionParameterTypeGenObject ),
    T_ConverterMissionParameterType( "Path",                  QT_TRANSLATE_NOOP("ADN_Tr", "Path" ),                   eMissionParameterTypePath ),
    T_ConverterMissionParameterType( "Point",                 QT_TRANSLATE_NOOP("ADN_Tr", "Point" ),                  eMissionParameterTypePoint ),
    T_ConverterMissionParameterType( "Polygon",               QT_TRANSLATE_NOOP("ADN_Tr", "Polygon" ),                eMissionParameterTypePolygon ),
    T_ConverterMissionParameterType( "Location",              QT_TRANSLATE_NOOP("ADN_Tr", "Location" ),               eMissionParameterTypeLocation ),
    T_ConverterMissionParameterType( "EquipmentType",         QT_TRANSLATE_NOOP("ADN_Tr", "Equipment type" ),         eMissionParameterTypeEquipmentType ),
    T_ConverterMissionParameterType( "ResourceType",          QT_TRANSLATE_NOOP("ADN_Tr", "Resource type" ),          eMissionParameterTypeDotationType ),
    T_ConverterMissionParameterType( "MaintenancePriorities", QT_TRANSLATE_NOOP("ADN_Tr", "Maintenance priorities" ), eMissionParameterTypeMaintenancePriorities ),
    T_ConverterMissionParameterType( "MedicalPriorities",     QT_TRANSLATE_NOOP("ADN_Tr", "Medical priorities" ),     eMissionParameterTypeMedicalPriorities ),
    T_ConverterMissionParameterType( "IndirectFire",          QT_TRANSLATE_NOOP("ADN_Tr", "Indirect fire" ),          eMissionParameterTypeIndirectFire ),
    T_ConverterMissionParameterType( "Limit",                 QT_TRANSLATE_NOOP("ADN_Tr", "Limit" ),                  eMissionParameterTypeLimit ),
    T_ConverterMissionParameterType( "PhaseLine",             QT_TRANSLATE_NOOP("ADN_Tr", "Phase line" ),             eMissionParameterTypePhaseLine ),
    T_ConverterMissionParameterType( "UrbanKnowledge",        QT_TRANSLATE_NOOP("ADN_Tr", "Urban knowledge" ),        eMissionParameterTypeUrbanBlock ),
    T_ConverterMissionParameterType( "LocationComposite",     QT_TRANSLATE_NOOP("ADN_Tr", "Location composite" ),     eMissionParameterTypeLocationComposite ),
    T_ConverterMissionParameterType( "ResourceNetworkNode",   QT_TRANSLATE_NOOP("ADN_Tr", "Resource network node" ),  eMissionParameterTypeResourceNetworkNode ),
    T_ConverterMissionParameterType( "ResourceNetworkType",   QT_TRANSLATE_NOOP("ADN_Tr", "Resource network type" ),  eMissionParameterTypeResourceNetworkType ),
    T_ConverterMissionParameterType( "Stage",                 QT_TRANSLATE_NOOP("ADN_Tr", "Stage" ),                  eMissionParameterTypeStage ),
    T_ConverterMissionParameterType( "LimaFunction",          QT_TRANSLATE_NOOP("ADN_Tr", "LimaFunction" ),           eMissionParameterTypeLimaFunction ),

    T_ConverterMissionParameterType( "", "", (E_MissionParameterType)-1 )
};

T_ConverterConstructorType ConstructorTypeConverter_[] =
{
    T_ConverterConstructorType( "raw",     QT_TRANSLATE_NOOP("ADN_Tr", "Raw" ), eRaw ),
    T_ConverterConstructorType( "density", QT_TRANSLATE_NOOP("ADN_Tr", "Density" ), eDensity ),

    T_ConverterConstructorType( "", "", ( E_ConstructorType )-1 )
};

T_ConverterPropagationModel PropagationModelConverter_[] =
{
    T_ConverterPropagationModel( "shapefile-input", QT_TRANSLATE_NOOP("ADN_Tr", "Shapefile input" ), eInputShap ),
    T_ConverterPropagationModel( "fire",  QT_TRANSLATE_NOOP("ADN_Tr", "Fire" ), eFire ),
    T_ConverterPropagationModel( "cloud", QT_TRANSLATE_NOOP("ADN_Tr", "Cloud" ), eCloud ),

    T_ConverterPropagationModel( "", "", (E_PropagationModel)-1 )
};

T_ConverterWorkspaceElements WorkspaceElementsConverter_[] =
{
    T_ConverterWorkspaceElements( "categories",       QT_TRANSLATE_NOOP("ADN_Tr", "Categories" ),           eCategories ),
    T_ConverterWorkspaceElements( "symbols",          QT_TRANSLATE_NOOP("ADN_Tr", "Symbols" ),              eSymbols ),
    T_ConverterWorkspaceElements( "urban",            QT_TRANSLATE_NOOP("ADN_Tr", "Urban" ),                eUrban ),
    T_ConverterWorkspaceElements( "nbc",              QT_TRANSLATE_NOOP("ADN_Tr", "NBC" ),                  eNBC ),
    T_ConverterWorkspaceElements( "launcher",         QT_TRANSLATE_NOOP("ADN_Tr", "Launchers" ),            eLaunchers ),
    T_ConverterWorkspaceElements( "resources",        QT_TRANSLATE_NOOP("ADN_Tr", "Supplies" ),             eSupplies ),
    T_ConverterWorkspaceElements( "fireclasses",      QT_TRANSLATE_NOOP("ADN_Tr", "Fires" ),                eFires ),
    T_ConverterWorkspaceElements( "drawings",         QT_TRANSLATE_NOOP("ADN_Tr", "Drawings" ),             eDrawings ),
    T_ConverterWorkspaceElements( "objects",          QT_TRANSLATE_NOOP("ADN_Tr", "Objects" ),              eObjects ),
    T_ConverterWorkspaceElements( "weapons",          QT_TRANSLATE_NOOP("ADN_Tr", "Weapon systems" ),       eWeapons ),
    T_ConverterWorkspaceElements( "activeprotection", QT_TRANSLATE_NOOP("ADN_Tr", "Active Protections" ),   eActiveProtections ),
    T_ConverterWorkspaceElements( "sensors",          QT_TRANSLATE_NOOP("ADN_Tr", "Sensors" ),              eSensors ),
    T_ConverterWorkspaceElements( "breakdowns",       QT_TRANSLATE_NOOP("ADN_Tr", "Breakdowns" ),           eBreakdowns ),
    T_ConverterWorkspaceElements( "equipments",       QT_TRANSLATE_NOOP("ADN_Tr", "Equipments" ),           eEquipments ),
    T_ConverterWorkspaceElements( "resourcenetworks", QT_TRANSLATE_NOOP("ADN_Tr", "Resource networks" ),    eResourceNetworks ),
    T_ConverterWorkspaceElements( "aiengine",         QT_TRANSLATE_NOOP("ADN_Tr", "Op. indicators" ),       eAiEngine ),
    T_ConverterWorkspaceElements( "missions",         QT_TRANSLATE_NOOP("ADN_Tr", "Missions" ),             eMissions ),
    T_ConverterWorkspaceElements( "models",           QT_TRANSLATE_NOOP("ADN_Tr", "Doctrine models" ),      eModels ),
    T_ConverterWorkspaceElements( "units",            QT_TRANSLATE_NOOP("ADN_Tr", "Units" ),                eUnits ),
    T_ConverterWorkspaceElements( "automata",         QT_TRANSLATE_NOOP("ADN_Tr", "Automata" ),             eAutomata ),
    T_ConverterWorkspaceElements( "communications",   QT_TRANSLATE_NOOP("ADN_Tr", "Jamming" ),              eCommunications ),
    T_ConverterWorkspaceElements( "humanfactors",     QT_TRANSLATE_NOOP("ADN_Tr", "Human factors" ),        eHumanFactors ),
    T_ConverterWorkspaceElements( "knowledgegroups",  QT_TRANSLATE_NOOP("ADN_Tr", "Knowledge groups" ),     eKnowledgeGroups ),
    T_ConverterWorkspaceElements( "crowds",           QT_TRANSLATE_NOOP("ADN_Tr", "Crowds" ),               eCrowds ),
    T_ConverterWorkspaceElements( "inhabitant",       QT_TRANSLATE_NOOP("ADN_Tr", "Populations" ),          eInhabitants ),
    T_ConverterWorkspaceElements( "reports",          QT_TRANSLATE_NOOP("ADN_Tr", "Reports" ),              eReports ),
    T_ConverterWorkspaceElements( "logistic",         QT_TRANSLATE_NOOP("ADN_Tr", "Log" ),                  eLogistic ),
    T_ConverterWorkspaceElements( "disasters",        QT_TRANSLATE_NOOP("ADN_Tr", "Disasters" ),            eDisasters ),
    T_ConverterWorkspaceElements( "languages",        QT_TRANSLATE_NOOP("ADN_Tr", "Languages" ),            eLanguages ),
    T_ConverterWorkspaceElements( "", "", (E_WorkspaceElements)-1 )
};

T_ConverterDays DaysConverter_[] =
{
    T_ConverterDays( "monday",    QT_TRANSLATE_NOOP( "ADN_Tr", "Monday" ),    eDays_Monday ),
    T_ConverterDays( "tuesday",   QT_TRANSLATE_NOOP( "ADN_Tr", "Tuesday" ),   eDays_Tuesday ),
    T_ConverterDays( "wednesday", QT_TRANSLATE_NOOP( "ADN_Tr", "Wednesday" ), eDays_Wednesday ),
    T_ConverterDays( "thursday",  QT_TRANSLATE_NOOP( "ADN_Tr", "Thursday" ),  eDays_Thursday ),
    T_ConverterDays( "friday",    QT_TRANSLATE_NOOP( "ADN_Tr", "Friday" ),    eDays_Friday ),
    T_ConverterDays( "saturday",  QT_TRANSLATE_NOOP( "ADN_Tr", "Saturday" ),  eDays_Saturday ),
    T_ConverterDays( "sunday",    QT_TRANSLATE_NOOP( "ADN_Tr", "Sunday" ),    eDays_Sunday ),

    T_ConverterDays( "", "", (E_Days)-1 )
};

T_ConverterContextParameters ContextParametersConverter_[] =
{
    T_ConverterContextParameters( "dangerDirection_",   QT_TRANSLATE_NOOP( "ADN_Tr", "Danger direction" ),  eContextParameters_Heading ),
    T_ConverterContextParameters( "phaseLines_",        QT_TRANSLATE_NOOP( "ADN_Tr", "Phase lines" ),       eContextParameters_Limas ),
    T_ConverterContextParameters( "boundaryLimit1_",    QT_TRANSLATE_NOOP( "ADN_Tr", "Limit 1" ),           eContextParameters_Limit1 ),
    T_ConverterContextParameters( "boundaryLimit2_",    QT_TRANSLATE_NOOP( "ADN_Tr", "Limit 2" ),           eContextParameters_Limit2 ),

    T_ConverterContextParameters( "", "", (E_ContextParameters)-1 )
};

T_ConverterEntityType EntityTypeConverter_[] =
{
    T_ConverterEntityType( "unit",    QT_TRANSLATE_NOOP( "ADN_Tr", "Unit" ),    eEntityType_Pawn ),
    T_ConverterEntityType( "automat", QT_TRANSLATE_NOOP( "ADN_Tr", "Automat" ), eEntityType_Automat ),
    T_ConverterEntityType( "crowd",   QT_TRANSLATE_NOOP( "ADN_Tr", "Crowd" ),   eEntityType_Population ),

    T_ConverterEntityType( "", "", (E_EntityType)-1 )
};
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromConsumptionType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromConsumptionType( E_ConsumptionType nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( ConsumptionTypeConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromVisionObject
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromVisionObject( E_VisionObject nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( VisionObjectConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromKeyPoint
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromKeyPoint( E_KeyPoint nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( KeyPointConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromAgentTypePion
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromAgentTypePion( E_AgentTypePion nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( AgentTypePionConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromAgentTypeAutomate
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromAgentTypeAutomate( E_AgentTypeAutomate nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( AgentTypeAutomateConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromNatureAtlasType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromNatureAtlasType( E_NatureAtlasType nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( NatureAtlasTypeConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromSpeedImpact
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromSpeedImpact( E_SpeedImpact nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( SpeedImpactConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromCrossingHeight
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromCrossingHeight( E_CrossingHeight nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( CrossingHeightConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromSensorWeatherModifiers
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromSensorWeatherModifiers( E_SensorWeatherModifiers nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( SensorWeatherModifiersConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromDoctorSkills
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromDoctorSkills( E_DoctorSkills nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( DoctorSkillsConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromProtectionType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromProtectionType( E_ProtectionType nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( ProtectionTypeConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromRadarType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromRadarType( E_RadarType nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( RadarTypeConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromEquipmentState
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromEquipmentState_ADN( E_EquipmentState_ADN nValue, E_Conversion nConversion )
{
    return ENT_Tr::InverseFindInConverter( EquipmentState_ADNConverter_, nValue, nConversion );
}

//-----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromTypeMunitionTirIndirect( E_TypeMunitionTirIndirect nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeMunitionTirIndirectConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromMissionParameterType
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromMissionParameterType( E_MissionParameterType nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionParameterTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromSupplyConvoyType
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromSupplyConvoyType( E_SupplyConvoyType nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( SupplyConvoyTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromConstructorType
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromConstructorType( E_ConstructorType nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ConstructorTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromPropagationModel
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromPropagationModel( E_PropagationModel nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PropagationModelConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromWorkspaceElement
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromWorkspaceElement( E_WorkspaceElements nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( WorkspaceElementsConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromDays
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromDays( E_Days nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DaysConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromContextParameters
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromContextParameters( E_ContextParameters nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ContextParametersConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromEntityType
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromEntityType( E_EntityType nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( EntityTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToConsumptionType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_ConsumptionType ADN_Tr::ConvertToConsumptionType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ConsumptionTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToVisionObject
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_VisionObject ADN_Tr::ConvertToVisionObject( const std::string& strName )
{
    return ENT_Tr::FindInConverter( VisionObjectConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToKeyPoint
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_KeyPoint ADN_Tr::ConvertToKeyPoint( const std::string& strName )
{
    return ENT_Tr::FindInConverter( KeyPointConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToAgentTypePion
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_AgentTypePion ADN_Tr::ConvertToAgentTypePion( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AgentTypePionConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToAgentTypeAutomate
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_AgentTypeAutomate ADN_Tr::ConvertToAgentTypeAutomate( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AgentTypeAutomateConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToNatureAtlasType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_NatureAtlasType ADN_Tr::ConvertToNatureAtlasType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( NatureAtlasTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToSpeedImpact
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
E_SpeedImpact ADN_Tr::ConvertToSpeedImpact( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SpeedImpactConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToCrossingHeight
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
E_CrossingHeight ADN_Tr::ConvertToCrossingHeight( const std::string& strName )
{
    return ENT_Tr::FindInConverter( CrossingHeightConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToSensorWeatherModifiers
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_SensorWeatherModifiers ADN_Tr::ConvertToSensorWeatherModifiers( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SensorWeatherModifiersConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToDoctorSkills
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_DoctorSkills ADN_Tr::ConvertToDoctorSkills( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DoctorSkillsConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToProtectionType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_ProtectionType ADN_Tr::ConvertToProtectionType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ProtectionTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToRadarType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_RadarType ADN_Tr::ConvertToRadarType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( RadarTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToEquipmentState
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
E_EquipmentState_ADN ADN_Tr::ConvertToEquipmentState_ADN( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EquipmentState_ADNConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeMunitionTirIndirect ADN_Tr::ConvertToTypeMunitionTirIndirect( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeMunitionTirIndirectConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToMissionParameterType
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
E_MissionParameterType ADN_Tr::ConvertToMissionParameterType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionParameterTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToMissionParameterType
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
E_SupplyConvoyType ADN_Tr::ConvertToSupplyConvoyType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SupplyConvoyTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToConstructorType
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
E_ConstructorType ADN_Tr::ConvertToConstructorType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ConstructorTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToPropagationModel
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
E_PropagationModel ADN_Tr::ConvertToPropagationModel( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PropagationModelConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToWorkspaceElements
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
E_WorkspaceElements ADN_Tr::ConvertToWorkspaceElements( const std::string& strName )
{
    return ENT_Tr::FindInConverter( WorkspaceElementsConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToDays
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
E_Days ADN_Tr::ConvertToDays( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DaysConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToContextParameters
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
E_ContextParameters ADN_Tr::ConvertToContextParameters( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ContextParametersConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToEntityType
// Created: ABR 2013-02-06
// -----------------------------------------------------------------------------
E_EntityType ADN_Tr::ConvertToEntityType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EntityTypeConverter_, strName );
}

#define INIT_TR( NAME )\
    BOOST_STATIC_ASSERT( COUNT_OF( NAME ## Converter_ ) == ( eNbr ## NAME ) + 1 );\
    InitTr( ( NAME ## Converter_ ), "ADN_Tr" );

// -----------------------------------------------------------------------------
// Name: ADN_Tr::InitTranslations
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
void ADN_Tr::InitTranslations()
{
    INIT_TR( AgentTypeAutomate );
    INIT_TR( AgentTypePion );
    INIT_TR( ConstructorType );
    INIT_TR( ConsumptionType );
    INIT_TR( ContextParameters );
    INIT_TR( CrossingHeight );
    INIT_TR( Days );
    INIT_TR( DoctorSkills );
    INIT_TR( EntityType );
    INIT_TR( EquipmentState_ADN );
    INIT_TR( KeyPoint );
    INIT_TR( MissionParameterType );
    INIT_TR( NatureAtlasType );
    INIT_TR( PropagationModel );
    INIT_TR( ProtectionType );
    INIT_TR( RadarType );
    INIT_TR( SensorWeatherModifiers );
    INIT_TR( SpeedImpact );
    INIT_TR( SupplyConvoyType );
    INIT_TR( TypeMunitionTirIndirect );
    INIT_TR( VisionObject );
    INIT_TR( WorkspaceElements );
}
