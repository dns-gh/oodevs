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
    T_ConverterFamilleDotation( "munition", QT_TRANSLATE_NOOP( "ENT_Tr", "munition" ), eFamilleDotation_Munition ),
    T_ConverterFamilleDotation( "carburant", QT_TRANSLATE_NOOP( "ENT_Tr", "carburant" ), eFamilleDotation_Carburant ),
    T_ConverterFamilleDotation( "explosif", QT_TRANSLATE_NOOP( "ENT_Tr", "explosif" ), eFamilleDotation_Explosif ),
    T_ConverterFamilleDotation( "mine", QT_TRANSLATE_NOOP( "ENT_Tr", "mine" ), eFamilleDotation_Mine ),
    T_ConverterFamilleDotation( "barbele", QT_TRANSLATE_NOOP( "ENT_Tr", "barbele" ), eFamilleDotation_Barbele ),
    T_ConverterFamilleDotation( "piece", QT_TRANSLATE_NOOP( "ENT_Tr", "piece" ), eFamilleDotation_Piece ),
    T_ConverterFamilleDotation( "ration", QT_TRANSLATE_NOOP( "ENT_Tr", "ration" ), eFamilleDotation_Ration ),
    T_ConverterFamilleDotation( "", "", (E_FamilleDotation)-1 )
};

ENT_Tr::T_ConverterFamilleMunition ENT_Tr::FamilleMunitionConverter_[] =
{
    T_ConverterFamilleMunition( "obus", QT_TRANSLATE_NOOP( "ENT_Tr", "obus" ), eFamilleMunition_Obus ),
    T_ConverterFamilleMunition( "missile air", QT_TRANSLATE_NOOP( "ENT_Tr", "missile air" ), eFamilleMunition_MissileAir ),
    T_ConverterFamilleMunition( "missile sol", QT_TRANSLATE_NOOP( "ENT_Tr", "missile sol" ), eFamilleMunition_MissileSol ),
    T_ConverterFamilleMunition( "mitraille", QT_TRANSLATE_NOOP( "ENT_Tr", "mitraille" ), eFamilleMunition_Mitraille ),
    T_ConverterFamilleMunition( "", "", (E_FamilleMunition)-1 )
};

ENT_Tr::T_ConverterGDH_Qualificatif ENT_Tr::GDH_QualificatifConverter_[] =
{
    T_ConverterGDH_Qualificatif( "at", QT_TRANSLATE_NOOP( "ENT_Tr", "at" ), eGDH_Qualificatif_At ),
    T_ConverterGDH_Qualificatif( "after", QT_TRANSLATE_NOOP( "ENT_Tr", "after" ), eGDH_Qualificatif_After ),
    T_ConverterGDH_Qualificatif( "as of", QT_TRANSLATE_NOOP( "ENT_Tr", "as of" ), eGDH_Qualificatif_AsOf ),
    T_ConverterGDH_Qualificatif( "before", QT_TRANSLATE_NOOP( "ENT_Tr", "before" ), eGDH_Qualificatif_Before ),
    T_ConverterGDH_Qualificatif( "cleared", QT_TRANSLATE_NOOP( "ENT_Tr", "cleared" ), eGDH_Qualificatif_Cleared ),
    T_ConverterGDH_Qualificatif( "due", QT_TRANSLATE_NOOP( "ENT_Tr", "due" ), eGDH_Qualificatif_Due ),
    T_ConverterGDH_Qualificatif( "end", QT_TRANSLATE_NOOP( "ENT_Tr", "end" ), eGDH_Qualificatif_End ),
    T_ConverterGDH_Qualificatif( "off", QT_TRANSLATE_NOOP( "ENT_Tr", "off" ), eGDH_Qualificatif_Off ),
    T_ConverterGDH_Qualificatif( "on", QT_TRANSLATE_NOOP( "ENT_Tr", "on" ), eGDH_Qualificatif_On ),
    T_ConverterGDH_Qualificatif( "since", QT_TRANSLATE_NOOP( "ENT_Tr", "since" ), eGDH_Qualificatif_Since ),
    T_ConverterGDH_Qualificatif( "start", QT_TRANSLATE_NOOP( "ENT_Tr", "start" ), eGDH_Qualificatif_Start ),
    T_ConverterGDH_Qualificatif( "until", QT_TRANSLATE_NOOP( "ENT_Tr", "until" ), eGDH_Qualificatif_Until ),
    T_ConverterGDH_Qualificatif( "wef", QT_TRANSLATE_NOOP( "ENT_Tr", "wef" ), eGDH_Qualificatif_Wef ),
    T_ConverterGDH_Qualificatif( "", "", (E_GDH_Qualificatif)-1 )
};

ENT_Tr::T_ConverterUnitIdentificationLevel ENT_Tr::UnitIdentificationLevelConverter_[] =
{
    T_ConverterUnitIdentificationLevel( "identifiee", QT_TRANSLATE_NOOP( "ENT_Tr", "identifiee" ), eUnitIdentificationLevel_Identifiee ),
    T_ConverterUnitIdentificationLevel( "reconnue", QT_TRANSLATE_NOOP( "ENT_Tr", "reconnue" ), eUnitIdentificationLevel_Reconnue ),
    T_ConverterUnitIdentificationLevel( "detectee", QT_TRANSLATE_NOOP( "ENT_Tr", "detectee" ), eUnitIdentificationLevel_Detectee ),
    T_ConverterUnitIdentificationLevel( "signale", QT_TRANSLATE_NOOP( "ENT_Tr", "signale" ), eUnitIdentificationLevel_Signale ),
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
    T_ConverterHumanWound( "non blesse", QT_TRANSLATE_NOOP( "ENT_Tr", "non blesse" ), eHumanWound_NonBlesse ),
    T_ConverterHumanWound( "mort", QT_TRANSLATE_NOOP( "ENT_Tr", "mort" ), eHumanWound_Mort ),
    T_ConverterHumanWound( "blesse urgence 1", QT_TRANSLATE_NOOP( "ENT_Tr", "blesse urgence 1" ), eHumanWound_BlesseUrgence1 ),
    T_ConverterHumanWound( "blesse urgence 2", QT_TRANSLATE_NOOP( "ENT_Tr", "blesse urgence 2" ), eHumanWound_BlesseUrgence2 ),
    T_ConverterHumanWound( "blesse urgence 3", QT_TRANSLATE_NOOP( "ENT_Tr", "blesse urgence 3" ), eHumanWound_BlesseUrgence3 ),
    T_ConverterHumanWound( "blesse urgence extreme", QT_TRANSLATE_NOOP( "ENT_Tr", "blesse urgence extreme" ), eHumanWound_BlesseUrgenceExtreme ),
    T_ConverterHumanWound( "", "", (E_HumanWound)-1 )
};

ENT_Tr::T_ConverterHumanRank ENT_Tr::HumanRankConverter_[] =
{
    T_ConverterHumanRank( "officier", QT_TRANSLATE_NOOP( "ENT_Tr", "officier" ), eHumanRank_Officier ),
    T_ConverterHumanRank( "sous officer", QT_TRANSLATE_NOOP( "ENT_Tr", "sous officer" ), eHumanRank_SousOfficer ),
    T_ConverterHumanRank( "mdr", QT_TRANSLATE_NOOP( "ENT_Tr", "mdr" ), eHumanRank_Mdr ),
    T_ConverterHumanRank( "", "", (E_HumanRank)-1 )
};

ENT_Tr::T_ConverterDiplomatie ENT_Tr::DiplomatieConverter_[] =
{
    T_ConverterDiplomatie( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "inconnu" ), eDiplomatie_Inconnu ),
    T_ConverterDiplomatie( "ami", QT_TRANSLATE_NOOP( "ENT_Tr", "ami" ), eDiplomatie_Ami ),
    T_ConverterDiplomatie( "ennemi", QT_TRANSLATE_NOOP( "ENT_Tr", "ennemi" ), eDiplomatie_Ennemi ),
    T_ConverterDiplomatie( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutre" ), eDiplomatie_Neutre ),
    T_ConverterDiplomatie( "", "", (E_Diplomatie)-1 )
};

ENT_Tr::T_ConverterChangeDiplomatieErrorCode ENT_Tr::ChangeDiplomatieErrorCodeConverter_[] =
{
    T_ConverterChangeDiplomatieErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeDiplomatieErrorCode_NoError ),
    T_ConverterChangeDiplomatieErrorCode( "error invalid camp", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid camp" ), eChangeDiplomatieErrorCode_ErrorInvalidCamp ),
    T_ConverterChangeDiplomatieErrorCode( "", "", (E_ChangeDiplomatieErrorCode)-1 )
};

ENT_Tr::T_ConverterEtatRapFor ENT_Tr::EtatRapForConverter_[] =
{
    T_ConverterEtatRapFor( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eEtatRapFor_None ),
    T_ConverterEtatRapFor( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutre" ), eEtatRapFor_Neutre ),
    T_ConverterEtatRapFor( "favorable", QT_TRANSLATE_NOOP( "ENT_Tr", "favorable" ), eEtatRapFor_Favorable ),
    T_ConverterEtatRapFor( "defavorable", QT_TRANSLATE_NOOP( "ENT_Tr", "defavorable" ), eEtatRapFor_Defavorable ),
    T_ConverterEtatRapFor( "", "", (E_EtatRapFor)-1 )
};

ENT_Tr::T_ConverterEtatCombatRencontre ENT_Tr::EtatCombatRencontreConverter_[] =
{
    T_ConverterEtatCombatRencontre( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eEtatCombatRencontre_None ),
    T_ConverterEtatCombatRencontre( "etat esquive", QT_TRANSLATE_NOOP( "ENT_Tr", "etat esquive" ), eEtatCombatRencontre_EtatEsquive ),
    T_ConverterEtatCombatRencontre( "etat fixe", QT_TRANSLATE_NOOP( "ENT_Tr", "etat fixe" ), eEtatCombatRencontre_EtatFixe ),
    T_ConverterEtatCombatRencontre( "etat poursuite mission", QT_TRANSLATE_NOOP( "ENT_Tr", "etat poursuite mission" ), eEtatCombatRencontre_EtatPoursuiteMission ),
    T_ConverterEtatCombatRencontre( "", "", (E_EtatCombatRencontre)-1 )
};

ENT_Tr::T_ConverterEtatOperationnel ENT_Tr::EtatOperationnelConverter_[] =
{
    T_ConverterEtatOperationnel( "detruit totalement", QT_TRANSLATE_NOOP( "ENT_Tr", "detruit totalement" ), eEtatOperationnel_DetruitTotalement ),
    T_ConverterEtatOperationnel( "detruit tactiquement", QT_TRANSLATE_NOOP( "ENT_Tr", "detruit tactiquement" ), eEtatOperationnel_DetruitTactiquement ),
    T_ConverterEtatOperationnel( "operationnel", QT_TRANSLATE_NOOP( "ENT_Tr", "operationnel" ), eEtatOperationnel_Operationnel ),
    T_ConverterEtatOperationnel( "", "", (E_EtatOperationnel)-1 )
};

ENT_Tr::T_ConverterRoe ENT_Tr::RoeConverter_[] =
{
    T_ConverterRoe( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoe_None ),
    T_ConverterRoe( "tir libre", QT_TRANSLATE_NOOP( "ENT_Tr", "tir libre" ), eRoe_TirLibre ),
    T_ConverterRoe( "tir sur riposte", QT_TRANSLATE_NOOP( "ENT_Tr", "tir sur riposte" ), eRoe_TirSurRiposte ),
    T_ConverterRoe( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "tir interdit" ), eRoe_TirInterdit ),
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
    T_ConverterUnitPosture( "posture mouvement", QT_TRANSLATE_NOOP( "ENT_Tr", "posture mouvement" ), eUnitPosture_PostureMouvement ),
    T_ConverterUnitPosture( "posture mouvement discret", QT_TRANSLATE_NOOP( "ENT_Tr", "posture mouvement discret" ), eUnitPosture_PostureMouvementDiscret ),
    T_ConverterUnitPosture( "posture arret", QT_TRANSLATE_NOOP( "ENT_Tr", "posture arret" ), eUnitPosture_PostureArret ),
    T_ConverterUnitPosture( "posture poste reflexe", QT_TRANSLATE_NOOP( "ENT_Tr", "posture poste reflexe" ), eUnitPosture_PosturePosteReflexe ),
    T_ConverterUnitPosture( "posture poste", QT_TRANSLATE_NOOP( "ENT_Tr", "posture poste" ), eUnitPosture_PosturePoste ),
    T_ConverterUnitPosture( "posture poste amenage", QT_TRANSLATE_NOOP( "ENT_Tr", "posture poste amenage" ), eUnitPosture_PosturePosteAmenage ),
    T_ConverterUnitPosture( "posture poste prepare genie", QT_TRANSLATE_NOOP( "ENT_Tr", "posture poste prepare genie" ), eUnitPosture_PosturePostePrepareGenie ),
    T_ConverterUnitPosture( "", "", (E_UnitPosture)-1 )
};

ENT_Tr::T_ConverterDisponibiliteAuTir ENT_Tr::DisponibiliteAuTirConverter_[] =
{
    T_ConverterDisponibiliteAuTir( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eDisponibiliteAuTir_None ),
    T_ConverterDisponibiliteAuTir( "pret au tir", QT_TRANSLATE_NOOP( "ENT_Tr", "pret au tir" ), eDisponibiliteAuTir_PretAuTir ),
    T_ConverterDisponibiliteAuTir( "indisponible", QT_TRANSLATE_NOOP( "ENT_Tr", "indisponible" ), eDisponibiliteAuTir_Indisponible ),
    T_ConverterDisponibiliteAuTir( "", "", (E_DisponibiliteAuTir)-1 )
};

ENT_Tr::T_ConverterRoePopulation ENT_Tr::RoePopulationConverter_[] =
{
    T_ConverterRoePopulation( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoePopulation_None ),
    T_ConverterRoePopulation( "emploi force interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "emploi force interdit" ), eRoePopulation_EmploiForceInterdit ),
    T_ConverterRoePopulation( "maintien a distance par moyens non letaux", QT_TRANSLATE_NOOP( "ENT_Tr", "maintien a distance par moyens non letaux" ), eRoePopulation_MaintienADistanceParMoyensNonLetaux ),
    T_ConverterRoePopulation( "dispersion par moyens de defense actifs", QT_TRANSLATE_NOOP( "ENT_Tr", "dispersion par moyens de defense actifs" ), eRoePopulation_DispersionParMoyensDeDefenseActifs ),
    T_ConverterRoePopulation( "armes letales autorisees", QT_TRANSLATE_NOOP( "ENT_Tr", "armes letales autorisees" ), eRoePopulation_ArmesLetalesAutorisees ),
    T_ConverterRoePopulation( "", "", (E_RoePopulation)-1 )
};

ENT_Tr::T_ConverterUnitFatigue ENT_Tr::UnitFatigueConverter_[] =
{
    T_ConverterUnitFatigue( "normal", QT_TRANSLATE_NOOP( "ENT_Tr", "normal" ), eUnitFatigue_Normal ),
    T_ConverterUnitFatigue( "fatigue", QT_TRANSLATE_NOOP( "ENT_Tr", "fatigue" ), eUnitFatigue_Fatigue ),
    T_ConverterUnitFatigue( "epuise", QT_TRANSLATE_NOOP( "ENT_Tr", "epuise" ), eUnitFatigue_Epuise ),
    T_ConverterUnitFatigue( "", "", (E_UnitFatigue)-1 )
};

ENT_Tr::T_ConverterUnitMoral ENT_Tr::UnitMoralConverter_[] =
{
    T_ConverterUnitMoral( "fanatique", QT_TRANSLATE_NOOP( "ENT_Tr", "fanatique" ), eUnitMoral_Fanatique ),
    T_ConverterUnitMoral( "bon", QT_TRANSLATE_NOOP( "ENT_Tr", "bon" ), eUnitMoral_Bon ),
    T_ConverterUnitMoral( "moyen", QT_TRANSLATE_NOOP( "ENT_Tr", "moyen" ), eUnitMoral_Moyen ),
    T_ConverterUnitMoral( "mauvais", QT_TRANSLATE_NOOP( "ENT_Tr", "mauvais" ), eUnitMoral_Mauvais ),
    T_ConverterUnitMoral( "", "", (E_UnitMoral)-1 )
};

ENT_Tr::T_ConverterUnitExperience ENT_Tr::UnitExperienceConverter_[] =
{
    T_ConverterUnitExperience( "veteran", QT_TRANSLATE_NOOP( "ENT_Tr", "veteran" ), eUnitExperience_Veteran ),
    T_ConverterUnitExperience( "experimente", QT_TRANSLATE_NOOP( "ENT_Tr", "experimente" ), eUnitExperience_Experimente ),
    T_ConverterUnitExperience( "conscrit", QT_TRANSLATE_NOOP( "ENT_Tr", "conscrit" ), eUnitExperience_Conscrit ),
    T_ConverterUnitExperience( "", "", (E_UnitExperience)-1 )
};

ENT_Tr::T_ConverterAutomateState ENT_Tr::AutomateStateConverter_[] =
{
    T_ConverterAutomateState( "embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "embraye" ), eAutomateState_Embraye ),
    T_ConverterAutomateState( "debraye", QT_TRANSLATE_NOOP( "ENT_Tr", "debraye" ), eAutomateState_Debraye ),
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
    T_ConverterPrecipitationType( "pas de precipitation", QT_TRANSLATE_NOOP( "ENT_Tr", "pas de precipitation" ), ePrecipitationType_PasDePrecipitation ),
    T_ConverterPrecipitationType( "tempete sable", QT_TRANSLATE_NOOP( "ENT_Tr", "tempete sable" ), ePrecipitationType_TempeteSable ),
    T_ConverterPrecipitationType( "brouillard", QT_TRANSLATE_NOOP( "ENT_Tr", "brouillard" ), ePrecipitationType_Brouillard ),
    T_ConverterPrecipitationType( "crachin", QT_TRANSLATE_NOOP( "ENT_Tr", "crachin" ), ePrecipitationType_Crachin ),
    T_ConverterPrecipitationType( "pluie", QT_TRANSLATE_NOOP( "ENT_Tr", "pluie" ), ePrecipitationType_Pluie ),
    T_ConverterPrecipitationType( "neige", QT_TRANSLATE_NOOP( "ENT_Tr", "neige" ), ePrecipitationType_Neige ),
    T_ConverterPrecipitationType( "", "", (E_PrecipitationType)-1 )
};

ENT_Tr::T_ConverterTypeLima ENT_Tr::TypeLimaConverter_[] =
{
    T_ConverterTypeLima( "ligne debouche", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne debouche" ), eTypeLima_LigneDebouche ),
    T_ConverterTypeLima( "ligne changement attitude", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne changement attitude" ), eTypeLima_LigneChangementAttitude ),
    T_ConverterTypeLima( "ligne coordination", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne coordination" ), eTypeLima_LigneCoordination ),
    T_ConverterTypeLima( "ligne interdire", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne interdire" ), eTypeLima_LigneInterdire ),
    T_ConverterTypeLima( "ligne objectif", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne objectif" ), eTypeLima_LigneObjectif ),
    T_ConverterTypeLima( "ligne coup arret", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne coup arret" ), eTypeLima_LigneCoupArret ),
    T_ConverterTypeLima( "ligne recueil", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne recueil" ), eTypeLima_LigneRecueil ),
    T_ConverterTypeLima( "ligne debut mission", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne debut mission" ), eTypeLima_LigneDebutMission ),
    T_ConverterTypeLima( "ligne fin mission", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne fin mission" ), eTypeLima_LigneFinMission ),
    T_ConverterTypeLima( "ligne identification accueil", QT_TRANSLATE_NOOP( "ENT_Tr", "ligne identification accueil" ), eTypeLima_LigneIdentificationAccueil ),
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
    T_ConverterPopulationAttitude( "calme", QT_TRANSLATE_NOOP( "ENT_Tr", "calme" ), ePopulationAttitude_Calme ),
    T_ConverterPopulationAttitude( "agitee", QT_TRANSLATE_NOOP( "ENT_Tr", "agitee" ), ePopulationAttitude_Agitee ),
    T_ConverterPopulationAttitude( "excitee", QT_TRANSLATE_NOOP( "ENT_Tr", "excitee" ), ePopulationAttitude_Excitee ),
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
    T_ConverterTypeCR( "operationel", QT_TRANSLATE_NOOP( "ENT_Tr", "operationel" ), eTypeCR_Operationel ),
    T_ConverterTypeCR( "evenement exceptionnel", QT_TRANSLATE_NOOP( "ENT_Tr", "evenement exceptionnel" ), eTypeCR_EvenementExceptionnel ),
    T_ConverterTypeCR( "warning", QT_TRANSLATE_NOOP( "ENT_Tr", "warning" ), eTypeCR_Warning ),
    T_ConverterTypeCR( "", "", (E_TypeCR)-1 )
};

ENT_Tr::T_ConverterFireEffectType ENT_Tr::FireEffectTypeConverter_[] =
{
    T_ConverterFireEffectType( "fumigene", QT_TRANSLATE_NOOP( "ENT_Tr", "fumigene" ), eFireEffectType_Fumigene ),
    T_ConverterFireEffectType( "eclairant", QT_TRANSLATE_NOOP( "ENT_Tr", "eclairant" ), eFireEffectType_Eclairant ),
    T_ConverterFireEffectType( "", "", (E_FireEffectType)-1 )
};

ENT_Tr::T_ConverterLogMaintenanceRegimeTravail ENT_Tr::LogMaintenanceRegimeTravailConverter_[] =
{
    T_ConverterLogMaintenanceRegimeTravail( "regime 1", QT_TRANSLATE_NOOP( "ENT_Tr", "regime 1" ), eLogMaintenanceRegimeTravail_Regime1 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 2", QT_TRANSLATE_NOOP( "ENT_Tr", "regime 2" ), eLogMaintenanceRegimeTravail_Regime2 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 3", QT_TRANSLATE_NOOP( "ENT_Tr", "regime 3" ), eLogMaintenanceRegimeTravail_Regime3 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 4", QT_TRANSLATE_NOOP( "ENT_Tr", "regime 4" ), eLogMaintenanceRegimeTravail_Regime4 ),
    T_ConverterLogMaintenanceRegimeTravail( "", "", (E_LogMaintenanceRegimeTravail)-1 )
};

ENT_Tr::T_ConverterLogMaintenanceTraitementEtat ENT_Tr::LogMaintenanceTraitementEtatConverter_[] =
{
    T_ConverterLogMaintenanceTraitementEtat( "deplacement vers chaine", QT_TRANSLATE_NOOP( "ENT_Tr", "deplacement vers chaine" ), eLogMaintenanceTraitementEtat_DeplacementVersChaine ),
    T_ConverterLogMaintenanceTraitementEtat( "attente disponibilite remorqueur", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite remorqueur" ), eLogMaintenanceTraitementEtat_AttenteDisponibiliteRemorqueur ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "remorqueur deplacement aller" ), eLogMaintenanceTraitementEtat_RemorqueurDeplacementAller ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "remorqueur chargement" ), eLogMaintenanceTraitementEtat_RemorqueurChargement ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "remorqueur deplacement retour" ), eLogMaintenanceTraitementEtat_RemorqueurDeplacementRetour ),
    T_ConverterLogMaintenanceTraitementEtat( "remorqueur dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "remorqueur dechargement" ), eLogMaintenanceTraitementEtat_RemorqueurDechargement ),
    T_ConverterLogMaintenanceTraitementEtat( "diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "diagnostique" ), eLogMaintenanceTraitementEtat_Diagnostique ),
    T_ConverterLogMaintenanceTraitementEtat( "attente prise en charge par niveau superieur", QT_TRANSLATE_NOOP( "ENT_Tr", "attente prise en charge par niveau superieur" ), eLogMaintenanceTraitementEtat_AttentePriseEnChargeParNiveauSuperieur ),
    T_ConverterLogMaintenanceTraitementEtat( "attente disponibilite pieces", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite pieces" ), eLogMaintenanceTraitementEtat_AttenteDisponibilitePieces ),
    T_ConverterLogMaintenanceTraitementEtat( "attente disponibilite reparateur", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite reparateur" ), eLogMaintenanceTraitementEtat_AttenteDisponibiliteReparateur ),
    T_ConverterLogMaintenanceTraitementEtat( "reparation", QT_TRANSLATE_NOOP( "ENT_Tr", "reparation" ), eLogMaintenanceTraitementEtat_Reparation ),
    T_ConverterLogMaintenanceTraitementEtat( "retour pion", QT_TRANSLATE_NOOP( "ENT_Tr", "retour pion" ), eLogMaintenanceTraitementEtat_RetourPion ),
    T_ConverterLogMaintenanceTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "termine" ), eLogMaintenanceTraitementEtat_Termine ),
    T_ConverterLogMaintenanceTraitementEtat( "", "", (E_LogMaintenanceTraitementEtat)-1 )
};

ENT_Tr::T_ConverterLogRavitaillementTraitementEtat ENT_Tr::LogRavitaillementTraitementEtatConverter_[] =
{
    T_ConverterLogRavitaillementTraitementEtat( "convoi en attente camions", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi en attente camions" ), eLogRavitaillementTraitementEtat_ConvoiEnAttenteCamions ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi constitution", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi constitution" ), eLogRavitaillementTraitementEtat_ConvoiConstitution ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi deplacement vers point chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi deplacement vers point chargement" ), eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointChargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi chargement" ), eLogRavitaillementTraitementEtat_ConvoiChargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi deplacement vers point dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi deplacement vers point dechargement" ), eLogRavitaillementTraitementEtat_ConvoiDeplacementVersPointDechargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi dechargement" ), eLogRavitaillementTraitementEtat_ConvoiDechargement ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi deplacement retour" ), eLogRavitaillementTraitementEtat_ConvoiDeplacementRetour ),
    T_ConverterLogRavitaillementTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "termine" ), eLogRavitaillementTraitementEtat_Termine ),
    T_ConverterLogRavitaillementTraitementEtat( "", "", (E_LogRavitaillementTraitementEtat)-1 )
};

ENT_Tr::T_ConverterMsgLogRavitaillementChangeQuotasAck ENT_Tr::MsgLogRavitaillementChangeQuotasAckConverter_[] =
{
    T_ConverterMsgLogRavitaillementChangeQuotasAck( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eMsgLogRavitaillementChangeQuotasAck_NoError ),
    T_ConverterMsgLogRavitaillementChangeQuotasAck( "error invalid donneur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid donneur" ), eMsgLogRavitaillementChangeQuotasAck_ErrorInvalidDonneur ),
    T_ConverterMsgLogRavitaillementChangeQuotasAck( "error invalid receveur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid receveur" ), eMsgLogRavitaillementChangeQuotasAck_ErrorInvalidReceveur ),
    T_ConverterMsgLogRavitaillementChangeQuotasAck( "", "", (E_MsgLogRavitaillementChangeQuotasAck)-1 )
};

ENT_Tr::T_ConverterMsgLogRavitaillementPousserFluxAck ENT_Tr::MsgLogRavitaillementPousserFluxAckConverter_[] =
{
    T_ConverterMsgLogRavitaillementPousserFluxAck( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eMsgLogRavitaillementPousserFluxAck_NoError ),
    T_ConverterMsgLogRavitaillementPousserFluxAck( "error invalid donneur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid donneur" ), eMsgLogRavitaillementPousserFluxAck_ErrorInvalidDonneur ),
    T_ConverterMsgLogRavitaillementPousserFluxAck( "error invalid receveur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid receveur" ), eMsgLogRavitaillementPousserFluxAck_ErrorInvalidReceveur ),
    T_ConverterMsgLogRavitaillementPousserFluxAck( "", "", (E_MsgLogRavitaillementPousserFluxAck)-1 )
};

ENT_Tr::T_ConverterLogSanteTraitementEtat ENT_Tr::LogSanteTraitementEtatConverter_[] =
{
    T_ConverterLogSanteTraitementEtat( "attente disponibilite ambulance releve", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite ambulance releve" ), eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceReleve ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance releve deplacement aller" ), eLogSanteTraitementEtat_AmbulanceReleveDeplacementAller ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance releve chargement" ), eLogSanteTraitementEtat_AmbulanceReleveChargement ),
    T_ConverterLogSanteTraitementEtat( "attente chargement complet ambulance releve", QT_TRANSLATE_NOOP( "ENT_Tr", "attente chargement complet ambulance releve" ), eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceReleve ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance releve deplacement retour" ), eLogSanteTraitementEtat_AmbulanceReleveDeplacementRetour ),
    T_ConverterLogSanteTraitementEtat( "ambulance releve dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance releve dechargement" ), eLogSanteTraitementEtat_AmbulanceReleveDechargement ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite medecin pour diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite medecin pour diagnostique" ), eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourDiagnostique ),
    T_ConverterLogSanteTraitementEtat( "diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "diagnostique" ), eLogSanteTraitementEtat_Diagnostique ),
    T_ConverterLogSanteTraitementEtat( "recherche secteur tri", QT_TRANSLATE_NOOP( "ENT_Tr", "recherche secteur tri" ), eLogSanteTraitementEtat_RechercheSecteurTri ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite medecin pour tri", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite medecin pour tri" ), eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourTri ),
    T_ConverterLogSanteTraitementEtat( "tri", QT_TRANSLATE_NOOP( "ENT_Tr", "tri" ), eLogSanteTraitementEtat_Tri ),
    T_ConverterLogSanteTraitementEtat( "recherche secteur soin", QT_TRANSLATE_NOOP( "ENT_Tr", "recherche secteur soin" ), eLogSanteTraitementEtat_RechercheSecteurSoin ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite medecin pour soin", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite medecin pour soin" ), eLogSanteTraitementEtat_AttenteDisponibiliteMedecinPourSoin ),
    T_ConverterLogSanteTraitementEtat( "soin", QT_TRANSLATE_NOOP( "ENT_Tr", "soin" ), eLogSanteTraitementEtat_Soin ),
    T_ConverterLogSanteTraitementEtat( "hospitalisation", QT_TRANSLATE_NOOP( "ENT_Tr", "hospitalisation" ), eLogSanteTraitementEtat_Hospitalisation ),
    T_ConverterLogSanteTraitementEtat( "attente disponibilite ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite ambulance ramassage" ), eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceRamassage ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance ramassage chargement" ), eLogSanteTraitementEtat_AmbulanceRamassageChargement ),
    T_ConverterLogSanteTraitementEtat( "attente chargement complet ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "attente chargement complet ambulance ramassage" ), eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceRamassage ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance ramassage deplacement aller" ), eLogSanteTraitementEtat_AmbulanceRamassageDeplacementAller ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance ramassage dechargement" ), eLogSanteTraitementEtat_AmbulanceRamassageDechargement ),
    T_ConverterLogSanteTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "termine" ), eLogSanteTraitementEtat_Termine ),
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
    T_ConverterObjectType( "bouchon mines", QT_TRANSLATE_NOOP( "ENT_Tr", "bouchon mines" ), eObjectType_BouchonMines ),
    T_ConverterObjectType( "zone minee lineaire", QT_TRANSLATE_NOOP( "ENT_Tr", "zone minee lineaire" ), eObjectType_ZoneMineeLineaire ),
    T_ConverterObjectType( "zone minee par dispersion", QT_TRANSLATE_NOOP( "ENT_Tr", "zone minee par dispersion" ), eObjectType_ZoneMineeParDispersion ),
    T_ConverterObjectType( "fosse anti char", QT_TRANSLATE_NOOP( "ENT_Tr", "fosse anti char" ), eObjectType_FosseAntiChar ),
    T_ConverterObjectType( "abattis", QT_TRANSLATE_NOOP( "ENT_Tr", "abattis" ), eObjectType_Abattis ),
    T_ConverterObjectType( "barricade", QT_TRANSLATE_NOOP( "ENT_Tr", "barricade" ), eObjectType_Barricade ),
    T_ConverterObjectType( "eboulement", QT_TRANSLATE_NOOP( "ENT_Tr", "eboulement" ), eObjectType_Eboulement ),
    T_ConverterObjectType( "destruction route", QT_TRANSLATE_NOOP( "ENT_Tr", "destruction route" ), eObjectType_DestructionRoute ),
    T_ConverterObjectType( "destruction pont", QT_TRANSLATE_NOOP( "ENT_Tr", "destruction pont" ), eObjectType_DestructionPont ),
    T_ConverterObjectType( "pont flottant continu", QT_TRANSLATE_NOOP( "ENT_Tr", "pont flottant continu" ), eObjectType_PontFlottantContinu ),
    T_ConverterObjectType( "poste tir", QT_TRANSLATE_NOOP( "ENT_Tr", "poste tir" ), eObjectType_PosteTir ),
    T_ConverterObjectType( "zone protegee", QT_TRANSLATE_NOOP( "ENT_Tr", "zone protegee" ), eObjectType_ZoneProtegee ),
    T_ConverterObjectType( "zone implantation canon", QT_TRANSLATE_NOOP( "ENT_Tr", "zone implantation canon" ), eObjectType_ZoneImplantationCanon ),
    T_ConverterObjectType( "zone implantation cobra", QT_TRANSLATE_NOOP( "ENT_Tr", "zone implantation cobra" ), eObjectType_ZoneImplantationCobra ),
    T_ConverterObjectType( "zone implantation lrm", QT_TRANSLATE_NOOP( "ENT_Tr", "zone implantation lrm" ), eObjectType_ZoneImplantationLrm ),
    T_ConverterObjectType( "site franchissement", QT_TRANSLATE_NOOP( "ENT_Tr", "site franchissement" ), eObjectType_SiteFranchissement ),
    T_ConverterObjectType( "nuage nbc", QT_TRANSLATE_NOOP( "ENT_Tr", "nuage nbc" ), eObjectType_NuageNbc ),
    T_ConverterObjectType( "plot ravitaillement", QT_TRANSLATE_NOOP( "ENT_Tr", "plot ravitaillement" ), eObjectType_PlotRavitaillement ),
    T_ConverterObjectType( "site decontamination", QT_TRANSLATE_NOOP( "ENT_Tr", "site decontamination" ), eObjectType_SiteDecontamination ),
    T_ConverterObjectType( "zone brouillage brod", QT_TRANSLATE_NOOP( "ENT_Tr", "zone brouillage brod" ), eObjectType_ZoneBrouillageBrod ),
    T_ConverterObjectType( "rota", QT_TRANSLATE_NOOP( "ENT_Tr", "rota" ), eObjectType_Rota ),
    T_ConverterObjectType( "zone nbc", QT_TRANSLATE_NOOP( "ENT_Tr", "zone nbc" ), eObjectType_ZoneNbc ),
    T_ConverterObjectType( "zone brouillage bromure", QT_TRANSLATE_NOOP( "ENT_Tr", "zone brouillage bromure" ), eObjectType_ZoneBrouillageBromure ),
    T_ConverterObjectType( "aire poser", QT_TRANSLATE_NOOP( "ENT_Tr", "aire poser" ), eObjectType_AirePoser ),
    T_ConverterObjectType( "piste", QT_TRANSLATE_NOOP( "ENT_Tr", "piste" ), eObjectType_Piste ),
    T_ConverterObjectType( "plateforme", QT_TRANSLATE_NOOP( "ENT_Tr", "plateforme" ), eObjectType_Plateforme ),
    T_ConverterObjectType( "zone mobilite amelioree", QT_TRANSLATE_NOOP( "ENT_Tr", "zone mobilite amelioree" ), eObjectType_ZoneMobiliteAmelioree ),
    T_ConverterObjectType( "zone poser helicoptere", QT_TRANSLATE_NOOP( "ENT_Tr", "zone poser helicoptere" ), eObjectType_ZonePoserHelicoptere ),
    T_ConverterObjectType( "aire logistique", QT_TRANSLATE_NOOP( "ENT_Tr", "aire logistique" ), eObjectType_AireLogistique ),
    T_ConverterObjectType( "itineraire logistique", QT_TRANSLATE_NOOP( "ENT_Tr", "itineraire logistique" ), eObjectType_ItineraireLogistique ),
    T_ConverterObjectType( "camp prisonniers", QT_TRANSLATE_NOOP( "ENT_Tr", "camp prisonniers" ), eObjectType_CampPrisonniers ),
    T_ConverterObjectType( "camp refugies", QT_TRANSLATE_NOOP( "ENT_Tr", "camp refugies" ), eObjectType_CampRefugies ),
    T_ConverterObjectType( "poste controle", QT_TRANSLATE_NOOP( "ENT_Tr", "poste controle" ), eObjectType_PosteControle ),
    T_ConverterObjectType( "terrain largage", QT_TRANSLATE_NOOP( "ENT_Tr", "terrain largage" ), eObjectType_TerrainLargage ),
    T_ConverterObjectType( "zone interdite mouvement", QT_TRANSLATE_NOOP( "ENT_Tr", "zone interdite mouvement" ), eObjectType_ZoneInterditeMouvement ),
    T_ConverterObjectType( "zone interdite tir", QT_TRANSLATE_NOOP( "ENT_Tr", "zone interdite tir" ), eObjectType_ZoneInterditeTir ),
    T_ConverterObjectType( "zone implantation mortier", QT_TRANSLATE_NOOP( "ENT_Tr", "zone implantation mortier" ), eObjectType_ZoneImplantationMortier ),
    T_ConverterObjectType( "pont flottant discontinu", QT_TRANSLATE_NOOP( "ENT_Tr", "pont flottant discontinu" ), eObjectType_PontFlottantDiscontinu ),
    T_ConverterObjectType( "installation", QT_TRANSLATE_NOOP( "ENT_Tr", "installation" ), eObjectType_Installation ),
    T_ConverterObjectType( "", "", (E_ObjectType)-1 )
};

ENT_Tr::T_ConverterMissionGenSousTypeObstacle ENT_Tr::MissionGenSousTypeObstacleConverter_[] =
{
    T_ConverterMissionGenSousTypeObstacle( "preliminaire", QT_TRANSLATE_NOOP( "ENT_Tr", "preliminaire" ), eMissionGenSousTypeObstacle_Preliminaire ),
    T_ConverterMissionGenSousTypeObstacle( "de manoeuvre", QT_TRANSLATE_NOOP( "ENT_Tr", "de manoeuvre" ), eMissionGenSousTypeObstacle_DeManoeuvre ),
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

ENT_Tr::T_ConverterAutomateOrderFormation ENT_Tr::AutomateOrderFormationConverter_[] =
{
    T_ConverterAutomateOrderFormation( "un echelon", QT_TRANSLATE_NOOP( "ENT_Tr", "un echelon" ), eAutomateOrderFormation_UnEchelon ),
    T_ConverterAutomateOrderFormation( "deux echelons", QT_TRANSLATE_NOOP( "ENT_Tr", "deux echelons" ), eAutomateOrderFormation_DeuxEchelons ),
    T_ConverterAutomateOrderFormation( "", "", (E_AutomateOrderFormation)-1 )
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
    InitTr( GDH_QualificatifConverter_, "ENT_Tr" );
    InitTr( UnitIdentificationLevelConverter_, "ENT_Tr" );
    InitTr( NatureLevelConverter_, "ENT_Tr" );
    InitTr( HumanWoundConverter_, "ENT_Tr" );
    InitTr( HumanRankConverter_, "ENT_Tr" );
    InitTr( DiplomatieConverter_, "ENT_Tr" );
    InitTr( ChangeDiplomatieErrorCodeConverter_, "ENT_Tr" );
    InitTr( EtatRapForConverter_, "ENT_Tr" );
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
    InitTr( MsgLogRavitaillementChangeQuotasAckConverter_, "ENT_Tr" );
    InitTr( MsgLogRavitaillementPousserFluxAckConverter_, "ENT_Tr" );
    InitTr( LogSanteTraitementEtatConverter_, "ENT_Tr" );
    InitTr( ObjectErrorCodeConverter_, "ENT_Tr" );
    InitTr( ObjectTypeConverter_, "ENT_Tr" );
    InitTr( MissionGenSousTypeObstacleConverter_, "ENT_Tr" );
    InitTr( OrderErrorCodeConverter_, "ENT_Tr" );
    InitTr( OrderStateConverter_, "ENT_Tr" );
    InitTr( AutomateOrderFormationConverter_, "ENT_Tr" );
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
// Name: ENT_Tr::ConvertFromGDH_Qualificatif
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromGDH_Qualificatif( E_GDH_Qualificatif nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( GDH_QualificatifConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromChangeDiplomatieErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromChangeDiplomatieErrorCode( E_ChangeDiplomatieErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ChangeDiplomatieErrorCodeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromMsgLogRavitaillementChangeQuotasAck
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMsgLogRavitaillementChangeQuotasAck( E_MsgLogRavitaillementChangeQuotasAck nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MsgLogRavitaillementChangeQuotasAckConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMsgLogRavitaillementPousserFluxAck
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMsgLogRavitaillementPousserFluxAck( E_MsgLogRavitaillementPousserFluxAck nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MsgLogRavitaillementPousserFluxAckConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromAutomateOrderFormation
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomateOrderFormation( E_AutomateOrderFormation nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AutomateOrderFormationConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertToGDH_Qualificatif
// Created: AGR
//-----------------------------------------------------------------------------
E_GDH_Qualificatif ENT_Tr::ConvertToGDH_Qualificatif( const std::string& strName )
{
    return ENT_Tr::FindInConverter( GDH_QualificatifConverter_, strName );
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
// Name: ENT_Tr::ConvertToChangeDiplomatieErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ChangeDiplomatieErrorCode ENT_Tr::ConvertToChangeDiplomatieErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ChangeDiplomatieErrorCodeConverter_, strName );
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
// Name: ENT_Tr::ConvertToMsgLogRavitaillementChangeQuotasAck
// Created: AGR
//-----------------------------------------------------------------------------
E_MsgLogRavitaillementChangeQuotasAck ENT_Tr::ConvertToMsgLogRavitaillementChangeQuotasAck( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MsgLogRavitaillementChangeQuotasAckConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMsgLogRavitaillementPousserFluxAck
// Created: AGR
//-----------------------------------------------------------------------------
E_MsgLogRavitaillementPousserFluxAck ENT_Tr::ConvertToMsgLogRavitaillementPousserFluxAck( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MsgLogRavitaillementPousserFluxAckConverter_, strName );
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
// Name: ENT_Tr::ConvertToAutomateOrderFormation
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomateOrderFormation ENT_Tr::ConvertToAutomateOrderFormation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomateOrderFormationConverter_, strName );
}

