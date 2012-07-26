// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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

// -----------------------------------------------------------------------------
// Name: ADN_Tr::InitTranslations
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
ADN_Tr::T_ConverterTypeMunitionTirIndirect ADN_Tr::munitionTirIndirectConverter_[] =
{
    T_ConverterTypeMunitionTirIndirect( "explosif",  QT_TRANSLATE_NOOP( "ADN_Tr", "explosive" ),          eTypeMunitionTirIndirect_Explosif ),
    T_ConverterTypeMunitionTirIndirect( "grenade",   QT_TRANSLATE_NOOP( "ADN_Tr", "grenade" ),            eTypeMunitionTirIndirect_Grenade ),
    T_ConverterTypeMunitionTirIndirect( "aced",      QT_TRANSLATE_NOOP( "ADN_Tr", "anti tank" ),          eTypeMunitionTirIndirect_Aced ),
    T_ConverterTypeMunitionTirIndirect( "fumigene",  QT_TRANSLATE_NOOP( "ADN_Tr", "smoke" ),              eTypeMunitionTirIndirect_Fumigene ),
    T_ConverterTypeMunitionTirIndirect( "eclairant", QT_TRANSLATE_NOOP( "ADN_Tr", "illumination shell" ), eTypeMunitionTirIndirect_Eclairant ),
    T_ConverterTypeMunitionTirIndirect( "mine",      QT_TRANSLATE_NOOP( "ADN_Tr", "mine" ),               eTypeMunitionTirIndirect_Mine ),
    T_ConverterTypeMunitionTirIndirect( "effect",    QT_TRANSLATE_NOOP( "ADN_Tr", "effect" ),             eTypeMunitionTirIndirect_Effect ),
    T_ConverterTypeMunitionTirIndirect( "", "", (E_TypeMunitionTirIndirect)-1 )
};

ADN_Tr::T_ConverterConsumptionType ADN_Tr::consumptionTypeConverter_[] =
{
    T_ConverterConsumptionType( "ArretMoteurEteint" , QT_TRANSLATE_NOOP( "ADN_Tr" , "Parked, engine stopped" ), eEngineStopped ) ,
    T_ConverterConsumptionType( "ArretMoteurAllume" , QT_TRANSLATE_NOOP( "ADN_Tr" , "Parked, engine running" ), eEngineStarted ) ,
    T_ConverterConsumptionType( "Deplacement" ,       QT_TRANSLATE_NOOP( "ADN_Tr" , "Moving" ),                 eMoving ) ,
    T_ConverterConsumptionType( "EnTravaux" ,         QT_TRANSLATE_NOOP( "ADN_Tr" , "Working" ),                eWorking ) ,
    T_ConverterConsumptionType( "" , "" , (E_ConsumptionType)-1 )
};

ADN_Tr::T_ConverterTimeCategory ADN_Tr::timeCategoryConverter_[] =
{
    T_ConverterTimeCategory( "JourSansNuage"         , QT_TRANSLATE_NOOP("ADN_Tr" ,"Very sunny day"),       eTimeTypeDay_NoCloud            ),
    T_ConverterTimeCategory( "JourPeuNuageux"        , QT_TRANSLATE_NOOP("ADN_Tr" ,"Sunny day"),            eTimeTypeDay_LightCloud         ),
    T_ConverterTimeCategory( "JourMoyennementNuageux", QT_TRANSLATE_NOOP("ADN_Tr" ,"Partly cloudy day"),    eTimeTypeDay_MiddleCloud        ),
    T_ConverterTimeCategory( "JourAssezNuageux"      , QT_TRANSLATE_NOOP("ADN_Tr" ,"Cloudy day"),           eTimeTypeDay_HeavyCloud         ),
    T_ConverterTimeCategory( "JourTresNuageux"       , QT_TRANSLATE_NOOP("ADN_Tr" ,"Very cloudy day"),      eTimeTypeDay_FullCloud          ),
    T_ConverterTimeCategory( "NuitPleineLune"        , QT_TRANSLATE_NOOP("ADN_Tr" ,"Full moon night"),      eTimeTypeNight_FullMoon         ),
    T_ConverterTimeCategory( "NuitTroisQuartDeLune"  , QT_TRANSLATE_NOOP("ADN_Tr" ,"Waning moon night"),    eTimeTypeNight_ThreeQuarterMoon ),
    T_ConverterTimeCategory( "NuitDemiLune"          , QT_TRANSLATE_NOOP("ADN_Tr" ,"Quarter moon night"),   eTimeTypeNight_HalfMoon         ),
    T_ConverterTimeCategory( "NuitQuartDeLune"       , QT_TRANSLATE_NOOP("ADN_Tr" ,"Crescent moon night"),  eTimeTypeNight_OneQuarterMoon   ),
    T_ConverterTimeCategory( "NuitNouvelleLune"      , QT_TRANSLATE_NOOP("ADN_Tr" ,"New moon night"),       eTimeTypeNight_NewMoon          ),
    T_ConverterTimeCategory( "Eclairant"             , QT_TRANSLATE_NOOP("ADN_Tr" ,"Illumination shell"),   eTimeTypeNight_LightingRocket   ),
    T_ConverterTimeCategory( "", "", (E_TimeCategory)-1 )
};

ADN_Tr::T_ConverterVisionObject ADN_Tr::visionObjectConverter_[] =
{
    T_ConverterVisionObject( "Sol"   , QT_TRANSLATE_NOOP("ADN_Tr" ,"Ground"),     eVisionGround ),
    T_ConverterVisionObject( "Vide"  , QT_TRANSLATE_NOOP("ADN_Tr" ,"Empty"),      eVisionEmpty  ),
    T_ConverterVisionObject( "Foret" , QT_TRANSLATE_NOOP("ADN_Tr" ,"Forest"),     eVisionForest ),
    T_ConverterVisionObject( "Urbain", QT_TRANSLATE_NOOP("ADN_Tr" ,"Urban area"), eVisionUrban  ),
    T_ConverterVisionObject( "", "", (E_VisionObject)-1 )
};

ADN_Tr::T_ConverterKeyPoint ADN_Tr::keyPointConverter_[] =
{
    T_ConverterKeyPoint( "forest",     QT_TRANSLATE_NOOP( "ADN_Tr" , "forest" ),     eKeyPointForest ),
    T_ConverterKeyPoint( "urban area", QT_TRANSLATE_NOOP( "ADN_Tr" , "urban area" ), eKeyPointUrban ),
    T_ConverterKeyPoint( "crossroad",  QT_TRANSLATE_NOOP( "ADN_Tr" , "crossroad" ),  eKeyPointCrossroads ),
    T_ConverterKeyPoint( "bridge",     QT_TRANSLATE_NOOP( "ADN_Tr" , "bridge" ),     eKeyPointBridge ),
    T_ConverterKeyPoint( "lima",       QT_TRANSLATE_NOOP( "ADN_Tr" , "lima" ),       eKeyPointLima ),
    T_ConverterKeyPoint( "" , "" , (E_KeyPoint)-1 )
};

ADN_Tr::T_ConverterAgentTypePion ADN_Tr::agentTypePionConverter_[] =
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
    T_ConverterAgentTypePion( "", "", (E_AgentTypePion)-1 )
};

ADN_Tr::T_ConverterAgentTypeAutomate ADN_Tr::agentTypeAutomateConverter_[] =
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

ADN_Tr::T_ConverterNatureAtlasType ADN_Tr::natureAtlasTypeConverter_[] =
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

ADN_Tr::T_ConverterSpeedImpact ADN_Tr::speedImpactConverter_[] =
{
    T_ConverterSpeedImpact( "AuPlusLent",        QT_TRANSLATE_NOOP( "ADN_Tr", "Slowest" ),           eSpeedImpact_AuPlusLent ),
    T_ConverterSpeedImpact( "VitesseMaxObjet",   QT_TRANSLATE_NOOP( "ADN_Tr", "Object, Max speed" ), eSpeedImpact_VitesseMaxObjet ),
    T_ConverterSpeedImpact( "VitesseMaxAgent",   QT_TRANSLATE_NOOP( "ADN_Tr", "Unit, Max speed" ),   eSpeedImpact_VitesseMaxAgent ),
    T_ConverterSpeedImpact( "VitesseBlocUrbain", QT_TRANSLATE_NOOP( "ADN_Tr", "Urban block speed" ), eSpeedImpact_VitesseBlocUrbain ),
    T_ConverterSpeedImpact( "", "", (E_SpeedImpact)-1 )
};

ADN_Tr::T_ConverterCrossingHeight ADN_Tr::crossingHeightConverter_[] =
{
    T_ConverterCrossingHeight( "0m",  QT_TRANSLATE_NOOP( "ADN_Tr", "0m" ),  eCrossingHeight_Never ),
    T_ConverterCrossingHeight( "1m",  QT_TRANSLATE_NOOP( "ADN_Tr", "1m" ),  eCrossingHeight_LowAreas ),
    T_ConverterCrossingHeight( ">1m", QT_TRANSLATE_NOOP( "ADN_Tr", ">1m" ), eCrossingHeight_Always ),
    T_ConverterCrossingHeight( "", "", (E_CrossingHeight)-1 )
};

ADN_Tr::T_ConverterSensorWeatherModifiers ADN_Tr::sensorWeatherModifiersConverter_[] =
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

ADN_Tr::T_ConverterDoctorSkills ADN_Tr::doctorSkillsConverter_[] =
{
    T_ConverterDoctorSkills( "U1", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded seriousness level 1" ), eDoctorSkills_U1 ),
    T_ConverterDoctorSkills( "U2", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded seriousness level 2" ), eDoctorSkills_U2 ),
    T_ConverterDoctorSkills( "U3", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded seriousness level 3" ), eDoctorSkills_U3 ),
    T_ConverterDoctorSkills( "UE", QT_TRANSLATE_NOOP( "ADN_Tr", "Wounded extreme seriousness" ), eDoctorSkills_UE ),
    T_ConverterDoctorSkills( "", "", (E_DoctorSkills)-1 )
};

ADN_Tr::T_ConverterProtectionType ADN_Tr::protectionTypeConverter_[] =
{
    T_ConverterProtectionType( "materiel", QT_TRANSLATE_NOOP( "ADN_Tr", "equipment" ), eProtectionType_Material ),
    T_ConverterProtectionType( "humain",   QT_TRANSLATE_NOOP( "ADN_Tr", "human" ),     eProtectionType_Human ),
    T_ConverterProtectionType( "foule",    QT_TRANSLATE_NOOP( "ADN_Tr", "crowd" ),     eProtectionType_Crowd ),
    T_ConverterProtectionType( "", "", (E_ProtectionType)-1 )
};


ADN_Tr::T_ConverterBreakdownType ADN_Tr::breakdownTypeConverter_[] =
{
    T_ConverterBreakdownType( "EA", QT_TRANSLATE_NOOP( "ADN_Tr", "Eletronic" ), eBreakdownType_EA ),
    T_ConverterBreakdownType( "M",  QT_TRANSLATE_NOOP( "ADN_Tr", "Mobility" ),  eBreakdownType_M ),
    T_ConverterBreakdownType( "", "", (E_BreakdownType)-1 )
};

ADN_Tr::T_ConverterBreakdownNTI ADN_Tr::breakdownNTIConverter_[] =
{
    T_ConverterBreakdownNTI( "NTI1", QT_TRANSLATE_NOOP( "ADN_Tr", "Seriousness level 1" ), eBreakdownNTI_NTI1 ),
    T_ConverterBreakdownNTI( "NTI2", QT_TRANSLATE_NOOP( "ADN_Tr", "Seriousness level 2" ), eBreakdownNTI_NTI2 ),
    T_ConverterBreakdownNTI( "NTI3", QT_TRANSLATE_NOOP( "ADN_Tr", "Seriousness level 3" ), eBreakdownNTI_NTI3 ),
    T_ConverterBreakdownNTI( "", "", (E_BreakdownNTI)-1 )
};

ADN_Tr::T_ConverterRadarType ADN_Tr::radarTypeConverter_[] =
{
    T_ConverterRadarType( "radar",          QT_TRANSLATE_NOOP( "ADN_Tr", "radar" ),           eRadarType_Radar ),
    T_ConverterRadarType( "ecoute",         QT_TRANSLATE_NOOP( "ADN_Tr", "listening" ),       eRadarType_Ecoute ),
    T_ConverterRadarType( "ecoute radar",   QT_TRANSLATE_NOOP( "ADN_Tr", "radar listening" ), eRadarType_EcouteRadar ),
    T_ConverterRadarType( "", "", (E_RadarType)-1 )
};

ADN_Tr::T_ConverterEquipmentState ADN_Tr::equipmentStateConverter_[] =
{
    T_ConverterEquipmentState( "ReparableAvecEvacuation", QT_TRANSLATE_NOOP("ADN_Tr", "Maintenance support needed" ), eEquipmentState_ADN_FixableWithEvac ),
    T_ConverterEquipmentState( "ReparableSurPlace"      , QT_TRANSLATE_NOOP("ADN_Tr", "On site fixable"),             eEquipmentState_ADN_FixableInPlace ),
    T_ConverterEquipmentState( "Detruit"                , QT_TRANSLATE_NOOP("ADN_Tr", "Destroyed"),                   eEquipmentState_ADN_Destroyed ),
    T_ConverterEquipmentState( "", "", (E_EquipmentState_ADN)-1 )
};

ADN_Tr::T_ConverterMissionParameterType ADN_Tr::missionParameterTypeConverter_[] =
{
    T_ConverterMissionParameterType( "Agent",                 QT_TRANSLATE_NOOP("ADN_Tr", "Agent" ),                 eMissionParameterTypeAgent ),
    T_ConverterMissionParameterType( "Automat",               QT_TRANSLATE_NOOP("ADN_Tr", "Automat" ),               eMissionParameterTypeAutomate ),
    T_ConverterMissionParameterType( "Boolean",               QT_TRANSLATE_NOOP("ADN_Tr", "Boolean" ),               eMissionParameterTypeBool ),
    T_ConverterMissionParameterType( "Integer",               QT_TRANSLATE_NOOP("ADN_Tr", "Integer" ),               eMissionParameterTypeNumeric ),
    T_ConverterMissionParameterType( "String",                QT_TRANSLATE_NOOP("ADN_Tr", "String" ),                eMissionParameterTypeString ),
    T_ConverterMissionParameterType( "Heading",               QT_TRANSLATE_NOOP("ADN_Tr", "Heading" ),               eMissionParameterTypeDirection ),
    T_ConverterMissionParameterType( "DateTime",              QT_TRANSLATE_NOOP("ADN_Tr", "DateTime" ),              eMissionParameterTypeGDH ),
    T_ConverterMissionParameterType( "Enumeration",           QT_TRANSLATE_NOOP("ADN_Tr", "Enumeration" ),           eMissionParameterTypeEnumeration ),
    T_ConverterMissionParameterType( "NatureAtlas",           QT_TRANSLATE_NOOP("ADN_Tr", "NatureAtlas" ),           eMissionParameterTypeNatureAtlas ),
    T_ConverterMissionParameterType( "AgentKnowledge",        QT_TRANSLATE_NOOP("ADN_Tr", "AgentKnowledge" ),        eMissionParameterTypeAgentKnowledge ),
    T_ConverterMissionParameterType( "ObjectKnowledge",       QT_TRANSLATE_NOOP("ADN_Tr", "ObjectKnowledge" ),       eMissionParameterTypeObjectKnowledge ),
    T_ConverterMissionParameterType( "CrowdKnowledge",        QT_TRANSLATE_NOOP("ADN_Tr", "CrowdKnowledge" ),        eMissionParameterTypePopulationKnowledge ),
    T_ConverterMissionParameterType( "PlannedWork",           QT_TRANSLATE_NOOP("ADN_Tr", "PlannedWork" ),           eMissionParameterTypeGenObject ),
    T_ConverterMissionParameterType( "Path",                  QT_TRANSLATE_NOOP("ADN_Tr", "Path" ),                  eMissionParameterTypePath ),
    T_ConverterMissionParameterType( "Point",                 QT_TRANSLATE_NOOP("ADN_Tr", "Point" ),                 eMissionParameterTypePoint ),
    T_ConverterMissionParameterType( "Polygon",               QT_TRANSLATE_NOOP("ADN_Tr", "Polygon" ),               eMissionParameterTypePolygon ),
    T_ConverterMissionParameterType( "EquipmentType",         QT_TRANSLATE_NOOP("ADN_Tr", "EquipmentType" ),         eMissionParameterTypeEquipmentType ),
    T_ConverterMissionParameterType( "ResourceType",          QT_TRANSLATE_NOOP("ADN_Tr", "ResourceType" ),          eMissionParameterTypeDotationType ),
    T_ConverterMissionParameterType( "MaintenancePriorities", QT_TRANSLATE_NOOP("ADN_Tr", "MaintenancePriorities" ), eMissionParameterTypeMaintenancePriorities ),
    T_ConverterMissionParameterType( "MedicalPriorities",     QT_TRANSLATE_NOOP("ADN_Tr", "MedicalPriorities" ),     eMissionParameterTypeMedicalPriorities ),
    T_ConverterMissionParameterType( "IndirectFire",          QT_TRANSLATE_NOOP("ADN_Tr", "IndirectFire" ),          eMissionParameterTypeIndirectFire ),
    T_ConverterMissionParameterType( "Limit",                 QT_TRANSLATE_NOOP("ADN_Tr", "Limit" ),                 eMissionParameterTypeLimit ),
    T_ConverterMissionParameterType( "PhaseLine",             QT_TRANSLATE_NOOP("ADN_Tr", "PhaseLine" ),             eMissionParameterTypePhaseLine ),
    T_ConverterMissionParameterType( "UrbanKnowledge",        QT_TRANSLATE_NOOP("ADN_Tr", "UrbanKnowledge" ),        eMissionParameterTypeUrbanBlock ),
    T_ConverterMissionParameterType( "LocationComposite",     QT_TRANSLATE_NOOP("ADN_Tr", "LocationComposite" ),     eMissionParameterTypeLocationComposite ),
    T_ConverterMissionParameterType( "ResourceNetwork",       QT_TRANSLATE_NOOP("ADN_Tr", "ResourceNetwork" ),       eMissionParameterTypeResourceNetwork ),

    T_ConverterMissionParameterType( "", "", (E_MissionParameterType)-1 )
};

ADN_Tr::T_ConverterAnchorType ADN_Tr::anchorTypeConverter_[] =
{
    T_ConverterAnchorType( "nothing",      QT_TRANSLATE_NOOP("ADN_Tr", "nothing" ), eNoAnchor ),
    T_ConverterAnchorType( "start", QT_TRANSLATE_NOOP("ADN_Tr", "start" ), eStart ),
    T_ConverterAnchorType( "end",   QT_TRANSLATE_NOOP("ADN_Tr", "end" ), eEnd ),

    T_ConverterAnchorType( "", "", (E_AnchorType)-1 )
};

ADN_Tr::T_ConverterConstructorType ADN_Tr::constructorTypeConverter_[] =
{
    T_ConverterConstructorType( "raw",     QT_TRANSLATE_NOOP("ADN_Tr", "Raw" ), eRaw ),
    T_ConverterConstructorType( "density", QT_TRANSLATE_NOOP("ADN_Tr", "Density" ), eDensity ),

    T_ConverterConstructorType( "", "", ( E_ConstructorType )-1 )
};

ADN_Tr::T_ConverterPropagationModel ADN_Tr::propagationModelConverter_[] =
{
    T_ConverterPropagationModel( "input", QT_TRANSLATE_NOOP("ADN_Tr", "Input" ), eInput ),
    T_ConverterPropagationModel( "fire",  QT_TRANSLATE_NOOP("ADN_Tr", "Fire" ), eFire ),
    T_ConverterPropagationModel( "cloud", QT_TRANSLATE_NOOP("ADN_Tr", "Cloud" ), eCloud ),

    T_ConverterPropagationModel( "", "", (E_PropagationModel)-1 )
};

ADN_Tr::T_ConverterEquipmentCategory ADN_Tr::equipmentCategoryConverter_[] =
{
    T_ConverterEquipmentCategory( "VUT",                        QT_TRANSLATE_NOOP("ADN_Tr", "VUT" ),                eVUT ),
    T_ConverterEquipmentCategory( "VTT",                        QT_TRANSLATE_NOOP("ADN_Tr", "VTT" ),                eVTT ),
    T_ConverterEquipmentCategory( "VNBLIN",                     QT_TRANSLATE_NOOP("ADN_Tr", "VNBLIN" ),             eVNBLIN ),
    T_ConverterEquipmentCategory( "VCIMEC",                     QT_TRANSLATE_NOOP("ADN_Tr", "VCIMEC" ),             eVCIMEC ),
    T_ConverterEquipmentCategory( "VCD",                        QT_TRANSLATE_NOOP("ADN_Tr", "VCD" ),                eVCD ),
    T_ConverterEquipmentCategory( "VATM",                       QT_TRANSLATE_NOOP("ADN_Tr", "VATM" ),               eVATM ),
    T_ConverterEquipmentCategory( "RSM",                        QT_TRANSLATE_NOOP("ADN_Tr", "RSM" ),                eRSM ),
    T_ConverterEquipmentCategory( "MORT",                       QT_TRANSLATE_NOOP("ADN_Tr", "MORT" ),               eMORT ),
    T_ConverterEquipmentCategory( "MISSS",                      QT_TRANSLATE_NOOP("ADN_Tr", "MISSS" ),              eMISSS ),
    T_ConverterEquipmentCategory( "MISSA",                      QT_TRANSLATE_NOOP("ADN_Tr", "MISSA" ),              eMISSA ),
    T_ConverterEquipmentCategory( "MISGAC",                     QT_TRANSLATE_NOOP("ADN_Tr", "MISGAC" ),             eMISGAC ),
    T_ConverterEquipmentCategory( "MISAS",                      QT_TRANSLATE_NOOP("ADN_Tr", "MISAS" ),              eMISAS ),
    T_ConverterEquipmentCategory( "MECA",                       QT_TRANSLATE_NOOP("ADN_Tr", "MECA" ),               eMECA ),
    T_ConverterEquipmentCategory( "HTTU",                       QT_TRANSLATE_NOOP("ADN_Tr", "HTTU" ),               eHTTU ),
    T_ConverterEquipmentCategory( "HT",                         QT_TRANSLATE_NOOP("ADN_Tr", "HT" ),                 eHT ),
    T_ConverterEquipmentCategory( "HMT",                        QT_TRANSLATE_NOOP("ADN_Tr", "HMT" ),                eHMT ),
    T_ConverterEquipmentCategory( "HLT",                        QT_TRANSLATE_NOOP("ADN_Tr", "HLT" ),                eHLT ),
    T_ConverterEquipmentCategory( "HLO",                        QT_TRANSLATE_NOOP("ADN_Tr", "HLO" ),                eHLO ),
    T_ConverterEquipmentCategory( "HARM",                       QT_TRANSLATE_NOOP("ADN_Tr", "HARM" ),               eHARM ),
    T_ConverterEquipmentCategory( "EQFCHT",                     QT_TRANSLATE_NOOP("ADN_Tr", "EQFCHT" ),             eEQFCHT ),
    T_ConverterEquipmentCategory( "EQCHIM",                     QT_TRANSLATE_NOOP("ADN_Tr", "EQCHIM" ),             eEQCHIM ),
    T_ConverterEquipmentCategory( "CHALEG",                     QT_TRANSLATE_NOOP("ADN_Tr", "CHALEG" ),             eCHALEG ),
    T_ConverterEquipmentCategory( "CHABAT",                     QT_TRANSLATE_NOOP("ADN_Tr", "CHABAT" ),             eCHABAT ),
    T_ConverterEquipmentCategory( "CANAC",                      QT_TRANSLATE_NOOP("ADN_Tr", "CANAC" ),              eCANAC ),
    T_ConverterEquipmentCategory( "CAN",                        QT_TRANSLATE_NOOP("ADN_Tr", "CAN" ),                eCAN ),
    T_ConverterEquipmentCategory( "BLIND",                      QT_TRANSLATE_NOOP("ADN_Tr", "BLIND" ),              eBLIND ),
    T_ConverterEquipmentCategory( "AVVF",                       QT_TRANSLATE_NOOP("ADN_Tr", "AVVF" ),               eAVVF ),
    T_ConverterEquipmentCategory( "ASA",                        QT_TRANSLATE_NOOP("ADN_Tr", "ASA" ),                eASA ),
    T_ConverterEquipmentCategory( "ANT",                        QT_TRANSLATE_NOOP("ADN_Tr", "ANT" ),                eANT ),
    T_ConverterEquipmentCategory( "ALT",                        QT_TRANSLATE_NOOP("ADN_Tr", "ALT" ),                eALT ),
    T_ConverterEquipmentCategory( "ACQOBJ",                     QT_TRANSLATE_NOOP("ADN_Tr", "ACQOBJ" ),             eACQOBJ ),
    T_ConverterEquipmentCategory( "HM",                         QT_TRANSLATE_NOOP("ADN_Tr", "HM" ),                 eHM ),
    T_ConverterEquipmentCategory( "AVION",                      QT_TRANSLATE_NOOP("ADN_Tr", "AVION" ),              eAVION ),
    T_ConverterEquipmentCategory( "HAC",                        QT_TRANSLATE_NOOP("ADN_Tr", "HAC" ),                eHAC ),
    T_ConverterEquipmentCategory( "HR",                         QT_TRANSLATE_NOOP("ADN_Tr", "HR" ),                 eHR ),
    T_ConverterEquipmentCategory( "HAM",                        QT_TRANSLATE_NOOP("ADN_Tr", "HAM" ),                eHAM ),
    T_ConverterEquipmentCategory( "HAH",                        QT_TRANSLATE_NOOP("ADN_Tr", "HAH" ),                eHAH ),
    T_ConverterEquipmentCategory( "VCI",                        QT_TRANSLATE_NOOP("ADN_Tr", "VCI" ),                eVCI ),
    T_ConverterEquipmentCategory( "Char",                       QT_TRANSLATE_NOOP("ADN_Tr", "Char" ),               eChar ),
    T_ConverterEquipmentCategory( "Mortier",                    QT_TRANSLATE_NOOP("ADN_Tr", "Mortar" ),             eMortier ),
    T_ConverterEquipmentCategory( "Artillerie",                 QT_TRANSLATE_NOOP("ADN_Tr", "Artillery" ),          eArtillerie ),
    T_ConverterEquipmentCategory( "Hommes",                     QT_TRANSLATE_NOOP("ADN_Tr", "Men" ),                eHommes ),
    T_ConverterEquipmentCategory( "Armes L�g�res",              QT_TRANSLATE_NOOP("ADN_Tr", "Light weapons" ),      eArmesLegeres ),
    T_ConverterEquipmentCategory( "Mat�riel Franchissement",    QT_TRANSLATE_NOOP("ADN_Tr", "Crossing equipment" ), eMaterielFranchissement ),
    T_ConverterEquipmentCategory( "Autres",                     QT_TRANSLATE_NOOP("ADN_Tr", "Others" ),             eAutres ),
    T_ConverterEquipmentCategory( "", "", (E_EquipmentCategory)-1 )
};

ADN_Tr::T_ConverterWorkspaceElements ADN_Tr::workspaceElementsConverter_[] =
{
    T_ConverterWorkspaceElements( "categories",       QT_TRANSLATE_NOOP("ADN_Tr", "Categories" ),           eCategories ),
    T_ConverterWorkspaceElements( "symbols",          QT_TRANSLATE_NOOP("ADN_Tr", "Symbols" ),              eSymbols ),
    T_ConverterWorkspaceElements( "unitsymbols",      QT_TRANSLATE_NOOP("ADN_Tr", "UnitSymbols" ),          eUnitSymbols ),
    T_ConverterWorkspaceElements( "urban",            QT_TRANSLATE_NOOP("ADN_Tr", "Urban" ),                eUrban ),
    T_ConverterWorkspaceElements( "nbc",              QT_TRANSLATE_NOOP("ADN_Tr", "NBC" ),                  eNBC ),
    T_ConverterWorkspaceElements( "launcher",         QT_TRANSLATE_NOOP("ADN_Tr", "Launchers" ),            eLaunchers ),
    T_ConverterWorkspaceElements( "equipment",        QT_TRANSLATE_NOOP("ADN_Tr", "Resources" ),            eEquipement ),
    T_ConverterWorkspaceElements( "fireclasses",      QT_TRANSLATE_NOOP("ADN_Tr", "Fires" ),                eFireClasses ),
    T_ConverterWorkspaceElements( "drawings",         QT_TRANSLATE_NOOP("ADN_Tr", "Drawings" ),             eDrawings ),
    T_ConverterWorkspaceElements( "objects",          QT_TRANSLATE_NOOP("ADN_Tr", "Objects" ),              eObjects ),
    T_ConverterWorkspaceElements( "weapons",          QT_TRANSLATE_NOOP("ADN_Tr", "Weapon systems" ),       eWeapons ),
    T_ConverterWorkspaceElements( "activeprotection", QT_TRANSLATE_NOOP("ADN_Tr", "Active Protections" ),   eActiveProtections ),
    T_ConverterWorkspaceElements( "sensors",          QT_TRANSLATE_NOOP("ADN_Tr", "Sensors" ),              eSensors ),
    T_ConverterWorkspaceElements( "breakdowns",       QT_TRANSLATE_NOOP("ADN_Tr", "Breakdowns" ),           eBreakdowns ),
    T_ConverterWorkspaceElements( "composantes",      QT_TRANSLATE_NOOP("ADN_Tr", "Equipments" ),           eComposantes ),
    T_ConverterWorkspaceElements( "resourcenetworks", QT_TRANSLATE_NOOP("ADN_Tr", "Resource networks" ),    eResourceNetworks ),
    T_ConverterWorkspaceElements( "aiengine",         QT_TRANSLATE_NOOP("ADN_Tr", "Op. indicators" ),       eAiEngine ),
    T_ConverterWorkspaceElements( "missions",         QT_TRANSLATE_NOOP("ADN_Tr", "Missions" ),             eMissions ),
    T_ConverterWorkspaceElements( "models",           QT_TRANSLATE_NOOP("ADN_Tr", "Doctrine models" ),      eModels ),
    T_ConverterWorkspaceElements( "units",            QT_TRANSLATE_NOOP("ADN_Tr", "Units" ),                eUnits ),
    T_ConverterWorkspaceElements( "automata",         QT_TRANSLATE_NOOP("ADN_Tr", "Automata" ),             eAutomata ),
    T_ConverterWorkspaceElements( "communications",   QT_TRANSLATE_NOOP("ADN_Tr", "Jamming" ),              eCommunications ),
    T_ConverterWorkspaceElements( "humanfactors",     QT_TRANSLATE_NOOP("ADN_Tr", "Human factors" ),        eHumanFactors ),
    T_ConverterWorkspaceElements( "knowledgegroups",  QT_TRANSLATE_NOOP("ADN_Tr", "Knowledge groups" ),     eKnowledgeGroups ),
    T_ConverterWorkspaceElements( "population",       QT_TRANSLATE_NOOP("ADN_Tr", "Crowds" ),               ePopulation ),
    T_ConverterWorkspaceElements( "people",           QT_TRANSLATE_NOOP("ADN_Tr", "Populations" ),          ePeople ),
    T_ConverterWorkspaceElements( "reports",          QT_TRANSLATE_NOOP("ADN_Tr", "Reports" ),              eReports ),
    T_ConverterWorkspaceElements( "logistic",         QT_TRANSLATE_NOOP("ADN_Tr", "Log" ),                  eLogistic ),
    T_ConverterWorkspaceElements( "", "", (E_WorkspaceElements)-1 )
};

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromLocation
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromLocation( E_Location  nValue, E_Conversion nConversion )
{
    return ENT_Tr::ConvertFromLocation( nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromConsumptionType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromConsumptionType( E_ConsumptionType nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( consumptionTypeConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromTimeCategory
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromTimeCategory( E_TimeCategory nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( timeCategoryConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromVisionObject
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromVisionObject( E_VisionObject nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( visionObjectConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromKeyPoint
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromKeyPoint( E_KeyPoint nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( keyPointConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromAgentTypePion
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromAgentTypePion( E_AgentTypePion nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( agentTypePionConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromAgentTypeAutomate
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromAgentTypeAutomate( E_AgentTypeAutomate nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( agentTypeAutomateConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromNatureAtlasType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromNatureAtlasType( E_NatureAtlasType nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( natureAtlasTypeConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromSpeedImpact
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromSpeedImpact( E_SpeedImpact nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( speedImpactConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromCrossingHeight
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromCrossingHeight( E_CrossingHeight nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( crossingHeightConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromSensorWeatherModifiers
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromSensorWeatherModifiers( E_SensorWeatherModifiers nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( sensorWeatherModifiersConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromDoctorSkills
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromDoctorSkills( E_DoctorSkills nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( doctorSkillsConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromProtectionType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromProtectionType( E_ProtectionType nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( protectionTypeConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromBreakdownType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromBreakdownType( E_BreakdownType nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( breakdownTypeConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromBreakdownNTI
// Created: SBO 2005-09-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromBreakdownNTI( E_BreakdownNTI nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( breakdownNTIConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromRadarType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromRadarType( E_RadarType nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( radarTypeConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromEquipmentState
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromEquipmentState( E_EquipmentState_ADN nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( equipmentStateConverter_, nValue, nConversion );
}

//-----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromTypeMunitionTirIndirect( E_TypeMunitionTirIndirect nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( munitionTirIndirectConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromMissionParameterType
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromMissionParameterType( E_MissionParameterType nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( missionParameterTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromAnchorType
// Created: LGY 2011-11-28
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromAnchorType( E_AnchorType nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( anchorTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromConstructorType
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromConstructorType( E_ConstructorType nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( constructorTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromPropagationModel
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromPropagationModel( E_PropagationModel nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( propagationModelConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromEquipmentCategory
// Created: ABR 2012-03-30
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromEquipmentCategory( E_EquipmentCategory nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( equipmentCategoryConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromWorkspaceElement
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromWorkspaceElement( E_WorkspaceElements nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( workspaceElementsConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToLocation
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_Location ADN_Tr::ConvertToLocation( const std::string& strName )
{
    return ENT_Tr::ConvertToLocation( strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToConsumptionType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_ConsumptionType ADN_Tr::ConvertToConsumptionType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( consumptionTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToTimeCategory
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_TimeCategory ADN_Tr::ConvertToTimeCategory( const std::string& strName )
{
    return ADN_Tr::FindInConverter( timeCategoryConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToVisionObject
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_VisionObject ADN_Tr::ConvertToVisionObject( const std::string& strName )
{
    return ADN_Tr::FindInConverter( visionObjectConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToKeyPoint
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_KeyPoint ADN_Tr::ConvertToKeyPoint( const std::string& strName )
{
    return ADN_Tr::FindInConverter( keyPointConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToAgentTypePion
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_AgentTypePion ADN_Tr::ConvertToAgentTypePion( const std::string& strName )
{
    return ADN_Tr::FindInConverter( agentTypePionConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToAgentTypeAutomate
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_AgentTypeAutomate ADN_Tr::ConvertToAgentTypeAutomate( const std::string& strName )
{
    return ADN_Tr::FindInConverter( agentTypeAutomateConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToNatureAtlasType
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_NatureAtlasType ADN_Tr::ConvertToNatureAtlasType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( natureAtlasTypeConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToSpeedImpact
// Created: APE 2005-02-23
// -----------------------------------------------------------------------------
E_SpeedImpact ADN_Tr::ConvertToSpeedImpact( const std::string& strName )
{
    return ADN_Tr::FindInConverter( speedImpactConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToCrossingHeight
// Created: JSR 2011-01-03
// -----------------------------------------------------------------------------
E_CrossingHeight ADN_Tr::ConvertToCrossingHeight( const std::string& strName )
{
    return ADN_Tr::FindInConverter( crossingHeightConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToSensorWeatherModifiers
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_SensorWeatherModifiers ADN_Tr::ConvertToSensorWeatherModifiers( const std::string& strName )
{
    return ADN_Tr::FindInConverter( sensorWeatherModifiersConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToDoctorSkills
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_DoctorSkills ADN_Tr::ConvertToDoctorSkills( const std::string& strName )
{
    return ADN_Tr::FindInConverter( doctorSkillsConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToProtectionType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_ProtectionType ADN_Tr::ConvertToProtectionType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( protectionTypeConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToBreakdownType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_BreakdownType ADN_Tr::ConvertToBreakdownType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( breakdownTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToBreakdownNTI
// Created: SBO 2005-09-07
// -----------------------------------------------------------------------------
E_BreakdownNTI ADN_Tr::ConvertToBreakdownNTI( const std::string& strName )
{
    return ADN_Tr::FindInConverter( breakdownNTIConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToRadarType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_RadarType ADN_Tr::ConvertToRadarType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( radarTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToEquipmentState
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
E_EquipmentState_ADN ADN_Tr::ConvertToEquipmentState( const std::string& strName )
{
    return ADN_Tr::FindInConverter( equipmentStateConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeMunitionTirIndirect ADN_Tr::ConvertToTypeMunitionTirIndirect( const std::string& strName )
{
    return ADN_Tr::FindInConverter( munitionTirIndirectConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToMissionParameterType
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
E_MissionParameterType ADN_Tr::ConvertToMissionParameterType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( missionParameterTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToAnchorType
// Created: LGY 2011-11-28
// -----------------------------------------------------------------------------
E_AnchorType ADN_Tr::ConvertToAnchorType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( anchorTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToConstructorType
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
E_ConstructorType ADN_Tr::ConvertToConstructorType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( constructorTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToPropagationModel
// Created: JSR 2011-12-21
// -----------------------------------------------------------------------------
E_PropagationModel ADN_Tr::ConvertToPropagationModel( const std::string& strName )
{
    return ADN_Tr::FindInConverter( propagationModelConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToEquipmentCategory
// Created: ABR 2012-03-30
// -----------------------------------------------------------------------------
E_EquipmentCategory ADN_Tr::ConvertToEquipmentCategory( const std::string& strName )
{
    return ADN_Tr::FindInConverter( equipmentCategoryConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToWorkspaceElements
// Created: ABR 2012-07-24
// -----------------------------------------------------------------------------
E_WorkspaceElements ADN_Tr::ConvertToWorkspaceElements( const std::string& strName )
{
    return ADN_Tr::FindInConverter( workspaceElementsConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::InitTranslations
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
void ADN_Tr::InitTranslations()
{
    InitTr( consumptionTypeConverter_, "ADN_Tr" );
    InitTr( timeCategoryConverter_, "ADN_Tr" );
    InitTr( visionObjectConverter_, "ADN_Tr" );
    InitTr( keyPointConverter_, "ADN_Tr" );
    InitTr( agentTypePionConverter_, "ADN_Tr" );
    InitTr( agentTypeAutomateConverter_, "ADN_Tr" );
    InitTr( natureAtlasTypeConverter_, "ADN_Tr" );
    InitTr( speedImpactConverter_, "ADN_Tr" );
    InitTr( crossingHeightConverter_, "ADN_Tr" );
    InitTr( sensorWeatherModifiersConverter_, "ADN_Tr" );
    InitTr( doctorSkillsConverter_, "ADN_Tr" );
    InitTr( protectionTypeConverter_, "ADN_Tr" );
    InitTr( breakdownTypeConverter_, "ADN_Tr" );
    InitTr( breakdownNTIConverter_, "ADN_Tr" );
    InitTr( radarTypeConverter_, "ADN_Tr" );
    InitTr( equipmentStateConverter_, "ADN_Tr" );
    InitTr( munitionTirIndirectConverter_ , "ADN_Tr" );
    InitTr( missionParameterTypeConverter_, "ADN_Tr" );
    InitTr( anchorTypeConverter_, "ADN_Tr" );
    InitTr( constructorTypeConverter_, "ADN_Tr" );
    InitTr( propagationModelConverter_, "ADN_Tr" );
    InitTr( equipmentCategoryConverter_, "ADN_Tr" );
    InitTr( workspaceElementsConverter_, "ADN_Tr" );
}
