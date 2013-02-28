// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "ENT_Tr_Gen.h"

ENT_Tr::T_ConverterLocationType ENT_Tr::LocationTypeConverter_[] =
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

ENT_Tr::T_ConverterDotationFamily ENT_Tr::DotationFamilyConverter_[] =
{
    T_ConverterDotationFamily( "munition", QT_TRANSLATE_NOOP( "ENT_Tr", "Ammunition" ), eDotationFamily_Munition ),
    T_ConverterDotationFamily( "carburant", QT_TRANSLATE_NOOP( "ENT_Tr", "Fuel" ), eDotationFamily_Carburant ),
    T_ConverterDotationFamily( "explosif", QT_TRANSLATE_NOOP( "ENT_Tr", "Explosive" ), eDotationFamily_Explosif ),
    T_ConverterDotationFamily( "mine", QT_TRANSLATE_NOOP( "ENT_Tr", "Mine" ), eDotationFamily_Mine ),
    T_ConverterDotationFamily( "barbele", QT_TRANSLATE_NOOP( "ENT_Tr", "Barbed wire" ), eDotationFamily_Barbele ),
    T_ConverterDotationFamily( "piece", QT_TRANSLATE_NOOP( "ENT_Tr", "Parts" ), eDotationFamily_Piece ),
    T_ConverterDotationFamily( "ration", QT_TRANSLATE_NOOP( "ENT_Tr", "Food/water" ), eDotationFamily_Ration ),
    T_ConverterDotationFamily( "agent extincteur", QT_TRANSLATE_NOOP( "ENT_Tr", "Extinguisher agent" ), eDotationFamily_AgentExtincteur ),
    T_ConverterDotationFamily( "energie", QT_TRANSLATE_NOOP( "ENT_Tr", "Energy" ), eDotationFamily_Energy ),
    T_ConverterDotationFamily( "funeraire", QT_TRANSLATE_NOOP( "ENT_Tr", "Funeral" ), eDotationFamily_Funeraire ),
    T_ConverterDotationFamily( "", "", (E_DotationFamily)-1 )
};

ENT_Tr::T_ConverterAmmunitionType ENT_Tr::AmmunitionTypeConverter_[] =
{
    T_ConverterAmmunitionType( "Obus",       QT_TRANSLATE_NOOP( "ENT_Tr", "Shell" ),       eAmmunitionType_Obus ),
    T_ConverterAmmunitionType( "MissileSol", QT_TRANSLATE_NOOP( "ENT_Tr", "Missile" ),     eAmmunitionType_MissileSol ),
    T_ConverterAmmunitionType( "MissileAir", QT_TRANSLATE_NOOP( "ENT_Tr", "Air missile" ), eAmmunitionType_MissileAir ),
    T_ConverterAmmunitionType( "Mitraille",  QT_TRANSLATE_NOOP( "ENT_Tr", "Bullet" ),      eAmmunitionType_Mitraille ),
    T_ConverterAmmunitionType( "ALR",        QT_TRANSLATE_NOOP( "ENT_Tr", "ALR" ),         eAmmunitionType_ALR ),
    T_ConverterAmmunitionType( "", "", (E_AmmunitionType)-1 )
};

ENT_Tr::T_ConverterNatureLevel ENT_Tr::NatureLevelConverter_[] =
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

ENT_Tr::T_ConverterDiplomacy ENT_Tr::DiplomacyConverter_[] =
{
    T_ConverterDiplomacy( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "unknown" ), eDiplomacy_Inconnu ),
    T_ConverterDiplomacy( "ami", QT_TRANSLATE_NOOP( "ENT_Tr", "friendly" ), eDiplomacy_Ami ),
    T_ConverterDiplomacy( "ennemi", QT_TRANSLATE_NOOP( "ENT_Tr", "enemy" ), eDiplomacy_Ennemi ),
    T_ConverterDiplomacy( "neutre", QT_TRANSLATE_NOOP( "ENT_Tr", "neutral" ), eDiplomacy_Neutre ),
    T_ConverterDiplomacy( "", "", (E_Diplomacy)-1 )
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
    T_ConverterRoe( "tir libre", QT_TRANSLATE_NOOP( "ENT_Tr", "free fire" ), eRoe_TirLibre ),
    T_ConverterRoe( "tir sur riposte", QT_TRANSLATE_NOOP( "ENT_Tr", "retaliation only" ), eRoe_TirSurRiposte ),
    T_ConverterRoe( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "forbidden fire" ), eRoe_TirInterdit ),
    T_ConverterRoe( "", "", (E_Roe)-1 )
};

ENT_Tr::T_ConverterUnitPosture ENT_Tr::UnitPostureConverter_[] =
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

ENT_Tr::T_ConverterUnitStress ENT_Tr::UnitStressConverter_[] =
{
    T_ConverterUnitStress( "calm", QT_TRANSLATE_NOOP( "ENT_Tr", "calm" ), eUnitStress_Calm ),
    T_ConverterUnitStress( "worried", QT_TRANSLATE_NOOP( "ENT_Tr", "worried" ), eUnitStress_Worried ),
    T_ConverterUnitStress( "stressed", QT_TRANSLATE_NOOP( "ENT_Tr", "stressed" ), eUnitStress_Stressed ),
    T_ConverterUnitStress( "", "", (E_UnitStress)-1 )
};

ENT_Tr::T_ConverterLightingType ENT_Tr::LightingTypeConverter_[] =
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

ENT_Tr::T_ConverterWeatherType ENT_Tr::WeatherTypeConverter_[] =
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
    T_ConverterLogMedicalHandlingStatus( "recherche secteur tri", QT_TRANSLATE_NOOP( "ENT_Tr", "looking for triage area" ), eLogMedicalHandlingStatus_RechercheSecteurTri ),
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

ENT_Tr::T_ConverterLogFuneralHandlingStatus ENT_Tr::LogFuneralHandlingStatusConverter_[] =
{
    T_ConverterLogFuneralHandlingStatus( "waiting for handling", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for handling" ), eLogFuneralHandlingStatus_WaitingForHandling ),
    T_ConverterLogFuneralHandlingStatus( "transporting unpackaged", QT_TRANSLATE_NOOP( "ENT_Tr", "transporting unpackaged" ), eLogFuneralHandlingStatus_TransportingUnpackaged ),
    T_ConverterLogFuneralHandlingStatus( "waiting for packaging", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for packaging" ), eLogFuneralHandlingStatus_WaitingForPackaging ),
    T_ConverterLogFuneralHandlingStatus( "packaging", QT_TRANSLATE_NOOP( "ENT_Tr", "packaging" ), eLogFuneralHandlingStatus_Packaging ),
    T_ConverterLogFuneralHandlingStatus( "waiting for transporter", QT_TRANSLATE_NOOP( "ENT_Tr", "waiting for transporter" ), eLogFuneralHandlingStatus_WaitingForTransporter ),
    T_ConverterLogFuneralHandlingStatus( "transporting packaged", QT_TRANSLATE_NOOP( "ENT_Tr", "transporting packaged" ), eLogFuneralHandlingStatus_TransportingPackaged ),
    T_ConverterLogFuneralHandlingStatus( "finished", QT_TRANSLATE_NOOP( "ENT_Tr", "finished" ), eLogFuneralHandlingStatus_Finished ),
    T_ConverterLogFuneralHandlingStatus( "", "", (E_LogFuneralHandlingStatus)-1 )
};

ENT_Tr::T_ConverterDemolitionTargetType ENT_Tr::DemolitionTargetTypeConverter_[] =
{
    T_ConverterDemolitionTargetType( "preliminary", QT_TRANSLATE_NOOP( "ENT_Tr", "preliminary" ), eDemolitionTargetType_Preliminary ),
    T_ConverterDemolitionTargetType( "reserved", QT_TRANSLATE_NOOP( "ENT_Tr", "reserved" ), eDemolitionTargetType_Reserved ),
    T_ConverterDemolitionTargetType( "", "", (E_DemolitionTargetType)-1 )
};

ENT_Tr::T_ConverterPopulationAttitude ENT_Tr::PopulationAttitudeConverter_[] =
{
    T_ConverterPopulationAttitude( "calme", QT_TRANSLATE_NOOP( "ENT_Tr", "quiet" ), ePopulationAttitude_Calme ),
    T_ConverterPopulationAttitude( "agitee", QT_TRANSLATE_NOOP( "ENT_Tr", "agitated" ), ePopulationAttitude_Agitee ),
    T_ConverterPopulationAttitude( "excitee", QT_TRANSLATE_NOOP( "ENT_Tr", "nervous" ), ePopulationAttitude_Excitee ),
    T_ConverterPopulationAttitude( "agressive", QT_TRANSLATE_NOOP( "ENT_Tr", "agressive" ), ePopulationAttitude_Agressive ),
    T_ConverterPopulationAttitude( "", "", (E_PopulationAttitude)-1 )
};

ENT_Tr::T_ConverterLocation ENT_Tr::LocationConverter_[] =
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

ENT_Tr::T_ConverterCrossingType ENT_Tr::CrossingTypeConverter_[] =
{
    T_ConverterCrossingType( "bridge"        , QT_TRANSLATE_NOOP( "ENT_Tr", "bridge" )        , eBridge ),
    T_ConverterCrossingType( "highway"       , QT_TRANSLATE_NOOP( "ENT_Tr", "highway" )       , eHighway ),
    T_ConverterCrossingType( "main road"     , QT_TRANSLATE_NOOP( "ENT_Tr", "main road" )     , eMainRoad ),
    T_ConverterCrossingType( "secondary road", QT_TRANSLATE_NOOP( "ENT_Tr", "secondary road" ), eSecondaryRoad ),
    T_ConverterCrossingType( "country road"  , QT_TRANSLATE_NOOP( "ENT_Tr", "country road" )  , eCountryRoad ),
    T_ConverterCrossingType( ""       , QT_TRANSLATE_NOOP( "ENT_Tr", "" )              , eNothing ),
    T_ConverterCrossingType( "", "", ( E_CrossingType ) - 1 )
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
    T_ConverterHumanRank( "Officier", QT_TRANSLATE_NOOP( "ENT_Tr", "Officer" ), eHumanRank_Officier ),
    T_ConverterHumanRank( "SousOfficier", QT_TRANSLATE_NOOP( "ENT_Tr", "Warrant officer" ), eHumanRank_SousOfficer ),
    T_ConverterHumanRank( "MilitaireDuRang", QT_TRANSLATE_NOOP( "ENT_Tr", "Trooper" ), eHumanRank_Mdr ),
    T_ConverterHumanRank( "", "", (E_HumanRank)-1 )
};

ENT_Tr::T_ConverterHumanState ENT_Tr::HumanStateConverter_ [] =
{
    T_ConverterHumanState( "healthy", QT_TRANSLATE_NOOP( "ENT_Tr", "Healthy" ), eHumanState_Healthy ),
    T_ConverterHumanState( "injured", QT_TRANSLATE_NOOP( "ENT_Tr", "Injured" ), eHumanState_Injured ),
    T_ConverterHumanState( "dead",    QT_TRANSLATE_NOOP( "ENT_Tr", "Dead" ), eHumanState_Dead ),
    T_ConverterHumanState( "", "", ( E_HumanState ) - 1 )
};

ENT_Tr::T_ConverterHumanLocation ENT_Tr::HumanLocationConverter_ [] =
{
    T_ConverterHumanLocation( "battlefield", QT_TRANSLATE_NOOP( "ENT_Tr", "On battlefield" ), eHumanLocation_Battlefield ),
    T_ConverterHumanLocation( "maintenance", QT_TRANSLATE_NOOP( "ENT_Tr", "Assigned to maintenance" ), eHumanLocation_Maintenance ),
    T_ConverterHumanLocation( "medical",     QT_TRANSLATE_NOOP( "ENT_Tr", "In medical treatment" ), eHumanLocation_Medical),
    T_ConverterHumanLocation( "", "", ( E_HumanLocation ) - 1 )
};

ENT_Tr::T_ConverterEquipmentState ENT_Tr::EquipmentStateConverter_ [] =
{
    T_ConverterEquipmentState( "Disponible",              QT_TRANSLATE_NOOP( "ENT_Tr", "Available" ), eEquipmentState_Available ),
    T_ConverterEquipmentState( "Detruit",                 QT_TRANSLATE_NOOP( "ENT_Tr", "Destroyed" ), eEquipmentState_Destroyed ),
    T_ConverterEquipmentState( "ReparableAvecEvacuation", QT_TRANSLATE_NOOP( "ENT_Tr", "Maintenance support needed" ), eEquipmentState_RepairableWithEvacuation ),
    T_ConverterEquipmentState( "ReparableSurPlace",       QT_TRANSLATE_NOOP( "ENT_Tr", "On site fixable" ), eEquipmentState_OnSiteFixable ),
    T_ConverterEquipmentState( "EnMaintenance",           QT_TRANSLATE_NOOP( "ENT_Tr", "In maintenance" ), eEquipmentState_InMaintenance ),
    T_ConverterEquipmentState( "Prisonnier",              QT_TRANSLATE_NOOP( "ENT_Tr", "Prisoner" ), eEquipmentState_Prisonner ),
    T_ConverterEquipmentState( "", "", ( E_EquipmentState ) - 1 )
};

ENT_Tr::T_ConverterInjuriesSeriousness ENT_Tr::InjuriesSeriousnessConverter_ [] =
{
    T_ConverterInjuriesSeriousness( "U1", QT_TRANSLATE_NOOP( "ENT_Tr", "U1" ), eInjuriesSeriousness_U1 ),
    T_ConverterInjuriesSeriousness( "U2", QT_TRANSLATE_NOOP( "ENT_Tr", "U2" ), eInjuriesSeriousness_U2 ),
    T_ConverterInjuriesSeriousness( "U3", QT_TRANSLATE_NOOP( "ENT_Tr", "U3" ), eInjuriesSeriousness_U3 ),
    T_ConverterInjuriesSeriousness( "UE", QT_TRANSLATE_NOOP( "ENT_Tr", "UE" ), eInjuriesSeriousness_UE ),
    T_ConverterInjuriesSeriousness( "", "", ( E_InjuriesSeriousness ) - 1 )
};

ENT_Tr::T_ConverterGhostType ENT_Tr::GhostTypeConverter_ [] =
{
    T_ConverterGhostType( "agent",   QT_TRANSLATE_NOOP( "ENT_Tr", "Agent" ),   eGhostType_Agent ),
    T_ConverterGhostType( "automat", QT_TRANSLATE_NOOP( "ENT_Tr", "Automat" ), eGhostType_Automat ),
    T_ConverterGhostType( "invalid", QT_TRANSLATE_NOOP( "ENT_Tr", "Invalid" ), eGhostType_Invalid ),
    T_ConverterGhostType( "", "", ( E_GhostType ) - 1 )
};

ENT_Tr::T_ConverterNbcState ENT_Tr::NbcStateConverter_ [] =
{
    T_ConverterNbcState( "liquid",  QT_TRANSLATE_NOOP( "ENT_Tr", "Liquid" ),  eNbcState_Liquid ),
    T_ConverterNbcState( "gaseous", QT_TRANSLATE_NOOP( "ENT_Tr", "Gaseous" ), eNbcState_Gaseous ),
    T_ConverterNbcState( "", "", ( E_NbcState ) - 1 )
};

ENT_Tr::T_ConverterModes ENT_Tr::ModesConverter_ [] =
{
    T_ConverterModes( "none",        QT_TRANSLATE_NOOP( "ENT_Tr", "None" ),        eModes_None          ),
    T_ConverterModes( "default",     QT_TRANSLATE_NOOP( "ENT_Tr", "Default" ),     eModes_Default       ),
    T_ConverterModes( "prepare",     QT_TRANSLATE_NOOP( "ENT_Tr", "Prepare" ),     eModes_Prepare       ),
    T_ConverterModes( "terrain",     QT_TRANSLATE_NOOP( "ENT_Tr", "Terrain" ),     eModes_Terrain       ),
    T_ConverterModes( "living_area", QT_TRANSLATE_NOOP( "ENT_Tr", "Living Area" ), eModes_LivingArea    ),
    T_ConverterModes( "game",        QT_TRANSLATE_NOOP( "ENT_Tr", "Game" ),        eModes_Gaming          ),
    T_ConverterModes( "planning",    QT_TRANSLATE_NOOP( "ENT_Tr", "Planning" ),    eModes_Planning      ),
    T_ConverterModes( "replay",      QT_TRANSLATE_NOOP( "ENT_Tr", "Replay" ),      eModes_Replay        ),
    T_ConverterModes( "all",         QT_TRANSLATE_NOOP( "ENT_Tr", "All" ),         eModes_All           ),
    T_ConverterModes( "", "", ( E_Modes ) - 1 )
};

ENT_Tr::T_ConverterAgentNbcSuit ENT_Tr::AgentNbcSuitConverter_[] =
{
    T_ConverterAgentNbcSuit( "none",   QT_TRANSLATE_NOOP( "ENT_Tr", "None" ),    eAgentNone ),
    T_ConverterAgentNbcSuit( "level1", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 1" ), eAgentNbcSuitLevel1 ),
    T_ConverterAgentNbcSuit( "level2", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 2" ), eAgentNbcSuitLevel2 ),
    T_ConverterAgentNbcSuit( "level3", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 3" ), eAgentNbcSuitLevel3 ),
    T_ConverterAgentNbcSuit( "level4", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 4" ), eAgentNbcSuitLevel4 ),
    T_ConverterAgentNbcSuit( "level5", QT_TRANSLATE_NOOP( "ENT_Tr", "Level 5" ), eAgentNbcSuitLevel5 ),
    T_ConverterAgentNbcSuit( "", "", (E_AgentNbcSuit)-1 )
};

ENT_Tr::T_ConverterLayerTypes ENT_Tr::LayerTypesConverter_[] =
{
    T_ConverterLayerTypes( "units",          QT_TRANSLATE_NOOP( "ENT_Tr", "Units" ),          eLayerTypes_Agent ),
    T_ConverterLayerTypes( "objects",        QT_TRANSLATE_NOOP( "ENT_Tr", "Objects" ),        eLayerTypes_Objects ),
    T_ConverterLayerTypes( "automata",       QT_TRANSLATE_NOOP( "ENT_Tr", "Automata" ),       eLayerTypes_Automats ),
    T_ConverterLayerTypes( "formations",     QT_TRANSLATE_NOOP( "ENT_Tr", "Formations" ),     eLayerTypes_Formations ),
    T_ConverterLayerTypes( "crowds",         QT_TRANSLATE_NOOP( "ENT_Tr", "Crowds" ),         eLayerTypes_Populations ),
    T_ConverterLayerTypes( "urban_blocks",   QT_TRANSLATE_NOOP( "ENT_Tr", "Urban blocks" ),   eLayerTypes_UrbanObjects ),
    T_ConverterLayerTypes( "tactical_lines", QT_TRANSLATE_NOOP( "ENT_Tr", "Tactical lines" ), eLayerTypes_TacticalLines ),
    T_ConverterLayerTypes( "ghosts",         QT_TRANSLATE_NOOP( "ENT_Tr", "Ghosts" ),         eLayerTypes_Ghosts ),
    T_ConverterLayerTypes( "fog",            QT_TRANSLATE_NOOP( "ENT_Tr", "Fog" ),            eLayerTypes_Fog ),
    T_ConverterLayerTypes( "parties",        QT_TRANSLATE_NOOP( "ENT_Tr", "Parties" ),        eLayerTypes_Parties ),
    T_ConverterLayerTypes( "drawings",       QT_TRANSLATE_NOOP( "ENT_Tr", "Drawings" ),       eLayerTypes_Drawers ),
    T_ConverterLayerTypes( "knowledges",     QT_TRANSLATE_NOOP( "ENT_Tr", "Knowledges" ),     eLayerTypes_Knowledges ),
    T_ConverterLayerTypes( "inhabitants",    QT_TRANSLATE_NOOP( "ENT_Tr", "Inhabitants" ),    eLayerTypes_Inhabitants ),
    T_ConverterLayerTypes( "", "", (E_LayerTypes)-1 )
};

//-----------------------------------------------------------------------------
// Name: ENT_Tr::InitTranslations
// Created: AGR
//-----------------------------------------------------------------------------
void ENT_Tr::InitTranslations()
{
    InitTr( LocationTypeConverter_, "ENT_Tr" );
    InitTr( DotationFamilyConverter_, "ENT_Tr" );
    InitTr( AmmunitionTypeConverter_, "ENT_Tr" );
    InitTr( NatureLevelConverter_, "ENT_Tr" );
    InitTr( DiplomacyConverter_, "ENT_Tr" );
    InitTr( ForceRatioStatusConverter_, "ENT_Tr" );
    InitTr( MeetingEngagementStatusConverter_, "ENT_Tr" );
    InitTr( OperationalStatusConverter_, "ENT_Tr" );
    InitTr( RoeConverter_, "ENT_Tr" );
    InitTr( UnitPostureConverter_, "ENT_Tr" );
    InitTr( FireAvailabilityConverter_, "ENT_Tr" );
    InitTr( PopulationRoeConverter_, "ENT_Tr" );
    InitTr( UnitTirednessConverter_, "ENT_Tr" );
    InitTr( UnitMoraleConverter_, "ENT_Tr" );
    InitTr( UnitExperienceConverter_, "ENT_Tr" );
    InitTr( UnitStressConverter_, "ENT_Tr" );
    InitTr( LightingTypeConverter_, "ENT_Tr" );
    InitTr( WeatherTypeConverter_, "ENT_Tr" );
    InitTr( LogMaintenanceHandlingStatusConverter_, "ENT_Tr" );
    InitTr( LogSupplyHandlingStatusConverter_, "ENT_Tr" );
    InitTr( LogMedicalHandlingStatusConverter_, "ENT_Tr" );
    InitTr( LogFuneralHandlingStatusConverter_, "ENT_Tr" );
    InitTr( DemolitionTargetTypeConverter_, "ENT_Tr" );
    InitTr( PopulationAttitudeConverter_, "ENT_Tr" );
    InitTr( LocationConverter_, "ENT_Tr" );
    InitTr( CrossingTypeConverter_, "ENT_Tr" );
    InitTr( HumanWoundConverter_, "ENT_Tr" );
    InitTr( HumanRankConverter_, "ENT_Tr" );
    InitTr( HumanStateConverter_, "ENT_Tr" );
    InitTr( HumanLocationConverter_, "ENT_Tr" );
    InitTr( EquipmentStateConverter_, "ENT_Tr" );
    InitTr( InjuriesSeriousnessConverter_, "ENT_Tr" );
    InitTr( GhostTypeConverter_, "ENT_Tr" );
    InitTr( NbcStateConverter_, "ENT_Tr" );
    InitTr( ModesConverter_, "ENT_Tr" );
    InitTr( AgentNbcSuitConverter_, "ENT_Tr" );
    InitTr( LayerTypesConverter_, "ENT_Tr" );
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
// Name: ENT_Tr::ConvertFromAmmunitionType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAmmunitionType( E_AmmunitionType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AmmunitionTypeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromDiplomacy
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDiplomacy( E_Diplomacy nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DiplomacyConverter_, nValue, nConverterType );
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

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitStress
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitStress( E_UnitStress nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitStressConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLightingType
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLightingType( E_LightingType nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LightingTypeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromWeatherType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromWeatherType( E_WeatherType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( WeatherTypeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromLogMedicalHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogMedicalHandlingStatus( E_LogMedicalHandlingStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogMedicalHandlingStatusConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLogFuneralHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLogFuneralHandlingStatus( E_LogFuneralHandlingStatus nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LogFuneralHandlingStatusConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromPopulationAttitude
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationAttitude( E_PopulationAttitude nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( PopulationAttitudeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromLocation
// Created: PHC
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLocation( E_Location nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LocationConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromCrossingType
// Created: LGY 2011-07-19
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromCrossingType( E_CrossingType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( CrossingTypeConverter_, nValue, nConverterType );
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

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromHumanState
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromHumanState( E_HumanState nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( HumanStateConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromHumanLocation
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromHumanLocation( E_HumanLocation nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( HumanLocationConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromEquipmentState
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromEquipmentState( E_EquipmentState nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( EquipmentStateConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromInjuriesSeriousness
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromInjuriesSeriousness( E_InjuriesSeriousness nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( InjuriesSeriousnessConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromGhostType
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromGhostType( E_GhostType nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( GhostTypeConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromNbcState
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromNbcState( E_NbcState nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( NbcStateConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromModes
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromModes( E_Modes nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ModesConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr_Gen::ConvertFromAgentNbcSuit
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAgentNbcSuit( E_AgentNbcSuit nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AgentNbcSuitConverter_, nValue, nConverterType );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr_Gen::ConvertFromLayerType
// Created: LGY 2013-02-27
// -----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromLayerType( E_LayerTypes nValue, E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( LayerTypesConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertToAmmunitionType
// Created: AGR
//-----------------------------------------------------------------------------
E_AmmunitionType ENT_Tr::ConvertToAmmunitionType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AmmunitionTypeConverter_, strName );
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
// Name: ENT_Tr::ConvertToDiplomacy
// Created: AGR
//-----------------------------------------------------------------------------
E_Diplomacy ENT_Tr::ConvertToDiplomacy( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DiplomacyConverter_, strName );
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

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitStress
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
E_UnitStress ENT_Tr::ConvertToUnitStress( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitStressConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLightingType
// Created: ABR 2011-11-15
// -----------------------------------------------------------------------------
E_LightingType ENT_Tr::ConvertToLightingType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LightingTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToWeatherType
// Created: AGR
//-----------------------------------------------------------------------------
E_WeatherType ENT_Tr::ConvertToWeatherType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( WeatherTypeConverter_, strName );
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
// Name: ENT_Tr::ConvertToLogMedicalHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_LogMedicalHandlingStatus ENT_Tr::ConvertToLogMedicalHandlingStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogMedicalHandlingStatusConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLogFuneralHandlingStatus
// Created: AGR
//-----------------------------------------------------------------------------
E_LogFuneralHandlingStatus ENT_Tr::ConvertToLogFuneralHandlingStatus( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LogFuneralHandlingStatusConverter_, strName );
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
// Name: ENT_Tr::ConvertToPopulationAttitude
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationAttitude ENT_Tr::ConvertToPopulationAttitude( const std::string& strName )
{
    return ENT_Tr::FindInConverter( PopulationAttitudeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToLocation
// Created: PHC
//-----------------------------------------------------------------------------
E_Location ENT_Tr::ConvertToLocation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LocationConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToCrossingType
// Created: LGY 2011-07-19
// -----------------------------------------------------------------------------
E_CrossingType ENT_Tr::ConvertToCrossingType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( CrossingTypeConverter_, strName );
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

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToHumanState
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
E_HumanState ENT_Tr::ConvertToHumanState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( HumanStateConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToHumanLocation
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
E_HumanLocation ENT_Tr::ConvertToHumanLocation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( HumanLocationConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToEquipmentState
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
E_EquipmentState ENT_Tr::ConvertToEquipmentState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( EquipmentStateConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToInjuriesSeriousness
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
E_InjuriesSeriousness ENT_Tr::ConvertToInjuriesSeriousness( const std::string& strName )
{
    return ENT_Tr::FindInConverter( InjuriesSeriousnessConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToGhostType
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
E_GhostType ENT_Tr::ConvertToGhostType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( GhostTypeConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToNbcState
// Created: ABR 2011-11-29
// -----------------------------------------------------------------------------
E_NbcState ENT_Tr::ConvertToNbcState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( NbcStateConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToModes
// Created: ABR 2012-05-10
// -----------------------------------------------------------------------------
E_Modes ENT_Tr::ConvertToModes( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ModesConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr_Gen::ConvertToAgentNbcSuit
// Created: LGY 2012-11-23
// -----------------------------------------------------------------------------
E_AgentNbcSuit ENT_Tr::ConvertToAgentNbcSuit( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AgentNbcSuitConverter_, strName );
}

// -----------------------------------------------------------------------------
// Name: ENT_Tr_Gen::ConvertToLayerType
// Created: LGY 2013-02-27
// -----------------------------------------------------------------------------
E_LayerTypes ENT_Tr::ConvertToLayerType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( LayerTypesConverter_, strName );
}
