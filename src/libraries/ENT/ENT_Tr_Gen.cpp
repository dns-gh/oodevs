// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-11 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_ENT_Tr_Skeleton.cpp $
// $Author: Ape $
// $Modtime: 3/03/05 16:59 $
// $Revision: 3 $
// $Workfile: AGR_ENT_Tr_Skeleton.cpp $
//
// *****************************************************************************

#include "ENT_Tr_Gen.h"


ENT_Tr::T_ConverterTypeLocalisation ENT_Tr::TypeLocalisationConverter_[] =
{
    T_ConverterTypeLocalisation( "circle", QT_TRANSLATE_NOOP( "ENT_Tr", "circle" ), eTypeLocalisation_Circle ),
    T_ConverterTypeLocalisation( "ellipse", QT_TRANSLATE_NOOP( "ENT_Tr", "ellipse" ), eTypeLocalisation_Ellipse ),
    T_ConverterTypeLocalisation( "line", QT_TRANSLATE_NOOP( "ENT_Tr", "line" ), eTypeLocalisation_Line ),
    T_ConverterTypeLocalisation( "polygon", QT_TRANSLATE_NOOP( "ENT_Tr", "polygon" ), eTypeLocalisation_Polygon ),
    T_ConverterTypeLocalisation( "point", QT_TRANSLATE_NOOP( "ENT_Tr", "point" ), eTypeLocalisation_Point ),
    T_ConverterTypeLocalisation( "sector", QT_TRANSLATE_NOOP( "ENT_Tr", "sector" ), eTypeLocalisation_Sector ),
    T_ConverterTypeLocalisation( "", "", (E_TypeLocalisation)-1 )
};

ENT_Tr::T_ConverterFamilleDotation ENT_Tr::FamilleDotationConverter_[] =
{
    T_ConverterFamilleDotation( "munition", QT_TRANSLATE_NOOP( "ENT_Tr", "ammunition" ), eFamilleDotation_Munition ),
    T_ConverterFamilleDotation( "carburant", QT_TRANSLATE_NOOP( "ENT_Tr", "fuel" ), eFamilleDotation_Carburant ),
    T_ConverterFamilleDotation( "explosif", QT_TRANSLATE_NOOP( "ENT_Tr", "explosive" ), eFamilleDotation_Explosif ),
    T_ConverterFamilleDotation( "mine", QT_TRANSLATE_NOOP( "ENT_Tr", "mine" ), eFamilleDotation_Mine ),
    T_ConverterFamilleDotation( "barbele", QT_TRANSLATE_NOOP( "ENT_Tr", "barbed wire" ), eFamilleDotation_Barbele ),
    T_ConverterFamilleDotation( "piece", QT_TRANSLATE_NOOP( "ENT_Tr", "patch" ), eFamilleDotation_Piece ),
    T_ConverterFamilleDotation( "ration", QT_TRANSLATE_NOOP( "ENT_Tr", "daily food ration" ), eFamilleDotation_Ration ),
    T_ConverterFamilleDotation( "", "", (E_FamilleDotation)-1 )
};

ENT_Tr::T_ConverterFamilleMunition ENT_Tr::FamilleMunitionConverter_[] =
{
    T_ConverterFamilleMunition( "obus", QT_TRANSLATE_NOOP( "ENT_Tr", "shell" ), eFamilleMunition_Obus ),
    T_ConverterFamilleMunition( "missile air", QT_TRANSLATE_NOOP( "ENT_Tr", "air missile" ), eFamilleMunition_MissileAir ),
    T_ConverterFamilleMunition( "missile sol", QT_TRANSLATE_NOOP( "ENT_Tr", "groudn missile" ), eFamilleMunition_MissileSol ),
    T_ConverterFamilleMunition( "mitraille", QT_TRANSLATE_NOOP( "ENT_Tr", "hail of bullets" ), eFamilleMunition_Mitraille ),
    T_ConverterFamilleMunition( "", "", (E_FamilleMunition)-1 )
};

ENT_Tr::T_ConverterUnitIdentificationLevel ENT_Tr::UnitIdentificationLevelConverter_[] =
{
    T_ConverterUnitIdentificationLevel( "identifiee", QT_TRANSLATE_NOOP( "ENT_Tr", "identified" ), eUnitIdentificationLevel_Identifiee ),
    T_ConverterUnitIdentificationLevel( "reconnue", QT_TRANSLATE_NOOP( "ENT_Tr", "recognized" ), eUnitIdentificationLevel_Reconnue ),
    T_ConverterUnitIdentificationLevel( "detectee", QT_TRANSLATE_NOOP( "ENT_Tr", "detected" ), eUnitIdentificationLevel_Detectee ),
    T_ConverterUnitIdentificationLevel( "signale", QT_TRANSLATE_NOOP( "ENT_Tr", "reported" ), eUnitIdentificationLevel_Signale ),
    T_ConverterUnitIdentificationLevel( "", "", (E_UnitIdentificationLevel)-1 )
};

ENT_Tr::T_ConverterNatureLevel ENT_Tr::NatureLevelConverter_[] =
{
    T_ConverterNatureLevel( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eNatureLevel_None ),
    T_ConverterNatureLevel( "o", QT_TRANSLATE_NOOP( "ENT_Tr", "o" ), eNatureLevel_o ),
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

ENT_Tr::T_ConverterHumanWound ENT_Tr::HumanWoundConverter_[] =
{
    T_ConverterHumanWound( "non blesse", QT_TRANSLATE_NOOP( "ENT_Tr", "uninjured" ), eHumanWound_NonBlesse ),
    T_ConverterHumanWound( "mort", QT_TRANSLATE_NOOP( "ENT_Tr", "dead" ), eHumanWound_Mort ),
    T_ConverterHumanWound( "blesse urgence 1", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded level 1" ), eHumanWound_BlesseUrgence1 ),
    T_ConverterHumanWound( "blesse urgence 2", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded level 2" ), eHumanWound_BlesseUrgence2 ),
    T_ConverterHumanWound( "blesse urgence 3", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded level 3" ), eHumanWound_BlesseUrgence3 ),
    T_ConverterHumanWound( "blesse urgence extreme", QT_TRANSLATE_NOOP( "ENT_Tr", "seriously wounded" ), eHumanWound_BlesseUrgenceExtreme ),
    T_ConverterHumanWound( "", "", (E_HumanWound)-1 )
};

ENT_Tr::T_ConverterHumanRank ENT_Tr::HumanRankConverter_[] =
{
    T_ConverterHumanRank( "officier", QT_TRANSLATE_NOOP( "ENT_Tr", "officer" ), eHumanRank_Officier ),
    T_ConverterHumanRank( "sous officer", QT_TRANSLATE_NOOP( "ENT_Tr", "warrant officer" ), eHumanRank_SousOfficer ),
    T_ConverterHumanRank( "mdr", QT_TRANSLATE_NOOP( "ENT_Tr", "private" ), eHumanRank_Mdr ),
    T_ConverterHumanRank( "", "", (E_HumanRank)-1 )
};

ENT_Tr::T_ConverterDiplomatie ENT_Tr::DiplomatieConverter_[] =
{
    T_ConverterDiplomatie( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "unknown" ), eDiplomatie_Inconnu ),
    T_ConverterDiplomatie( "ami", QT_TRANSLATE_NOOP( "ENT_Tr", "friendly" ), eDiplomatie_Ami ),
    T_ConverterDiplomatie( "ennemi", QT_TRANSLATE_NOOP( "ENT_Tr", "enemy" ), eDiplomatie_Ennemi ),
    T_ConverterDiplomatie( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutral" ), eDiplomatie_Neutre ),
    T_ConverterDiplomatie( "", "", (E_Diplomatie)-1 )
};

ENT_Tr::T_ConverterChangeDiplomacyErrorCode ENT_Tr::ChangeDiplomacyErrorCodeConverter_[] =
{
    T_ConverterChangeDiplomacyErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeDiplomacyErrorCode_NoError ),
    T_ConverterChangeDiplomacyErrorCode( "error invalid camp", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid camp" ), eChangeDiplomacyErrorCode_ErrorInvalidCamp ),
    T_ConverterChangeDiplomacyErrorCode( "", "", (E_ChangeDiplomacyErrorCode)-1 )
};

ENT_Tr::T_ConverterEtatRapFor ENT_Tr::EtatRapForConverter_[] =
{
    T_ConverterEtatRapFor( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eEtatRapFor_None ),
    T_ConverterEtatRapFor( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutral" ), eEtatRapFor_Neutre ),
    T_ConverterEtatRapFor( "favorable", QT_TRANSLATE_NOOP( "ENT_Tr", "favorable" ), eEtatRapFor_Favorable ),
    T_ConverterEtatRapFor( "defavorable", QT_TRANSLATE_NOOP( "ENT_Tr", "defavorable" ), eEtatRapFor_Defavorable ),
    T_ConverterEtatRapFor( "", "", (E_EtatRapFor)-1 )
};

ENT_Tr::T_ConverterTypeObstacle ENT_Tr::TypeObstacleForConverter_[] =
{
    T_ConverterTypeObstacle( "initial", QT_TRANSLATE_NOOP( "ENT_Tr", "initial" ), eTypeObstacle_Preliminaire ),
    T_ConverterTypeObstacle( "reserved", QT_TRANSLATE_NOOP( "ENT_Tr", "reserved" ), eTypeObstacle_DeManoeuvre ),
    T_ConverterTypeObstacle( "", "", (E_TypeObstacle)-1 )
};

ENT_Tr::T_ConverterEtatCombatRencontre ENT_Tr::EtatCombatRencontreConverter_[] =
{
    T_ConverterEtatCombatRencontre( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eEtatCombatRencontre_None ),
    T_ConverterEtatCombatRencontre( "etat esquive", QT_TRANSLATE_NOOP( "ENT_Tr", "withdrawing" ), eEtatCombatRencontre_EtatEsquive ),
    T_ConverterEtatCombatRencontre( "etat fixe", QT_TRANSLATE_NOOP( "ENT_Tr", "fixing" ), eEtatCombatRencontre_EtatFixe ),
    T_ConverterEtatCombatRencontre( "etat poursuite mission", QT_TRANSLATE_NOOP( "ENT_Tr", "nothing to report" ), eEtatCombatRencontre_EtatPoursuiteMission ),
    T_ConverterEtatCombatRencontre( "", "", (E_EtatCombatRencontre)-1 )
};

ENT_Tr::T_ConverterEtatOperationnel ENT_Tr::EtatOperationnelConverter_[] =
{
    T_ConverterEtatOperationnel( "detruit totalement", QT_TRANSLATE_NOOP( "ENT_Tr", "totally destroyed" ), eEtatOperationnel_DetruitTotalement ),
    T_ConverterEtatOperationnel( "detruit tactiquement", QT_TRANSLATE_NOOP( "ENT_Tr", "non operational" ), eEtatOperationnel_DetruitTactiquement ),
    T_ConverterEtatOperationnel( "operationnel", QT_TRANSLATE_NOOP( "ENT_Tr", "operational" ), eEtatOperationnel_Operationnel ),
    T_ConverterEtatOperationnel( "", "", (E_EtatOperationnel)-1 )
};

ENT_Tr::T_ConverterRoe ENT_Tr::RoeConverter_[] =
{
    T_ConverterRoe( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoe_None ),
    T_ConverterRoe( "tir libre", QT_TRANSLATE_NOOP( "ENT_Tr", "free fire" ), eRoe_TirLibre ),
    T_ConverterRoe( "tir sur riposte", QT_TRANSLATE_NOOP( "ENT_Tr", "fire back only" ), eRoe_TirSurRiposte ),
    T_ConverterRoe( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "fire upon order only" ), eRoe_TirInterdit ),
    T_ConverterRoe( "", "", (E_Roe)-1 )
};

ENT_Tr::T_ConverterUnitAttrErrorCode ENT_Tr::UnitAttrErrorCodeConverter_[] =
{
    T_ConverterUnitAttrErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eUnitAttrErrorCode_NoError ),
    T_ConverterUnitAttrErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), eUnitAttrErrorCode_ErrorInvalidUnit ),
    T_ConverterUnitAttrErrorCode( "error automate embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "error automate embraye" ), eUnitAttrErrorCode_ErrorAutomateEmbraye ),
    T_ConverterUnitAttrErrorCode( "error invalid attribute", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid attribute" ), eUnitAttrErrorCode_ErrorInvalidAttribute ),
    T_ConverterUnitAttrErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eUnitAttrErrorCode_ErrorUnitSurrendered ),
    T_ConverterUnitAttrErrorCode( "", "", (E_UnitAttrErrorCode)-1 )
};

ENT_Tr::T_ConverterUnitPosture ENT_Tr::UnitPostureConverter_[] =
{
    T_ConverterUnitPosture( "posture mouvement", QT_TRANSLATE_NOOP( "ENT_Tr", "standard posture" ), eUnitPosture_PostureMouvement ),
    T_ConverterUnitPosture( "posture mouvement discret", QT_TRANSLATE_NOOP( "ENT_Tr", "infiltration posture" ), eUnitPosture_PostureMouvementDiscret ),
    T_ConverterUnitPosture( "posture arret", QT_TRANSLATE_NOOP( "ENT_Tr", "stopping posture" ), eUnitPosture_PostureArret ),
    T_ConverterUnitPosture( "posture poste reflexe", QT_TRANSLATE_NOOP( "ENT_Tr", "short stopping posture" ), eUnitPosture_PosturePosteReflexe ),
    T_ConverterUnitPosture( "posture poste", QT_TRANSLATE_NOOP( "ENT_Tr", "quick deployment posture" ), eUnitPosture_PosturePoste ),
    T_ConverterUnitPosture( "posture poste amenage", QT_TRANSLATE_NOOP( "ENT_Tr", "deployment posture" ), eUnitPosture_PosturePosteAmenage ),
    T_ConverterUnitPosture( "posture poste prepare genie", QT_TRANSLATE_NOOP( "ENT_Tr", "engineer enhanced deployment posture" ), eUnitPosture_PosturePostePrepareGenie ),
    T_ConverterUnitPosture( "", "", (E_UnitPosture)-1 )
};

ENT_Tr::T_ConverterDisponibiliteAuTir ENT_Tr::DisponibiliteAuTirConverter_[] =
{
    T_ConverterDisponibiliteAuTir( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eDisponibiliteAuTir_None ),
    T_ConverterDisponibiliteAuTir( "pret au tir", QT_TRANSLATE_NOOP( "ENT_Tr", "ready to fire" ), eDisponibiliteAuTir_PretAuTir ),
    T_ConverterDisponibiliteAuTir( "indisponible", QT_TRANSLATE_NOOP( "ENT_Tr", "unavailable for fire" ), eDisponibiliteAuTir_Indisponible ),
    T_ConverterDisponibiliteAuTir( "", "", (E_DisponibiliteAuTir)-1 )
};

ENT_Tr::T_ConverterRoePopulation ENT_Tr::RoePopulationConverter_[] =
{
    T_ConverterRoePopulation( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoePopulation_None ),
    T_ConverterRoePopulation( "emploi force interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "use of force forbidden" ), eRoePopulation_EmploiForceInterdit ),
    T_ConverterRoePopulation( "maintien a distance par moyens non letaux", QT_TRANSLATE_NOOP( "ENT_Tr", "holding civilians in position by usage of non lethal means" ), eRoePopulation_MaintienADistanceParMoyensNonLetaux ),
    T_ConverterRoePopulation( "dispersion par moyens de defense actifs", QT_TRANSLATE_NOOP( "ENT_Tr", "dispersing civilians by active defense means" ), eRoePopulation_DispersionParMoyensDeDefenseActifs ),
    T_ConverterRoePopulation( "armes letales autorisees", QT_TRANSLATE_NOOP( "ENT_Tr", "lethal weapons authorized for use" ), eRoePopulation_ArmesLetalesAutorisees ),
    T_ConverterRoePopulation( "", "", (E_RoePopulation)-1 )
};

ENT_Tr::T_ConverterUnitFatigue ENT_Tr::UnitFatigueConverter_[] =
{
    T_ConverterUnitFatigue( "normal", QT_TRANSLATE_NOOP( "ENT_Tr", "not tired" ), eUnitFatigue_Normal ),
    T_ConverterUnitFatigue( "fatigue", QT_TRANSLATE_NOOP( "ENT_Tr", "tired" ), eUnitFatigue_Fatigue ),
    T_ConverterUnitFatigue( "epuise", QT_TRANSLATE_NOOP( "ENT_Tr", "exhausted" ), eUnitFatigue_Epuise ),
    T_ConverterUnitFatigue( "", "", (E_UnitFatigue)-1 )
};

ENT_Tr::T_ConverterUnitMoral ENT_Tr::UnitMoralConverter_[] =
{
    T_ConverterUnitMoral( "fanatique", QT_TRANSLATE_NOOP( "ENT_Tr", "fanatic" ), eUnitMoral_Fanatique ),
    T_ConverterUnitMoral( "bon", QT_TRANSLATE_NOOP( "ENT_Tr", "good" ), eUnitMoral_Bon ),
    T_ConverterUnitMoral( "moyen", QT_TRANSLATE_NOOP( "ENT_Tr", "regular" ), eUnitMoral_Moyen ),
    T_ConverterUnitMoral( "mauvais", QT_TRANSLATE_NOOP( "ENT_Tr", "bad" ), eUnitMoral_Mauvais ),
    T_ConverterUnitMoral( "", "", (E_UnitMoral)-1 )
};

ENT_Tr::T_ConverterUnitExperience ENT_Tr::UnitExperienceConverter_[] =
{
    T_ConverterUnitExperience( "veteran", QT_TRANSLATE_NOOP( "ENT_Tr", "veteran" ), eUnitExperience_Veteran ),
    T_ConverterUnitExperience( "experimente", QT_TRANSLATE_NOOP( "ENT_Tr", "experienced" ), eUnitExperience_Experimente ),
    T_ConverterUnitExperience( "conscrit", QT_TRANSLATE_NOOP( "ENT_Tr", "rookie" ), eUnitExperience_Conscrit ),
    T_ConverterUnitExperience( "", "", (E_UnitExperience)-1 )
};

ENT_Tr::T_ConverterAutomateState ENT_Tr::AutomateStateConverter_[] =
{
    T_ConverterAutomateState( "embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "engaged" ), eAutomateState_Embraye ),
    T_ConverterAutomateState( "debraye", QT_TRANSLATE_NOOP( "ENT_Tr", "disengaged" ), eAutomateState_Debraye ),
    T_ConverterAutomateState( "", "", (E_AutomateState)-1 )
};

ENT_Tr::T_ConverterSetAutomateModeErrorCode ENT_Tr::SetAutomateModeErrorCodeConverter_[] =
{
    T_ConverterSetAutomateModeErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eSetAutomateModeErrorCode_NoError ),
    T_ConverterSetAutomateModeErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), eSetAutomateModeErrorCode_ErrorInvalidUnit ),
    T_ConverterSetAutomateModeErrorCode( "error unit not automate", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit not automate" ), eSetAutomateModeErrorCode_ErrorUnitNotAutomate ),
    T_ConverterSetAutomateModeErrorCode( "error already embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "error already embraye" ), eSetAutomateModeErrorCode_ErrorAlreadyEmbraye ),
    T_ConverterSetAutomateModeErrorCode( "error already debraye", QT_TRANSLATE_NOOP( "ENT_Tr", "error already debraye" ), eSetAutomateModeErrorCode_ErrorAlreadyDebraye ),
    T_ConverterSetAutomateModeErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eSetAutomateModeErrorCode_ErrorUnitSurrendered ),
    T_ConverterSetAutomateModeErrorCode( "", "", (E_SetAutomateModeErrorCode)-1 )
};

ENT_Tr::T_ConverterCtrlErrorCode ENT_Tr::CtrlErrorCodeConverter_[] =
{
    T_ConverterCtrlErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eCtrlErrorCode_NoError ),
    T_ConverterCtrlErrorCode( "error already started", QT_TRANSLATE_NOOP( "ENT_Tr", "error already started" ), eCtrlErrorCode_ErrorAlreadyStarted ),
    T_ConverterCtrlErrorCode( "error not started", QT_TRANSLATE_NOOP( "ENT_Tr", "error not started" ), eCtrlErrorCode_ErrorNotStarted ),
    T_ConverterCtrlErrorCode( "error not paused", QT_TRANSLATE_NOOP( "ENT_Tr", "error not paused" ), eCtrlErrorCode_ErrorNotPaused ),
    T_ConverterCtrlErrorCode( "error already paused", QT_TRANSLATE_NOOP( "ENT_Tr", "error already paused" ), eCtrlErrorCode_ErrorAlreadyPaused ),
    T_ConverterCtrlErrorCode( "error invalid time factor", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid time factor" ), eCtrlErrorCode_ErrorInvalidTimeFactor ),
    T_ConverterCtrlErrorCode( "", "", (E_CtrlErrorCode)-1 )
};

ENT_Tr::T_ConverterEtatSim ENT_Tr::EtatSimConverter_[] =
{
    T_ConverterEtatSim( "running", QT_TRANSLATE_NOOP( "ENT_Tr", "running" ), eEtatSim_Running ),
    T_ConverterEtatSim( "paused", QT_TRANSLATE_NOOP( "ENT_Tr", "paused" ), eEtatSim_Paused ),
    T_ConverterEtatSim( "stopped", QT_TRANSLATE_NOOP( "ENT_Tr", "stopped" ), eEtatSim_Stopped ),
    T_ConverterEtatSim( "", "", (E_EtatSim)-1 )
};

ENT_Tr::T_ConverterPrecipitationType ENT_Tr::PrecipitationTypeConverter_[] =
{
    T_ConverterPrecipitationType( "pas de precipitation", QT_TRANSLATE_NOOP( "ENT_Tr", "no rainfall" ), ePrecipitationType_PasDePrecipitation ),
    T_ConverterPrecipitationType( "tempete sable", QT_TRANSLATE_NOOP( "ENT_Tr", "sandstorm" ), ePrecipitationType_TempeteSable ),
    T_ConverterPrecipitationType( "brouillard", QT_TRANSLATE_NOOP( "ENT_Tr", "fog" ), ePrecipitationType_Brouillard ),
    T_ConverterPrecipitationType( "crachin", QT_TRANSLATE_NOOP( "ENT_Tr", "drizzle" ), ePrecipitationType_Crachin ),
    T_ConverterPrecipitationType( "pluie", QT_TRANSLATE_NOOP( "ENT_Tr", "rain" ), ePrecipitationType_Pluie ),
    T_ConverterPrecipitationType( "neige", QT_TRANSLATE_NOOP( "ENT_Tr", "snow" ), ePrecipitationType_Neige ),
    T_ConverterPrecipitationType( "", "", (E_PrecipitationType)-1 )
};

ENT_Tr::T_ConverterTypeLima ENT_Tr::TypeLimaConverter_[] =
{
    T_ConverterTypeLima( "ligne debouche", QT_TRANSLATE_NOOP( "ENT_Tr", "departure line" ), eTypeLima_LigneDebouche ),
    T_ConverterTypeLima( "ligne changement attitude", QT_TRANSLATE_NOOP( "ENT_Tr", "change of posture line" ), eTypeLima_LigneChangementAttitude ),
    T_ConverterTypeLima( "ligne coordination", QT_TRANSLATE_NOOP( "ENT_Tr", "coordination line" ), eTypeLima_LigneCoordination ),
    T_ConverterTypeLima( "ligne interdire", QT_TRANSLATE_NOOP( "ENT_Tr", "deny line" ), eTypeLima_LigneInterdire ),
    T_ConverterTypeLima( "ligne objectif", QT_TRANSLATE_NOOP( "ENT_Tr", "objective line" ), eTypeLima_LigneObjectif ),
    T_ConverterTypeLima( "ligne coup arret", QT_TRANSLATE_NOOP( "ENT_Tr", "blocking line" ), eTypeLima_LigneCoupArret ),
    T_ConverterTypeLima( "ligne recueil", QT_TRANSLATE_NOOP( "ENT_Tr", "rearward passage line" ), eTypeLima_LigneRecueil ),
    T_ConverterTypeLima( "ligne debut mission", QT_TRANSLATE_NOOP( "ENT_Tr", "mission: start line" ), eTypeLima_LigneDebutMission ),
    T_ConverterTypeLima( "ligne fin mission", QT_TRANSLATE_NOOP( "ENT_Tr", "mission: complete line" ), eTypeLima_LigneFinMission ),
    T_ConverterTypeLima( "ligne identification accueil", QT_TRANSLATE_NOOP( "ENT_Tr", "checkpoint line" ), eTypeLima_LigneIdentificationAccueil ),
    T_ConverterTypeLima( "", "", (E_TypeLima)-1 )
};

ENT_Tr::T_ConverterInfoContextErrorCode ENT_Tr::InfoContextErrorCodeConverter_[] =
{
    T_ConverterInfoContextErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eInfoContextErrorCode_NoError ),
    T_ConverterInfoContextErrorCode( "error invalid id", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid id" ), eInfoContextErrorCode_ErrorInvalidId ),
    T_ConverterInfoContextErrorCode( "error invalid geometry", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid geometry" ), eInfoContextErrorCode_ErrorInvalidGeometry ),
    T_ConverterInfoContextErrorCode( "error invalid lima function", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid lima function" ), eInfoContextErrorCode_ErrorInvalidLimaFunction ),
    T_ConverterInfoContextErrorCode( "error invalid diffusion", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid diffusion" ), eInfoContextErrorCode_ErrorInvalidDiffusion ),
    T_ConverterInfoContextErrorCode( "", "", (E_InfoContextErrorCode)-1 )
};

ENT_Tr::T_ConverterPopulationAttitude ENT_Tr::PopulationAttitudeConverter_[] =
{
    T_ConverterPopulationAttitude( "calme", QT_TRANSLATE_NOOP( "ENT_Tr", "quiet" ), ePopulationAttitude_Calme ),
    T_ConverterPopulationAttitude( "agitee", QT_TRANSLATE_NOOP( "ENT_Tr", "turbulent" ), ePopulationAttitude_Agitee ),
    T_ConverterPopulationAttitude( "excitee", QT_TRANSLATE_NOOP( "ENT_Tr", "agitated" ), ePopulationAttitude_Excitee ),
    T_ConverterPopulationAttitude( "agressive", QT_TRANSLATE_NOOP( "ENT_Tr", "agressive" ), ePopulationAttitude_Agressive ),
    T_ConverterPopulationAttitude( "", "", (E_PopulationAttitude)-1 )
};

ENT_Tr::T_ConverterPopulationAttrErrorCode ENT_Tr::PopulationAttrErrorCodeConverter_[] =
{
    T_ConverterPopulationAttrErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), ePopulationAttrErrorCode_NoError ),
    T_ConverterPopulationAttrErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), ePopulationAttrErrorCode_ErrorInvalidUnit ),
    T_ConverterPopulationAttrErrorCode( "error invalid attribute", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid attribute" ), ePopulationAttrErrorCode_ErrorInvalidAttribute ),
    T_ConverterPopulationAttrErrorCode( "", "", (E_PopulationAttrErrorCode)-1 )
};

ENT_Tr::T_ConverterTypeCR ENT_Tr::TypeCRConverter_[] =
{
    T_ConverterTypeCR( "message", QT_TRANSLATE_NOOP( "ENT_Tr", "message" ), eTypeCR_Message ),
    T_ConverterTypeCR( "operationel", QT_TRANSLATE_NOOP( "ENT_Tr", "operational" ), eTypeCR_Operationel ),
    T_ConverterTypeCR( "evenement exceptionnel", QT_TRANSLATE_NOOP( "ENT_Tr", "exceptional event" ), eTypeCR_EvenementExceptionnel ),
    T_ConverterTypeCR( "warning", QT_TRANSLATE_NOOP( "ENT_Tr", "warning" ), eTypeCR_Warning ),
    T_ConverterTypeCR( "", "", (E_TypeCR)-1 )
};

ENT_Tr::T_ConverterFireEffectType ENT_Tr::FireEffectTypeConverter_[] =
{
    T_ConverterFireEffectType( "fumigene", QT_TRANSLATE_NOOP( "ENT_Tr", "smoke" ), eFireEffectType_Fumigene ),
    T_ConverterFireEffectType( "eclairant", QT_TRANSLATE_NOOP( "ENT_Tr", "lightning" ), eFireEffectType_Eclairant ),
    T_ConverterFireEffectType( "", "", (E_FireEffectType)-1 )
};

ENT_Tr::T_ConverterLogMaintenanceRegimeTravail ENT_Tr::LogMaintenanceRegimeTravailConverter_[] =
{
    T_ConverterLogMaintenanceRegimeTravail( "regime 1", QT_TRANSLATE_NOOP( "ENT_Tr", "work mode 1" ), eLogMaintenanceRegimeTravail_Regime1 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 2", QT_TRANSLATE_NOOP( "ENT_Tr", "work mode 2" ), eLogMaintenanceRegimeTravail_Regime2 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 3", QT_TRANSLATE_NOOP( "ENT_Tr", "work mode 3" ), eLogMaintenanceRegimeTravail_Regime3 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 4", QT_TRANSLATE_NOOP( "ENT_Tr", "work mode 4" ), eLogMaintenanceRegimeTravail_Regime4 ),
    T_ConverterLogMaintenanceRegimeTravail( "", "", (E_LogMaintenanceRegimeTravail)-1 )
};

ENT_Tr::T_ConverterLogMaintenanceTraitementEtat ENT_Tr::LogMaintenanceTraitementEtatConverter_[] =
{
    T_ConverterLogMaintenanceTraitementEtat( "deplacement vers chaine", QT_TRANSLATE_NOOP( "ENT_Tr", "moving toward logistic system" ), eLogMaintenanceTraitementEtat_DeplacementVersChaine ),
    T_ConverterLogMaintenanceTraitementEtat( "attente disponibilite remorqueur", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for available tow-truck" ), eLogMaintenanceTraitementEtat_AttenteDisponibiliteRemorqueur ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck moving to target" ), eLogMaintenanceTraitementEtat_RemorqueurDeplacementAller ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck loading" ), eLogMaintenanceTraitementEtat_RemorqueurChargement ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck moving from target" ), eLogMaintenanceTraitementEtat_RemorqueurDeplacementRetour ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck unloading" ), eLogMaintenanceTraitementEtat_RemorqueurDechargement ),
    T_ConverterLogMaintenanceTraitementEtat( "diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "diagnostic" ), eLogMaintenanceTraitementEtat_Diagnostique ),
    T_ConverterLogMaintenanceTraitementEtat( "attente prise en charge par niveau superieur", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting to be handled by higher level" ), eLogMaintenanceTraitementEtat_AttentePriseEnChargeParNiveauSuperieur ),
    T_ConverterLogMaintenanceTraitementEtat( "attente disponibilite pieces", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for available parts" ), eLogMaintenanceTraitementEtat_AttenteDisponibilitePieces ),
    T_ConverterLogMaintenanceTraitementEtat( "attente disponibilite reparateur", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for available repairer" ), eLogMaintenanceTraitementEtat_AttenteDisponibiliteReparateur ),
    T_ConverterLogMaintenanceTraitementEtat( "reparation", QT_TRANSLATE_NOOP( "ENT_Tr", "repairation" ), eLogMaintenanceTraitementEtat_Reparation ),
    T_ConverterLogMaintenanceTraitementEtat( "retour pion", QT_TRANSLATE_NOOP( "ENT_Tr", "unit returning" ), eLogMaintenanceTraitementEtat_RetourPion ),
    T_ConverterLogMaintenanceTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogMaintenanceTraitementEtat_Termine ),
    T_ConverterLogMaintenanceTraitementEtat( "", "", (E_LogMaintenanceTraitementEtat)-1 )
};

ENT_Tr::T_ConverterLogRavitaillementTraitementEtat ENT_Tr::LogRavitaillementTraitementEtatConverter_[] =
{
    T_ConverterLogRavitaillementTraitementEtat( "convoi en attente camions", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy waiting for trucks" ), eLogRavitaillementTraitementEtat_ConvoiEnAttenteCamions ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi constitution", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy setting up" ), eLogRavitaillementTraitementEtat_ConvoiConstitution ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi deplacement vers point chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy moving to loading point" ), eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointChargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy loading" ), eLogRavitaillementTraitementEtat_ConvoiChargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi deplacement vers point dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy moving to unloading point" ), eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointDechargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy unloading" ), eLogRavitaillementTraitementEtat_ConvoiDechargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy moving back" ), eLogRavitaillementTraitementEtat_ConvoiDeplacementRetour ),
    T_ConverterLogRavitaillementTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogRavitaillementTraitementEtat_Termine ),
    T_ConverterLogRavitaillementTraitementEtat( "", "", (E_LogRavitaillementTraitementEtat)-1 )
};

ENT_Tr::T_ConverterMsgLogSupplyChangeQuotasAck ENT_Tr::MsgLogSupplyChangeQuotasAckConverter_[] =
{
    T_ConverterMsgLogSupplyChangeQuotasAck( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eMsgLogSupplyChangeQuotasAck_NoError ),
    T_ConverterMsgLogSupplyChangeQuotasAck( "error invalid donneur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid supplier" ), eMsgLogSupplyChangeQuotasAck_ErrorInvalidDonneur ),
    T_ConverterMsgLogSupplyChangeQuotasAck( "error invalid receveur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid receiver" ), eMsgLogSupplyChangeQuotasAck_ErrorInvalidReceveur ),
    T_ConverterMsgLogSupplyChangeQuotasAck( "", "", (E_MsgLogSupplyChangeQuotasAck)-1 )
};

ENT_Tr::T_ConverterMsgLogSupplyPushFlowAck ENT_Tr::MsgLogSupplyPushFlowAckConverter_[] =
{
    T_ConverterMsgLogSupplyPushFlowAck( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eMsgLogSupplyPushFlowAck_NoError ),
    T_ConverterMsgLogSupplyPushFlowAck( "error invalid donneur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid supplier" ), eMsgLogSupplyPushFlowAck_ErrorInvalidDonneur ),
    T_ConverterMsgLogSupplyPushFlowAck( "error invalid receveur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid receiver" ), eMsgLogSupplyPushFlowAck_ErrorInvalidReceveur ),
    T_ConverterMsgLogSupplyPushFlowAck( "", "", (E_MsgLogSupplyPushFlowAck)-1 )
};

ENT_Tr::T_ConverterLogSanteTraitementEtat ENT_Tr::LogSanteTraitementEtatConverter_[] =
{
    T_ConverterLogSanteTraitementEtat( "attente disponibilite ambulance releve", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for evacuation ambulance" ), eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceReleve ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance coming" ), eLogSanteTraitementEtat_AmbulanceReleveDeplacementAller ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance loading" ), eLogSanteTraitementEtat_AmbulanceReleveChargement ),
    T_ConverterLogSanteTraitementEtat( "attente chargement complet ambulance releve", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for evacuation ambulance loading" ), eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceReleve ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance going back" ), eLogSanteTraitementEtat_AmbulanceReleveDeplacementRetour ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance unloading" ), eLogSanteTraitementEtat_AmbulanceReleveDechargement ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite medecin pour diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for doctor" ), eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourDiagnostique ),
    T_ConverterLogSanteTraitementEtat( "diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "diagnostic" ), eLogSanteTraitementEtat_Diagnostique ),
    T_ConverterLogSanteTraitementEtat( "recherche secteur tri", QT_TRANSLATE_NOOP( "ENT_Tr", "looking for sorting area" ), eLogSanteTraitementEtat_RechercheSecteurTri ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite medecin pour tri", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for sorting doctor" ), eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourTri ),
    T_ConverterLogSanteTraitementEtat( "tri", QT_TRANSLATE_NOOP( "ENT_Tr", "sorting" ), eLogSanteTraitementEtat_Tri ),
    T_ConverterLogSanteTraitementEtat( "recherche secteur soin", QT_TRANSLATE_NOOP( "ENT_Tr", "looking for treatment area" ), eLogSanteTraitementEtat_RechercheSecteurSoin ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite medecin pour soin", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for doctor (treatment)" ), eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourSoin ),
    T_ConverterLogSanteTraitementEtat( "soin", QT_TRANSLATE_NOOP( "ENT_Tr", "treatment" ), eLogSanteTraitementEtat_Soin ),
    T_ConverterLogSanteTraitementEtat( "hospitalisation", QT_TRANSLATE_NOOP( "ENT_Tr", "hospitalization" ), eLogSanteTraitementEtat_Hospitalisation ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for collection ambulance" ), eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceRamassage ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "collection ambulance loading" ), eLogSanteTraitementEtat_AmbulanceRamassageChargement ),
    T_ConverterLogSanteTraitementEtat( "attente chargement complet ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for collection ambulance loading" ), eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceRamassage ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "collection ambulance coming" ), eLogSanteTraitementEtat_AmbulanceRamassageDeplacementAller ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "collection ambulance unloading" ), eLogSanteTraitementEtat_AmbulanceRamassageDechargement ),
    T_ConverterLogSanteTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogSanteTraitementEtat_Termine ),
    T_ConverterLogSanteTraitementEtat( "", "", (E_LogSanteTraitementEtat)-1 )
};

ENT_Tr::T_ConverterObjectErrorCode ENT_Tr::ObjectErrorCodeConverter_[] =
{
    T_ConverterObjectErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eObjectErrorCode_NoError ),
    T_ConverterObjectErrorCode( "error invalid object", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid object" ), eObjectErrorCode_ErrorInvalidObject ),
    T_ConverterObjectErrorCode( "error invalid id", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid id" ), eObjectErrorCode_ErrorInvalidId ),
    T_ConverterObjectErrorCode( "error invalid camp", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid camp" ), eObjectErrorCode_ErrorInvalidCamp ),
    T_ConverterObjectErrorCode( "error invalid localisation", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid localisation" ), eObjectErrorCode_ErrorInvalidLocalisation ),
    T_ConverterObjectErrorCode( "error missing specific attributes", QT_TRANSLATE_NOOP( "ENT_Tr", "error missing specific attributes" ), eObjectErrorCode_ErrorMissingSpecificAttributes ),
    T_ConverterObjectErrorCode( "error invalid specific attributes", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid specific attributes" ), eObjectErrorCode_ErrorInvalidSpecificAttributes ),
    T_ConverterObjectErrorCode( "", "", (E_ObjectErrorCode)-1 )
};

ENT_Tr::T_ConverterObjectType ENT_Tr::ObjectTypeConverter_[] =
{
    T_ConverterObjectType( "bouchon mines", QT_TRANSLATE_NOOP( "ENT_Tr", "minejam" ), eObjectType_BouchonMines ),
    T_ConverterObjectType( "zone minee lineaire", QT_TRANSLATE_NOOP( "ENT_Tr", "linear mined area" ), eObjectType_ZoneMineeLineaire ),
    T_ConverterObjectType( "zone minee par dispersion", QT_TRANSLATE_NOOP( "ENT_Tr", "mined area (dispersed)" ), eObjectType_ZoneMineeParDispersion ),
    T_ConverterObjectType( "fosse anti char", QT_TRANSLATE_NOOP( "ENT_Tr", "anti tank obstacle" ), eObjectType_FosseAntiChar ),
    T_ConverterObjectType( "abattis", QT_TRANSLATE_NOOP( "ENT_Tr", "abatis" ), eObjectType_Abattis ),
    T_ConverterObjectType( "barricade", QT_TRANSLATE_NOOP( "ENT_Tr", "barricade" ), eObjectType_Barricade ),
    T_ConverterObjectType( "eboulement", QT_TRANSLATE_NOOP( "ENT_Tr", "fall" ), eObjectType_Eboulement ),
    T_ConverterObjectType( "destruction route", QT_TRANSLATE_NOOP( "ENT_Tr", "road destruction" ), eObjectType_DestructionRoute ),
    T_ConverterObjectType( "destruction pont", QT_TRANSLATE_NOOP( "ENT_Tr", "bridge destruction" ), eObjectType_DestructionPont ),
    T_ConverterObjectType( "pont flottant continu", QT_TRANSLATE_NOOP( "ENT_Tr", "continuous bridge" ), eObjectType_PontFlottantContinu ),
    T_ConverterObjectType( "poste tir", QT_TRANSLATE_NOOP( "ENT_Tr", "firing position" ), eObjectType_PosteTir ),
    T_ConverterObjectType( "zone protegee", QT_TRANSLATE_NOOP( "ENT_Tr", "protected area" ), eObjectType_ZoneProtegee ),
    T_ConverterObjectType( "zone implantation canon", QT_TRANSLATE_NOOP( "ENT_Tr", "deployment area" ), eObjectType_ZoneImplantationCanon ),
    T_ConverterObjectType( "zone implantation cobra", QT_TRANSLATE_NOOP( "ENT_Tr", "deployment area" ), eObjectType_ZoneImplantationCobra ),
    T_ConverterObjectType( "zone implantation lrm", QT_TRANSLATE_NOOP( "ENT_Tr", "deployment area" ), eObjectType_ZoneImplantationLrm ),
    T_ConverterObjectType( "site franchissement", QT_TRANSLATE_NOOP( "ENT_Tr", "crossing site" ), eObjectType_SiteFranchissement ),
    T_ConverterObjectType( "nuage nbc", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc cloud" ), eObjectType_NuageNbc ),
    T_ConverterObjectType( "plot ravitaillement", QT_TRANSLATE_NOOP( "ENT_Tr", "replenishment site" ), eObjectType_PlotRavitaillement ),
    T_ConverterObjectType( "site decontamination", QT_TRANSLATE_NOOP( "ENT_Tr", "decontamination site" ), eObjectType_SiteDecontamination ),
    T_ConverterObjectType( "zone brouillage brod", QT_TRANSLATE_NOOP( "ENT_Tr", "jam area" ), eObjectType_ZoneBrouillageBrod ),
    T_ConverterObjectType( "rota", QT_TRANSLATE_NOOP( "ENT_Tr", "rota" ), eObjectType_Rota ),
    T_ConverterObjectType( "zone nbc", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc area" ), eObjectType_ZoneNbc ),
    T_ConverterObjectType( "zone brouillage bromure", QT_TRANSLATE_NOOP( "ENT_Tr", "jam area" ), eObjectType_ZoneBrouillageBromure ),
    T_ConverterObjectType( "aire poser", QT_TRANSLATE_NOOP( "ENT_Tr", "land area" ), eObjectType_AirePoser ),
    T_ConverterObjectType( "piste", QT_TRANSLATE_NOOP( "ENT_Tr", "track" ), eObjectType_Piste ),
    T_ConverterObjectType( "plateforme", QT_TRANSLATE_NOOP( "ENT_Tr", "platforme" ), eObjectType_Plateforme ),
    T_ConverterObjectType( "zone mobilite amelioree", QT_TRANSLATE_NOOP( "ENT_Tr", "mobility enhanced area" ), eObjectType_ZoneMobiliteAmelioree ),
    T_ConverterObjectType( "zone poser helicoptere", QT_TRANSLATE_NOOP( "ENT_Tr", "land area for helicopter" ), eObjectType_ZonePoserHelicoptere ),
    T_ConverterObjectType( "aire logistique", QT_TRANSLATE_NOOP( "ENT_Tr", "logistic area" ), eObjectType_AireLogistique ),
    T_ConverterObjectType( "itineraire logistique", QT_TRANSLATE_NOOP( "ENT_Tr", "logistic route" ), eObjectType_ItineraireLogistique ),
    T_ConverterObjectType( "camp prisonniers", QT_TRANSLATE_NOOP( "ENT_Tr", "prisoners camp" ), eObjectType_CampPrisonniers ),
    T_ConverterObjectType( "camp refugies", QT_TRANSLATE_NOOP( "ENT_Tr", "refugees camp" ), eObjectType_CampRefugies ),
    T_ConverterObjectType( "poste controle", QT_TRANSLATE_NOOP( "ENT_Tr", "checkpoint" ), eObjectType_PosteControle ),
    T_ConverterObjectType( "terrain largage", QT_TRANSLATE_NOOP( "ENT_Tr", "dropping area" ), eObjectType_TerrainLargage ),
    T_ConverterObjectType( "zone interdite mouvement", QT_TRANSLATE_NOOP( "ENT_Tr", "keep out area" ), eObjectType_ZoneInterditeMouvement ),
    T_ConverterObjectType( "zone interdite tir", QT_TRANSLATE_NOOP( "ENT_Tr", "fire forbidden area" ), eObjectType_ZoneInterditeTir ),
    T_ConverterObjectType( "zone implantation mortier", QT_TRANSLATE_NOOP( "ENT_Tr", "deployment arrea (mortar)" ), eObjectType_ZoneImplantationMortier ),
    T_ConverterObjectType( "pont flottant discontinu", QT_TRANSLATE_NOOP( "ENT_Tr", "rafting bridge" ), eObjectType_PontFlottantDiscontinu ),
    T_ConverterObjectType( "installation", QT_TRANSLATE_NOOP( "ENT_Tr", "critical infrastructure" ), eObjectType_Installation ),
    T_ConverterObjectType( "", "", (E_ObjectType)-1 )
};

ENT_Tr::T_ConverterMissionGenSousTypeObstacle ENT_Tr::MissionGenSousTypeObstacleConverter_[] =
{
    T_ConverterMissionGenSousTypeObstacle( "preliminaire", QT_TRANSLATE_NOOP( "ENT_Tr", "small sized" ), eMissionGenSousTypeObstacle_Preliminaire ),
    T_ConverterMissionGenSousTypeObstacle( "de manoeuvre", QT_TRANSLATE_NOOP( "ENT_Tr", "reserved" ), eMissionGenSousTypeObstacle_DeManoeuvre ),
    T_ConverterMissionGenSousTypeObstacle( "", "", (E_MissionGenSousTypeObstacle)-1 )
};

ENT_Tr::T_ConverterOrderErrorCode ENT_Tr::OrderErrorCodeConverter_[] =
{
    T_ConverterOrderErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eOrderErrorCode_NoError ),
    T_ConverterOrderErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), eOrderErrorCode_ErrorInvalidUnit ),
    T_ConverterOrderErrorCode( "error invalid limit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid limit" ), eOrderErrorCode_ErrorInvalidLimit ),
    T_ConverterOrderErrorCode( "error invalid lima", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid lima" ), eOrderErrorCode_ErrorInvalidLima ),
    T_ConverterOrderErrorCode( "error invalid mission", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid mission" ), eOrderErrorCode_ErrorInvalidMission ),
    T_ConverterOrderErrorCode( "error invalid mission parameters", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid mission parameters" ), eOrderErrorCode_ErrorInvalidMissionParameters ),
    T_ConverterOrderErrorCode( "error unit cannot receive order", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit cannot receive order" ), eOrderErrorCode_ErrorUnitCannotReceiveOrder ),
    T_ConverterOrderErrorCode( "error invalid order conduite", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid order conduite" ), eOrderErrorCode_ErrorInvalidOrderConduite ),
    T_ConverterOrderErrorCode( "error invalid order mission", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid order mission" ), eOrderErrorCode_ErrorInvalidOrderMission ),
    T_ConverterOrderErrorCode( "error invalid order initial", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid order initial" ), eOrderErrorCode_ErrorInvalidOrderInitial ),
    T_ConverterOrderErrorCode( "error invalid order conduite parameters", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid order conduite parameters" ), eOrderErrorCode_ErrorInvalidOrderConduiteParameters ),
    T_ConverterOrderErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eOrderErrorCode_ErrorUnitSurrendered ),
    T_ConverterOrderErrorCode( "error invalid lima function", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid lima function" ), eOrderErrorCode_ErrorInvalidLimaFunction ),
    T_ConverterOrderErrorCode( "", "", (E_OrderErrorCode)-1 )
};

ENT_Tr::T_ConverterOrderState ENT_Tr::OrderStateConverter_[] =
{
    T_ConverterOrderState( "started", QT_TRANSLATE_NOOP( "ENT_Tr", "started" ), eOrderState_Started ),
    T_ConverterOrderState( "stopped", QT_TRANSLATE_NOOP( "ENT_Tr", "stopped" ), eOrderState_Stopped ),
    T_ConverterOrderState( "", "", (E_OrderState)-1 )
};

ENT_Tr::T_ConverterAutomatOrderFormation ENT_Tr::AutomatOrderFormationConverter_[] =
{
    T_ConverterAutomatOrderFormation( "un echelon", QT_TRANSLATE_NOOP( "ENT_Tr", "1 echelon formation" ), eAutomatOrderFormation_UnEchelon ),
    T_ConverterAutomatOrderFormation( "deux echelons", QT_TRANSLATE_NOOP( "ENT_Tr", "2 echelons formation" ), eAutomatOrderFormation_DeuxEchelons ),
    T_ConverterAutomatOrderFormation( "", "", (E_AutomatOrderFormation)-1 )
};

//-----------------------------------------------------------------------------
// Name: ENT_Tr::InitTranslations
// Created: AGR
//-----------------------------------------------------------------------------
void ENT_Tr::InitTranslations()
{
        InitTr( TypeLocalisationConverter_, "ENT_Tr" );
    InitTr( FamilleDotationConverter_, "ENT_Tr" );
    InitTr( FamilleMunitionConverter_, "ENT_Tr" );
    InitTr( UnitIdentificationLevelConverter_, "ENT_Tr" );
    InitTr( NatureLevelConverter_, "ENT_Tr" );
    InitTr( HumanWoundConverter_, "ENT_Tr" );
    InitTr( HumanRankConverter_, "ENT_Tr" );
    InitTr( DiplomatieConverter_, "ENT_Tr" );
    InitTr( ChangeDiplomacyErrorCodeConverter_, "ENT_Tr" );
    InitTr( EtatRapForConverter_, "ENT_Tr" );
    InitTr( TypeObstacleForConverter_, "ENT_Tr" );
    InitTr( EtatCombatRencontreConverter_, "ENT_Tr" );
    InitTr( EtatOperationnelConverter_, "ENT_Tr" );
    InitTr( RoeConverter_, "ENT_Tr" );
    InitTr( UnitAttrErrorCodeConverter_, "ENT_Tr" );
    InitTr( UnitPostureConverter_, "ENT_Tr" );
    InitTr( DisponibiliteAuTirConverter_, "ENT_Tr" );
    InitTr( RoePopulationConverter_, "ENT_Tr" );
    InitTr( UnitFatigueConverter_, "ENT_Tr" );
    InitTr( UnitMoralConverter_, "ENT_Tr" );
    InitTr( UnitExperienceConverter_, "ENT_Tr" );
    InitTr( AutomateStateConverter_, "ENT_Tr" );
    InitTr( SetAutomateModeErrorCodeConverter_, "ENT_Tr" );
    InitTr( CtrlErrorCodeConverter_, "ENT_Tr" );
    InitTr( EtatSimConverter_, "ENT_Tr" );
    InitTr( PrecipitationTypeConverter_, "ENT_Tr" );
    InitTr( TypeLimaConverter_, "ENT_Tr" );
    InitTr( InfoContextErrorCodeConverter_, "ENT_Tr" );
    InitTr( PopulationAttitudeConverter_, "ENT_Tr" );
    InitTr( PopulationAttrErrorCodeConverter_, "ENT_Tr" );
    InitTr( TypeCRConverter_, "ENT_Tr" );
    InitTr( FireEffectTypeConverter_, "ENT_Tr" );
    InitTr( LogMaintenanceRegimeTravailConverter_, "ENT_Tr" );
    InitTr( LogMaintenanceTraitementEtatConverter_, "ENT_Tr" );
    InitTr( LogRavitaillementTraitementEtatConverter_, "ENT_Tr" );
    InitTr( MsgLogSupplyChangeQuotasAckConverter_, "ENT_Tr" );
    InitTr( MsgLogSupplyPushFlowAckConverter_, "ENT_Tr" );
    InitTr( LogSanteTraitementEtatConverter_, "ENT_Tr" );
    InitTr( ObjectErrorCodeConverter_, "ENT_Tr" );
    InitTr( ObjectTypeConverter_, "ENT_Tr" );
    InitTr( MissionGenSousTypeObstacleConverter_, "ENT_Tr" );
    InitTr( OrderErrorCodeConverter_, "ENT_Tr" );
    InitTr( OrderStateConverter_, "ENT_Tr" );
    InitTr( AutomatOrderFormationConverter_, "ENT_Tr" );
}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTypeLocalisation
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTypeLocalisation( E_TypeLocalisation nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeLocalisationConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromFamilleDotation
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFamilleDotation( E_FamilleDotation nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( FamilleDotationConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromFamilleMunition
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFamilleMunition( E_FamilleMunition nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( FamilleMunitionConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitIdentificationLevel
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitIdentificationLevel( E_UnitIdentificationLevel nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitIdentificationLevelConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromNatureLevel
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromNatureLevel( E_NatureLevel nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( NatureLevelConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromHumanWound
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromHumanWound( E_HumanWound nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( HumanWoundConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromHumanRank
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromHumanRank( E_HumanRank nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( HumanRankConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromDiplomatie
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDiplomatie( E_Diplomatie nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DiplomatieConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromChangeDiplomacyErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromChangeDiplomacyErrorCode( E_ChangeDiplomacyErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ChangeDiplomacyErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromEtatRapFor
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromEtatRapFor( E_EtatRapFor nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( EtatRapForConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTypeObstacle
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTypeObstacle( E_TypeObstacle nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeObstacleForConverter_, nValue, nConverterType );
}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromEtatCombatRencontre
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromEtatCombatRencontre( E_EtatCombatRencontre nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( EtatCombatRencontreConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromEtatOperationnel
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromEtatOperationnel( E_EtatOperationnel nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( EtatOperationnelConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromRoe
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromRoe( E_Roe nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( RoeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitAttrErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitAttrErrorCode( E_UnitAttrErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitAttrErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitPosture
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitPosture( E_UnitPosture nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitPostureConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromDisponibiliteAuTir
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDisponibiliteAuTir( E_DisponibiliteAuTir nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DisponibiliteAuTirConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromRoePopulation
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromRoePopulation( E_RoePopulation nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( RoePopulationConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitFatigue
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitFatigue( E_UnitFatigue nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitFatigueConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitMoral
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitMoral( E_UnitMoral nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitMoralConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitExperience
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitExperience( E_UnitExperience nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitExperienceConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromAutomateState
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomateState( E_AutomateState nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AutomateStateConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromSetAutomateModeErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromSetAutomateModeErrorCode( E_SetAutomateModeErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( SetAutomateModeErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromCtrlErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromCtrlErrorCode( E_CtrlErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( CtrlErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromEtatSim
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromEtatSim( E_EtatSim nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( EtatSimConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromPrecipitationType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPrecipitationType( E_PrecipitationType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PrecipitationTypeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTypeLima
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTypeLima( E_TypeLima nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeLimaConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromInfoContextErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromInfoContextErrorCode( E_InfoContextErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( InfoContextErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromPopulationAttitude
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationAttitude( E_PopulationAttitude nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PopulationAttitudeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromPopulationAttrErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationAttrErrorCode( E_PopulationAttrErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PopulationAttrErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTypeCR
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTypeCR( E_TypeCR nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeCRConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromFireEffectType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFireEffectType( E_FireEffectType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( FireEffectTypeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLogMaintenanceRegimeTravail
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogMaintenanceRegimeTravail( E_LogMaintenanceRegimeTravail nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogMaintenanceRegimeTravailConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLogMaintenanceTraitementEtat
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogMaintenanceTraitementEtat( E_LogMaintenanceTraitementEtat nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogMaintenanceTraitementEtatConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLogRavitaillementTraitementEtat
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogRavitaillementTraitementEtat( E_LogRavitaillementTraitementEtat nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogRavitaillementTraitementEtatConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMsgLogSupplyChangeQuotasAck
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMsgLogSupplyChangeQuotasAck( E_MsgLogSupplyChangeQuotasAck nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MsgLogSupplyChangeQuotasAckConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMsgLogSupplyPushFlowAck
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMsgLogSupplyPushFlowAck( E_MsgLogSupplyPushFlowAck nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MsgLogSupplyPushFlowAckConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLogSanteTraitementEtat
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogSanteTraitementEtat( E_LogSanteTraitementEtat nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogSanteTraitementEtatConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromObjectErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromObjectErrorCode( E_ObjectErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ObjectErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromObjectType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromObjectType( E_ObjectType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ObjectTypeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMissionGenSousTypeObstacle
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMissionGenSousTypeObstacle( E_MissionGenSousTypeObstacle nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionGenSousTypeObstacleConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromOrderErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromOrderErrorCode( E_OrderErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( OrderErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromOrderState
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromOrderState( E_OrderState nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( OrderStateConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromAutomatOrderFormation
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomatOrderFormation( E_AutomatOrderFormation nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AutomatOrderFormationConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTypeLocalisation
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeLocalisation ENT_Tr::ConvertToTypeLocalisation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeLocalisationConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToFamilleDotation
// Created: AGR
//-----------------------------------------------------------------------------
E_FamilleDotation ENT_Tr::ConvertToFamilleDotation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( FamilleDotationConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToFamilleMunition
// Created: AGR
//-----------------------------------------------------------------------------
E_FamilleMunition ENT_Tr::ConvertToFamilleMunition( const std::string& strName )
{
    return ENT_Tr::FindInConverter( FamilleMunitionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitIdentificationLevel
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitIdentificationLevel ENT_Tr::ConvertToUnitIdentificationLevel( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitIdentificationLevelConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToNatureLevel
// Created: AGR
//-----------------------------------------------------------------------------
E_NatureLevel ENT_Tr::ConvertToNatureLevel( const std::string& strName )
{
    return ENT_Tr::FindInConverter( NatureLevelConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToHumanWound
// Created: AGR
//-----------------------------------------------------------------------------
E_HumanWound ENT_Tr::ConvertToHumanWound( const std::string& strName )
{
    return ENT_Tr::FindInConverter( HumanWoundConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToHumanRank
// Created: AGR
//-----------------------------------------------------------------------------
E_HumanRank ENT_Tr::ConvertToHumanRank( const std::string& strName )
{
    return ENT_Tr::FindInConverter( HumanRankConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToDiplomatie
// Created: AGR
//-----------------------------------------------------------------------------
E_Diplomatie ENT_Tr::ConvertToDiplomatie( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DiplomatieConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToChangeDiplomacyErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ChangeDiplomacyErrorCode ENT_Tr::ConvertToChangeDiplomacyErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ChangeDiplomacyErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToEtatRapFor
// Created: AGR
//-----------------------------------------------------------------------------
E_EtatRapFor ENT_Tr::ConvertToEtatRapFor( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EtatRapForConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTypeObstacle
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeObstacle ENT_Tr::ConvertToTypeObstacle( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeObstacleForConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToEtatCombatRencontre
// Created: AGR
//-----------------------------------------------------------------------------
E_EtatCombatRencontre ENT_Tr::ConvertToEtatCombatRencontre( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EtatCombatRencontreConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToEtatOperationnel
// Created: AGR
//-----------------------------------------------------------------------------
E_EtatOperationnel ENT_Tr::ConvertToEtatOperationnel( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EtatOperationnelConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToRoe
// Created: AGR
//-----------------------------------------------------------------------------
E_Roe ENT_Tr::ConvertToRoe( const std::string& strName )
{
    return ENT_Tr::FindInConverter( RoeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitAttrErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitAttrErrorCode ENT_Tr::ConvertToUnitAttrErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitAttrErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitPosture
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitPosture ENT_Tr::ConvertToUnitPosture( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitPostureConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToDisponibiliteAuTir
// Created: AGR
//-----------------------------------------------------------------------------
E_DisponibiliteAuTir ENT_Tr::ConvertToDisponibiliteAuTir( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DisponibiliteAuTirConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToRoePopulation
// Created: AGR
//-----------------------------------------------------------------------------
E_RoePopulation ENT_Tr::ConvertToRoePopulation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( RoePopulationConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitFatigue
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitFatigue ENT_Tr::ConvertToUnitFatigue( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitFatigueConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitMoral
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitMoral ENT_Tr::ConvertToUnitMoral( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitMoralConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitExperience
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitExperience ENT_Tr::ConvertToUnitExperience( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitExperienceConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAutomateState
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomateState ENT_Tr::ConvertToAutomateState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomateStateConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToSetAutomateModeErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_SetAutomateModeErrorCode ENT_Tr::ConvertToSetAutomateModeErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SetAutomateModeErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToCtrlErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_CtrlErrorCode ENT_Tr::ConvertToCtrlErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( CtrlErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToEtatSim
// Created: AGR
//-----------------------------------------------------------------------------
E_EtatSim ENT_Tr::ConvertToEtatSim( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EtatSimConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPrecipitationType
// Created: AGR
//-----------------------------------------------------------------------------
E_PrecipitationType ENT_Tr::ConvertToPrecipitationType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PrecipitationTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTypeLima
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeLima ENT_Tr::ConvertToTypeLima( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeLimaConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToInfoContextErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_InfoContextErrorCode ENT_Tr::ConvertToInfoContextErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( InfoContextErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationAttitude
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationAttitude ENT_Tr::ConvertToPopulationAttitude( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PopulationAttitudeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationAttrErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationAttrErrorCode ENT_Tr::ConvertToPopulationAttrErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PopulationAttrErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTypeCR
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeCR ENT_Tr::ConvertToTypeCR( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeCRConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToFireEffectType
// Created: AGR
//-----------------------------------------------------------------------------
E_FireEffectType ENT_Tr::ConvertToFireEffectType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( FireEffectTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLogMaintenanceRegimeTravail
// Created: AGR
//-----------------------------------------------------------------------------
E_LogMaintenanceRegimeTravail ENT_Tr::ConvertToLogMaintenanceRegimeTravail( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogMaintenanceRegimeTravailConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLogMaintenanceTraitementEtat
// Created: AGR
//-----------------------------------------------------------------------------
E_LogMaintenanceTraitementEtat ENT_Tr::ConvertToLogMaintenanceTraitementEtat( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogMaintenanceTraitementEtatConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLogRavitaillementTraitementEtat
// Created: AGR
//-----------------------------------------------------------------------------
E_LogRavitaillementTraitementEtat ENT_Tr::ConvertToLogRavitaillementTraitementEtat( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogRavitaillementTraitementEtatConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMsgLogSupplyChangeQuotasAck
// Created: AGR
//-----------------------------------------------------------------------------
E_MsgLogSupplyChangeQuotasAck ENT_Tr::ConvertToMsgLogSupplyChangeQuotasAck( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MsgLogSupplyChangeQuotasAckConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMsgLogSupplyPushFlowAck
// Created: AGR
//-----------------------------------------------------------------------------
E_MsgLogSupplyPushFlowAck ENT_Tr::ConvertToMsgLogSupplyPushFlowAck( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MsgLogSupplyPushFlowAckConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLogSanteTraitementEtat
// Created: AGR
//-----------------------------------------------------------------------------
E_LogSanteTraitementEtat ENT_Tr::ConvertToLogSanteTraitementEtat( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogSanteTraitementEtatConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToObjectErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ObjectErrorCode ENT_Tr::ConvertToObjectErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ObjectErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToObjectType
// Created: AGR
//-----------------------------------------------------------------------------
E_ObjectType ENT_Tr::ConvertToObjectType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ObjectTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMissionGenSousTypeObstacle
// Created: AGR
//-----------------------------------------------------------------------------
E_MissionGenSousTypeObstacle ENT_Tr::ConvertToMissionGenSousTypeObstacle( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionGenSousTypeObstacleConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToOrderErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_OrderErrorCode ENT_Tr::ConvertToOrderErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( OrderErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToOrderState
// Created: AGR
//-----------------------------------------------------------------------------
E_OrderState ENT_Tr::ConvertToOrderState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( OrderStateConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAutomatOrderFormation
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomatOrderFormation ENT_Tr::ConvertToAutomatOrderFormation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomatOrderFormationConverter_, strName );
}

