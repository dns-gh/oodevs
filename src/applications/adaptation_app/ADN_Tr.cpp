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

ADN_Tr::T_ConverterLocation ADN_Tr::locationConverter_[] =
{
    T_ConverterLocation( "inconnu",            QT_TRANSLATE_NOOP( "ADN_Tr", "inconnu" ),            eLocation_Unknown            ),

    T_ConverterLocation( "foret",              QT_TRANSLATE_NOOP( "ADN_Tr", "foret" ),              eLocation_Forest             ),
    T_ConverterLocation( "plantation",         QT_TRANSLATE_NOOP( "ADN_Tr", "plantation" ),         eLocation_Plantation         ),
    T_ConverterLocation( "marais",             QT_TRANSLATE_NOOP( "ADN_Tr", "marais" ),             eLocation_Swamp              ),
    T_ConverterLocation( "dune",               QT_TRANSLATE_NOOP( "ADN_Tr", "dune" ),               eLocation_Dune               ),
    T_ConverterLocation( "urbain",             QT_TRANSLATE_NOOP( "ADN_Tr", "urbain" ),             eLocation_Urban              ),
    T_ConverterLocation( "etendue eau",        QT_TRANSLATE_NOOP( "ADN_Tr", "etendue eau" ),        eLocation_Water              ),
    T_ConverterLocation( "glace",              QT_TRANSLATE_NOOP( "ADN_Tr", "glace" ),              eLocation_Ice                ),

    T_ConverterLocation( "lisiere",            QT_TRANSLATE_NOOP( "ADN_Tr", "lisiere" ),            eLocation_ForestBorder       ),
    T_ConverterLocation( "banlieue",           QT_TRANSLATE_NOOP( "ADN_Tr", "banlieue" ),           eLocation_UrbanBorder        ),
    T_ConverterLocation( "cote",               QT_TRANSLATE_NOOP( "ADN_Tr", "cote" ),               eLocation_WaterBorder        ),
    T_ConverterLocation( "bord de plantation", QT_TRANSLATE_NOOP( "ADN_Tr", "bord de plantation" ), eLocation_PlantationBorder   ),
    T_ConverterLocation( "bord de marais",     QT_TRANSLATE_NOOP( "ADN_Tr", "bord de marais" ),     eLocation_SwampBorder        ),
    T_ConverterLocation( "bord de dune",       QT_TRANSLATE_NOOP( "ADN_Tr", "bord de dune" ),       eLocation_DuneBorder         ),
    T_ConverterLocation( "bord de glace",      QT_TRANSLATE_NOOP( "ADN_Tr", "bord de glace" ),      eLocation_IceBorder          ),

    T_ConverterLocation( "chemin",             QT_TRANSLATE_NOOP( "ADN_Tr", "chemin" ),             eLocation_SmallRoad          ),
    T_ConverterLocation( "autoroute",          QT_TRANSLATE_NOOP( "ADN_Tr", "autoroute" ),          eLocation_Motorway           ),
    T_ConverterLocation( "nationale",          QT_TRANSLATE_NOOP( "ADN_Tr", "nationale" ),          eLocation_LargeRoad          ),
    T_ConverterLocation( "departementale",     QT_TRANSLATE_NOOP( "ADN_Tr", "departementale" ),     eLocation_MediumRoad         ),
    T_ConverterLocation( "ruisseau",           QT_TRANSLATE_NOOP( "ADN_Tr", "ruisseau" ),           eLocation_SmallRiver         ),
    T_ConverterLocation( "riviere",            QT_TRANSLATE_NOOP( "ADN_Tr", "riviere" ),            eLocation_MediumRiver        ),
    T_ConverterLocation( "riviere importante", QT_TRANSLATE_NOOP( "ADN_Tr", "riviere importante" ), eLocation_LargeRiver         ),
    T_ConverterLocation( "pont",               QT_TRANSLATE_NOOP( "ADN_Tr", "pont" ),               eLocation_Bridge             ),
    T_ConverterLocation( "falaise",            QT_TRANSLATE_NOOP( "ADN_Tr", "falaise" ),            eLocation_Cliff              ),
    T_ConverterLocation( "voie ferree",        QT_TRANSLATE_NOOP( "ADN_Tr", "voie ferree" ),        eLocation_Railroad           ),
    T_ConverterLocation( "carrefour",          QT_TRANSLATE_NOOP( "ADN_Tr", "carrefour" ),          eLocation_Crossroad          ),
    T_ConverterLocation( "",                   QT_TRANSLATE_NOOP( "ADN_Tr", "" ),                   (E_Location)-1               )
};


ADN_Tr::T_ConverterConsumptionType ADN_Tr::consumptionTypeConverter_[] =
{
    T_ConverterConsumptionType( "ArretMoteurEteint" , QT_TRANSLATE_NOOP( "ADN_Tr" , "ArretMoteurEteint" ) , eEngineStopped ) ,
    T_ConverterConsumptionType( "ArretMoteurAllume" , QT_TRANSLATE_NOOP( "ADN_Tr" , "ArretMoteurAllume" ) , eEngineStarted ) ,
    T_ConverterConsumptionType( "Deplacement" ,       QT_TRANSLATE_NOOP( "ADN_Tr" , "Deplacement" ) ,       eMoving ) ,
    T_ConverterConsumptionType( "EnTravaux" ,         QT_TRANSLATE_NOOP( "ADN_Tr" , "EnTravaux" ) ,         eWorking ) ,
    T_ConverterConsumptionType( "" , "" , (E_ConsumptionType)-1 )
};


ADN_Tr::T_ConverterTimeCategory ADN_Tr::timeCategoryConverter_[] =
{
    T_ConverterTimeCategory( "JourSansNuage"         , QT_TRANSLATE_NOOP("ADN_Tr" ,"JourSansNuage")         , eTimeTypeDay_NoCloud            ),
    T_ConverterTimeCategory( "JourPeuNuageux"        , QT_TRANSLATE_NOOP("ADN_Tr" ,"JourPeuNuageux")        , eTimeTypeDay_LightCloud         ),
    T_ConverterTimeCategory( "JourMoyennementNuageux", QT_TRANSLATE_NOOP("ADN_Tr" ,"JourMoyennementNuageux"), eTimeTypeDay_MiddleCloud        ),
    T_ConverterTimeCategory( "JourAssezNuageux"      , QT_TRANSLATE_NOOP("ADN_Tr" ,"JourAssezNuageux")      , eTimeTypeDay_HeavyCloud         ),
    T_ConverterTimeCategory( "JourTresNuageux"       , QT_TRANSLATE_NOOP("ADN_Tr" ,"JourTresNuageux")       , eTimeTypeDay_FullCloud          ),
    T_ConverterTimeCategory( "NuitPleineLune"        , QT_TRANSLATE_NOOP("ADN_Tr" ,"NuitPleineLune")        , eTimeTypeNight_FullMoon         ),
    T_ConverterTimeCategory( "NuitTroisQuartDeLune"  , QT_TRANSLATE_NOOP("ADN_Tr" ,"NuitTroisQuartDeLune")  , eTimeTypeNight_ThreeQuarterMoon ),
    T_ConverterTimeCategory( "NuitDemiLune"          , QT_TRANSLATE_NOOP("ADN_Tr" ,"NuitDemiLune")          , eTimeTypeNight_HalfMoon         ),
    T_ConverterTimeCategory( "NuitQuartDeLune"       , QT_TRANSLATE_NOOP("ADN_Tr" ,"NuitQuartDeLune")       , eTimeTypeNight_OneQuarterMoon   ),
    T_ConverterTimeCategory( "NuitNouvelleLune"      , QT_TRANSLATE_NOOP("ADN_Tr" ,"NuitNouvelleLune")      , eTimeTypeNight_NewMoon          ),
    T_ConverterTimeCategory( "Eclairant"             , QT_TRANSLATE_NOOP("ADN_Tr" ,"Eclairant")             , eTimeTypeNight_LightingRocket   ),
    T_ConverterTimeCategory( ""                      , ""                                                   , (E_TimeCategory)-1              )
};


ADN_Tr::T_ConverterVisionObject ADN_Tr::visionObjectConverter_[] =
{
    T_ConverterVisionObject( "Sol"    ,  QT_TRANSLATE_NOOP("ADN_Tr" ,"Sol")   ,   eVisionGround      ) ,
    T_ConverterVisionObject( "Vide"   ,  QT_TRANSLATE_NOOP("ADN_Tr" ,"Vide")  ,   eVisionEmpty       ) ,
    T_ConverterVisionObject( "Foret"  ,  QT_TRANSLATE_NOOP("ADN_Tr" ,"Foret") ,   eVisionForest      ) ,
    T_ConverterVisionObject( "Urbain" ,  QT_TRANSLATE_NOOP("ADN_Tr" ,"Urbain"),   eVisionUrban       ) ,
    T_ConverterVisionObject( ""       ,  ""                                   ,   (E_VisionObject)-1 )
};


ADN_Tr::T_ConverterKeyPoint ADN_Tr::keyPointConverter_[] =
{
    T_ConverterKeyPoint( "foret" ,       QT_TRANSLATE_NOOP( "ADN_Tr" , "foret" ) ,     eKeyPointForest ) ,
    T_ConverterKeyPoint( "urbain" ,      QT_TRANSLATE_NOOP( "ADN_Tr" , "urbain" ) ,    eKeyPointUrban ) ,
    T_ConverterKeyPoint( "carrefour" ,   QT_TRANSLATE_NOOP( "ADN_Tr" , "carrefour" ) , eKeyPointCrossroads ) ,
    T_ConverterKeyPoint( "pont" ,        QT_TRANSLATE_NOOP( "ADN_Tr" , "pont" ) ,      eKeyPointBridge ) ,
    T_ConverterKeyPoint( "lima" ,        QT_TRANSLATE_NOOP( "ADN_Tr" , "lima" ) ,      eKeyPointLima ) ,
    T_ConverterKeyPoint( "" , "" , (E_KeyPoint)-1 )
};


ADN_Tr::T_ConverterAgentTypePion ADN_Tr::agentTypePionConverter_[] =
{
    T_ConverterAgentTypePion( "Pion INF"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion INF" ),                           eAgentTypePionINF ),
    T_ConverterAgentTypePion( "Pion ASA"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion ASA" ),                           eAgentTypePionASA ),
    T_ConverterAgentTypePion( "Pion ALAT"                       ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion ALAT" ),                          eAgentTypePionALAT ),
    T_ConverterAgentTypePion( "Pion RENS"                       ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion RENS" ),                          eAgentTypePionRENS ),
    T_ConverterAgentTypePion( "Pion NBC"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion NBC" ),                           eAgentTypePionNBC ),
    T_ConverterAgentTypePion( "Pion ABC"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion ABC" ),                           eAgentTypePionABC ),
    T_ConverterAgentTypePion( "Pion GEN"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion GEN" ),                           eAgentTypePionGEN ),
    T_ConverterAgentTypePion( "Pion ASS"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion ASS" ),                           eAgentTypePionASS ),
    T_ConverterAgentTypePion( "Pion TRANS"                      ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion TRANS" ),                         eAgentTypePionTRANS ),
    T_ConverterAgentTypePion( "Pion LOG TC2"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG TC2" ),                       eAgentTypePionLOGTC2 ),
    T_ConverterAgentTypePion( "Pion LOG BLD Sante"              ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG BLD Sante" ),                 eAgentTypePionLOGBLDSante ),
    T_ConverterAgentTypePion( "Pion LOG BLD Maintenance"        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG BLD Maintenance" ),           eAgentTypePionLOGBLDMaintenance ),
    T_ConverterAgentTypePion( "Pion LOG BLD Ravitaillement"     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG BLD Ravitaillement" ),        eAgentTypePionLOGBLDRavitaillement ),
    T_ConverterAgentTypePion( "Pion LOG BLT Sante"              ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG BLT Sante" ),                 eAgentTypePionLOGBLTSante ),
    T_ConverterAgentTypePion( "Pion LOG BLT Maintenance"        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG BLT Maintenance" ),           eAgentTypePionLOGBLTMaintenance ),
    T_ConverterAgentTypePion( "Pion LOG BLT Ravitaillement"     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG BLT Ravitaillement" ),        eAgentTypePionLOGBLTRavitaillement ),
    T_ConverterAgentTypePion( "Pion LOG Convoi"                 ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion LOG Convoi" ),                    eAgentTypePionLOGConvoi ),
    T_ConverterAgentTypePion( "Pion CIRCULATION"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion CIRCULATION" ),                   eAgentTypePionCIRCULATION ),
    T_ConverterAgentTypePion( "Pion REFUGIE"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion REFUGIE" ),                       eAgentTypePionREFUGIE ),
    T_ConverterAgentTypePion( "Pion MILICE"                     ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion MILICE" ),                        eAgentTypePionMILICE ),
    T_ConverterAgentTypePion( "Pion JOINT"                      ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion JOINT" ),                         eAgentTypePionJOINT ),
    T_ConverterAgentTypePion( "Pion ASY"                        ,QT_TRANSLATE_NOOP( "ADN_Tr", "Pion ASY" ),                           eAgentTypePionASY ),
    T_ConverterAgentTypePion( "",         "",          (E_AgentTypePion)-1 )
};


ADN_Tr::T_ConverterAgentTypeAutomate ADN_Tr::agentTypeAutomateConverter_[] =
{
    T_ConverterAgentTypeAutomate( "Automate INF"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate INF" ),                       eAgentTypeAutomateINF ),
    T_ConverterAgentTypeAutomate( "Automate ASA"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate ASA" ),                       eAgentTypeAutomateASA ),
    T_ConverterAgentTypeAutomate( "Automate ALAT"                   ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate ALAT" ),                      eAgentTypeAutomateALAT ),
    T_ConverterAgentTypeAutomate( "Automate RENS"                   ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate RENS" ),                      eAgentTypeAutomateRENS ),
    T_ConverterAgentTypeAutomate( "Automate NBC"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate NBC" ),                       eAgentTypeAutomateNBC ),
    T_ConverterAgentTypeAutomate( "Automate ABC"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate ABC" ),                       eAgentTypeAutomateABC ),
    T_ConverterAgentTypeAutomate( "Automate GEN"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate GEN" ),                       eAgentTypeAutomateGEN ),
    T_ConverterAgentTypeAutomate( "Automate ASS"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate ASS" ),                       eAgentTypeAutomateASS ),
    T_ConverterAgentTypeAutomate( "Automate TRANS"                  ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate TRANS" ),                     eAgentTypeAutomateTRANS ),
    T_ConverterAgentTypeAutomate( "Automate LOG TC2"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG TC2" ),                   eAgentTypeAutomateLOGTC2 ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLD Sante"          ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG BLD Sante" ),             eAgentTypeAutomateLOGBLDSante ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLD Maintenance"    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG BLD Maintenance" ),       eAgentTypeAutomateLOGBLDMaintenance ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLD Ravitaillement" ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG BLD Ravitaillement" ),    eAgentTypeAutomateLOGBLDRavitaillement ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLT Sante"          ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG BLT Sante" ),             eAgentTypeAutomateLOGBLTSante ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLT Maintenance"    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG BLT Maintenance" ),       eAgentTypeAutomateLOGBLTMaintenance ),
    T_ConverterAgentTypeAutomate( "Automate LOG BLT Ravitaillement" ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate LOG BLT Ravitaillement" ),    eAgentTypeAutomateLOGBLTRavitaillement ),
    T_ConverterAgentTypeAutomate( "Automate CIRCULATION"            ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate CIRCULATION" ),               eAgentTypeAutomateCIRCULATION ),  
    T_ConverterAgentTypeAutomate( "Automate REFUGIE"                ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate REFUGIE" ),                   eAgentTypeAutomateREFUGIE ),
    T_ConverterAgentTypeAutomate( "Automate MILICE"                 ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate MILICE" ),                    eAgentTypeAutomateMILICE ),
    T_ConverterAgentTypeAutomate( "Automate JOINT"                  ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate JOINT" ),                     eAgentTypeAutomateJOINT ),
    T_ConverterAgentTypeAutomate( "Automate ASY"                    ,QT_TRANSLATE_NOOP( "ADN_Tr", "Automate ASY" ),                       eAgentTypeAutomateASY ),
    T_ConverterAgentTypeAutomate( "",         "",          (E_AgentTypeAutomate)-1 )
};


ADN_Tr::T_ConverterNatureAtlasType ADN_Tr::natureAtlasTypeConverter_[] =
{
    T_ConverterNatureAtlasType( "PC"              , QT_TRANSLATE_NOOP("ADN_Tr" ,"PC"    )          , eNatureAtlas_PC             ),
    T_ConverterNatureAtlasType( "ASS"             , QT_TRANSLATE_NOOP("ADN_Tr" ,"ASS"   )          , eNatureAtlas_ASS            ),
    T_ConverterNatureAtlasType( "Log"             , QT_TRANSLATE_NOOP("ADN_Tr" ,"Log"   )          , eNatureAtlas_Log            ),
    T_ConverterNatureAtlasType( "None"            , QT_TRANSLATE_NOOP("ADN_Tr" ,"None")            , eNatureAtlas_None           ),
    T_ConverterNatureAtlasType( "Blinde"          , QT_TRANSLATE_NOOP("ADN_Tr" ,"Blinde")          , eNatureAtlas_Armored        ),
    T_ConverterNatureAtlasType( "Mortier"         , QT_TRANSLATE_NOOP("ADN_Tr" ,"Mortier")         , eNatureAtlas_Mortar         ),
    T_ConverterNatureAtlasType( "Lance roquette"  , QT_TRANSLATE_NOOP("ADN_Tr" ,"Lance roquette")  , eNatureAtlas_RocketLauncher ),
    T_ConverterNatureAtlasType( "Vehicule"        , QT_TRANSLATE_NOOP("ADN_Tr" ,"Vehicule"   )     , eNatureAtlas_Vehicule       ),
    T_ConverterNatureAtlasType( "Personnel a pied", QT_TRANSLATE_NOOP("ADN_Tr" ,"Personnel a pied"), eNatureAtlas_PersonnelAPied ),
    T_ConverterNatureAtlasType( "Helicoptere"     , QT_TRANSLATE_NOOP("ADN_Tr" ,"Helicoptere")     , eNatureAtlas_Helicoptere    ),
    T_ConverterNatureAtlasType( "ASA"             , QT_TRANSLATE_NOOP("ADN_Tr" ,"ASA")             , eNatureAtlas_ASA            ),
    T_ConverterNatureAtlasType( "Indefini"        , QT_TRANSLATE_NOOP("ADN_Tr" ,"Indefini")        , eNatureAtlas_Indefini       ),
    T_ConverterNatureAtlasType( ""                , ""                                             , (E_NatureAtlasType)-1       )
};


ADN_Tr::T_ConverterSpeedImpact ADN_Tr::speedImpactConverter_[] =
{
    T_ConverterSpeedImpact( "AuPlusLent",      QT_TRANSLATE_NOOP( "ADN_Tr", "AuPlusLent" ),      eSpeedImpact_AuPlusLent ),
    T_ConverterSpeedImpact( "VitesseMaxObjet", QT_TRANSLATE_NOOP( "ADN_Tr", "VitesseMaxObjet" ), eSpeedImpact_VitesseMaxObjet ),
    T_ConverterSpeedImpact( "VitesseMaxAgent", QT_TRANSLATE_NOOP( "ADN_Tr", "VitesseMaxAgent" ), eSpeedImpact_VitesseMaxAgent ),
    T_ConverterSpeedImpact( "", "", (E_SpeedImpact)-1 )
};


ADN_Tr::T_ConverterSensorWeatherModifiers ADN_Tr::sensorWeatherModifiersConverter_[] =
{
    T_ConverterSensorWeatherModifiers( "PasDePrecipitation", QT_TRANSLATE_NOOP( "ADN_Tr", "PasDePrecipitation" ),eSensorWeatherModifiers_PasDePrecipitation ),
    T_ConverterSensorWeatherModifiers( "TempeteDeSable",     QT_TRANSLATE_NOOP( "ADN_Tr", "TempeteDeSable" ),    eSensorWeatherModifiers_TempeteSable ),
    T_ConverterSensorWeatherModifiers( "Brouillard",         QT_TRANSLATE_NOOP( "ADN_Tr", "Brouillard" ),        eSensorWeatherModifiers_Brouillard ),
    T_ConverterSensorWeatherModifiers( "Crachin",            QT_TRANSLATE_NOOP( "ADN_Tr", "Crachin" ),           eSensorWeatherModifiers_Crachin ),
    T_ConverterSensorWeatherModifiers( "Pluie",              QT_TRANSLATE_NOOP( "ADN_Tr", "Pluie" ),             eSensorWeatherModifiers_Pluie ),
    T_ConverterSensorWeatherModifiers( "Neige",              QT_TRANSLATE_NOOP( "ADN_Tr", "Neige" ),             eSensorWeatherModifiers_Neige ),
    T_ConverterSensorWeatherModifiers( "Fumigene",           QT_TRANSLATE_NOOP( "ADN_Tr", "Fumigene" ),          eSensorWeatherModifiers_Fumigene ),
    T_ConverterSensorWeatherModifiers( "", "", (E_SensorWeatherModifiers)-1 )
};


ADN_Tr::T_ConverterDoctorSkills ADN_Tr::doctorSkillsConverter_[] =
{
    T_ConverterDoctorSkills( "U1",              QT_TRANSLATE_NOOP( "ADN_Tr", "U1" ),             eDoctorSkills_U1 ),
    T_ConverterDoctorSkills( "U2",              QT_TRANSLATE_NOOP( "ADN_Tr", "U2" ),             eDoctorSkills_U2 ),
    T_ConverterDoctorSkills( "U3",              QT_TRANSLATE_NOOP( "ADN_Tr", "U3" ),             eDoctorSkills_U3 ),
    T_ConverterDoctorSkills( "UE",              QT_TRANSLATE_NOOP( "ADN_Tr", "UE" ),             eDoctorSkills_UE ),
    T_ConverterDoctorSkills( "", "", (E_DoctorSkills)-1 )
};


ADN_Tr::T_ConverterProtectionType ADN_Tr::protectionTypeConverter_[] =
{
    T_ConverterProtectionType( "materiel",   QT_TRANSLATE_NOOP( "ADN_Tr", "materiel" ),  eProtectionType_Material ),
    T_ConverterProtectionType( "humain",     QT_TRANSLATE_NOOP( "ADN_Tr", "humain" ),    eProtectionType_Human ),
    T_ConverterProtectionType( "", "", (E_ProtectionType)-1 )
};


ADN_Tr::T_ConverterBreakdownType ADN_Tr::breakdownTypeConverter_[] =
{
    T_ConverterBreakdownType( "EA",   QT_TRANSLATE_NOOP( "ADN_Tr", "EA" ),  eBreakdownType_EA ),
    T_ConverterBreakdownType( "M",    QT_TRANSLATE_NOOP( "ADN_Tr", "M" ),   eBreakdownType_M ),
    T_ConverterBreakdownType( "", "", (E_BreakdownType)-1 )
};

ADN_Tr::T_ConverterBreakdownNTI ADN_Tr::breakdownNTIConverter_[] =
{
    T_ConverterBreakdownNTI( "NTI1",   QT_TRANSLATE_NOOP( "ADN_Tr", "NTI1" ),  eBreakdownNTI_NTI1 ),
    T_ConverterBreakdownNTI( "NTI2",   QT_TRANSLATE_NOOP( "ADN_Tr", "NTI2" ),  eBreakdownNTI_NTI2 ),
    T_ConverterBreakdownNTI( "NTI3",   QT_TRANSLATE_NOOP( "ADN_Tr", "NTI3" ),  eBreakdownNTI_NTI3 ),
    T_ConverterBreakdownNTI( "", "", (E_BreakdownNTI)-1 )
};

ADN_Tr::T_ConverterMunitionType ADN_Tr::munitionTypeConverter_[] =
{
    T_ConverterMunitionType( "Obus",          QT_TRANSLATE_NOOP( "ADN_Tr", "Obus" ),            eMunitionType_Obus ),
    T_ConverterMunitionType( "MissileSol",    QT_TRANSLATE_NOOP( "ADN_Tr", "MissileSol" ),      eMunitionType_MissileSol ),
    T_ConverterMunitionType( "MissileAir",    QT_TRANSLATE_NOOP( "ADN_Tr", "MissileAir" ),      eMunitionType_MissileAir ),
    T_ConverterMunitionType( "Mitraille",     QT_TRANSLATE_NOOP( "ADN_Tr", "Mitraille" ),       eMunitionType_Mitraille ),
    T_ConverterMunitionType( "", "", (E_MunitionType)-1 )
};


ADN_Tr::T_ConverterCapacityMission ADN_Tr::capacityMissionConverter_[] =
{
    T_ConverterCapacityMission( "Civilian law enforcement"                    , QT_TRANSLATE_NOOP( "ADN_Tr", "Civilian law enforcement"                    ), eCivilianLawEnforcement                 ),
        T_ConverterCapacityMission( "Command operations"                          , QT_TRANSLATE_NOOP( "ADN_Tr", "Command operations"                          ), eCommandOperations                      ),
        T_ConverterCapacityMission( "Couverture aerienne"                         , QT_TRANSLATE_NOOP( "ADN_Tr", "Couverture aerienne"                         ), eCouvertureAerienne                     ),
        T_ConverterCapacityMission( "Joint intelligence"                          , QT_TRANSLATE_NOOP( "ADN_Tr", "Joint intelligence"                          ), eJointIntelligence                      ),
        T_ConverterCapacityMission( "Morale, welfare recreation"                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Morale, welfare recreation"                  ), eMoraleWelfareRecreation                ),
        T_ConverterCapacityMission( "Reglage electronique"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Reglage electronique"                        ), eReglageElectronique                    ),
        T_ConverterCapacityMission( "Remplacement"                                , QT_TRANSLATE_NOOP( "ADN_Tr", "Remplacement"                                ), eRemplacement                           ),
        T_ConverterCapacityMission( "Service mortuaire"                           , QT_TRANSLATE_NOOP( "ADN_Tr", "Service mortuaire"                           ), eServiceMortuaire                       ),
        T_ConverterCapacityMission( "Signals intelligence (SIGINT)"               , QT_TRANSLATE_NOOP( "ADN_Tr", "Signals intelligence (SIGINT)"               ), eSignalsIntelligence                    ),
        T_ConverterCapacityMission( "Theatre missile defence"                     , QT_TRANSLATE_NOOP( "ADN_Tr", "Theatre missile defence"                     ), eTheatreMissileDefence                  ),
        T_ConverterCapacityMission( "Utilitaire"                                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Utilitaire"                                  ), eUtilitaire                             ),
        T_ConverterCapacityMission( "Aeroporte"                                   , QT_TRANSLATE_NOOP( "ADN_Tr", "Aeroporte"                                   ), eAéroporte                              ),
        T_ConverterCapacityMission( "Affaires publiques"                          , QT_TRANSLATE_NOOP( "ADN_Tr", "Affaires publiques"                          ), eAffairesPubliques                      ),
        T_ConverterCapacityMission( "Affaires publiques, communication"           , QT_TRANSLATE_NOOP( "ADN_Tr", "Affaires publiques, communication"           ), eAffairesPubliques_Communication        ),
        T_ConverterCapacityMission( "Affaires publiques, joint information"       , QT_TRANSLATE_NOOP( "ADN_Tr", "Affaires publiques, joint information"       ), eAffairesPubliques_JointInformation     ),
        T_ConverterCapacityMission( "Alpin"                                       , QT_TRANSLATE_NOOP( "ADN_Tr", "Alpin"                                       ), eAlpin                                  ),
        T_ConverterCapacityMission( "Amphibie"                                    , QT_TRANSLATE_NOOP( "ADN_Tr", "Amphibie"                                    ), eAmphibie                               ),
        T_ConverterCapacityMission( "Arctique"                                    , QT_TRANSLATE_NOOP( "ADN_Tr", "Arctique"                                    ), eArctique                               ),
        T_ConverterCapacityMission( "Artillerie - reconnaissance"                 , QT_TRANSLATE_NOOP( "ADN_Tr", "Artillerie - reconnaissance"                 ), eArtillerieReconnaissance               ),
        T_ConverterCapacityMission( "Attaque"                                     , QT_TRANSLATE_NOOP( "ADN_Tr", "Attaque"                                     ), eAttaque                                ),
        T_ConverterCapacityMission( "Attaque aerienne"                            , QT_TRANSLATE_NOOP( "ADN_Tr", "Attaque aerienne"                            ), eAttaqueAerienne                        ),
        T_ConverterCapacityMission( "C2"                                          , QT_TRANSLATE_NOOP( "ADN_Tr", "C2"                                          ), eC2                                     ),
        T_ConverterCapacityMission( "Chemin de fer"                               , QT_TRANSLATE_NOOP( "ADN_Tr", "Chemin de fer"                               ), eCheminDeFer                            ),
        T_ConverterCapacityMission( "Ciblage"                                     , QT_TRANSLATE_NOOP( "ADN_Tr", "Ciblage"                                     ), eCiblage                                ),
        T_ConverterCapacityMission( "Contre espionnage"                           , QT_TRANSLATE_NOOP( "ADN_Tr", "Contre espionnage"                           ), eContreEspionnage                       ),
        T_ConverterCapacityMission( "Eclairage"                                   , QT_TRANSLATE_NOOP( "ADN_Tr", "Eclairage"                                   ), eEclairage                              ),
        T_ConverterCapacityMission( "Entretien"                                   , QT_TRANSLATE_NOOP( "ADN_Tr", "Entretien"                                   ), eEntretien                              ),
        T_ConverterCapacityMission( "Entretien, materiel"                         , QT_TRANSLATE_NOOP( "ADN_Tr", "Entretien, materiel"                         ), eEntretienMateriel                      ),
        T_ConverterCapacityMission( "Entretien, missile"                          , QT_TRANSLATE_NOOP( "ADN_Tr", "Entretien, missile"                          ), eEntretienMissile                       ),
        T_ConverterCapacityMission( "Entretien, opto-electronique"                , QT_TRANSLATE_NOOP( "ADN_Tr", "Entretien, opto-electronique"                ), eEntretienOptoElectronique              ),
        T_ConverterCapacityMission( "Espionnage"                                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Espionnage"                                  ), eEspionnage                             ),
        T_ConverterCapacityMission( "Espionnage militaire, aerial exploitation"   , QT_TRANSLATE_NOOP( "ADN_Tr", "Espionnage militaire, aerial exploitation"   ), eEspionnageMilitaireAerialExploitation  ),
        T_ConverterCapacityMission( "Espionnage militaire, operation"             , QT_TRANSLATE_NOOP( "ADN_Tr", "Espionnage militaire, operation"             ), eEspionnageMilitaireOperation           ),
        T_ConverterCapacityMission( "Espionnage militaire, tactical exploitation" , QT_TRANSLATE_NOOP( "ADN_Tr", "Espionnage militaire, tactical exploitation" ), eEspionnageMilitaireTacticalExploitation),
        T_ConverterCapacityMission( "Evacuation sanitaire"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Evacuation sanitaire"                        ), eEvacuationSanitaire                    ),
        T_ConverterCapacityMission( "Finance"                                     , QT_TRANSLATE_NOOP( "ADN_Tr", "Finance"                                     ), eFinance                                ),
        T_ConverterCapacityMission( "Genie, combat"                               , QT_TRANSLATE_NOOP( "ADN_Tr", "Genie, combat"                               ), eGenieCombat                            ),
        T_ConverterCapacityMission( "Genie, construction"                         , QT_TRANSLATE_NOOP( "ADN_Tr", "Genie, construction"                         ), eGenieConstruction                      ),
        T_ConverterCapacityMission( "Genie, construction navale"                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Genie, construction navale"                  ), eGenieConstructionNavale                ),
        T_ConverterCapacityMission( "Guerre electronique"                         , QT_TRANSLATE_NOOP( "ADN_Tr", "Guerre electronique"                         ), eGuerreElectronique                     ),
        T_ConverterCapacityMission( "Guerre electronique, interception"           , QT_TRANSLATE_NOOP( "ADN_Tr", "Guerre electronique, interception"           ), eGuerreElectroniqueInterception         ),
        T_ConverterCapacityMission( "Guerre electronique, brouillage"             , QT_TRANSLATE_NOOP( "ADN_Tr", "Guerre electronique, brouillage"             ), eGuerreElectroniqueBrouillage           ),
        T_ConverterCapacityMission( "Guerre electronique, detection"              , QT_TRANSLATE_NOOP( "ADN_Tr", "Guerre electronique, detection"              ), eGuerreElectroniqueDétection            ),
        T_ConverterCapacityMission( "Interrogation"                               , QT_TRANSLATE_NOOP( "ADN_Tr", "Interrogation"                               ), eInterrogation                          ),
        T_ConverterCapacityMission( "Main d'oeuvre"                               , QT_TRANSLATE_NOOP( "ADN_Tr", "Main d'oeuvre"                               ), eMainDOeuvre                            ),
        T_ConverterCapacityMission( "Maintien de la paix"                         , QT_TRANSLATE_NOOP( "ADN_Tr", "Maintien de la paix"                         ), eMaintienDeLaPaix                       ),
        T_ConverterCapacityMission( "Marine"                                      , QT_TRANSLATE_NOOP( "ADN_Tr", "Marine"                                      ), eMarine                                 ),
        T_ConverterCapacityMission( "Mine de contre-mesure"                       , QT_TRANSLATE_NOOP( "ADN_Tr", "Mine de contre-mesure"                       ), eMineDeContreMesure                     ),
        T_ConverterCapacityMission( "NBC, biologique"                             , QT_TRANSLATE_NOOP( "ADN_Tr", "NBC, biologique"                             ), eNBCBiologique                          ),
        T_ConverterCapacityMission( "NBC, chimique"                               , QT_TRANSLATE_NOOP( "ADN_Tr", "NBC, chimique"                               ), eNBCChimique                            ),
        T_ConverterCapacityMission( "NBC, chimique, fumee"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "NBC, chimique, fumee"                        ), eNBCChimiqueFumee                       ),
        T_ConverterCapacityMission( "NBC, decontamination"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "NBC, decontamination"                        ), eNBCDecontamination                     ),
        T_ConverterCapacityMission( "NBC, decontamination chimique"               , QT_TRANSLATE_NOOP( "ADN_Tr", "NBC, decontamination chimique"               ), eNBCDecontaminationChimique             ),
        T_ConverterCapacityMission( "NBC, nucleaire"                              , QT_TRANSLATE_NOOP( "ADN_Tr", "NBC, nucleaire"                              ), eNBCNucleaire                           ),
        T_ConverterCapacityMission( "Non renseigne"                               , QT_TRANSLATE_NOOP( "ADN_Tr", "Non renseigne"                               ), eNonRenseigne                           ),
        T_ConverterCapacityMission( "Observation"                                 , QT_TRANSLATE_NOOP( "ADN_Tr", "Observation"                                 ), eObservation                            ),
        T_ConverterCapacityMission( "Observation, capteur"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Observation, capteur"                        ), eObservationCapteur                     ),
        T_ConverterCapacityMission( "Observation, longue portee"                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Observation, longue portee"                  ), eObservationLonguePortee                ),
        T_ConverterCapacityMission( "Observation, meteo"                          , QT_TRANSLATE_NOOP( "ADN_Tr", "Observation, meteo"                          ), eObservationMeteo                       ),
        T_ConverterCapacityMission( "Observation, module terrestre"               , QT_TRANSLATE_NOOP( "ADN_Tr", "Observation, module terrestre"               ), eObservationModuleTerrestre             ),
        T_ConverterCapacityMission( "Purification de l'eau"                       , QT_TRANSLATE_NOOP( "ADN_Tr", "Purification de l'eau"                       ), ePurificationDeLEau                     ),
        T_ConverterCapacityMission( "Ravitaillement (class I)"                    , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (class I)"                    ), eRavitaillement_ClasseI                 ),
        T_ConverterCapacityMission( "Ravitaillement (class II)"                   , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (class II)"                   ), eRavitaillement_ClasseII                ),
        T_ConverterCapacityMission( "Ravitaillement (class III aviation)"         , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (class III aviation)"         ), eRavitaillement_ClasseIIIAviation       ),
        T_ConverterCapacityMission( "Ravitaillement (class III)"                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (class III)"                  ), eRavitaillement_ClasseIII               ),
        T_ConverterCapacityMission( "Ravitaillement (class IV)"                   , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (class IV)"                   ), eRavitaillement_ClasseIV                ),
        T_ConverterCapacityMission( "Ravitaillement (class V)"                    , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (class V)"                    ), eRavitaillement_ClasseV                 ),
        T_ConverterCapacityMission( "Ravitaillement (eau)"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement (eau)"                        ), eRavitaillement_Eau                     ),
        T_ConverterCapacityMission( "Ravitaillement, blanchisserie/bain"          , QT_TRANSLATE_NOOP( "ADN_Tr", "Ravitaillement, blanchisserie/bain"          ), eRavitaillementBlanchisserieBain        ),
        T_ConverterCapacityMission( "Recherche & sauvetage"                       , QT_TRANSLATE_NOOP( "ADN_Tr", "Recherche & sauvetage"                       ), eRechercheEtSauvetage                   ),
        T_ConverterCapacityMission( "Reconnaissance"                              , QT_TRANSLATE_NOOP( "ADN_Tr", "Reconnaissance"                              ), eReconnaissance                         ),
        T_ConverterCapacityMission( "Reconnaissance aerienne tactique"            , QT_TRANSLATE_NOOP( "ADN_Tr", "Reconnaissance aerienne tactique"            ), eReconnaissanceAerienneTactique         ),
        T_ConverterCapacityMission( "Recuperation"                                , QT_TRANSLATE_NOOP( "ADN_Tr", "Recuperation"                                ), eRecuperation                           ),
        T_ConverterCapacityMission( "Service du personnel"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Service du personnel"                        ), eServiceDuPersonnel                     ),
        T_ConverterCapacityMission( "Service juridique"                           , QT_TRANSLATE_NOOP( "ADN_Tr", "Service juridique"                           ), eServiceJuridique                       ),
        T_ConverterCapacityMission( "Support aerien rapproche"                    , QT_TRANSLATE_NOOP( "ADN_Tr", "Support aerien rapproche"                    ), eSupportAerienRapproche                 ),
        T_ConverterCapacityMission( "Transmissions, forward communications"       , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, forward communications"       ), eTransmissions_ForwardCommunications    ),
        T_ConverterCapacityMission( "Transmissions, node centre"                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, node centre"                  ), eTransmissions_NodeCentre               ),
        T_ConverterCapacityMission( "Transmissions, node, large extension"        , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, node, large extension"        ), eTransmissions_NodeLargeExtension       ),
        T_ConverterCapacityMission( "Transmissions, node, small extension"        , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, node, small extension"        ), eTransmissions_NodeSmallExtension       ),
        T_ConverterCapacityMission( "Transmissions, radio relay"                  , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, radio relay"                  ), eTransmissions_RadioRelay               ),
        T_ConverterCapacityMission( "Transmissions, radio tactical satellite"     , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, radio tactical satellite"     ), eTransmissions_RadioTacticalSatellite   ),
        T_ConverterCapacityMission( "Transmissions, radio teletype"               , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, radio teletype"               ), eTransmissions_RadioTeletype            ),
        T_ConverterCapacityMission( "Transmissions, support"                      , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, support"                      ), eTransmissions_Support                  ),
        T_ConverterCapacityMission( "Transmissions, radio"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Transmissions, radio"                        ), eTransmissions_Radio                    ),
        T_ConverterCapacityMission( "Transport, allocation des mouvements"        , QT_TRANSLATE_NOOP( "ADN_Tr", "Transport, allocation des mouvements"        ), eTransport_AllocationDesMouvements      ),
        T_ConverterCapacityMission( "Transport, APOD/APOE"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Transport, APOD/APOE"                        ), eTransport_APOD_APOE                    ),
        T_ConverterCapacityMission( "Transport, SPOD/SPOE"                        , QT_TRANSLATE_NOOP( "ADN_Tr", "Transport, SPOD/SPOE"                        ), eTransport_SPOD_SPOE                    ),
        T_ConverterCapacityMission( "Transport, missile"                          , QT_TRANSLATE_NOOP( "ADN_Tr", "Transport, missile"                          ), eTransportMissile                       ),
        T_ConverterCapacityMission( "Verrouillage de cible"                       , QT_TRANSLATE_NOOP( "ADN_Tr", "Verrouillage de cible"                       ), eVerrouillageDeCible                    ),
        T_ConverterCapacityMission( "Verrouillage de cible, flash"                , QT_TRANSLATE_NOOP( "ADN_Tr", "Verrouillage de cible, flash"                ), eVerrouillageDeCible_Flash              ),
        T_ConverterCapacityMission( "Verrouillage de cible, radar"                , QT_TRANSLATE_NOOP( "ADN_Tr", "Verrouillage de cible, radar"                ), eVerrouillageDeCible_Radar              ),
        T_ConverterCapacityMission( "Verrouillage de cible, sonore"               , QT_TRANSLATE_NOOP( "ADN_Tr", "Verrouillage de cible, sonore"               ), eVerrouillageDeCible_Sonore             ),
        T_ConverterCapacityMission( "", "", (E_CapacityMission)-1 )
};


ADN_Tr::T_ConverterRadarType ADN_Tr::radarTypeConverter_[] =
{
    T_ConverterRadarType( "radar",          QT_TRANSLATE_NOOP( "ADN_Tr", "radar" ),         eRadarType_Radar ),
        T_ConverterRadarType( "ecoute",         QT_TRANSLATE_NOOP( "ADN_Tr", "ecoute" ),        eRadarType_Ecoute ),
        T_ConverterRadarType( "ecoute radar",   QT_TRANSLATE_NOOP( "ADN_Tr", "ecoute radar" ),  eRadarType_EcouteRadar ),
        T_ConverterRadarType( "", "", (E_RadarType)-1 )
};

ADN_Tr::T_ConverterStockCategory ADN_Tr::stockCategoryConverter_[] =
{
    T_ConverterStockCategory( "Unite essence", QT_TRANSLATE_NOOP("ADN_Tr" ,"Unite essence"), eStockCategory_UniteFuel ),
    T_ConverterStockCategory( "Unite feu TD" , QT_TRANSLATE_NOOP("ADN_Tr" ,"Unite feu TD") , eStockCategory_UniteFireDirect ),
    T_ConverterStockCategory( "Unite feu -TD", QT_TRANSLATE_NOOP("ADN_Tr" ,"Unite feu -TD"), eStockCategory_UniteFireNotDirect ),
    T_ConverterStockCategory( "Unite vivre"  , QT_TRANSLATE_NOOP("ADN_Tr" ,"Unite vivre")  , eStockCategory_UniteSupply ),
    T_ConverterStockCategory( "Pieces"       , QT_TRANSLATE_NOOP("ADN_Tr" ,"Pieces")       , eStockCategory_Piece ),
    T_ConverterStockCategory( "",  "",   (E_StockCategory)-1 )
};

ADN_Tr::T_ConverterDeviceCategory ADN_Tr::deviceCategoryConverter_[] =
{
    T_ConverterDeviceCategory( "VCI"                    , QT_TRANSLATE_NOOP("ADN_Tr" ,"VCI")                    , eDeviceCategory_VCI ),
    T_ConverterDeviceCategory( "Char"                   , QT_TRANSLATE_NOOP("ADN_Tr" ,"Char")                   , eDeviceCategory_Char ),
    T_ConverterDeviceCategory( "Mortier"                , QT_TRANSLATE_NOOP("ADN_Tr" ,"Mortier")                , eDeviceCategory_Mortier ),
    T_ConverterDeviceCategory( "Artillerie"             , QT_TRANSLATE_NOOP("ADN_Tr" ,"Artillerie")             , eDeviceCategory_Artillerie ),
    T_ConverterDeviceCategory( "Hommes"                 , QT_TRANSLATE_NOOP("ADN_Tr" ,"Hommes")                 , eDeviceCategory_Hommes ),
    T_ConverterDeviceCategory( "Armes Légères"          , QT_TRANSLATE_NOOP("ADN_Tr" ,"Armes Légères")          , eDeviceCategory_ArmesLegeres ),
    T_ConverterDeviceCategory( "Matériel Franchissement", QT_TRANSLATE_NOOP("ADN_Tr" ,"Matériel Franchissement"), eDeviceCategory_MaterielFranchissement ),
    T_ConverterDeviceCategory( "Autres"                 , QT_TRANSLATE_NOOP("ADN_Tr" ,"Autres")                 , eDeviceCategory_Autres ),
    T_ConverterDeviceCategory( "",  "",   (E_DeviceCategory)-1 )
};

ADN_Tr::T_ConverterEquipmentState ADN_Tr::equipmentStateConverter_[] =
{
    T_ConverterEquipmentState( "ReparableAvecEvacuation", QT_TRANSLATE_NOOP("ADN_Tr", "ReparableAvecEvacuation" ), eEquipmentState_FixableWithEvac ),
    T_ConverterEquipmentState( "ReparableSurPlace"      , QT_TRANSLATE_NOOP("ADN_Tr", "ReparableSurPlace" )      , eEquipmentState_FixableInPlace ),
    T_ConverterEquipmentState( "Detruit"                , QT_TRANSLATE_NOOP("ADN_Tr", "Detruit" )                , eEquipmentState_Destroyed ),
    T_ConverterEquipmentState( "", "", (E_EquipmentState)-1 )
};

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromSMission
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromSMission( E_SMission nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( smissionConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromLocation
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromLocation( E_Location  nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( locationConverter_, nValue, nConversion );
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
// Name: ADN_Tr::ConvertFromMunitionType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromMunitionType( E_MunitionType nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( munitionTypeConverter_, nValue, nConversion );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromCapacityMission
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromCapacityMission( E_CapacityMission nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( capacityMissionConverter_, nValue, nConversion );
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
// Name: ADN_Tr::ConvertFromStockCategory
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromStockCategory( E_StockCategory nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( stockCategoryConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromDeviceCategory
// Created: SBO 2006-01-12
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromDeviceCategory( E_DeviceCategory nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( deviceCategoryConverter_, nValue, nConversion );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromEquipmentState
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromEquipmentState( E_EquipmentState nValue, E_Conversion nConversion )
{
    return ADN_Tr::InverseFindInConverter( equipmentStateConverter_, nValue, nConversion );
}

//-----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertFromTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ADN_Tr::ConvertFromTypeMunitionTirIndirect( E_TypeMunitionTirIndirect nValue, E_Conversion nConverterType )
{
    return ADN_Tr::InverseFindInConverter( TypeMunitionTirIndirectConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToSMission
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_SMission ADN_Tr::ConvertToSMission( const std::string& strName )
{
    return ADN_Tr::FindInConverter( smissionConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToLocation
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
E_Location ADN_Tr::ConvertToLocation( const std::string& strName )
{
    return ADN_Tr::FindInConverter( locationConverter_, strName );
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
// Name: ADN_Tr::ConvertToMunitionType
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_MunitionType ADN_Tr::ConvertToMunitionType( const std::string& strName )
{
    return ADN_Tr::FindInConverter( munitionTypeConverter_, strName );
}


// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToCapacityMission
// Created: APE 2005-03-07
// -----------------------------------------------------------------------------
E_CapacityMission ADN_Tr::ConvertToCapacityMission( const std::string& strName )
{
    return ADN_Tr::FindInConverter( capacityMissionConverter_, strName );
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
// Name: ADN_Tr::ConvertToStockCategory
// Created: SBO 2006-01-10
// -----------------------------------------------------------------------------
E_StockCategory ADN_Tr::ConvertToStockCategory( const std::string& strName )
{
    return ADN_Tr::FindInConverter( stockCategoryConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToStockCategory
// Created: SBO 2006-01-12
// -----------------------------------------------------------------------------
E_DeviceCategory ADN_Tr::ConvertToDeviceCategory( const std::string& strName )
{
    return ADN_Tr::FindInConverter( deviceCategoryConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToEquipmentState
// Created: SBO 2006-07-28
// -----------------------------------------------------------------------------
E_EquipmentState ADN_Tr::ConvertToEquipmentState( const std::string& strName )
{
    return ADN_Tr::FindInConverter( equipmentStateConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ADN_Tr::ConvertToTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeMunitionTirIndirect ADN_Tr::ConvertToTypeMunitionTirIndirect( const std::string& strName )
{
    return ADN_Tr::FindInConverter( TypeMunitionTirIndirectConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::InitTranslations
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
void ADN_Tr::InitTranslations()
{
    InitTr( smissionConverter_, "ADN_Tr" );
    InitTr( locationConverter_, "ADN_Tr" );
    InitTr( consumptionTypeConverter_, "ADN_Tr" );
    InitTr( timeCategoryConverter_, "ADN_Tr" );
    InitTr( visionObjectConverter_, "ADN_Tr" );
    InitTr( keyPointConverter_, "ADN_Tr" );
    InitTr( agentTypePionConverter_, "ADN_Tr" );
    InitTr( agentTypeAutomateConverter_, "ADN_Tr" );
    InitTr( natureAtlasTypeConverter_, "ADN_Tr" );
    InitTr( speedImpactConverter_, "ADN_Tr" );
    InitTr( sensorWeatherModifiersConverter_, "ADN_Tr" );
    InitTr( doctorSkillsConverter_, "ADN_Tr" );
    InitTr( protectionTypeConverter_, "ADN_Tr" );
    InitTr( breakdownTypeConverter_, "ADN_Tr" );
    InitTr( breakdownNTIConverter_, "ADN_Tr" );
    InitTr( munitionTypeConverter_, "ADN_Tr" );
    InitTr( capacityMissionConverter_, "ADN_Tr" );
    InitTr( radarTypeConverter_, "ADN_Tr" );
    InitTr( stockCategoryConverter_, "ADN_Tr" );
    InitTr( deviceCategoryConverter_, "ADN_Tr" );
    InitTr( equipmentStateConverter_, "ADN_Tr" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Tr::InitTranslations
// Created: APE 2005-02-18
// -----------------------------------------------------------------------------
ADN_Tr::T_ConverterTypeMunitionTirIndirect ADN_Tr::TypeMunitionTirIndirectConverter_[] =
{
    T_ConverterTypeMunitionTirIndirect( "explosif", QT_TRANSLATE_NOOP( "ENT_Tr", "explosif" ), eTypeMunitionTirIndirect_Explosif ),
    T_ConverterTypeMunitionTirIndirect( "grenade", QT_TRANSLATE_NOOP( "ENT_Tr", "grenade" ), eTypeMunitionTirIndirect_Grenade ),
    T_ConverterTypeMunitionTirIndirect( "aced", QT_TRANSLATE_NOOP( "ENT_Tr", "aced" ), eTypeMunitionTirIndirect_Aced ),
    T_ConverterTypeMunitionTirIndirect( "fumigene", QT_TRANSLATE_NOOP( "ENT_Tr", "fumigene" ), eTypeMunitionTirIndirect_Fumigene ),
    T_ConverterTypeMunitionTirIndirect( "eclairant", QT_TRANSLATE_NOOP( "ENT_Tr", "eclairant" ), eTypeMunitionTirIndirect_Eclairant ),
    T_ConverterTypeMunitionTirIndirect( "mine", QT_TRANSLATE_NOOP( "ENT_Tr", "mine" ), eTypeMunitionTirIndirect_Mine ),
    T_ConverterTypeMunitionTirIndirect( "", "", (E_TypeMunitionTirIndirect)-1 )
};