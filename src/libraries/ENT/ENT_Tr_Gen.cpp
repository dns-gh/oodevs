// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ENT_Tr_Gen.h"


ENT_Tr::T_ConverterChangeHierarchyErrorCode ENT_Tr::ChangeHierarchyErrorCodeConverter_[] =
{
    T_ConverterChangeHierarchyErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeHierarchyErrorCode_NoError ),
    T_ConverterChangeHierarchyErrorCode( "error invalid pion", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid pion" ), eChangeHierarchyErrorCode_ErrorInvalidPion ),
    T_ConverterChangeHierarchyErrorCode( "error invalid automate", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate" ), eChangeHierarchyErrorCode_ErrorInvalidAutomate ),
    T_ConverterChangeHierarchyErrorCode( "error invalid automate tc2", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate tc2" ), eChangeHierarchyErrorCode_ErrorInvalidAutomateTc2 ),
    T_ConverterChangeHierarchyErrorCode( "error invalid automate maintenance", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate maintenance" ), eChangeHierarchyErrorCode_ErrorInvalidAutomateMaintenance ),
    T_ConverterChangeHierarchyErrorCode( "error invalid automate sante", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate sante" ), eChangeHierarchyErrorCode_ErrorInvalidAutomateSante ),
    T_ConverterChangeHierarchyErrorCode( "error invalid automate ravitaillement", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate ravitaillement" ), eChangeHierarchyErrorCode_ErrorInvalidAutomateRavitaillement ),
    T_ConverterChangeHierarchyErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eChangeHierarchyErrorCode_ErrorUnitSurrendered ),
    T_ConverterChangeHierarchyErrorCode( "error invalid camp", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid camp" ), eChangeHierarchyErrorCode_ErrorInvalidCamp ),
    T_ConverterChangeHierarchyErrorCode( "error invalid groupe connaissance", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid groupe connaissance" ), eChangeHierarchyErrorCode_ErrorInvalidGroupeConnaissance ),
    T_ConverterChangeHierarchyErrorCode( "error camps incompatibles", QT_TRANSLATE_NOOP( "ENT_Tr", "error camps incompatibles" ), eChangeHierarchyErrorCode_ErrorCampsIncompatibles ),
    T_ConverterChangeHierarchyErrorCode( "", "", (E_ChangeHierarchyErrorCode)-1 )
};

ENT_Tr::T_ConverterLocationType ENT_Tr::LocationTypeConverter_[] =
{
    T_ConverterLocationType( "circle", QT_TRANSLATE_NOOP( "ENT_Tr", "circle" ), eLocationType_Circle ),
    T_ConverterLocationType( "ellipse", QT_TRANSLATE_NOOP( "ENT_Tr", "ellipse" ), eLocationType_Ellipse ),
    T_ConverterLocationType( "line", QT_TRANSLATE_NOOP( "ENT_Tr", "line" ), eLocationType_Line ),
    T_ConverterLocationType( "polygon", QT_TRANSLATE_NOOP( "ENT_Tr", "polygon" ), eLocationType_Polygon ),
    T_ConverterLocationType( "point", QT_TRANSLATE_NOOP( "ENT_Tr", "point" ), eLocationType_Point ),
    T_ConverterLocationType( "sector", QT_TRANSLATE_NOOP( "ENT_Tr", "sector" ), eLocationType_Sector ),
    T_ConverterLocationType( "", "", (E_LocationType)-1 )
};

ENT_Tr::T_ConverterDotationFamily ENT_Tr::DotationFamilyConverter_[] =
{
    T_ConverterDotationFamily( "munition", QT_TRANSLATE_NOOP( "ENT_Tr", "ammunition" ), eDotationFamily_Munition ),
    T_ConverterDotationFamily( "carburant", QT_TRANSLATE_NOOP( "ENT_Tr", "fuel" ), eDotationFamily_Carburant ),
    T_ConverterDotationFamily( "explosif", QT_TRANSLATE_NOOP( "ENT_Tr", "explosive" ), eDotationFamily_Explosif ),
    T_ConverterDotationFamily( "mine", QT_TRANSLATE_NOOP( "ENT_Tr", "mine" ), eDotationFamily_Mine ),
    T_ConverterDotationFamily( "barbele", QT_TRANSLATE_NOOP( "ENT_Tr", "barbed wire" ), eDotationFamily_Barbele ),
    T_ConverterDotationFamily( "piece", QT_TRANSLATE_NOOP( "ENT_Tr", "parts" ), eDotationFamily_Piece ),
    T_ConverterDotationFamily( "ration", QT_TRANSLATE_NOOP( "ENT_Tr", "food/water" ), eDotationFamily_Ration ),
    T_ConverterDotationFamily( "", "", (E_DotationFamily)-1 )
};

ENT_Tr::T_ConverterAmmunitionFamily ENT_Tr::AmmunitionFamilyConverter_[] =
{
    T_ConverterAmmunitionFamily( "obus", QT_TRANSLATE_NOOP( "ENT_Tr", "shell" ), eAmmunitionFamily_Obus ),
    T_ConverterAmmunitionFamily( "missile air", QT_TRANSLATE_NOOP( "ENT_Tr", "air missile" ), eAmmunitionFamily_MissileAir ),
    T_ConverterAmmunitionFamily( "missile sol", QT_TRANSLATE_NOOP( "ENT_Tr", "missile" ), eAmmunitionFamily_MissileSol ),
    T_ConverterAmmunitionFamily( "mitraille", QT_TRANSLATE_NOOP( "ENT_Tr", "bullet" ), eAmmunitionFamily_Mitraille ),
    T_ConverterAmmunitionFamily( "", "", (E_AmmunitionFamily)-1 )
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
    T_ConverterHumanWound( "non blesse", QT_TRANSLATE_NOOP( "ENT_Tr", "unwounded" ), eHumanWound_NonBlesse ),
    T_ConverterHumanWound( "mort", QT_TRANSLATE_NOOP( "ENT_Tr", "dead" ), eHumanWound_Mort ),
    T_ConverterHumanWound( "blesse urgence 1", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded seriousness level 1" ), eHumanWound_BlesseUrgence1 ),
    T_ConverterHumanWound( "blesse urgence 2", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded seriousness level 2" ), eHumanWound_BlesseUrgence2 ),
    T_ConverterHumanWound( "blesse urgence 3", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded seriousness level 3" ), eHumanWound_BlesseUrgence3 ),
    T_ConverterHumanWound( "blesse urgence extreme", QT_TRANSLATE_NOOP( "ENT_Tr", "wounded extreme seriousness" ), eHumanWound_BlesseUrgenceExtreme ),
    T_ConverterHumanWound( "", "", (E_HumanWound)-1 )
};

ENT_Tr::T_ConverterHumanRank ENT_Tr::HumanRankConverter_[] =
{
    T_ConverterHumanRank( "officier", QT_TRANSLATE_NOOP( "ENT_Tr", "officer" ), eHumanRank_Officier ),
    T_ConverterHumanRank( "sous officer", QT_TRANSLATE_NOOP( "ENT_Tr", "warrant officer" ), eHumanRank_SousOfficer ),
    T_ConverterHumanRank( "mdr", QT_TRANSLATE_NOOP( "ENT_Tr", "trooper" ), eHumanRank_Mdr ),
    T_ConverterHumanRank( "", "", (E_HumanRank)-1 )
};

ENT_Tr::T_ConverterDiplomacy ENT_Tr::DiplomacyConverter_[] =
{
    T_ConverterDiplomacy( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "unknown" ), eDiplomacy_Inconnu ),
    T_ConverterDiplomacy( "ami", QT_TRANSLATE_NOOP( "ENT_Tr", "friendly" ), eDiplomacy_Ami ),
    T_ConverterDiplomacy( "ennemi", QT_TRANSLATE_NOOP( "ENT_Tr", "enemy" ), eDiplomacy_Ennemi ),
    T_ConverterDiplomacy( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutral" ), eDiplomacy_Neutre ),
    T_ConverterDiplomacy( "", "", (E_Diplomacy)-1 )
};

ENT_Tr::T_ConverterChangeDiplomacyErrorCode ENT_Tr::ChangeDiplomacyErrorCodeConverter_[] =
{
    T_ConverterChangeDiplomacyErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeDiplomacyErrorCode_NoError ),
    T_ConverterChangeDiplomacyErrorCode( "error invalid camp", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid camp" ), eChangeDiplomacyErrorCode_ErrorInvalidCamp ),
    T_ConverterChangeDiplomacyErrorCode( "", "", (E_ChangeDiplomacyErrorCode)-1 )
};

ENT_Tr::T_ConverterForceRatioStatus ENT_Tr::ForceRatioStatusConverter_[] =
{
    T_ConverterForceRatioStatus( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eForceRatioStatus_None ),
    T_ConverterForceRatioStatus( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "even" ), eForceRatioStatus_Neutre ),
    T_ConverterForceRatioStatus( "favorable", QT_TRANSLATE_NOOP( "ENT_Tr", "high" ), eForceRatioStatus_Favorable ),
    T_ConverterForceRatioStatus( "defavorable", QT_TRANSLATE_NOOP( "ENT_Tr", "low" ), eForceRatioStatus_Defavorable ),
    T_ConverterForceRatioStatus( "", "", (E_ForceRatioStatus)-1 )
};

ENT_Tr::T_ConverterMeetingEngagementStatus ENT_Tr::MeetingEngagementStatusConverter_[] =
{
    T_ConverterMeetingEngagementStatus( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eMeetingEngagementStatus_None ),
    T_ConverterMeetingEngagementStatus( "etat esquive", QT_TRANSLATE_NOOP( "ENT_Tr", "withdrawing" ), eMeetingEngagementStatus_EtatEsquive ),
    T_ConverterMeetingEngagementStatus( "etat fixe", QT_TRANSLATE_NOOP( "ENT_Tr", "fixed" ), eMeetingEngagementStatus_EtatFixe ),
    T_ConverterMeetingEngagementStatus( "etat poursuite mission", QT_TRANSLATE_NOOP( "ENT_Tr", "carrying out mission" ), eMeetingEngagementStatus_EtatPoursuiteMission ),
    T_ConverterMeetingEngagementStatus( "", "", (E_MeetingEngagementStatus)-1 )
};

ENT_Tr::T_ConverterOperationalStatus ENT_Tr::OperationalStatusConverter_[] =
{
    T_ConverterOperationalStatus( "detruit totalement", QT_TRANSLATE_NOOP( "ENT_Tr", "totally destroyed" ), eOperationalStatus_DetruitTotalement ),
    T_ConverterOperationalStatus( "detruit tactiquement", QT_TRANSLATE_NOOP( "ENT_Tr", "tactically destroyed" ), eOperationalStatus_DetruitTactiquement ),
    T_ConverterOperationalStatus( "operationnel", QT_TRANSLATE_NOOP( "ENT_Tr", "operational" ), eOperationalStatus_Operationnel ),
    T_ConverterOperationalStatus( "", "", (E_OperationalStatus)-1 )
};

ENT_Tr::T_ConverterRoe ENT_Tr::RoeConverter_[] =
{
    T_ConverterRoe( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoe_None ),
    T_ConverterRoe( "tir libre", QT_TRANSLATE_NOOP( "ENT_Tr", "free fire" ), eRoe_TirLibre ),
    T_ConverterRoe( "tir sur riposte", QT_TRANSLATE_NOOP( "ENT_Tr", "retaliation only" ), eRoe_TirSurRiposte ),
    T_ConverterRoe( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "forbidden fire" ), eRoe_TirInterdit ),
    T_ConverterRoe( "", "", (E_Roe)-1 )
};

ENT_Tr::T_ConverterUnitErrorCode ENT_Tr::UnitErrorCodeConverter_[] =
{
    T_ConverterUnitErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eUnitErrorCode_NoError ),
    T_ConverterUnitErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), eUnitErrorCode_ErrorInvalidUnit ),
    T_ConverterUnitErrorCode( "error automate embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "error automate embraye" ), eUnitErrorCode_ErrorAutomateEmbraye ),
    T_ConverterUnitErrorCode( "error invalid attribute", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid attribute" ), eUnitErrorCode_ErrorInvalidAttribute ),
    T_ConverterUnitErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eUnitErrorCode_ErrorUnitSurrendered ),
    T_ConverterUnitErrorCode( "", "", (E_UnitErrorCode)-1 )
};

ENT_Tr::T_ConverterUnitPosture ENT_Tr::UnitPostureConverter_[] =
{
    T_ConverterUnitPosture( "posture mouvement", QT_TRANSLATE_NOOP( "ENT_Tr", "moving" ), eUnitPosture_PostureMouvement ),
    T_ConverterUnitPosture( "posture mouvement discret", QT_TRANSLATE_NOOP( "ENT_Tr", "infiltrating" ), eUnitPosture_PostureMouvementDiscret ),
    T_ConverterUnitPosture( "posture arret", QT_TRANSLATE_NOOP( "ENT_Tr", "stopping" ), eUnitPosture_PostureArret ),
    T_ConverterUnitPosture( "posture poste reflexe", QT_TRANSLATE_NOOP( "ENT_Tr", "stopped" ), eUnitPosture_PosturePosteReflexe ),
    T_ConverterUnitPosture( "posture poste", QT_TRANSLATE_NOOP( "ENT_Tr", "parked" ), eUnitPosture_PosturePoste ),
    T_ConverterUnitPosture( "posture poste amenage", QT_TRANSLATE_NOOP( "ENT_Tr", "parked on self-prepared area" ), eUnitPosture_PosturePosteAmenage ),
    T_ConverterUnitPosture( "posture poste prepare genie", QT_TRANSLATE_NOOP( "ENT_Tr", "parked on engineer-prepared area" ), eUnitPosture_PosturePostePrepareGenie ),
    T_ConverterUnitPosture( "", "", (E_UnitPosture)-1 )
};

ENT_Tr::T_ConverterFireAvailability ENT_Tr::FireAvailabilityConverter_[] =
{
    T_ConverterFireAvailability( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eFireAvailability_None ),
    T_ConverterFireAvailability( "pret au tir", QT_TRANSLATE_NOOP( "ENT_Tr", "ready to fire" ), eFireAvailability_PretAuTir ),
    T_ConverterFireAvailability( "indisponible", QT_TRANSLATE_NOOP( "ENT_Tr", "unavailable" ), eFireAvailability_Indisponible ),
    T_ConverterFireAvailability( "", "", (E_FireAvailability)-1 )
};

ENT_Tr::T_ConverterPopulationRoe ENT_Tr::PopulationRoeConverter_[] =
{
    T_ConverterPopulationRoe( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), ePopulationRoe_None ),
    T_ConverterPopulationRoe( "emploi force interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "force shall not be used" ), ePopulationRoe_EmploiForceInterdit ),
    T_ConverterPopulationRoe( "maintien a distance par moyens non letaux", QT_TRANSLATE_NOOP( "ENT_Tr", "non lethal weapons use allowed" ), ePopulationRoe_MaintienADistanceParMoyensNonLetaux ),
    T_ConverterPopulationRoe( "dispersion par moyens de defense actifs", QT_TRANSLATE_NOOP( "ENT_Tr", "active defense allowed" ), ePopulationRoe_DispersionParMoyensDeDefenseActifs ),
    T_ConverterPopulationRoe( "armes letales autorisees", QT_TRANSLATE_NOOP( "ENT_Tr", "lethal weapon use allowed" ), ePopulationRoe_ArmesLetalesAutorisees ),
    T_ConverterPopulationRoe( "", "", (E_PopulationRoe)-1 )
};

ENT_Tr::T_ConverterUnitTiredness ENT_Tr::UnitTirednessConverter_[] =
{
    T_ConverterUnitTiredness( "normal", QT_TRANSLATE_NOOP( "ENT_Tr", "not tired" ), eUnitTiredness_Normal ),
    T_ConverterUnitTiredness( "fatigue", QT_TRANSLATE_NOOP( "ENT_Tr", "tired" ), eUnitTiredness_Fatigue ),
    T_ConverterUnitTiredness( "epuise", QT_TRANSLATE_NOOP( "ENT_Tr", "exhausted" ), eUnitTiredness_Epuise ),
    T_ConverterUnitTiredness( "", "", (E_UnitTiredness)-1 )
};

ENT_Tr::T_ConverterUnitMorale ENT_Tr::UnitMoraleConverter_[] =
{
    T_ConverterUnitMorale( "fanatique", QT_TRANSLATE_NOOP( "ENT_Tr", "fanatical" ), eUnitMorale_Fanatique ),
    T_ConverterUnitMorale( "bon", QT_TRANSLATE_NOOP( "ENT_Tr", "high" ), eUnitMorale_Bon ),
    T_ConverterUnitMorale( "moyen", QT_TRANSLATE_NOOP( "ENT_Tr", "standard" ), eUnitMorale_Moyen ),
    T_ConverterUnitMorale( "mauvais", QT_TRANSLATE_NOOP( "ENT_Tr", "low" ), eUnitMorale_Mauvais ),
    T_ConverterUnitMorale( "", "", (E_UnitMorale)-1 )
};

ENT_Tr::T_ConverterUnitExperience ENT_Tr::UnitExperienceConverter_[] =
{
    T_ConverterUnitExperience( "veteran", QT_TRANSLATE_NOOP( "ENT_Tr", "veteran" ), eUnitExperience_Veteran ),
    T_ConverterUnitExperience( "experimente", QT_TRANSLATE_NOOP( "ENT_Tr", "experienced" ), eUnitExperience_Experimente ),
    T_ConverterUnitExperience( "conscrit", QT_TRANSLATE_NOOP( "ENT_Tr", "newbie" ), eUnitExperience_Conscrit ),
    T_ConverterUnitExperience( "", "", (E_UnitExperience)-1 )
};

ENT_Tr::T_ConverterAutomatMode ENT_Tr::AutomatModeConverter_[] =
{
    T_ConverterAutomatMode( "embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "engaged" ), eAutomatMode_Embraye ),
    T_ConverterAutomatMode( "debraye", QT_TRANSLATE_NOOP( "ENT_Tr", "disengaged" ), eAutomatMode_Debraye ),
    T_ConverterAutomatMode( "", "", (E_AutomatMode)-1 )
};

ENT_Tr::T_ConverterSetAutomatModeErrorCode ENT_Tr::SetAutomatModeErrorCodeConverter_[] =
{
    T_ConverterSetAutomatModeErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eSetAutomatModeErrorCode_NoError ),
    T_ConverterSetAutomatModeErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), eSetAutomatModeErrorCode_ErrorInvalidUnit ),
    T_ConverterSetAutomatModeErrorCode( "error not allowed", QT_TRANSLATE_NOOP( "ENT_Tr", "error not allowed" ), eSetAutomatModeErrorCode_ErrorNotAllowed ),
    T_ConverterSetAutomatModeErrorCode( "", "", (E_SetAutomatModeErrorCode)-1 )
};

ENT_Tr::T_ConverterControlErrorCode ENT_Tr::ControlErrorCodeConverter_[] =
{
    T_ConverterControlErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eControlErrorCode_NoError ),
    T_ConverterControlErrorCode( "error already started", QT_TRANSLATE_NOOP( "ENT_Tr", "error already started" ), eControlErrorCode_ErrorAlreadyStarted ),
    T_ConverterControlErrorCode( "error not started", QT_TRANSLATE_NOOP( "ENT_Tr", "error not started" ), eControlErrorCode_ErrorNotStarted ),
    T_ConverterControlErrorCode( "error not paused", QT_TRANSLATE_NOOP( "ENT_Tr", "error not paused" ), eControlErrorCode_ErrorNotPaused ),
    T_ConverterControlErrorCode( "error already paused", QT_TRANSLATE_NOOP( "ENT_Tr", "error already paused" ), eControlErrorCode_ErrorAlreadyPaused ),
    T_ConverterControlErrorCode( "error invalid time factor", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid time factor" ), eControlErrorCode_ErrorInvalidTimeFactor ),
    T_ConverterControlErrorCode( "", "", (E_ControlErrorCode)-1 )
};

ENT_Tr::T_ConverterSimulationState ENT_Tr::SimulationStateConverter_[] =
{
    T_ConverterSimulationState( "running", QT_TRANSLATE_NOOP( "ENT_Tr", "running" ), eSimulationState_Running ),
    T_ConverterSimulationState( "paused", QT_TRANSLATE_NOOP( "ENT_Tr", "paused" ), eSimulationState_Paused ),
    T_ConverterSimulationState( "stopped", QT_TRANSLATE_NOOP( "ENT_Tr", "stopped" ), eSimulationState_Stopped ),
    T_ConverterSimulationState( "", "", (E_SimulationState)-1 )
};

ENT_Tr::T_ConverterPrecipitationType ENT_Tr::PrecipitationTypeConverter_[] =
{
    T_ConverterPrecipitationType( "pas de precipitation", QT_TRANSLATE_NOOP( "ENT_Tr", "no rainfall" ), ePrecipitationType_PasDePrecipitation ),
    T_ConverterPrecipitationType( "tempete sable", QT_TRANSLATE_NOOP( "ENT_Tr", "sand storm" ), ePrecipitationType_TempeteSable ),
    T_ConverterPrecipitationType( "brouillard", QT_TRANSLATE_NOOP( "ENT_Tr", "fog" ), ePrecipitationType_Brouillard ),
    T_ConverterPrecipitationType( "crachin", QT_TRANSLATE_NOOP( "ENT_Tr", "drizzle" ), ePrecipitationType_Crachin ),
    T_ConverterPrecipitationType( "pluie", QT_TRANSLATE_NOOP( "ENT_Tr", "rain" ), ePrecipitationType_Pluie ),
    T_ConverterPrecipitationType( "neige", QT_TRANSLATE_NOOP( "ENT_Tr", "snow" ), ePrecipitationType_Neige ),
    T_ConverterPrecipitationType( "", "", (E_PrecipitationType)-1 )
};

ENT_Tr::T_ConverterFireEffectType ENT_Tr::FireEffectTypeConverter_[] =
{
    T_ConverterFireEffectType( "fumigene", QT_TRANSLATE_NOOP( "ENT_Tr", "smoke" ), eFireEffectType_Fumigene ),
    T_ConverterFireEffectType( "eclairant", QT_TRANSLATE_NOOP( "ENT_Tr", "illumination shell" ), eFireEffectType_Eclairant ),
    T_ConverterFireEffectType( "", "", (E_FireEffectType)-1 )
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

ENT_Tr::T_ConverterLimaType ENT_Tr::LimaTypeConverter_[] =
{
    T_ConverterLimaType( "ligne debouche", QT_TRANSLATE_NOOP( "ENT_Tr", "line of departure" ), eLimaType_LigneDebouche ),
    T_ConverterLimaType( "ligne changement attitude", QT_TRANSLATE_NOOP( "ENT_Tr", "posture switching line" ), eLimaType_LigneChangementAttitude ),
    T_ConverterLimaType( "ligne coordination", QT_TRANSLATE_NOOP( "ENT_Tr", "coordination line" ), eLimaType_LigneCoordination ),
    T_ConverterLimaType( "ligne interdire", QT_TRANSLATE_NOOP( "ENT_Tr", "deny line" ), eLimaType_LigneInterdire ),
    T_ConverterLimaType( "ligne objectif", QT_TRANSLATE_NOOP( "ENT_Tr", "line of objectives" ), eLimaType_LigneObjectif ),
    T_ConverterLimaType( "ligne coup arret", QT_TRANSLATE_NOOP( "ENT_Tr", "block line" ), eLimaType_LigneCoupArret ),
    T_ConverterLimaType( "ligne recueil", QT_TRANSLATE_NOOP( "ENT_Tr", "rearward line" ), eLimaType_LigneRecueil ),
    T_ConverterLimaType( "ligne debut mission", QT_TRANSLATE_NOOP( "ENT_Tr", "starting line" ), eLimaType_LigneDebutMission ),
    T_ConverterLimaType( "ligne fin mission", QT_TRANSLATE_NOOP( "ENT_Tr", "finish line" ), eLimaType_LigneFinMission ),
    T_ConverterLimaType( "ligne identification accueil", QT_TRANSLATE_NOOP( "ENT_Tr", "identification and reception line" ), eLimaType_LigneIdentificationAccueil ),
    T_ConverterLimaType( "", "", (E_LimaType)-1 )
};

ENT_Tr::T_ConverterLogMaintenanceRegimeTravail ENT_Tr::LogMaintenanceRegimeTravailConverter_[] =
{
    T_ConverterLogMaintenanceRegimeTravail( "regime 1", QT_TRANSLATE_NOOP( "ENT_Tr", "shift type 1" ), eLogMaintenanceRegimeTravail_Regime1 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 2", QT_TRANSLATE_NOOP( "ENT_Tr", "shift type 2" ), eLogMaintenanceRegimeTravail_Regime2 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 3", QT_TRANSLATE_NOOP( "ENT_Tr", "shift type 3" ), eLogMaintenanceRegimeTravail_Regime3 ),
    T_ConverterLogMaintenanceRegimeTravail( "regime 4", QT_TRANSLATE_NOOP( "ENT_Tr", "shift type 4" ), eLogMaintenanceRegimeTravail_Regime4 ),
    T_ConverterLogMaintenanceRegimeTravail( "", "", (E_LogMaintenanceRegimeTravail)-1 )
};

ENT_Tr::T_ConverterLogMaintenanceHandlingStatus ENT_Tr::LogMaintenanceHandlingStatusConverter_[] =
{
    T_ConverterLogMaintenanceHandlingStatus( "deplacement vers chaine", QT_TRANSLATE_NOOP( "ENT_Tr", "moving toward logistic system" ), eLogMaintenanceHandlingStatus_DeplacementVersChaine ),
    T_ConverterLogMaintenanceHandlingStatus( "attente disponibilite remorqueur", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for available tow-truck" ), eLogMaintenanceHandlingStatus_AttenteDisponibiliteRemorqueur ),
    T_ConverterLogMaintenanceHandlingStatus( "remorqueur deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck moving to target" ), eLogMaintenanceHandlingStatus_RemorqueurDeplacementAller ),
    T_ConverterLogMaintenanceHandlingStatus( "remorqueur chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck loading" ), eLogMaintenanceHandlingStatus_RemorqueurChargement ),
    T_ConverterLogMaintenanceHandlingStatus( "remorqueur deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck moving from target" ), eLogMaintenanceHandlingStatus_RemorqueurDeplacementRetour ),
    T_ConverterLogMaintenanceHandlingStatus( "remorqueur dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "tow-truck unloading" ), eLogMaintenanceHandlingStatus_RemorqueurDechargement ),
    T_ConverterLogMaintenanceHandlingStatus( "diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "diagnostic" ), eLogMaintenanceHandlingStatus_Diagnostique ),
    T_ConverterLogMaintenanceHandlingStatus( "attente prise en charge par niveau superieur", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting to be handled by higher level" ), eLogMaintenanceHandlingStatus_AttentePriseEnChargeParNiveauSuperieur ),
    T_ConverterLogMaintenanceHandlingStatus( "attente disponibilite pieces", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for available parts" ), eLogMaintenanceHandlingStatus_AttenteDisponibilitePieces ),
    T_ConverterLogMaintenanceHandlingStatus( "attente disponibilite reparateur", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for available repairer" ), eLogMaintenanceHandlingStatus_AttenteDisponibiliteReparateur ),
    T_ConverterLogMaintenanceHandlingStatus( "reparation", QT_TRANSLATE_NOOP( "ENT_Tr", "repairing" ), eLogMaintenanceHandlingStatus_Reparation ),
    T_ConverterLogMaintenanceHandlingStatus( "retour pion", QT_TRANSLATE_NOOP( "ENT_Tr", "unit returning" ), eLogMaintenanceHandlingStatus_RetourPion ),
    T_ConverterLogMaintenanceHandlingStatus( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogMaintenanceHandlingStatus_Termine ),
    T_ConverterLogMaintenanceHandlingStatus( "", "", (E_LogMaintenanceHandlingStatus)-1 )
};

ENT_Tr::T_ConverterLogSupplyHandlingStatus ENT_Tr::LogSupplyHandlingStatusConverter_[] =
{
    T_ConverterLogSupplyHandlingStatus( "convoi en attente camions", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy waiting for trucks" ), eLogSupplyHandlingStatus_ConvoiEnAttenteCamions ),
    T_ConverterLogSupplyHandlingStatus( "convoi constitution", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy setting up" ), eLogSupplyHandlingStatus_ConvoiConstitution ),
    T_ConverterLogSupplyHandlingStatus( "convoi deplacement vers point chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy moving to loading point" ), eLogSupplyHandlingStatus_ConvoiDeplacementVersPointChargement ),
    T_ConverterLogSupplyHandlingStatus( "convoi chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy loading" ), eLogSupplyHandlingStatus_ConvoiChargement ),
    T_ConverterLogSupplyHandlingStatus( "convoi deplacement vers point dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy moving to unloading point" ), eLogSupplyHandlingStatus_ConvoiDeplacementVersPointDechargement ),
    T_ConverterLogSupplyHandlingStatus( "convoi dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy unloading" ), eLogSupplyHandlingStatus_ConvoiDechargement ),
    T_ConverterLogSupplyHandlingStatus( "convoi deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "convoy moving back" ), eLogSupplyHandlingStatus_ConvoiDeplacementRetour ),
    T_ConverterLogSupplyHandlingStatus( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogSupplyHandlingStatus_Termine ),
    T_ConverterLogSupplyHandlingStatus( "", "", (E_LogSupplyHandlingStatus)-1 )
};

ENT_Tr::T_ConverterMsgLogSupplyChangeQuotasAck ENT_Tr::MsgLogSupplyChangeQuotasAckConverter_[] =
{
    T_ConverterMsgLogSupplyChangeQuotasAck( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eMsgLogSupplyChangeQuotasAck_NoError ),
    T_ConverterMsgLogSupplyChangeQuotasAck( "error invalid donneur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid donneur" ), eMsgLogSupplyChangeQuotasAck_ErrorInvalidDonneur ),
    T_ConverterMsgLogSupplyChangeQuotasAck( "error invalid receveur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid receveur" ), eMsgLogSupplyChangeQuotasAck_ErrorInvalidReceveur ),
    T_ConverterMsgLogSupplyChangeQuotasAck( "", "", (E_MsgLogSupplyChangeQuotasAck)-1 )
};

ENT_Tr::T_ConverterMsgLogSupplyPushFlowAck ENT_Tr::MsgLogSupplyPushFlowAckConverter_[] =
{
    T_ConverterMsgLogSupplyPushFlowAck( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eMsgLogSupplyPushFlowAck_NoError ),
    T_ConverterMsgLogSupplyPushFlowAck( "error invalid donneur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid donneur" ), eMsgLogSupplyPushFlowAck_ErrorInvalidDonneur ),
    T_ConverterMsgLogSupplyPushFlowAck( "error invalid receveur", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid receveur" ), eMsgLogSupplyPushFlowAck_ErrorInvalidReceveur ),
    T_ConverterMsgLogSupplyPushFlowAck( "", "", (E_MsgLogSupplyPushFlowAck)-1 )
};

ENT_Tr::T_ConverterLogMedicalHandlingStatus ENT_Tr::LogMedicalHandlingStatusConverter_[] =
{
    T_ConverterLogMedicalHandlingStatus( "attente disponibilite ambulance releve", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for evacuation ambulance" ), eLogMedicalHandlingStatus_AttenteDisponibiliteAmbulanceReleve ),
    T_ConverterLogMedicalHandlingStatus( "ambulance releve deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance coming" ), eLogMedicalHandlingStatus_AmbulanceReleveDeplacementAller ),
    T_ConverterLogMedicalHandlingStatus( "ambulance releve chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance loading" ), eLogMedicalHandlingStatus_AmbulanceReleveChargement ),
    T_ConverterLogMedicalHandlingStatus( "attente chargement complet ambulance releve", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for evacuation ambulance loading" ), eLogMedicalHandlingStatus_AttenteChargementCompletAmbulanceReleve ),
    T_ConverterLogMedicalHandlingStatus( "ambulance releve deplacement retour", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance going back" ), eLogMedicalHandlingStatus_AmbulanceReleveDeplacementRetour ),
    T_ConverterLogMedicalHandlingStatus( "ambulance releve dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation ambulance unloading" ), eLogMedicalHandlingStatus_AmbulanceReleveDechargement ),
    T_ConverterLogMedicalHandlingStatus( "attente disponibilite medecin pour diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for doctor" ), eLogMedicalHandlingStatus_AttenteDisponibiliteMedecinPourDiagnostique ),
    T_ConverterLogMedicalHandlingStatus( "diagnostique", QT_TRANSLATE_NOOP( "ENT_Tr", "diagnostic" ), eLogMedicalHandlingStatus_Diagnostique ),
    T_ConverterLogMedicalHandlingStatus( "recherche secteur tri", QT_TRANSLATE_NOOP( "ENT_Tr", "looking for triange area" ), eLogMedicalHandlingStatus_RechercheSecteurTri ),
    T_ConverterLogMedicalHandlingStatus( "attente disponibilite medecin pour tri", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for triage" ), eLogMedicalHandlingStatus_AttenteDisponibiliteMedecinPourTri ),
    T_ConverterLogMedicalHandlingStatus( "tri", QT_TRANSLATE_NOOP( "ENT_Tr", "triage" ), eLogMedicalHandlingStatus_Tri ),
    T_ConverterLogMedicalHandlingStatus( "recherche secteur soin", QT_TRANSLATE_NOOP( "ENT_Tr", "looking for treatment area" ), eLogMedicalHandlingStatus_RechercheSecteurSoin ),
    T_ConverterLogMedicalHandlingStatus( "attente disponibilite medecin pour soin", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for doctor (treatment)" ), eLogMedicalHandlingStatus_AttenteDisponibiliteMedecinPourSoin ),
    T_ConverterLogMedicalHandlingStatus( "soin", QT_TRANSLATE_NOOP( "ENT_Tr", "treatment" ), eLogMedicalHandlingStatus_Soin ),
    T_ConverterLogMedicalHandlingStatus( "hospitalisation", QT_TRANSLATE_NOOP( "ENT_Tr", "hospitalization" ), eLogMedicalHandlingStatus_Hospitalisation ),
    T_ConverterLogMedicalHandlingStatus( "attente disponibilite ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for collection ambulance" ), eLogMedicalHandlingStatus_AttenteDisponibiliteAmbulanceRamassage ),
    T_ConverterLogMedicalHandlingStatus( "ambulance ramassage chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "collection ambulance loading" ), eLogMedicalHandlingStatus_AmbulanceRamassageChargement ),
    T_ConverterLogMedicalHandlingStatus( "attente chargement complet ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for collection ambulance loading" ), eLogMedicalHandlingStatus_AttenteChargementCompletAmbulanceRamassage ),
    T_ConverterLogMedicalHandlingStatus( "ambulance ramassage deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "collection ambulance coming" ), eLogMedicalHandlingStatus_AmbulanceRamassageDeplacementAller ),
    T_ConverterLogMedicalHandlingStatus( "ambulance ramassage dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "collection ambulance unloading" ), eLogMedicalHandlingStatus_AmbulanceRamassageDechargement ),
    T_ConverterLogMedicalHandlingStatus( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogMedicalHandlingStatus_Termine ),
    T_ConverterLogMedicalHandlingStatus( "", "", (E_LogMedicalHandlingStatus)-1 )
};

ENT_Tr::T_ConverterDemolitionTargetType ENT_Tr::DemolitionTargetTypeConverter_[] =
{
    T_ConverterDemolitionTargetType( "preliminary", QT_TRANSLATE_NOOP( "ENT_Tr", "preliminary" ), eDemolitionTargetType_Preliminary ),
    T_ConverterDemolitionTargetType( "reserved", QT_TRANSLATE_NOOP( "ENT_Tr", "reserved" ), eDemolitionTargetType_Reserved ),
    T_ConverterDemolitionTargetType( "", "", (E_DemolitionTargetType)-1 )
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

ENT_Tr::T_ConverterReportType ENT_Tr::ReportTypeConverter_[] =
{
    T_ConverterReportType( "message", QT_TRANSLATE_NOOP( "ENT_Tr", "message" ), eReportType_Message ),
    T_ConverterReportType( "operationel", QT_TRANSLATE_NOOP( "ENT_Tr", "operational" ), eReportType_Operationel ),
    T_ConverterReportType( "evenement exceptionnel", QT_TRANSLATE_NOOP( "ENT_Tr", "event" ), eReportType_EvenementExceptionnel ),
    T_ConverterReportType( "warning", QT_TRANSLATE_NOOP( "ENT_Tr", "warning" ), eReportType_Warning ),
    T_ConverterReportType( "", "", (E_ReportType)-1 )
};

ENT_Tr::T_ConverterAutomatOrderFormation ENT_Tr::AutomatOrderFormationConverter_[] =
{
    T_ConverterAutomatOrderFormation( "un echelon", QT_TRANSLATE_NOOP( "ENT_Tr", "one echelon" ), eAutomatOrderFormation_UnEchelon ),
    T_ConverterAutomatOrderFormation( "deux echelons", QT_TRANSLATE_NOOP( "ENT_Tr", "two echelons" ), eAutomatOrderFormation_DeuxEchelons ),
    T_ConverterAutomatOrderFormation( "", "", (E_AutomatOrderFormation)-1 )
};

ENT_Tr::T_ConverterPopulationErrorCode ENT_Tr::PopulationErrorCodeConverter_[] =
{
    T_ConverterPopulationErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), ePopulationErrorCode_NoError ),
    T_ConverterPopulationErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), ePopulationErrorCode_ErrorInvalidUnit ),
    T_ConverterPopulationErrorCode( "error invalid attribute", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid attribute" ), ePopulationErrorCode_ErrorInvalidAttribute ),
    T_ConverterPopulationErrorCode( "", "", (E_PopulationErrorCode)-1 )
};

ENT_Tr::T_ConverterPopulationAttitude ENT_Tr::PopulationAttitudeConverter_[] =
{
    T_ConverterPopulationAttitude( "calme", QT_TRANSLATE_NOOP( "ENT_Tr", "quiet" ), ePopulationAttitude_Calme ),
    T_ConverterPopulationAttitude( "agitee", QT_TRANSLATE_NOOP( "ENT_Tr", "agitated" ), ePopulationAttitude_Agitee ),
    T_ConverterPopulationAttitude( "excitee", QT_TRANSLATE_NOOP( "ENT_Tr", "nervous" ), ePopulationAttitude_Excitee ),
    T_ConverterPopulationAttitude( "agressive", QT_TRANSLATE_NOOP( "ENT_Tr", "agressive" ), ePopulationAttitude_Agressive ),
    T_ConverterPopulationAttitude( "", "", (E_PopulationAttitude)-1 )
};




//-----------------------------------------------------------------------------
// Name: ENT_Tr::InitTranslations
// Created: AGR
//-----------------------------------------------------------------------------
void ENT_Tr::InitTranslations()
{
    InitTr( ChangeHierarchyErrorCodeConverter_, "ENT_Tr" );
    InitTr( LocationTypeConverter_, "ENT_Tr" );
    InitTr( DotationFamilyConverter_, "ENT_Tr" );
    InitTr( AmmunitionFamilyConverter_, "ENT_Tr" );
    InitTr( UnitIdentificationLevelConverter_, "ENT_Tr" );
    InitTr( NatureLevelConverter_, "ENT_Tr" );
    InitTr( HumanWoundConverter_, "ENT_Tr" );
    InitTr( HumanRankConverter_, "ENT_Tr" );
    InitTr( DiplomacyConverter_, "ENT_Tr" );
    InitTr( ChangeDiplomacyErrorCodeConverter_, "ENT_Tr" );
    InitTr( ForceRatioStatusConverter_, "ENT_Tr" );
    InitTr( MeetingEngagementStatusConverter_, "ENT_Tr" );
    InitTr( OperationalStatusConverter_, "ENT_Tr" );
    InitTr( RoeConverter_, "ENT_Tr" );
    InitTr( UnitErrorCodeConverter_, "ENT_Tr" );
    InitTr( UnitPostureConverter_, "ENT_Tr" );
    InitTr( FireAvailabilityConverter_, "ENT_Tr" );
    InitTr( PopulationRoeConverter_, "ENT_Tr" );
    InitTr( UnitTirednessConverter_, "ENT_Tr" );
    InitTr( UnitMoraleConverter_, "ENT_Tr" );
    InitTr( UnitExperienceConverter_, "ENT_Tr" );
    InitTr( AutomatModeConverter_, "ENT_Tr" );
    InitTr( SetAutomatModeErrorCodeConverter_, "ENT_Tr" );
    InitTr( ControlErrorCodeConverter_, "ENT_Tr" );
    InitTr( SimulationStateConverter_, "ENT_Tr" );
    InitTr( PrecipitationTypeConverter_, "ENT_Tr" );
    InitTr( FireEffectTypeConverter_, "ENT_Tr" );
    InitTr( InfoContextErrorCodeConverter_, "ENT_Tr" );
    InitTr( LimaTypeConverter_, "ENT_Tr" );
    InitTr( LogMaintenanceRegimeTravailConverter_, "ENT_Tr" );
    InitTr( LogMaintenanceHandlingStatusConverter_, "ENT_Tr" );
    InitTr( LogSupplyHandlingStatusConverter_, "ENT_Tr" );
    InitTr( MsgLogSupplyChangeQuotasAckConverter_, "ENT_Tr" );
    InitTr( MsgLogSupplyPushFlowAckConverter_, "ENT_Tr" );
    InitTr( LogMedicalHandlingStatusConverter_, "ENT_Tr" );
    InitTr( DemolitionTargetTypeConverter_, "ENT_Tr" );
    InitTr( ObjectErrorCodeConverter_, "ENT_Tr" );
    InitTr( OrderErrorCodeConverter_, "ENT_Tr" );
    InitTr( ReportTypeConverter_, "ENT_Tr" );
    InitTr( AutomatOrderFormationConverter_, "ENT_Tr" );
    InitTr( PopulationErrorCodeConverter_, "ENT_Tr" );
    InitTr( PopulationAttitudeConverter_, "ENT_Tr" );

}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromChangeHierarchyErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromChangeHierarchyErrorCode( E_ChangeHierarchyErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ChangeHierarchyErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLocationType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLocationType( E_LocationType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LocationTypeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromDotationFamily
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDotationFamily( E_DotationFamily nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DotationFamilyConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromAmmunitionFamily
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAmmunitionFamily( E_AmmunitionFamily nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AmmunitionFamilyConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromDiplomacy
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDiplomacy( E_Diplomacy nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DiplomacyConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromForceRatioStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromForceRatioStatus( E_ForceRatioStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ForceRatioStatusConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMeetingEngagementStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMeetingEngagementStatus( E_MeetingEngagementStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MeetingEngagementStatusConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromOperationalStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromOperationalStatus( E_OperationalStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( OperationalStatusConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromUnitErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitErrorCode( E_UnitErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitErrorCodeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromFireAvailability
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFireAvailability( E_FireAvailability nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( FireAvailabilityConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromPopulationRoe
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationRoe( E_PopulationRoe nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PopulationRoeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitTiredness
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitTiredness( E_UnitTiredness nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitTirednessConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitMorale
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitMorale( E_UnitMorale nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitMoraleConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromAutomatMode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomatMode( E_AutomatMode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AutomatModeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromSetAutomatModeErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromSetAutomatModeErrorCode( E_SetAutomatModeErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( SetAutomatModeErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromControlErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromControlErrorCode( E_ControlErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ControlErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromSimulationState
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromSimulationState( E_SimulationState nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( SimulationStateConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromFireEffectType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFireEffectType( E_FireEffectType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( FireEffectTypeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromLimaType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLimaType( E_LimaType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LimaTypeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromLogMaintenanceHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( E_LogMaintenanceHandlingStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogMaintenanceHandlingStatusConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLogSupplyHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogSupplyHandlingStatus( E_LogSupplyHandlingStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogSupplyHandlingStatusConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromLogMedicalHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogMedicalHandlingStatus( E_LogMedicalHandlingStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogMedicalHandlingStatusConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromDemolitionTargetType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDemolitionTargetType( E_DemolitionTargetType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DemolitionTargetTypeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromReportType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromReportType( E_ReportType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ReportTypeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromPopulationErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationErrorCode( E_PopulationErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PopulationErrorCodeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertToChangeHierarchyErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ChangeHierarchyErrorCode ENT_Tr::ConvertToChangeHierarchyErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ChangeHierarchyErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLocationType
// Created: AGR
//-----------------------------------------------------------------------------
E_LocationType ENT_Tr::ConvertToLocationType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LocationTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToDotationFamily
// Created: AGR
//-----------------------------------------------------------------------------
E_DotationFamily ENT_Tr::ConvertToDotationFamily( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DotationFamilyConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAmmunitionFamily
// Created: AGR
//-----------------------------------------------------------------------------
E_AmmunitionFamily ENT_Tr::ConvertToAmmunitionFamily( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AmmunitionFamilyConverter_, strName );
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
// Name: ENT_Tr::ConvertToDiplomacy
// Created: AGR
//-----------------------------------------------------------------------------
E_Diplomacy ENT_Tr::ConvertToDiplomacy( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DiplomacyConverter_, strName );
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
// Name: ENT_Tr::ConvertToForceRatioStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_ForceRatioStatus ENT_Tr::ConvertToForceRatioStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ForceRatioStatusConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMeetingEngagementStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_MeetingEngagementStatus ENT_Tr::ConvertToMeetingEngagementStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MeetingEngagementStatusConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToOperationalStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_OperationalStatus ENT_Tr::ConvertToOperationalStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( OperationalStatusConverter_, strName );
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
// Name: ENT_Tr::ConvertToUnitErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitErrorCode ENT_Tr::ConvertToUnitErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitErrorCodeConverter_, strName );
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
// Name: ENT_Tr::ConvertToFireAvailability
// Created: AGR
//-----------------------------------------------------------------------------
E_FireAvailability ENT_Tr::ConvertToFireAvailability( const std::string& strName )
{
    return ENT_Tr::FindInConverter( FireAvailabilityConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationRoe
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationRoe ENT_Tr::ConvertToPopulationRoe( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PopulationRoeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitTiredness
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitTiredness ENT_Tr::ConvertToUnitTiredness( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitTirednessConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitMorale
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitMorale ENT_Tr::ConvertToUnitMorale( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitMoraleConverter_, strName );
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
// Name: ENT_Tr::ConvertToAutomatMode
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomatMode ENT_Tr::ConvertToAutomatMode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomatModeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToSetAutomatModeErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_SetAutomatModeErrorCode ENT_Tr::ConvertToSetAutomatModeErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SetAutomatModeErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToControlErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ControlErrorCode ENT_Tr::ConvertToControlErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ControlErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToSimulationState
// Created: AGR
//-----------------------------------------------------------------------------
E_SimulationState ENT_Tr::ConvertToSimulationState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SimulationStateConverter_, strName );
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
// Name: ENT_Tr::ConvertToFireEffectType
// Created: AGR
//-----------------------------------------------------------------------------
E_FireEffectType ENT_Tr::ConvertToFireEffectType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( FireEffectTypeConverter_, strName );
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
// Name: ENT_Tr::ConvertToLimaType
// Created: AGR
//-----------------------------------------------------------------------------
E_LimaType ENT_Tr::ConvertToLimaType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LimaTypeConverter_, strName );
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
// Name: ENT_Tr::ConvertToLogMaintenanceHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_LogMaintenanceHandlingStatus ENT_Tr::ConvertToLogMaintenanceHandlingStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogMaintenanceHandlingStatusConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLogSupplyHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_LogSupplyHandlingStatus ENT_Tr::ConvertToLogSupplyHandlingStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogSupplyHandlingStatusConverter_, strName );
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
// Name: ENT_Tr::ConvertToLogMedicalHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_LogMedicalHandlingStatus ENT_Tr::ConvertToLogMedicalHandlingStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogMedicalHandlingStatusConverter_, strName );
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
// Name: ENT_Tr::ConvertToDemolitionTargetType
// Created: AGR
//-----------------------------------------------------------------------------
E_DemolitionTargetType ENT_Tr::ConvertToDemolitionTargetType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DemolitionTargetTypeConverter_, strName );
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
// Name: ENT_Tr::ConvertToReportType
// Created: AGR
//-----------------------------------------------------------------------------
E_ReportType ENT_Tr::ConvertToReportType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ReportTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAutomatOrderFormation
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomatOrderFormation ENT_Tr::ConvertToAutomatOrderFormation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomatOrderFormationConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationErrorCode ENT_Tr::ConvertToPopulationErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PopulationErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationAttitude
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationAttitude ENT_Tr::ConvertToPopulationAttitude( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PopulationAttitudeConverter_, strName );
}



