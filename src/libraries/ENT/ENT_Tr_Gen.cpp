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

ENT_Tr::T_ConverterUnitNatureWeapon ENT_Tr::UnitNatureWeaponConverter_[] =
{
    T_ConverterUnitNatureWeapon( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eUnitNatureWeapon_None ),
    T_ConverterUnitNatureWeapon( "antichar", QT_TRANSLATE_NOOP( "ENT_Tr", "antichar" ), eUnitNatureWeapon_Antichar ),
    T_ConverterUnitNatureWeapon( "administration", QT_TRANSLATE_NOOP( "ENT_Tr", "administration" ), eUnitNatureWeapon_Administration ),
    T_ConverterUnitNatureWeapon( "defense aerienne", QT_TRANSLATE_NOOP( "ENT_Tr", "defense aerienne" ), eUnitNatureWeapon_DefenseAerienne ),
    T_ConverterUnitNatureWeapon( "centre soutien operations aeriennes", QT_TRANSLATE_NOOP( "ENT_Tr", "centre soutien operations aeriennes" ), eUnitNatureWeapon_CentreSoutienOperationsAeriennes ),
    T_ConverterUnitNatureWeapon( "blinde", QT_TRANSLATE_NOOP( "ENT_Tr", "blinde" ), eUnitNatureWeapon_Blinde ),
    T_ConverterUnitNatureWeapon( "aviation", QT_TRANSLATE_NOOP( "ENT_Tr", "aviation" ), eUnitNatureWeapon_Aviation ),
    T_ConverterUnitNatureWeapon( "cavalerie", QT_TRANSLATE_NOOP( "ENT_Tr", "cavalerie" ), eUnitNatureWeapon_Cavalerie ),
    T_ConverterUnitNatureWeapon( "genie", QT_TRANSLATE_NOOP( "ENT_Tr", "genie" ), eUnitNatureWeapon_Genie ),
    T_ConverterUnitNatureWeapon( "deminage", QT_TRANSLATE_NOOP( "ENT_Tr", "deminage" ), eUnitNatureWeapon_Deminage ),
    T_ConverterUnitNatureWeapon( "soutien artillerie", QT_TRANSLATE_NOOP( "ENT_Tr", "soutien artillerie" ), eUnitNatureWeapon_SoutienArtillerie ),
    T_ConverterUnitNatureWeapon( "artillerie", QT_TRANSLATE_NOOP( "ENT_Tr", "artillerie" ), eUnitNatureWeapon_Artillerie ),
    T_ConverterUnitNatureWeapon( "infanterie", QT_TRANSLATE_NOOP( "ENT_Tr", "infanterie" ), eUnitNatureWeapon_Infanterie ),
    T_ConverterUnitNatureWeapon( "guerre information", QT_TRANSLATE_NOOP( "ENT_Tr", "guerre information" ), eUnitNatureWeapon_GuerreInformation ),
    T_ConverterUnitNatureWeapon( "forces securite interne", QT_TRANSLATE_NOOP( "ENT_Tr", "forces securite interne" ), eUnitNatureWeapon_ForcesSecuriteInterne ),
    T_ConverterUnitNatureWeapon( "execution loi", QT_TRANSLATE_NOOP( "ENT_Tr", "execution loi" ), eUnitNatureWeapon_ExecutionLoi ),
    T_ConverterUnitNatureWeapon( "soutien atterrissage", QT_TRANSLATE_NOOP( "ENT_Tr", "soutien atterrissage" ), eUnitNatureWeapon_SoutienAtterrissage ),
    T_ConverterUnitNatureWeapon( "logistique", QT_TRANSLATE_NOOP( "ENT_Tr", "logistique" ), eUnitNatureWeapon_Logistique ),
    T_ConverterUnitNatureWeapon( "reparation", QT_TRANSLATE_NOOP( "ENT_Tr", "reparation" ), eUnitNatureWeapon_Reparation ),
    T_ConverterUnitNatureWeapon( "manoeuvre", QT_TRANSLATE_NOOP( "ENT_Tr", "manoeuvre" ), eUnitNatureWeapon_Manoeuvre ),
    T_ConverterUnitNatureWeapon( "medical", QT_TRANSLATE_NOOP( "ENT_Tr", "medical" ), eUnitNatureWeapon_Medical ),
    T_ConverterUnitNatureWeapon( "renseignement militaire", QT_TRANSLATE_NOOP( "ENT_Tr", "renseignement militaire" ), eUnitNatureWeapon_RenseignementMilitaire ),
    T_ConverterUnitNatureWeapon( "missile", QT_TRANSLATE_NOOP( "ENT_Tr", "missile" ), eUnitNatureWeapon_Missile ),
    T_ConverterUnitNatureWeapon( "nbc", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc" ), eUnitNatureWeapon_Nbc ),
    T_ConverterUnitNatureWeapon( "inconnue", QT_TRANSLATE_NOOP( "ENT_Tr", "inconnue" ), eUnitNatureWeapon_Inconnue ),
    T_ConverterUnitNatureWeapon( "non specifiee", QT_TRANSLATE_NOOP( "ENT_Tr", "non specifiee" ), eUnitNatureWeapon_NonSpecifiee ),
    T_ConverterUnitNatureWeapon( "reconnaissance", QT_TRANSLATE_NOOP( "ENT_Tr", "reconnaissance" ), eUnitNatureWeapon_Reconnaissance ),
    T_ConverterUnitNatureWeapon( "transmissions", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions" ), eUnitNatureWeapon_Transmissions ),
    T_ConverterUnitNatureWeapon( "ravitaillement", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement" ), eUnitNatureWeapon_Ravitaillement ),
    T_ConverterUnitNatureWeapon( "tactical air control party", QT_TRANSLATE_NOOP( "ENT_Tr", "tactical air control party" ), eUnitNatureWeapon_TacticalAirControlParty ),
    T_ConverterUnitNatureWeapon( "transport", QT_TRANSLATE_NOOP( "ENT_Tr", "transport" ), eUnitNatureWeapon_Transport ),
    T_ConverterUnitNatureWeapon( "", "", (E_UnitNatureWeapon)-1 )
};

ENT_Tr::T_ConverterUnitNatureSpecialization ENT_Tr::UnitNatureSpecializationConverter_[] =
{
    T_ConverterUnitNatureSpecialization( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eUnitNatureSpecialization_None ),
    T_ConverterUnitNatureSpecialization( "aerien", QT_TRANSLATE_NOOP( "ENT_Tr", "aerien" ), eUnitNatureSpecialization_Aerien ),
    T_ConverterUnitNatureSpecialization( "attaque aerienne", QT_TRANSLATE_NOOP( "ENT_Tr", "attaque aerienne" ), eUnitNatureSpecialization_AttaqueAerienne ),
    T_ConverterUnitNatureSpecialization( "aeroporte", QT_TRANSLATE_NOOP( "ENT_Tr", "aeroporte" ), eUnitNatureSpecialization_Aeroporte ),
    T_ConverterUnitNatureSpecialization( "blinde", QT_TRANSLATE_NOOP( "ENT_Tr", "blinde" ), eUnitNatureSpecialization_Blinde ),
    T_ConverterUnitNatureSpecialization( "compose", QT_TRANSLATE_NOOP( "ENT_Tr", "compose" ), eUnitNatureSpecialization_Compose ),
    T_ConverterUnitNatureSpecialization( "construction", QT_TRANSLATE_NOOP( "ENT_Tr", "construction" ), eUnitNatureSpecialization_Construction ),
    T_ConverterUnitNatureSpecialization( "dentiste", QT_TRANSLATE_NOOP( "ENT_Tr", "dentiste" ), eUnitNatureSpecialization_Dentiste ),
    T_ConverterUnitNatureSpecialization( "guerre electronique", QT_TRANSLATE_NOOP( "ENT_Tr", "guerre electronique" ), eUnitNatureSpecialization_GuerreElectronique ),
    T_ConverterUnitNatureSpecialization( "finance", QT_TRANSLATE_NOOP( "ENT_Tr", "finance" ), eUnitNatureSpecialization_Finance ),
    T_ConverterUnitNatureSpecialization( "voilure fixe", QT_TRANSLATE_NOOP( "ENT_Tr", "voilure fixe" ), eUnitNatureSpecialization_VoilureFixe ),
    T_ConverterUnitNatureSpecialization( "terrestre", QT_TRANSLATE_NOOP( "ENT_Tr", "terrestre" ), eUnitNatureSpecialization_Terrestre ),
    T_ConverterUnitNatureSpecialization( "helicoptere", QT_TRANSLATE_NOOP( "ENT_Tr", "helicoptere" ), eUnitNatureSpecialization_Helicoptere ),
    T_ConverterUnitNatureSpecialization( "jag", QT_TRANSLATE_NOOP( "ENT_Tr", "jag" ), eUnitNatureSpecialization_Jag ),
    T_ConverterUnitNatureSpecialization( "mecanise", QT_TRANSLATE_NOOP( "ENT_Tr", "mecanise" ), eUnitNatureSpecialization_Mecanise ),
    T_ConverterUnitNatureSpecialization( "police militaire", QT_TRANSLATE_NOOP( "ENT_Tr", "police militaire" ), eUnitNatureSpecialization_PoliceMilitaire ),
    T_ConverterUnitNatureSpecialization( "motorise", QT_TRANSLATE_NOOP( "ENT_Tr", "motorise" ), eUnitNatureSpecialization_Motorise ),
    T_ConverterUnitNatureSpecialization( "naval", QT_TRANSLATE_NOOP( "ENT_Tr", "naval" ), eUnitNatureSpecialization_Naval ),
    T_ConverterUnitNatureSpecialization( "service du personnel", QT_TRANSLATE_NOOP( "ENT_Tr", "service du personnel" ), eUnitNatureSpecialization_ServiceDuPersonnel ),
    T_ConverterUnitNatureSpecialization( "courrier", QT_TRANSLATE_NOOP( "ENT_Tr", "courrier" ), eUnitNatureSpecialization_Courrier ),
    T_ConverterUnitNatureSpecialization( "psychologique", QT_TRANSLATE_NOOP( "ENT_Tr", "psychologique" ), eUnitNatureSpecialization_Psychologique ),
    T_ConverterUnitNatureSpecialization( "chemin de fer", QT_TRANSLATE_NOOP( "ENT_Tr", "chemin de fer" ), eUnitNatureSpecialization_CheminDeFer ),
    T_ConverterUnitNatureSpecialization( "religieux", QT_TRANSLATE_NOOP( "ENT_Tr", "religieux" ), eUnitNatureSpecialization_Religieux ),
    T_ConverterUnitNatureSpecialization( "riverine", QT_TRANSLATE_NOOP( "ENT_Tr", "riverine" ), eUnitNatureSpecialization_Riverine ),
    T_ConverterUnitNatureSpecialization( "security police", QT_TRANSLATE_NOOP( "ENT_Tr", "security police" ), eUnitNatureSpecialization_SecurityPolice ),
    T_ConverterUnitNatureSpecialization( "patrouille cotiere", QT_TRANSLATE_NOOP( "ENT_Tr", "patrouille cotiere" ), eUnitNatureSpecialization_PatrouilleCotiere ),
    T_ConverterUnitNatureSpecialization( "chirurgie", QT_TRANSLATE_NOOP( "ENT_Tr", "chirurgie" ), eUnitNatureSpecialization_Chirurgie ),
    T_ConverterUnitNatureSpecialization( "veterinaire", QT_TRANSLATE_NOOP( "ENT_Tr", "veterinaire" ), eUnitNatureSpecialization_Veterinaire ),
    T_ConverterUnitNatureSpecialization( "decollage atterissage vertical court", QT_TRANSLATE_NOOP( "ENT_Tr", "decollage atterissage vertical court" ), eUnitNatureSpecialization_DecollageAtterissageVerticalCourt ),
    T_ConverterUnitNatureSpecialization( "", "", (E_UnitNatureSpecialization)-1 )
};

ENT_Tr::T_ConverterUnitNatureQualifier ENT_Tr::UnitNatureQualifierConverter_[] =
{
    T_ConverterUnitNatureQualifier( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eUnitNatureQualifier_None ),
    T_ConverterUnitNatureQualifier( "lourd", QT_TRANSLATE_NOOP( "ENT_Tr", "lourd" ), eUnitNatureQualifier_Lourd ),
    T_ConverterUnitNatureQualifier( "leger", QT_TRANSLATE_NOOP( "ENT_Tr", "leger" ), eUnitNatureQualifier_Leger ),
    T_ConverterUnitNatureQualifier( "moyen", QT_TRANSLATE_NOOP( "ENT_Tr", "moyen" ), eUnitNatureQualifier_Moyen ),
    T_ConverterUnitNatureQualifier( "", "", (E_UnitNatureQualifier)-1 )
};

ENT_Tr::T_ConverterUnitNatureCategory ENT_Tr::UnitNatureCategoryConverter_[] =
{
    T_ConverterUnitNatureCategory( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eUnitNatureCategory_None ),
    T_ConverterUnitNatureCategory( "combat", QT_TRANSLATE_NOOP( "ENT_Tr", "combat" ), eUnitNatureCategory_Combat ),
    T_ConverterUnitNatureCategory( "soutien logistique", QT_TRANSLATE_NOOP( "ENT_Tr", "soutien logistique" ), eUnitNatureCategory_SoutienLogistique ),
    T_ConverterUnitNatureCategory( "combat support", QT_TRANSLATE_NOOP( "ENT_Tr", "combat support" ), eUnitNatureCategory_CombatSupport ),
    T_ConverterUnitNatureCategory( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "inconnu" ), eUnitNatureCategory_Inconnu ),
    T_ConverterUnitNatureCategory( "", "", (E_UnitNatureCategory)-1 )
};

ENT_Tr::T_ConverterUnitNatureMobility ENT_Tr::UnitNatureMobilityConverter_[] =
{
    T_ConverterUnitNatureMobility( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eUnitNatureMobility_None ),
    T_ConverterUnitNatureMobility( "aeroporte", QT_TRANSLATE_NOOP( "ENT_Tr", "aeroporte" ), eUnitNatureMobility_Aeroporte ),
    T_ConverterUnitNatureMobility( "aerien compose", QT_TRANSLATE_NOOP( "ENT_Tr", "aerien compose" ), eUnitNatureMobility_AerienCompose ),
    T_ConverterUnitNatureMobility( "aerien voilure fixe", QT_TRANSLATE_NOOP( "ENT_Tr", "aerien voilure fixe" ), eUnitNatureMobility_AerienVoilureFixe ),
    T_ConverterUnitNatureMobility( "aerien plus leger que air", QT_TRANSLATE_NOOP( "ENT_Tr", "aerien plus leger que air" ), eUnitNatureMobility_AerienPlusLegerQueAir ),
    T_ConverterUnitNatureMobility( "aerien ailes rotatives", QT_TRANSLATE_NOOP( "ENT_Tr", "aerien ailes rotatives" ), eUnitNatureMobility_AerienAilesRotatives ),
    T_ConverterUnitNatureMobility( "aerien amphibie", QT_TRANSLATE_NOOP( "ENT_Tr", "aerien amphibie" ), eUnitNatureMobility_AerienAmphibie ),
    T_ConverterUnitNatureMobility( "animal monte", QT_TRANSLATE_NOOP( "ENT_Tr", "animal monte" ), eUnitNatureMobility_AnimalMonte ),
    T_ConverterUnitNatureMobility( "demonte", QT_TRANSLATE_NOOP( "ENT_Tr", "demonte" ), eUnitNatureMobility_Demonte ),
    T_ConverterUnitNatureMobility( "terrestre chemin de fer", QT_TRANSLATE_NOOP( "ENT_Tr", "terrestre chemin de fer" ), eUnitNatureMobility_TerrestreCheminDeFer ),
    T_ConverterUnitNatureMobility( "terrestre auto propulse", QT_TRANSLATE_NOOP( "ENT_Tr", "terrestre auto propulse" ), eUnitNatureMobility_TerrestreAutoPropulse ),
    T_ConverterUnitNatureMobility( "terrestre chenille", QT_TRANSLATE_NOOP( "ENT_Tr", "terrestre chenille" ), eUnitNatureMobility_TerrestreChenille ),
    T_ConverterUnitNatureMobility( "terrestre remorque", QT_TRANSLATE_NOOP( "ENT_Tr", "terrestre remorque" ), eUnitNatureMobility_TerrestreRemorque ),
    T_ConverterUnitNatureMobility( "terrestre roues", QT_TRANSLATE_NOOP( "ENT_Tr", "terrestre roues" ), eUnitNatureMobility_TerrestreRoues ),
    T_ConverterUnitNatureMobility( "maritime sous marin", QT_TRANSLATE_NOOP( "ENT_Tr", "maritime sous marin" ), eUnitNatureMobility_MaritimeSousMarin ),
    T_ConverterUnitNatureMobility( "maritime surface", QT_TRANSLATE_NOOP( "ENT_Tr", "maritime surface" ), eUnitNatureMobility_MaritimeSurface ),
    T_ConverterUnitNatureMobility( "inconnu", QT_TRANSLATE_NOOP( "ENT_Tr", "inconnu" ), eUnitNatureMobility_Inconnu ),
    T_ConverterUnitNatureMobility( "", "", (E_UnitNatureMobility)-1 )
};

ENT_Tr::T_ConverterUnitCapaciteMission ENT_Tr::UnitCapaciteMissionConverter_[] =
{
    T_ConverterUnitCapaciteMission( "civilian law enforcement", QT_TRANSLATE_NOOP( "ENT_Tr", "civilian law enforcement" ), eUnitCapaciteMission_CivilianLawEnforcement ),
    T_ConverterUnitCapaciteMission( "command operations", QT_TRANSLATE_NOOP( "ENT_Tr", "command operations" ), eUnitCapaciteMission_CommandOperations ),
    T_ConverterUnitCapaciteMission( "couverture aerienne", QT_TRANSLATE_NOOP( "ENT_Tr", "couverture aerienne" ), eUnitCapaciteMission_CouvertureAerienne ),
    T_ConverterUnitCapaciteMission( "joint intelligence", QT_TRANSLATE_NOOP( "ENT_Tr", "joint intelligence" ), eUnitCapaciteMission_JointIntelligence ),
    T_ConverterUnitCapaciteMission( "morale welfare recreation", QT_TRANSLATE_NOOP( "ENT_Tr", "morale welfare recreation" ), eUnitCapaciteMission_MoraleWelfareRecreation ),
    T_ConverterUnitCapaciteMission( "reglage electronique", QT_TRANSLATE_NOOP( "ENT_Tr", "reglage electronique" ), eUnitCapaciteMission_ReglageElectronique ),
    T_ConverterUnitCapaciteMission( "remplacement", QT_TRANSLATE_NOOP( "ENT_Tr", "remplacement" ), eUnitCapaciteMission_Remplacement ),
    T_ConverterUnitCapaciteMission( "service mortuaire", QT_TRANSLATE_NOOP( "ENT_Tr", "service mortuaire" ), eUnitCapaciteMission_ServiceMortuaire ),
    T_ConverterUnitCapaciteMission( "signals intelligence", QT_TRANSLATE_NOOP( "ENT_Tr", "signals intelligence" ), eUnitCapaciteMission_SignalsIntelligence ),
    T_ConverterUnitCapaciteMission( "theatre missile defence", QT_TRANSLATE_NOOP( "ENT_Tr", "theatre missile defence" ), eUnitCapaciteMission_TheatreMissileDefence ),
    T_ConverterUnitCapaciteMission( "utilitaire", QT_TRANSLATE_NOOP( "ENT_Tr", "utilitaire" ), eUnitCapaciteMission_Utilitaire ),
    T_ConverterUnitCapaciteMission( "aeroporte", QT_TRANSLATE_NOOP( "ENT_Tr", "aeroporte" ), eUnitCapaciteMission_Aeroporte ),
    T_ConverterUnitCapaciteMission( "affaires publiques", QT_TRANSLATE_NOOP( "ENT_Tr", "affaires publiques" ), eUnitCapaciteMission_AffairesPubliques ),
    T_ConverterUnitCapaciteMission( "affaires publiques communication", QT_TRANSLATE_NOOP( "ENT_Tr", "affaires publiques communication" ), eUnitCapaciteMission_AffairesPubliquesCommunication ),
    T_ConverterUnitCapaciteMission( "affaires publiques joint information", QT_TRANSLATE_NOOP( "ENT_Tr", "affaires publiques joint information" ), eUnitCapaciteMission_AffairesPubliquesJointInformation ),
    T_ConverterUnitCapaciteMission( "alpin", QT_TRANSLATE_NOOP( "ENT_Tr", "alpin" ), eUnitCapaciteMission_Alpin ),
    T_ConverterUnitCapaciteMission( "amphibie", QT_TRANSLATE_NOOP( "ENT_Tr", "amphibie" ), eUnitCapaciteMission_Amphibie ),
    T_ConverterUnitCapaciteMission( "arctique", QT_TRANSLATE_NOOP( "ENT_Tr", "arctique" ), eUnitCapaciteMission_Arctique ),
    T_ConverterUnitCapaciteMission( "artillerie reconnaissance", QT_TRANSLATE_NOOP( "ENT_Tr", "artillerie reconnaissance" ), eUnitCapaciteMission_ArtillerieReconnaissance ),
    T_ConverterUnitCapaciteMission( "attaque", QT_TRANSLATE_NOOP( "ENT_Tr", "attaque" ), eUnitCapaciteMission_Attaque ),
    T_ConverterUnitCapaciteMission( "attaque aerienne", QT_TRANSLATE_NOOP( "ENT_Tr", "attaque aerienne" ), eUnitCapaciteMission_AttaqueAerienne ),
    T_ConverterUnitCapaciteMission( "c2", QT_TRANSLATE_NOOP( "ENT_Tr", "c2" ), eUnitCapaciteMission_C2 ),
    T_ConverterUnitCapaciteMission( "chemin de fer", QT_TRANSLATE_NOOP( "ENT_Tr", "chemin de fer" ), eUnitCapaciteMission_CheminDeFer ),
    T_ConverterUnitCapaciteMission( "ciblage", QT_TRANSLATE_NOOP( "ENT_Tr", "ciblage" ), eUnitCapaciteMission_Ciblage ),
    T_ConverterUnitCapaciteMission( "contre espionnage", QT_TRANSLATE_NOOP( "ENT_Tr", "contre espionnage" ), eUnitCapaciteMission_ContreEspionnage ),
    T_ConverterUnitCapaciteMission( "eclairage", QT_TRANSLATE_NOOP( "ENT_Tr", "eclairage" ), eUnitCapaciteMission_Eclairage ),
    T_ConverterUnitCapaciteMission( "entretien", QT_TRANSLATE_NOOP( "ENT_Tr", "entretien" ), eUnitCapaciteMission_Entretien ),
    T_ConverterUnitCapaciteMission( "entretien materiel", QT_TRANSLATE_NOOP( "ENT_Tr", "entretien materiel" ), eUnitCapaciteMission_EntretienMateriel ),
    T_ConverterUnitCapaciteMission( "entretien missile", QT_TRANSLATE_NOOP( "ENT_Tr", "entretien missile" ), eUnitCapaciteMission_EntretienMissile ),
    T_ConverterUnitCapaciteMission( "entretien opto electronique", QT_TRANSLATE_NOOP( "ENT_Tr", "entretien opto electronique" ), eUnitCapaciteMission_EntretienOptoElectronique ),
    T_ConverterUnitCapaciteMission( "espionnage", QT_TRANSLATE_NOOP( "ENT_Tr", "espionnage" ), eUnitCapaciteMission_Espionnage ),
    T_ConverterUnitCapaciteMission( "espionnage militaire aerial exploitation", QT_TRANSLATE_NOOP( "ENT_Tr", "espionnage militaire aerial exploitation" ), eUnitCapaciteMission_EspionnageMilitaireAerialExploitation ),
    T_ConverterUnitCapaciteMission( "espionnage militaire operation", QT_TRANSLATE_NOOP( "ENT_Tr", "espionnage militaire operation" ), eUnitCapaciteMission_EspionnageMilitaireOperation ),
    T_ConverterUnitCapaciteMission( "espionnage militaire tactical exploitation", QT_TRANSLATE_NOOP( "ENT_Tr", "espionnage militaire tactical exploitation" ), eUnitCapaciteMission_EspionnageMilitaireTacticalExploitation ),
    T_ConverterUnitCapaciteMission( "evacuation sanitaire", QT_TRANSLATE_NOOP( "ENT_Tr", "evacuation sanitaire" ), eUnitCapaciteMission_EvacuationSanitaire ),
    T_ConverterUnitCapaciteMission( "finance", QT_TRANSLATE_NOOP( "ENT_Tr", "finance" ), eUnitCapaciteMission_Finance ),
    T_ConverterUnitCapaciteMission( "genie combat", QT_TRANSLATE_NOOP( "ENT_Tr", "genie combat" ), eUnitCapaciteMission_GenieCombat ),
    T_ConverterUnitCapaciteMission( "genie construction", QT_TRANSLATE_NOOP( "ENT_Tr", "genie construction" ), eUnitCapaciteMission_GenieConstruction ),
    T_ConverterUnitCapaciteMission( "genie construction navale", QT_TRANSLATE_NOOP( "ENT_Tr", "genie construction navale" ), eUnitCapaciteMission_GenieConstructionNavale ),
    T_ConverterUnitCapaciteMission( "guerre electronique", QT_TRANSLATE_NOOP( "ENT_Tr", "guerre electronique" ), eUnitCapaciteMission_GuerreElectronique ),
    T_ConverterUnitCapaciteMission( "guerre electronique interception", QT_TRANSLATE_NOOP( "ENT_Tr", "guerre electronique interception" ), eUnitCapaciteMission_GuerreElectroniqueInterception ),
    T_ConverterUnitCapaciteMission( "guerre electronique brouillage", QT_TRANSLATE_NOOP( "ENT_Tr", "guerre electronique brouillage" ), eUnitCapaciteMission_GuerreElectroniqueBrouillage ),
    T_ConverterUnitCapaciteMission( "guerre electronique detection", QT_TRANSLATE_NOOP( "ENT_Tr", "guerre electronique detection" ), eUnitCapaciteMission_GuerreElectroniqueDetection ),
    T_ConverterUnitCapaciteMission( "interrogation", QT_TRANSLATE_NOOP( "ENT_Tr", "interrogation" ), eUnitCapaciteMission_Interrogation ),
    T_ConverterUnitCapaciteMission( "main d oeuvre", QT_TRANSLATE_NOOP( "ENT_Tr", "main d oeuvre" ), eUnitCapaciteMission_MainDOeuvre ),
    T_ConverterUnitCapaciteMission( "maintien de la paix", QT_TRANSLATE_NOOP( "ENT_Tr", "maintien de la paix" ), eUnitCapaciteMission_MaintienDeLaPaix ),
    T_ConverterUnitCapaciteMission( "marine", QT_TRANSLATE_NOOP( "ENT_Tr", "marine" ), eUnitCapaciteMission_Marine ),
    T_ConverterUnitCapaciteMission( "mine de contre mesure", QT_TRANSLATE_NOOP( "ENT_Tr", "mine de contre mesure" ), eUnitCapaciteMission_MineDeContreMesure ),
    T_ConverterUnitCapaciteMission( "nbc biologique", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc biologique" ), eUnitCapaciteMission_NbcBiologique ),
    T_ConverterUnitCapaciteMission( "nbc chimique", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc chimique" ), eUnitCapaciteMission_NbcChimique ),
    T_ConverterUnitCapaciteMission( "nbc chimique fumee", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc chimique fumee" ), eUnitCapaciteMission_NbcChimiqueFumee ),
    T_ConverterUnitCapaciteMission( "nbc decontamination", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc decontamination" ), eUnitCapaciteMission_NbcDecontamination ),
    T_ConverterUnitCapaciteMission( "nbc decontamination chimique", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc decontamination chimique" ), eUnitCapaciteMission_NbcDecontaminationChimique ),
    T_ConverterUnitCapaciteMission( "nbc nucleaire", QT_TRANSLATE_NOOP( "ENT_Tr", "nbc nucleaire" ), eUnitCapaciteMission_NbcNucleaire ),
    T_ConverterUnitCapaciteMission( "non renseigne", QT_TRANSLATE_NOOP( "ENT_Tr", "non renseigne" ), eUnitCapaciteMission_NonRenseigne ),
    T_ConverterUnitCapaciteMission( "observation", QT_TRANSLATE_NOOP( "ENT_Tr", "observation" ), eUnitCapaciteMission_Observation ),
    T_ConverterUnitCapaciteMission( "observation capteur", QT_TRANSLATE_NOOP( "ENT_Tr", "observation capteur" ), eUnitCapaciteMission_ObservationCapteur ),
    T_ConverterUnitCapaciteMission( "observation longue portee", QT_TRANSLATE_NOOP( "ENT_Tr", "observation longue portee" ), eUnitCapaciteMission_ObservationLonguePortee ),
    T_ConverterUnitCapaciteMission( "observation meteo", QT_TRANSLATE_NOOP( "ENT_Tr", "observation meteo" ), eUnitCapaciteMission_ObservationMeteo ),
    T_ConverterUnitCapaciteMission( "observation module terrestre", QT_TRANSLATE_NOOP( "ENT_Tr", "observation module terrestre" ), eUnitCapaciteMission_ObservationModuleTerrestre ),
    T_ConverterUnitCapaciteMission( "purification de l eau", QT_TRANSLATE_NOOP( "ENT_Tr", "purification de l eau" ), eUnitCapaciteMission_PurificationDeLEau ),
    T_ConverterUnitCapaciteMission( "ravitaillement class i", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement class i" ), eUnitCapaciteMission_RavitaillementClassi ),
    T_ConverterUnitCapaciteMission( "ravitaillement class ii", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement class ii" ), eUnitCapaciteMission_RavitaillementClassii ),
    T_ConverterUnitCapaciteMission( "ravitaillement class iii aviation", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement class iii aviation" ), eUnitCapaciteMission_RavitaillementClassIiiAviation ),
    T_ConverterUnitCapaciteMission( "ravitaillement class iii", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement class iii" ), eUnitCapaciteMission_RavitaillementClassiii ),
    T_ConverterUnitCapaciteMission( "ravitaillement class iv", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement class iv" ), eUnitCapaciteMission_RavitaillementClassIv ),
    T_ConverterUnitCapaciteMission( "ravitaillement class v", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement class v" ), eUnitCapaciteMission_RavitaillementClassv ),
    T_ConverterUnitCapaciteMission( "ravitaillement eau", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement eau" ), eUnitCapaciteMission_RavitaillementEau ),
    T_ConverterUnitCapaciteMission( "ravitaillement blanchisserie bain", QT_TRANSLATE_NOOP( "ENT_Tr", "ravitaillement blanchisserie bain" ), eUnitCapaciteMission_RavitaillementBlanchisserieBain ),
    T_ConverterUnitCapaciteMission( "recherche et sauvetage", QT_TRANSLATE_NOOP( "ENT_Tr", "recherche et sauvetage" ), eUnitCapaciteMission_RechercheEtSauvetage ),
    T_ConverterUnitCapaciteMission( "reconnaissance", QT_TRANSLATE_NOOP( "ENT_Tr", "reconnaissance" ), eUnitCapaciteMission_Reconnaissance ),
    T_ConverterUnitCapaciteMission( "reconnaissance aerienne tactique", QT_TRANSLATE_NOOP( "ENT_Tr", "reconnaissance aerienne tactique" ), eUnitCapaciteMission_ReconnaissanceAerienneTactique ),
    T_ConverterUnitCapaciteMission( "recuperation", QT_TRANSLATE_NOOP( "ENT_Tr", "recuperation" ), eUnitCapaciteMission_Recuperation ),
    T_ConverterUnitCapaciteMission( "service du personnel", QT_TRANSLATE_NOOP( "ENT_Tr", "service du personnel" ), eUnitCapaciteMission_ServiceDuPersonnel ),
    T_ConverterUnitCapaciteMission( "service juridique", QT_TRANSLATE_NOOP( "ENT_Tr", "service juridique" ), eUnitCapaciteMission_ServiceJuridique ),
    T_ConverterUnitCapaciteMission( "support aerien rapproche", QT_TRANSLATE_NOOP( "ENT_Tr", "support aerien rapproche" ), eUnitCapaciteMission_SupportAerienRapproche ),
    T_ConverterUnitCapaciteMission( "transmissions forward communications", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions forward communications" ), eUnitCapaciteMission_TransmissionsForwardCommunications ),
    T_ConverterUnitCapaciteMission( "transmissions node centre", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions node centre" ), eUnitCapaciteMission_TransmissionsNodeCentre ),
    T_ConverterUnitCapaciteMission( "transmissions node large extension", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions node large extension" ), eUnitCapaciteMission_TransmissionsNodeLargeExtension ),
    T_ConverterUnitCapaciteMission( "transmissions node small extension", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions node small extension" ), eUnitCapaciteMission_TransmissionsNodeSmallExtension ),
    T_ConverterUnitCapaciteMission( "transmissions radio relay", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions radio relay" ), eUnitCapaciteMission_TransmissionsRadioRelay ),
    T_ConverterUnitCapaciteMission( "transmissions radio tactical satellite", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions radio tactical satellite" ), eUnitCapaciteMission_TransmissionsRadioTacticalSatellite ),
    T_ConverterUnitCapaciteMission( "transmissions radio teletype", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions radio teletype" ), eUnitCapaciteMission_TransmissionsRadioTeletype ),
    T_ConverterUnitCapaciteMission( "transmissions support", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions support" ), eUnitCapaciteMission_TransmissionsSupport ),
    T_ConverterUnitCapaciteMission( "transmissions radio", QT_TRANSLATE_NOOP( "ENT_Tr", "transmissions radio" ), eUnitCapaciteMission_TransmissionsRadio ),
    T_ConverterUnitCapaciteMission( "transport allocation des mouvements", QT_TRANSLATE_NOOP( "ENT_Tr", "transport allocation des mouvements" ), eUnitCapaciteMission_TransportAllocationDesMouvements ),
    T_ConverterUnitCapaciteMission( "transport apod apoe", QT_TRANSLATE_NOOP( "ENT_Tr", "transport apod apoe" ), eUnitCapaciteMission_TransportApodApoe ),
    T_ConverterUnitCapaciteMission( "transport spod spoe", QT_TRANSLATE_NOOP( "ENT_Tr", "transport spod spoe" ), eUnitCapaciteMission_TransportSpodSpoe ),
    T_ConverterUnitCapaciteMission( "transport missile", QT_TRANSLATE_NOOP( "ENT_Tr", "transport missile" ), eUnitCapaciteMission_TransportMissile ),
    T_ConverterUnitCapaciteMission( "verrouillage de cible", QT_TRANSLATE_NOOP( "ENT_Tr", "verrouillage de cible" ), eUnitCapaciteMission_VerrouillageDeCible ),
    T_ConverterUnitCapaciteMission( "verrouillage de cible flash", QT_TRANSLATE_NOOP( "ENT_Tr", "verrouillage de cible flash" ), eUnitCapaciteMission_VerrouillageDeCibleFlash ),
    T_ConverterUnitCapaciteMission( "verrouillage de cible radar", QT_TRANSLATE_NOOP( "ENT_Tr", "verrouillage de cible radar" ), eUnitCapaciteMission_VerrouillageDeCibleRadar ),
    T_ConverterUnitCapaciteMission( "verrouillage de cible sonore", QT_TRANSLATE_NOOP( "ENT_Tr", "verrouillage de cible sonore" ), eUnitCapaciteMission_VerrouillageDeCibleSonore ),
    T_ConverterUnitCapaciteMission( "", "", (E_UnitCapaciteMission)-1 )
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

ENT_Tr::T_ConverterTempsBordee ENT_Tr::TempsBordeeConverter_[] =
{
    T_ConverterTempsBordee( "temps 8 heures", QT_TRANSLATE_NOOP( "ENT_Tr", "temps 8 heures" ), eTempsBordee_Temps8Heures ),
    T_ConverterTempsBordee( "temps 12 heures", QT_TRANSLATE_NOOP( "ENT_Tr", "temps 12 heures" ), eTempsBordee_Temps12Heures ),
    T_ConverterTempsBordee( "temps 16 heures", QT_TRANSLATE_NOOP( "ENT_Tr", "temps 16 heures" ), eTempsBordee_Temps16Heures ),
    T_ConverterTempsBordee( "temps 20 heures", QT_TRANSLATE_NOOP( "ENT_Tr", "temps 20 heures" ), eTempsBordee_Temps20Heures ),
    T_ConverterTempsBordee( "temps 24 heures", QT_TRANSLATE_NOOP( "ENT_Tr", "temps 24 heures" ), eTempsBordee_Temps24Heures ),
    T_ConverterTempsBordee( "", "", (E_TempsBordee)-1 )
};

ENT_Tr::T_ConverterMissionALAT_PorteeAction ENT_Tr::MissionALAT_PorteeActionConverter_[] =
{
    T_ConverterMissionALAT_PorteeAction( "courte portee", QT_TRANSLATE_NOOP( "ENT_Tr", "courte portee" ), eMissionALAT_PorteeAction_CourtePortee ),
    T_ConverterMissionALAT_PorteeAction( "moyenne portee", QT_TRANSLATE_NOOP( "ENT_Tr", "moyenne portee" ), eMissionALAT_PorteeAction_MoyennePortee ),
    T_ConverterMissionALAT_PorteeAction( "longue portee", QT_TRANSLATE_NOOP( "ENT_Tr", "longue portee" ), eMissionALAT_PorteeAction_LonguePortee ),
    T_ConverterMissionALAT_PorteeAction( "sans munitions", QT_TRANSLATE_NOOP( "ENT_Tr", "sans munitions" ), eMissionALAT_PorteeAction_SansMunitions ),
    T_ConverterMissionALAT_PorteeAction( "", "", (E_MissionALAT_PorteeAction)-1 )
};

ENT_Tr::T_ConverterMissionALAT_AmbianceMission ENT_Tr::MissionALAT_AmbianceMissionConverter_[] =
{
    T_ConverterMissionALAT_AmbianceMission( "aa", QT_TRANSLATE_NOOP( "ENT_Tr", "aa" ), eMissionALAT_AmbianceMission_aa ),
    T_ConverterMissionALAT_AmbianceMission( "as", QT_TRANSLATE_NOOP( "ENT_Tr", "as" ), eMissionALAT_AmbianceMission_As ),
    T_ConverterMissionALAT_AmbianceMission( "polyvalent", QT_TRANSLATE_NOOP( "ENT_Tr", "polyvalent" ), eMissionALAT_AmbianceMission_Polyvalent ),
    T_ConverterMissionALAT_AmbianceMission( "", "", (E_MissionALAT_AmbianceMission)-1 )
};

ENT_Tr::T_ConverterModeDeploiement ENT_Tr::ModeDeploiementConverter_[] =
{
    T_ConverterModeDeploiement( "en carre", QT_TRANSLATE_NOOP( "ENT_Tr", "en carre" ), eModeDeploiement_EnCarre ),
    T_ConverterModeDeploiement( "en triangle", QT_TRANSLATE_NOOP( "ENT_Tr", "en triangle" ), eModeDeploiement_EnTriangle ),
    T_ConverterModeDeploiement( "nasse trois sections", QT_TRANSLATE_NOOP( "ENT_Tr", "nasse trois sections" ), eModeDeploiement_NasseTroisSections ),
    T_ConverterModeDeploiement( "nasse quatre sections", QT_TRANSLATE_NOOP( "ENT_Tr", "nasse quatre sections" ), eModeDeploiement_NasseQuatreSections ),
    T_ConverterModeDeploiement( "double rideau", QT_TRANSLATE_NOOP( "ENT_Tr", "double rideau" ), eModeDeploiement_DoubleRideau ),
    T_ConverterModeDeploiement( "simple rideau", QT_TRANSLATE_NOOP( "ENT_Tr", "simple rideau" ), eModeDeploiement_SimpleRideau ),
    T_ConverterModeDeploiement( "", "", (E_ModeDeploiement)-1 )
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
    T_ConverterOrderErrorCode( "", "", (E_OrderErrorCode)-1 )
};

ENT_Tr::T_ConverterOrderState ENT_Tr::OrderStateConverter_[] =
{
    T_ConverterOrderState( "started", QT_TRANSLATE_NOOP( "ENT_Tr", "started" ), eOrderState_Started ),
    T_ConverterOrderState( "cancelled", QT_TRANSLATE_NOOP( "ENT_Tr", "cancelled" ), eOrderState_Cancelled ),
    T_ConverterOrderState( "stopped", QT_TRANSLATE_NOOP( "ENT_Tr", "stopped" ), eOrderState_Stopped ),
    T_ConverterOrderState( "", "", (E_OrderState)-1 )
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

ENT_Tr::T_ConverterMissionGenUrgence ENT_Tr::MissionGenUrgenceConverter_[] =
{
    T_ConverterMissionGenUrgence( "serie minimum", QT_TRANSLATE_NOOP( "ENT_Tr", "serie minimum" ), eMissionGenUrgence_SerieMinimum ),
    T_ConverterMissionGenUrgence( "serie complementaire", QT_TRANSLATE_NOOP( "ENT_Tr", "serie complementaire" ), eMissionGenUrgence_SerieComplementaire ),
    T_ConverterMissionGenUrgence( "", "", (E_MissionGenUrgence)-1 )
};

ENT_Tr::T_ConverterMissionGenSousTypeObstacle ENT_Tr::MissionGenSousTypeObstacleConverter_[] =
{
    T_ConverterMissionGenSousTypeObstacle( "preliminaire", QT_TRANSLATE_NOOP( "ENT_Tr", "preliminaire" ), eMissionGenSousTypeObstacle_Preliminaire ),
    T_ConverterMissionGenSousTypeObstacle( "de manoeuvre", QT_TRANSLATE_NOOP( "ENT_Tr", "de manoeuvre" ), eMissionGenSousTypeObstacle_DeManoeuvre ),
    T_ConverterMissionGenSousTypeObstacle( "", "", (E_MissionGenSousTypeObstacle)-1 )
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

ENT_Tr::T_ConverterMissionGenPriorite ENT_Tr::MissionGenPrioriteConverter_[] =
{
    T_ConverterMissionGenPriorite( "prioritaire", QT_TRANSLATE_NOOP( "ENT_Tr", "prioritaire" ), eMissionGenPriorite_Prioritaire ),
    T_ConverterMissionGenPriorite( "normal", QT_TRANSLATE_NOOP( "ENT_Tr", "normal" ), eMissionGenPriorite_Normal ),
    T_ConverterMissionGenPriorite( "peu prioritaire", QT_TRANSLATE_NOOP( "ENT_Tr", "peu prioritaire" ), eMissionGenPriorite_PeuPrioritaire ),
    T_ConverterMissionGenPriorite( "", "", (E_MissionGenPriorite)-1 )
};

ENT_Tr::T_ConverterModeDeploiementSectionMistral ENT_Tr::ModeDeploiementSectionMistralConverter_[] =
{
    T_ConverterModeDeploiementSectionMistral( "omnidirectionnel", QT_TRANSLATE_NOOP( "ENT_Tr", "omnidirectionnel" ), eModeDeploiementSectionMistral_Omnidirectionnel ),
    T_ConverterModeDeploiementSectionMistral( "directionnel", QT_TRANSLATE_NOOP( "ENT_Tr", "directionnel" ), eModeDeploiementSectionMistral_Directionnel ),
    T_ConverterModeDeploiementSectionMistral( "", "", (E_ModeDeploiementSectionMistral)-1 )
};

ENT_Tr::T_ConverterTypeFosseAC ENT_Tr::TypeFosseACConverter_[] =
{
    T_ConverterTypeFosseAC( "simple", QT_TRANSLATE_NOOP( "ENT_Tr", "simple" ), eTypeFosseAC_Simple ),
    T_ConverterTypeFosseAC( "compose", QT_TRANSLATE_NOOP( "ENT_Tr", "compose" ), eTypeFosseAC_Compose ),
    T_ConverterTypeFosseAC( "", "", (E_TypeFosseAC)-1 )
};

ENT_Tr::T_ConverterGenTypePontage ENT_Tr::GenTypePontageConverter_[] =
{
    T_ConverterGenTypePontage( "continu", QT_TRANSLATE_NOOP( "ENT_Tr", "continu" ), eGenTypePontage_Continu ),
    T_ConverterGenTypePontage( "discontinu", QT_TRANSLATE_NOOP( "ENT_Tr", "discontinu" ), eGenTypePontage_Discontinu ),
    T_ConverterGenTypePontage( "", "", (E_GenTypePontage)-1 )
};

ENT_Tr::T_ConverterMissionInfNatureObjectif ENT_Tr::MissionInfNatureObjectifConverter_[] =
{
    T_ConverterMissionInfNatureObjectif( "elements terrain", QT_TRANSLATE_NOOP( "ENT_Tr", "elements terrain" ), eMissionInfNatureObjectif_ElementsTerrain ),
    T_ConverterMissionInfNatureObjectif( "objets", QT_TRANSLATE_NOOP( "ENT_Tr", "objets" ), eMissionInfNatureObjectif_Objets ),
    T_ConverterMissionInfNatureObjectif( "elements mobiles", QT_TRANSLATE_NOOP( "ENT_Tr", "elements mobiles" ), eMissionInfNatureObjectif_ElementsMobiles ),
    T_ConverterMissionInfNatureObjectif( "", "", (E_MissionInfNatureObjectif)-1 )
};

ENT_Tr::T_ConverterAutomateOrderFormation ENT_Tr::AutomateOrderFormationConverter_[] =
{
    T_ConverterAutomateOrderFormation( "un echelon", QT_TRANSLATE_NOOP( "ENT_Tr", "un echelon" ), eAutomateOrderFormation_UnEchelon ),
    T_ConverterAutomateOrderFormation( "deux echelons", QT_TRANSLATE_NOOP( "ENT_Tr", "deux echelons" ), eAutomateOrderFormation_DeuxEchelons ),
    T_ConverterAutomateOrderFormation( "", "", (E_AutomateOrderFormation)-1 )
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

ENT_Tr::T_ConverterMsgCtrlClientAnnouncement ENT_Tr::MsgCtrlClientAnnouncementConverter_[] =
{
    T_ConverterMsgCtrlClientAnnouncement( "mos", QT_TRANSLATE_NOOP( "ENT_Tr", "mos" ), eMsgCtrlClientAnnouncement_Mos ),
    T_ConverterMsgCtrlClientAnnouncement( "mos light", QT_TRANSLATE_NOOP( "ENT_Tr", "mos light" ), eMsgCtrlClientAnnouncement_MosLight ),
    T_ConverterMsgCtrlClientAnnouncement( "", "", (E_MsgCtrlClientAnnouncement)-1 )
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
    T_ConverterInfoContextErrorCode( "error invalid limit or lima used", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid limit or lima used" ), eInfoContextErrorCode_ErrorInvalidLimitOrLimaUsed ),
    T_ConverterInfoContextErrorCode( "", "", (E_InfoContextErrorCode)-1 )
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

ENT_Tr::T_ConverterChangeGroupeConnaissanceErrorCode ENT_Tr::ChangeGroupeConnaissanceErrorCodeConverter_[] =
{
    T_ConverterChangeGroupeConnaissanceErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeGroupeConnaissanceErrorCode_NoError ),
    T_ConverterChangeGroupeConnaissanceErrorCode( "error invalid automate", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate" ), eChangeGroupeConnaissanceErrorCode_ErrorInvalidAutomate ),
    T_ConverterChangeGroupeConnaissanceErrorCode( "error invalid camp", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid camp" ), eChangeGroupeConnaissanceErrorCode_ErrorInvalidCamp ),
    T_ConverterChangeGroupeConnaissanceErrorCode( "error invalid groupe connaissance", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid groupe connaissance" ), eChangeGroupeConnaissanceErrorCode_ErrorInvalidGroupeConnaissance ),
    T_ConverterChangeGroupeConnaissanceErrorCode( "", "", (E_ChangeGroupeConnaissanceErrorCode)-1 )
};

ENT_Tr::T_ConverterChangeLiensLogistiquesErrorCode ENT_Tr::ChangeLiensLogistiquesErrorCodeConverter_[] =
{
    T_ConverterChangeLiensLogistiquesErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeLiensLogistiquesErrorCode_NoError ),
    T_ConverterChangeLiensLogistiquesErrorCode( "error invalid automate", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate" ), eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomate ),
    T_ConverterChangeLiensLogistiquesErrorCode( "error invalid automate tc2", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate tc2" ), eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateTc2 ),
    T_ConverterChangeLiensLogistiquesErrorCode( "error invalid automate maintenance", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate maintenance" ), eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateMaintenance ),
    T_ConverterChangeLiensLogistiquesErrorCode( "error invalid automate sante", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate sante" ), eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateSante ),
    T_ConverterChangeLiensLogistiquesErrorCode( "error invalid automate ravitaillement", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate ravitaillement" ), eChangeLiensLogistiquesErrorCode_ErrorInvalidAutomateRavitaillement ),
    T_ConverterChangeLiensLogistiquesErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eChangeLiensLogistiquesErrorCode_ErrorUnitSurrendered ),
    T_ConverterChangeLiensLogistiquesErrorCode( "", "", (E_ChangeLiensLogistiquesErrorCode)-1 )
};

ENT_Tr::T_ConverterChangeAutomateErrorCode ENT_Tr::ChangeAutomateErrorCodeConverter_[] =
{
    T_ConverterChangeAutomateErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eChangeAutomateErrorCode_NoError ),
    T_ConverterChangeAutomateErrorCode( "error invalid pion", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid pion" ), eChangeAutomateErrorCode_ErrorInvalidPion ),
    T_ConverterChangeAutomateErrorCode( "error invalid automate", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid automate" ), eChangeAutomateErrorCode_ErrorInvalidAutomate ),
    T_ConverterChangeAutomateErrorCode( "error camps incompatibles", QT_TRANSLATE_NOOP( "ENT_Tr", "error camps incompatibles" ), eChangeAutomateErrorCode_ErrorCampsIncompatibles ),
    T_ConverterChangeAutomateErrorCode( "", "", (E_ChangeAutomateErrorCode)-1 )
};

ENT_Tr::T_ConverterCtrlErrorCodeLauncher ENT_Tr::CtrlErrorCodeLauncherConverter_[] =
{
    T_ConverterCtrlErrorCodeLauncher( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eCtrlErrorCodeLauncher_NoError ),
    T_ConverterCtrlErrorCodeLauncher( "error already started", QT_TRANSLATE_NOOP( "ENT_Tr", "error already started" ), eCtrlErrorCodeLauncher_ErrorAlreadyStarted ),
    T_ConverterCtrlErrorCodeLauncher( "error not started", QT_TRANSLATE_NOOP( "ENT_Tr", "error not started" ), eCtrlErrorCodeLauncher_ErrorNotStarted ),
    T_ConverterCtrlErrorCodeLauncher( "error not paused", QT_TRANSLATE_NOOP( "ENT_Tr", "error not paused" ), eCtrlErrorCodeLauncher_ErrorNotPaused ),
    T_ConverterCtrlErrorCodeLauncher( "error already paused", QT_TRANSLATE_NOOP( "ENT_Tr", "error already paused" ), eCtrlErrorCodeLauncher_ErrorAlreadyPaused ),
    T_ConverterCtrlErrorCodeLauncher( "error invalid time factor", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid time factor" ), eCtrlErrorCodeLauncher_ErrorInvalidTimeFactor ),
    T_ConverterCtrlErrorCodeLauncher( "error invalid scenario identifier", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid scenario identifier" ), eCtrlErrorCodeLauncher_ErrorInvalidScenarioIdentifier ),
    T_ConverterCtrlErrorCodeLauncher( "error error scenario init", QT_TRANSLATE_NOOP( "ENT_Tr", "error error scenario init" ), eCtrlErrorCodeLauncher_ErrorErrorScenarioInit ),
    T_ConverterCtrlErrorCodeLauncher( "error invalid data path", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid data path" ), eCtrlErrorCodeLauncher_ErrorInvalidDataPath ),
    T_ConverterCtrlErrorCodeLauncher( "error invalid data", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid data" ), eCtrlErrorCodeLauncher_ErrorInvalidData ),
    T_ConverterCtrlErrorCodeLauncher( "error invalid checkpoint identifier", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid checkpoint identifier" ), eCtrlErrorCodeLauncher_ErrorInvalidCheckpointIdentifier ),
    T_ConverterCtrlErrorCodeLauncher( "error error checkpoint init", QT_TRANSLATE_NOOP( "ENT_Tr", "error error checkpoint init" ), eCtrlErrorCodeLauncher_ErrorErrorCheckpointInit ),
    T_ConverterCtrlErrorCodeLauncher( "error sim killed by remote", QT_TRANSLATE_NOOP( "ENT_Tr", "error sim killed by remote" ), eCtrlErrorCodeLauncher_ErrorSimKilledByRemote ),
    T_ConverterCtrlErrorCodeLauncher( "", "", (E_CtrlErrorCodeLauncher)-1 )
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
    T_ConverterLogMaintenanceTraitementEtat( "attente retour pion", QT_TRANSLATE_NOOP( "ENT_Tr", "attente retour pion" ), eLogMaintenanceTraitementEtat_AttenteRetourPion ),
    T_ConverterLogMaintenanceTraitementEtat( "retour pion", QT_TRANSLATE_NOOP( "ENT_Tr", "retour pion" ), eLogMaintenanceTraitementEtat_RetourPion ),
    T_ConverterLogMaintenanceTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "termine" ), eLogMaintenanceTraitementEtat_Termine ),
    T_ConverterLogMaintenanceTraitementEtat( "", "", (E_LogMaintenanceTraitementEtat)-1 )
};

ENT_Tr::T_ConverterLogRavitaillementTraitementEtat ENT_Tr::LogRavitaillementTraitementEtatConverter_[] =
{
    T_ConverterLogRavitaillementTraitementEtat( "convoi en attente camions", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi en attente camions" ), eLogRavitaillementTraitementEtat_ConvoiEnAttenteCamions ),
    T_ConverterLogRavitaillementTraitementEtat( "convoi en attente chef convoi", QT_TRANSLATE_NOOP( "ENT_Tr", "convoi en attente chef convoi" ), eLogRavitaillementTraitementEtat_ConvoiEnAttenteChefConvoi ),
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
    T_ConverterMsgLogRavitaillementPousserFluxAck( "error ravitaillement en cours", QT_TRANSLATE_NOOP( "ENT_Tr", "error ravitaillement en cours" ), eMsgLogRavitaillementPousserFluxAck_ErrorRavitaillementEnCours ),
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
    T_ConverterLogSanteTraitementEtat( "attente disponibilite ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "attente disponibilite ambulance ramassage" ), eLogSanteTraitementEtat_AttenteDisponibiliteAmbulanceRamassage ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage chargement", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance ramassage chargement" ), eLogSanteTraitementEtat_AmbulanceRamassageChargement ),
    T_ConverterLogSanteTraitementEtat( "attente chargement complet ambulance ramassage", QT_TRANSLATE_NOOP( "ENT_Tr", "attente chargement complet ambulance ramassage" ), eLogSanteTraitementEtat_AttenteChargementCompletAmbulanceRamassage ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage deplacement aller", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance ramassage deplacement aller" ), eLogSanteTraitementEtat_AmbulanceRamassageDeplacementAller ),
    T_ConverterLogSanteTraitementEtat( "ambulance ramassage dechargement", QT_TRANSLATE_NOOP( "ENT_Tr", "ambulance ramassage dechargement" ), eLogSanteTraitementEtat_AmbulanceRamassageDechargement ),
    T_ConverterLogSanteTraitementEtat( "termine", QT_TRANSLATE_NOOP( "ENT_Tr", "termine" ), eLogSanteTraitementEtat_Termine ),
    T_ConverterLogSanteTraitementEtat( "", "", (E_LogSanteTraitementEtat)-1 )
};

ENT_Tr::T_ConverterAutomateState ENT_Tr::AutomateStateConverter_[] =
{
    T_ConverterAutomateState( "embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "embraye" ), eAutomateState_Embraye ),
    T_ConverterAutomateState( "debraye", QT_TRANSLATE_NOOP( "ENT_Tr", "debraye" ), eAutomateState_Debraye ),
    T_ConverterAutomateState( "", "", (E_AutomateState)-1 )
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

ENT_Tr::T_ConverterDisponibiliteAuTir ENT_Tr::DisponibiliteAuTirConverter_[] =
{
    T_ConverterDisponibiliteAuTir( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eDisponibiliteAuTir_None ),
    T_ConverterDisponibiliteAuTir( "pret au tir", QT_TRANSLATE_NOOP( "ENT_Tr", "pret au tir" ), eDisponibiliteAuTir_PretAuTir ),
    T_ConverterDisponibiliteAuTir( "indisponible", QT_TRANSLATE_NOOP( "ENT_Tr", "indisponible" ), eDisponibiliteAuTir_Indisponible ),
    T_ConverterDisponibiliteAuTir( "", "", (E_DisponibiliteAuTir)-1 )
};

ENT_Tr::T_ConverterRoe ENT_Tr::RoeConverter_[] =
{
    T_ConverterRoe( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoe_None ),
    T_ConverterRoe( "tir libre", QT_TRANSLATE_NOOP( "ENT_Tr", "tir libre" ), eRoe_TirLibre ),
    T_ConverterRoe( "tir sur riposte", QT_TRANSLATE_NOOP( "ENT_Tr", "tir sur riposte" ), eRoe_TirSurRiposte ),
    T_ConverterRoe( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "tir interdit" ), eRoe_TirInterdit ),
    T_ConverterRoe( "", "", (E_Roe)-1 )
};

ENT_Tr::T_ConverterRoePopulation ENT_Tr::RoePopulationConverter_[] =
{
    T_ConverterRoePopulation( "none", QT_TRANSLATE_NOOP( "ENT_Tr", "none" ), eRoePopulation_None ),
    T_ConverterRoePopulation( "tir interdit", QT_TRANSLATE_NOOP( "ENT_Tr", "tir interdit" ), eRoePopulation_TirInterdit ),
    T_ConverterRoePopulation( "tir sommation", QT_TRANSLATE_NOOP( "ENT_Tr", "tir sommation" ), eRoePopulation_TirSommation ),
    T_ConverterRoePopulation( "tir dissuasion", QT_TRANSLATE_NOOP( "ENT_Tr", "tir dissuasion" ), eRoePopulation_TirDissuasion ),
    T_ConverterRoePopulation( "tir neutralisation", QT_TRANSLATE_NOOP( "ENT_Tr", "tir neutralisation" ), eRoePopulation_TirNeutralisation ),
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

ENT_Tr::T_ConverterUnitAttrErrorCode ENT_Tr::UnitAttrErrorCodeConverter_[] =
{
    T_ConverterUnitAttrErrorCode( "no error", QT_TRANSLATE_NOOP( "ENT_Tr", "no error" ), eUnitAttrErrorCode_NoError ),
    T_ConverterUnitAttrErrorCode( "error invalid unit", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid unit" ), eUnitAttrErrorCode_ErrorInvalidUnit ),
    T_ConverterUnitAttrErrorCode( "error automate embraye", QT_TRANSLATE_NOOP( "ENT_Tr", "error automate embraye" ), eUnitAttrErrorCode_ErrorAutomateEmbraye ),
    T_ConverterUnitAttrErrorCode( "error invalid attribute", QT_TRANSLATE_NOOP( "ENT_Tr", "error invalid attribute" ), eUnitAttrErrorCode_ErrorInvalidAttribute ),
    T_ConverterUnitAttrErrorCode( "error unit surrendered", QT_TRANSLATE_NOOP( "ENT_Tr", "error unit surrendered" ), eUnitAttrErrorCode_ErrorUnitSurrendered ),
    T_ConverterUnitAttrErrorCode( "", "", (E_UnitAttrErrorCode)-1 )
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

ENT_Tr::T_ConverterAmbianceMission ENT_Tr::AmbianceMissionConverter_[] =
{
    T_ConverterAmbianceMission( "surete", QT_TRANSLATE_NOOP( "ENT_Tr", "surete" ), eAmbianceMission_Surete ),
    T_ConverterAmbianceMission( "vitesse", QT_TRANSLATE_NOOP( "ENT_Tr", "vitesse" ), eAmbianceMission_Vitesse ),
    T_ConverterAmbianceMission( "", "", (E_AmbianceMission)-1 )
};

ENT_Tr::T_ConverterTypeMunitionTirIndirect ENT_Tr::TypeMunitionTirIndirectConverter_[] =
{
    T_ConverterTypeMunitionTirIndirect( "explosif", QT_TRANSLATE_NOOP( "ENT_Tr", "explosif" ), eTypeMunitionTirIndirect_Explosif ),
    T_ConverterTypeMunitionTirIndirect( "grenade", QT_TRANSLATE_NOOP( "ENT_Tr", "grenade" ), eTypeMunitionTirIndirect_Grenade ),
    T_ConverterTypeMunitionTirIndirect( "aced", QT_TRANSLATE_NOOP( "ENT_Tr", "aced" ), eTypeMunitionTirIndirect_Aced ),
    T_ConverterTypeMunitionTirIndirect( "fumigene", QT_TRANSLATE_NOOP( "ENT_Tr", "fumigene" ), eTypeMunitionTirIndirect_Fumigene ),
    T_ConverterTypeMunitionTirIndirect( "eclairant", QT_TRANSLATE_NOOP( "ENT_Tr", "eclairant" ), eTypeMunitionTirIndirect_Eclairant ),
    T_ConverterTypeMunitionTirIndirect( "mine", QT_TRANSLATE_NOOP( "ENT_Tr", "mine" ), eTypeMunitionTirIndirect_Mine ),
    T_ConverterTypeMunitionTirIndirect( "", "", (E_TypeMunitionTirIndirect)-1 )
};

ENT_Tr::T_ConverterActionReagirFaceAEni ENT_Tr::ActionReagirFaceAEniConverter_[] =
{
    T_ConverterActionReagirFaceAEni( "detruire moyens", QT_TRANSLATE_NOOP( "ENT_Tr", "detruire moyens" ), eActionReagirFaceAEni_DetruireMoyens ),
    T_ConverterActionReagirFaceAEni( "defendre", QT_TRANSLATE_NOOP( "ENT_Tr", "defendre" ), eActionReagirFaceAEni_Defendre ),
    T_ConverterActionReagirFaceAEni( "demonter", QT_TRANSLATE_NOOP( "ENT_Tr", "demonter" ), eActionReagirFaceAEni_Demonter ),
    T_ConverterActionReagirFaceAEni( "", "", (E_ActionReagirFaceAEni)-1 )
};

ENT_Tr::T_ConverterTC2GererMaterielAvantDeplacement ENT_Tr::TC2GererMaterielAvantDeplacementConverter_[] =
{
    T_ConverterTC2GererMaterielAvantDeplacement( "deborder materiels vers bld", QT_TRANSLATE_NOOP( "ENT_Tr", "deborder materiels vers bld" ), eTC2GererMaterielAvantDeplacement_DeborderMaterielsVersBld ),
    T_ConverterTC2GererMaterielAvantDeplacement( "detruire materiels en reparation", QT_TRANSLATE_NOOP( "ENT_Tr", "detruire materiels en reparation" ), eTC2GererMaterielAvantDeplacement_DetruireMaterielsEnReparation ),
    T_ConverterTC2GererMaterielAvantDeplacement( "reparer materiels sur place", QT_TRANSLATE_NOOP( "ENT_Tr", "reparer materiels sur place" ), eTC2GererMaterielAvantDeplacement_ReparerMaterielsSurPlace ),
    T_ConverterTC2GererMaterielAvantDeplacement( "", "", (E_TC2GererMaterielAvantDeplacement)-1 )
};




ENT_Tr::T_ConverterAutomataMission ENT_Tr::automataMissionConverter_[] =
{
    T_ConverterAutomataMission( "Automate ABC ArmerPointDeControle", QT_TRANSLATE_NOOP( "ENT", "Automate ABC ArmerPointDeControle" ), eMission_Automate_ABC_ArmerPointDeControle ),
    T_ConverterAutomataMission( "Automate ABC OrganiserAccueilColonneRefugies", QT_TRANSLATE_NOOP( "ENT", "Automate ABC OrganiserAccueilColonneRefugies" ), eMission_Automate_ABC_OrganiserAccueilColonneRefugies ),
    T_ConverterAutomataMission( "Automate ABC ProtegerPC", QT_TRANSLATE_NOOP( "ENT", "Automate ABC ProtegerPC" ), eMission_Automate_ABC_ProtegerPC ),
    T_ConverterAutomataMission( "Automate ABC Appuyer", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Appuyer" ), eMission_Automate_ABC_Appuyer ),
    T_ConverterAutomataMission( "Automate ABC Attaquer", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Attaquer" ), eMission_Automate_ABC_Attaquer ),
    T_ConverterAutomataMission( "Automate ABC ContreAttaquer", QT_TRANSLATE_NOOP( "ENT", "Automate ABC ContreAttaquer" ), eMission_Automate_ABC_ContreAttaquer ),
    T_ConverterAutomataMission( "Automate ABC Controler", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Controler" ), eMission_Automate_ABC_Controler ),
    T_ConverterAutomataMission( "Automate ABC Couvrir", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Couvrir" ), eMission_Automate_ABC_Couvrir ),
    T_ConverterAutomataMission( "Automate ABC DonnerCoupArret", QT_TRANSLATE_NOOP( "ENT", "Automate ABC DonnerCoupArret" ), eMission_Automate_ABC_DonnerCoupArret ),
    T_ConverterAutomataMission( "Automate ABC Eclairer", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Eclairer" ), eMission_Automate_ABC_Eclairer ),
    T_ConverterAutomataMission( "Automate ABC EffectuerRaid", QT_TRANSLATE_NOOP( "ENT", "Automate ABC EffectuerRaid" ), eMission_Automate_ABC_EffectuerRaid ),
    T_ConverterAutomataMission( "Automate ABC Interdire", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Interdire" ), eMission_Automate_ABC_Interdire ),
    T_ConverterAutomataMission( "Automate ABC Jalonner", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Jalonner" ), eMission_Automate_ABC_Jalonner ),
    T_ConverterAutomataMission( "Automate ABC PrendrePreciserContact", QT_TRANSLATE_NOOP( "ENT", "Automate ABC PrendrePreciserContact" ), eMission_Automate_ABC_PrendrePreciserContact ),
    T_ConverterAutomataMission( "Automate ABC Reconnaitre", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Reconnaitre" ), eMission_Automate_ABC_Reconnaitre ),
    T_ConverterAutomataMission( "Automate ABC ReconnaitrePoint", QT_TRANSLATE_NOOP( "ENT", "Automate ABC ReconnaitrePoint" ), eMission_Automate_ABC_ReconnaitrePoint ),
    T_ConverterAutomataMission( "Automate ABC Reduire", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Reduire" ), eMission_Automate_ABC_Reduire ),
    T_ConverterAutomataMission( "Automate ABC ReleverEnDepassant", QT_TRANSLATE_NOOP( "ENT", "Automate ABC ReleverEnDepassant" ), eMission_Automate_ABC_ReleverEnDepassant ),
    T_ConverterAutomataMission( "Automate ABC Semparer", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Semparer" ), eMission_Automate_ABC_Semparer ),
    T_ConverterAutomataMission( "Automate ABC Tenir", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Tenir" ), eMission_Automate_ABC_Tenir ),
    T_ConverterAutomataMission( "Automate ABC Soutenir", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Soutenir" ), eMission_Automate_ABC_Soutenir ),
    T_ConverterAutomataMission( "Automate ABC Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Surveiller" ), eMission_Automate_ABC_Surveiller ),
    T_ConverterAutomataMission( "Automate ABC FlancGarder", QT_TRANSLATE_NOOP( "ENT", "Automate ABC FlancGarder" ), eMission_Automate_ABC_FlancGarder ),
    T_ConverterAutomataMission( "Automate ABC RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Automate ABC RenseignerSur" ), eMission_Automate_ABC_RenseignerSur ),
    T_ConverterAutomataMission( "Automate ABC Fixer", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Fixer" ), eMission_Automate_ABC_Fixer ),
    T_ConverterAutomataMission( "Automate ABC Freiner", QT_TRANSLATE_NOOP( "ENT", "Automate ABC Freiner" ), eMission_Automate_ABC_Freiner ),
    T_ConverterAutomataMission( "Automate ABC SurveillerItineraire", QT_TRANSLATE_NOOP( "ENT", "Automate ABC SurveillerItineraire" ), eMission_Automate_ABC_SurveillerItineraire ),
    T_ConverterAutomataMission( "Automate ABC RenseignerSurUnAxe", QT_TRANSLATE_NOOP( "ENT", "Automate ABC RenseignerSurUnAxe" ), eMission_Automate_ABC_RenseignerSurUnAxe ),
    T_ConverterAutomataMission( "Automate ABC RecueillirUnite", QT_TRANSLATE_NOOP( "ENT", "Automate ABC RecueillirUnite" ), eMission_Automate_ABC_RecueillirUnite ),
    T_ConverterAutomataMission( "Automate ABC EscorterUnite", QT_TRANSLATE_NOOP( "ENT", "Automate ABC EscorterUnite" ), eMission_Automate_ABC_EscorterUnite ),
    T_ConverterAutomataMission( "Automate ABC EscorterUnites", QT_TRANSLATE_NOOP( "ENT", "Automate ABC EscorterUnites" ), eMission_Automate_ABC_EscorterUnites ),
    T_ConverterAutomataMission( "Automate ABC AppuyerSurPosition", QT_TRANSLATE_NOOP( "ENT", "Automate ABC AppuyerSurPosition" ), eMission_Automate_ABC_AppuyerSurPosition ),
    T_ConverterAutomataMission( "Automate ALAT Heliporter", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Heliporter" ), eMission_Automate_ALAT_Heliporter ),
    T_ConverterAutomataMission( "Automate ALAT Jalonner", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Jalonner" ), eMission_Automate_ALAT_Jalonner ),
    T_ConverterAutomataMission( "Automate ALAT Helitransporter", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Helitransporter" ), eMission_Automate_ALAT_Helitransporter ),
    T_ConverterAutomataMission( "Automate ALAT ExtrairePersonnel", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT ExtrairePersonnel" ), eMission_Automate_ALAT_ExtrairePersonnel ),
    T_ConverterAutomataMission( "Automate ALAT Appuyer", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Appuyer" ), eMission_Automate_ALAT_Appuyer ),
    T_ConverterAutomataMission( "Automate ALAT Couvrir", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Couvrir" ), eMission_Automate_ALAT_Couvrir ),
    T_ConverterAutomataMission( "Automate ALAT Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Surveiller" ), eMission_Automate_ALAT_Surveiller ),
    T_ConverterAutomataMission( "Automate ALAT Reconnaitre", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Reconnaitre" ), eMission_Automate_ALAT_Reconnaitre ),
    T_ConverterAutomataMission( "Automate ALAT Freiner", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Freiner" ), eMission_Automate_ALAT_Freiner ),
    T_ConverterAutomataMission( "Automate ALAT Escorter", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Escorter" ), eMission_Automate_ALAT_Escorter ),
    T_ConverterAutomataMission( "Automate ALAT Attaquer", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT Attaquer" ), eMission_Automate_ALAT_Attaquer ),
    T_ConverterAutomataMission( "Automate ALAT RecueillirFormationAeromobile", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT RecueillirFormationAeromobile" ), eMission_Automate_ALAT_RecueillirFormationAeromobile ),
    T_ConverterAutomataMission( "Automate ALAT AttaquerFormationAeromobile", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT AttaquerFormationAeromobile" ), eMission_Automate_ALAT_AttaquerFormationAeromobile ),
    T_ConverterAutomataMission( "Automate ALAT DonnerCoupArret", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT DonnerCoupArret" ), eMission_Automate_ALAT_DonnerCoupArret ),
    T_ConverterAutomataMission( "Automate ALAT DetruireNeutraliserDansProfondeur", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT DetruireNeutraliserDansProfondeur" ), eMission_Automate_ALAT_DetruireNeutraliserDansProfondeur ),
    T_ConverterAutomataMission( "Automate ALAT DetruireNeutraliserDansZone", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT DetruireNeutraliserDansZone" ), eMission_Automate_ALAT_DetruireNeutraliserDansZone ),
    T_ConverterAutomataMission( "Automate ALAT EffectuerRechercheEtSauvetage", QT_TRANSLATE_NOOP( "ENT", "Automate ALAT EffectuerRechercheEtSauvetage" ), eMission_Automate_ALAT_EffectuerRechercheEtSauvetage ),
    T_ConverterAutomataMission( "Automate ASA DefendreZone", QT_TRANSLATE_NOOP( "ENT", "Automate ASA DefendreZone" ), eMission_Automate_ASA_DefendreZone ),
    T_ConverterAutomataMission( "Automate ASA DefendreSite", QT_TRANSLATE_NOOP( "ENT", "Automate ASA DefendreSite" ), eMission_Automate_ASA_DefendreSite ),
    T_ConverterAutomataMission( "Automate ASA Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate ASA Surveiller" ), eMission_Automate_ASA_Surveiller ),
    T_ConverterAutomataMission( "Automate ASA DefendreItineraire", QT_TRANSLATE_NOOP( "ENT", "Automate ASA DefendreItineraire" ), eMission_Automate_ASA_DefendreItineraire ),
    T_ConverterAutomataMission( "Automate ASA MISTRAL DefendreZone", QT_TRANSLATE_NOOP( "ENT", "Automate ASA MISTRAL DefendreZone" ), eMission_Automate_ASA_MISTRAL_DefendreZone ),
    T_ConverterAutomataMission( "Automate ASA MISTRAL DefendreSite", QT_TRANSLATE_NOOP( "ENT", "Automate ASA MISTRAL DefendreSite" ), eMission_Automate_ASA_MISTRAL_DefendreSite ),
    T_ConverterAutomataMission( "Automate ASA MISTRAL Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate ASA MISTRAL Surveiller" ), eMission_Automate_ASA_MISTRAL_Surveiller ),
    T_ConverterAutomataMission( "Automate ASA HAWK DefendreZone", QT_TRANSLATE_NOOP( "ENT", "Automate ASA HAWK DefendreZone" ), eMission_Automate_ASA_HAWK_DefendreZone ),
    T_ConverterAutomataMission( "Automate ASA HAWK DefendreSite", QT_TRANSLATE_NOOP( "ENT", "Automate ASA HAWK DefendreSite" ), eMission_Automate_ASA_HAWK_DefendreSite ),
    T_ConverterAutomataMission( "Automate ASA HAWK Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate ASA HAWK Surveiller" ), eMission_Automate_ASA_HAWK_Surveiller ),
    T_ConverterAutomataMission( "Automate ASA DefendreUnites", QT_TRANSLATE_NOOP( "ENT", "Automate ASA DefendreUnites" ), eMission_Automate_ASA_DefendreUnites ),
    T_ConverterAutomataMission( "Automate ASS SeDeployer", QT_TRANSLATE_NOOP( "ENT", "Automate ASS SeDeployer" ), eMission_Automate_ASS_SeDeployer ),
    T_ConverterAutomataMission( "Automate ASS ReconnaitreZonesDeploiement", QT_TRANSLATE_NOOP( "ENT", "Automate ASS ReconnaitreZonesDeploiement" ), eMission_Automate_ASS_ReconnaitreZonesDeploiement ),
    T_ConverterAutomataMission( "Automate ASS AcquerirObjectifs", QT_TRANSLATE_NOOP( "ENT", "Automate ASS AcquerirObjectifs" ), eMission_Automate_ASS_AcquerirObjectifs ),
    T_ConverterAutomataMission( "Automate GEN SeDeployer", QT_TRANSLATE_NOOP( "ENT", "Automate GEN SeDeployer" ), eMission_Automate_GEN_SeDeployer ),
    T_ConverterAutomataMission( "Automate GEN FaireFranchir", QT_TRANSLATE_NOOP( "ENT", "Automate GEN FaireFranchir" ), eMission_Automate_GEN_FaireFranchir ),
    T_ConverterAutomataMission( "Automate GEN RealiserSystemeObstacles", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RealiserSystemeObstacles" ), eMission_Automate_GEN_RealiserSystemeObstacles ),
    T_ConverterAutomataMission( "Automate GEN FranchirDeViveForce", QT_TRANSLATE_NOOP( "ENT", "Automate GEN FranchirDeViveForce" ), eMission_Automate_GEN_FranchirDeViveForce ),
    T_ConverterAutomataMission( "Automate GEN RetablirItineraires", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RetablirItineraires" ), eMission_Automate_GEN_RetablirItineraires ),
    T_ConverterAutomataMission( "Automate GEN AmenagerAireStationnement", QT_TRANSLATE_NOOP( "ENT", "Automate GEN AmenagerAireStationnement" ), eMission_Automate_GEN_AmenagerAireStationnement ),
    T_ConverterAutomataMission( "Automate GEN AmenagerTerrainPoserSommaire", QT_TRANSLATE_NOOP( "ENT", "Automate GEN AmenagerTerrainPoserSommaire" ), eMission_Automate_GEN_AmenagerTerrainPoserSommaire ),
    T_ConverterAutomataMission( "Automate GEN RealiserZonePoser", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RealiserZonePoser" ), eMission_Automate_GEN_RealiserZonePoser ),
    T_ConverterAutomataMission( "Automate GEN ReconnaitreZoneDeploiement", QT_TRANSLATE_NOOP( "ENT", "Automate GEN ReconnaitreZoneDeploiement" ), eMission_Automate_GEN_ReconnaitreZoneDeploiement ),
    T_ConverterAutomataMission( "Automate GEN RealiserCampPrisonniers", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RealiserCampPrisonniers" ), eMission_Automate_GEN_RealiserCampPrisonniers ),
    T_ConverterAutomataMission( "Automate GEN RealiserCampRefugies", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RealiserCampRefugies" ), eMission_Automate_GEN_RealiserCampRefugies ),
    T_ConverterAutomataMission( "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RealiserPlatesformesSommairesDeDeploiementInitial" ), eMission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial ),
    T_ConverterAutomataMission( "Automate GEN RealiserTravauxSommairesAuProfitPopulation", QT_TRANSLATE_NOOP( "ENT", "Automate GEN RealiserTravauxSommairesAuProfitPopulation" ), eMission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation ),
    T_ConverterAutomataMission( "Automate GEN AppuyerDebarquementTroupes", QT_TRANSLATE_NOOP( "ENT", "Automate GEN AppuyerDebarquementTroupes" ), eMission_Automate_GEN_AppuyerDebarquementTroupes ),
    T_ConverterAutomataMission( "Automate GEN OuvrirAxesProgression", QT_TRANSLATE_NOOP( "ENT", "Automate GEN OuvrirAxesProgression" ), eMission_Automate_GEN_OuvrirAxesProgression ),
    T_ConverterAutomataMission( "Automate INF AttaquerSouplesse", QT_TRANSLATE_NOOP( "ENT", "Automate INF AttaquerSouplesse" ), eMission_Automate_INF_AttaquerSouplesse ),
    T_ConverterAutomataMission( "Automate INF Appuyer", QT_TRANSLATE_NOOP( "ENT", "Automate INF Appuyer" ), eMission_Automate_INF_Appuyer ),
    T_ConverterAutomataMission( "Automate INF Controler", QT_TRANSLATE_NOOP( "ENT", "Automate INF Controler" ), eMission_Automate_INF_Controler ),
    T_ConverterAutomataMission( "Automate INF Couvrir", QT_TRANSLATE_NOOP( "ENT", "Automate INF Couvrir" ), eMission_Automate_INF_Couvrir ),
    T_ConverterAutomataMission( "Automate INF DonnerCoupArret", QT_TRANSLATE_NOOP( "ENT", "Automate INF DonnerCoupArret" ), eMission_Automate_INF_DonnerCoupArret ),
    T_ConverterAutomataMission( "Automate INF Eclairer", QT_TRANSLATE_NOOP( "ENT", "Automate INF Eclairer" ), eMission_Automate_INF_Eclairer ),
    T_ConverterAutomataMission( "Automate INF FlancGarder", QT_TRANSLATE_NOOP( "ENT", "Automate INF FlancGarder" ), eMission_Automate_INF_FlancGarder ),
    T_ConverterAutomataMission( "Automate INF Freiner", QT_TRANSLATE_NOOP( "ENT", "Automate INF Freiner" ), eMission_Automate_INF_Freiner ),
    T_ConverterAutomataMission( "Automate INF Harceler", QT_TRANSLATE_NOOP( "ENT", "Automate INF Harceler" ), eMission_Automate_INF_Harceler ),
    T_ConverterAutomataMission( "Automate INF Interdire", QT_TRANSLATE_NOOP( "ENT", "Automate INF Interdire" ), eMission_Automate_INF_Interdire ),
    T_ConverterAutomataMission( "Automate INF MenerDefenseUsure", QT_TRANSLATE_NOOP( "ENT", "Automate INF MenerDefenseUsure" ), eMission_Automate_INF_MenerDefenseUsure ),
    T_ConverterAutomataMission( "Automate INF PrendrePreciserMaintenirContact", QT_TRANSLATE_NOOP( "ENT", "Automate INF PrendrePreciserMaintenirContact" ), eMission_Automate_INF_PrendrePreciserMaintenirContact ),
    T_ConverterAutomataMission( "Automate INF ReconnaissanceOffensive", QT_TRANSLATE_NOOP( "ENT", "Automate INF ReconnaissanceOffensive" ), eMission_Automate_INF_ReconnaissanceOffensive ),
    T_ConverterAutomataMission( "Automate INF RecueillirUnite", QT_TRANSLATE_NOOP( "ENT", "Automate INF RecueillirUnite" ), eMission_Automate_INF_RecueillirUnite ),
    T_ConverterAutomataMission( "Automate INF Reduire", QT_TRANSLATE_NOOP( "ENT", "Automate INF Reduire" ), eMission_Automate_INF_Reduire ),
    T_ConverterAutomataMission( "Automate INF Relever", QT_TRANSLATE_NOOP( "ENT", "Automate INF Relever" ), eMission_Automate_INF_Relever ),
    T_ConverterAutomataMission( "Automate INF RompreContact", QT_TRANSLATE_NOOP( "ENT", "Automate INF RompreContact" ), eMission_Automate_INF_RompreContact ),
    T_ConverterAutomataMission( "Automate INF Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate INF Surveiller" ), eMission_Automate_INF_Surveiller ),
    T_ConverterAutomataMission( "Automate INF Semparer", QT_TRANSLATE_NOOP( "ENT", "Automate INF Semparer" ), eMission_Automate_INF_Semparer ),
    T_ConverterAutomataMission( "Automate INF Fixer", QT_TRANSLATE_NOOP( "ENT", "Automate INF Fixer" ), eMission_Automate_INF_Fixer ),
    T_ConverterAutomataMission( "Automate INF AttaquerEnTerrainDifficile", QT_TRANSLATE_NOOP( "ENT", "Automate INF AttaquerEnTerrainDifficile" ), eMission_Automate_INF_AttaquerEnTerrainDifficile ),
    T_ConverterAutomataMission( "Automate INF BarrerDirection", QT_TRANSLATE_NOOP( "ENT", "Automate INF BarrerDirection" ), eMission_Automate_INF_BarrerDirection ),
    T_ConverterAutomataMission( "Automate INF DefendreFerme", QT_TRANSLATE_NOOP( "ENT", "Automate INF DefendreFerme" ), eMission_Automate_INF_DefendreFerme ),
    T_ConverterAutomataMission( "Automate INF Tenir", QT_TRANSLATE_NOOP( "ENT", "Automate INF Tenir" ), eMission_Automate_INF_Tenir ),
    T_ConverterAutomataMission( "Automate INF AppuyerUnFreinage", QT_TRANSLATE_NOOP( "ENT", "Automate INF AppuyerUnFreinage" ), eMission_Automate_INF_AppuyerUnFreinage ),
    T_ConverterAutomataMission( "Automate INF AppuyerUneAttaque", QT_TRANSLATE_NOOP( "ENT", "Automate INF AppuyerUneAttaque" ), eMission_Automate_INF_AppuyerUneAttaque ),
    T_ConverterAutomataMission( "Automate INF Soutenir", QT_TRANSLATE_NOOP( "ENT", "Automate INF Soutenir" ), eMission_Automate_INF_Soutenir ),
    T_ConverterAutomataMission( "Automate INF EscorterUnite", QT_TRANSLATE_NOOP( "ENT", "Automate INF EscorterUnite" ), eMission_Automate_INF_EscorterUnite ),
    T_ConverterAutomataMission( "Automate INF EscorterUnites", QT_TRANSLATE_NOOP( "ENT", "Automate INF EscorterUnites" ), eMission_Automate_INF_EscorterUnites ),
    T_ConverterAutomataMission( "Automate INF AppuyerSurPosition", QT_TRANSLATE_NOOP( "ENT", "Automate INF AppuyerSurPosition" ), eMission_Automate_INF_AppuyerSurPosition ),
    T_ConverterAutomataMission( "Automate LOG SeDeployer", QT_TRANSLATE_NOOP( "ENT", "Automate LOG SeDeployer" ), eMission_Automate_LOG_SeDeployer ),
    T_ConverterAutomataMission( "Automate LOG AppuyerMouvement", QT_TRANSLATE_NOOP( "ENT", "Automate LOG AppuyerMouvement" ), eMission_Automate_LOG_AppuyerMouvement ),
    T_ConverterAutomataMission( "Automate LOG ReconnaitreItineraire", QT_TRANSLATE_NOOP( "ENT", "Automate LOG ReconnaitreItineraire" ), eMission_Automate_LOG_ReconnaitreItineraire ),
    T_ConverterAutomataMission( "Automate LOG Surveiller", QT_TRANSLATE_NOOP( "ENT", "Automate LOG Surveiller" ), eMission_Automate_LOG_Surveiller ),
    T_ConverterAutomataMission( "Automate LOG TransporterUnites", QT_TRANSLATE_NOOP( "ENT", "Automate LOG TransporterUnites" ), eMission_Automate_LOG_TransporterUnites ),
    T_ConverterAutomataMission( "Automate LOG AppuyerMouvementDansZone", QT_TRANSLATE_NOOP( "ENT", "Automate LOG AppuyerMouvementDansZone" ), eMission_Automate_LOG_AppuyerMouvementDansZone ),
    T_ConverterAutomataMission( "Automate LOG MettreEnOeuvreZoneStationnement", QT_TRANSLATE_NOOP( "ENT", "Automate LOG MettreEnOeuvreZoneStationnement" ), eMission_Automate_LOG_MettreEnOeuvreZoneStationnement ),
    T_ConverterAutomataMission( "Automate LOG AppuyerFranchissementDansZone", QT_TRANSLATE_NOOP( "ENT", "Automate LOG AppuyerFranchissementDansZone" ), eMission_Automate_LOG_AppuyerFranchissementDansZone ),
    T_ConverterAutomataMission( "Automate LOG BloquerZone", QT_TRANSLATE_NOOP( "ENT", "Automate LOG BloquerZone" ), eMission_Automate_LOG_BloquerZone ),
    T_ConverterAutomataMission( "Automate LOG ReconnaitreZoneContaminee", QT_TRANSLATE_NOOP( "ENT", "Automate LOG ReconnaitreZoneContaminee" ), eMission_Automate_LOG_ReconnaitreZoneContaminee ),
    T_ConverterAutomataMission( "Automate LOG ReconnaitreZoneDeDeploiement", QT_TRANSLATE_NOOP( "ENT", "Automate LOG ReconnaitreZoneDeDeploiement" ), eMission_Automate_LOG_ReconnaitreZoneDeDeploiement ),
    T_ConverterAutomataMission( "Automate LOG ArmerPIAs", QT_TRANSLATE_NOOP( "ENT", "Automate LOG ArmerPIAs" ), eMission_Automate_LOG_ArmerPIAs ),
    T_ConverterAutomataMission( "Automate Test MoveTo", QT_TRANSLATE_NOOP( "ENT", "Automate Test MoveTo" ), eMission_Automate_Test_MoveTo ),
    T_ConverterAutomataMission( "Automate Decrocher", QT_TRANSLATE_NOOP( "ENT", "Automate Decrocher" ), eMission_Automate_Decrocher ),
    T_ConverterAutomataMission( "Automate ReleverSurPlace", QT_TRANSLATE_NOOP( "ENT", "Automate ReleverSurPlace" ), eMission_Automate_ReleverSurPlace ),
    T_ConverterAutomataMission( "Automate FaireMouvement", QT_TRANSLATE_NOOP( "ENT", "Automate FaireMouvement" ), eMission_Automate_FaireMouvement ),
    T_ConverterAutomataMission( "Automate SeFaireDecontaminer", QT_TRANSLATE_NOOP( "ENT", "Automate SeFaireDecontaminer" ), eMission_Automate_SeFaireDecontaminer ),
    T_ConverterAutomataMission( "Automate Franchir", QT_TRANSLATE_NOOP( "ENT", "Automate Franchir" ), eMission_Automate_Franchir ),
    T_ConverterAutomataMission( "Automate Stationner", QT_TRANSLATE_NOOP( "ENT", "Automate Stationner" ), eMission_Automate_Stationner ),
    T_ConverterAutomataMission( "Automate SeFaireTransporter", QT_TRANSLATE_NOOP( "ENT", "Automate SeFaireTransporter" ), eMission_Automate_SeFaireTransporter ),
    T_ConverterAutomataMission( "Automate SeRecompleter", QT_TRANSLATE_NOOP( "ENT", "Automate SeRecompleter" ), eMission_Automate_SeRecompleter ),
    T_ConverterAutomataMission( "Automate SeRendre", QT_TRANSLATE_NOOP( "ENT", "Automate SeRendre" ), eMission_Automate_SeRendre ),
    T_ConverterAutomataMission( "Automate FaireMouvementVersCampRefugies", QT_TRANSLATE_NOOP( "ENT", "Automate FaireMouvementVersCampRefugies" ), eMission_Automate_FaireMouvementVersCampRefugies ),
    T_ConverterAutomataMission( "Automate Generique", QT_TRANSLATE_NOOP( "ENT", "Automate Generique" ), eMission_Automate_Generique ),
    T_ConverterAutomataMission( "Automate NBC ReconnaitreUnAxe", QT_TRANSLATE_NOOP( "ENT", "Automate NBC ReconnaitreUnAxe" ), eMission_Automate_NBC_ReconnaitreUnAxe ),
    T_ConverterAutomataMission( "Automate NBC ReconnaitreUneZone", QT_TRANSLATE_NOOP( "ENT", "Automate NBC ReconnaitreUneZone" ), eMission_Automate_NBC_ReconnaitreUneZone ),
    T_ConverterAutomataMission( "Automate NBC ArmerUnSiteDeDecontamination", QT_TRANSLATE_NOOP( "ENT", "Automate NBC ArmerUnSiteDeDecontamination" ), eMission_Automate_NBC_ArmerUnSiteDeDecontamination ),
    T_ConverterAutomataMission( "Automate NBC DecontaminerUneZone", QT_TRANSLATE_NOOP( "ENT", "Automate NBC DecontaminerUneZone" ), eMission_Automate_NBC_DecontaminerUneZone ),
    T_ConverterAutomataMission( "Automate NBC ReconnaitreDesSitesROTA", QT_TRANSLATE_NOOP( "ENT", "Automate NBC ReconnaitreDesSitesROTA" ), eMission_Automate_NBC_ReconnaitreDesSitesROTA ),
    T_ConverterAutomataMission( "Automate RENS ROHUM RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROHUM RenseignerSur" ), eMission_Automate_RENS_ROHUM_RenseignerSur ),
    T_ConverterAutomataMission( "Automate RENS ROHUM OrienterGuider", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROHUM OrienterGuider" ), eMission_Automate_RENS_ROHUM_OrienterGuider ),
    T_ConverterAutomataMission( "Automate RENS ROHUM SExfiltrer", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROHUM SExfiltrer" ), eMission_Automate_RENS_ROHUM_SExfiltrer ),
    T_ConverterAutomataMission( "Automate RENS ROIM CL289 RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROIM CL289 RenseignerSur" ), eMission_Automate_RENS_ROIM_CL289_RenseignerSur ),
    T_ConverterAutomataMission( "Automate RENS ROIM SDTI RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROIM SDTI RenseignerSur" ), eMission_Automate_RENS_ROIM_SDTI_RenseignerSur ),
    T_ConverterAutomataMission( "Automate RENS ROEM Appuyer", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROEM Appuyer" ), eMission_Automate_RENS_ROEM_Appuyer ),
    T_ConverterAutomataMission( "Automate RENS ROEM RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Automate RENS ROEM RenseignerSur" ), eMission_Automate_RENS_ROEM_RenseignerSur ),
    T_ConverterAutomataMission( "Automate RENS RADINT RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Automate RENS RADINT RenseignerSur" ), eMission_Automate_RENS_RADINT_RenseignerSur ),
    T_ConverterAutomataMission( "Automate TRANS Commander", QT_TRANSLATE_NOOP( "ENT", "Automate TRANS Commander" ), eMission_Automate_TRANS_Commander ),

    T_ConverterAutomataMission( "", "", (E_AutomataMission)-1  )
};


ENT_Tr::T_ConverterUnitMission ENT_Tr::unitMissionConverter_[] =
{
    T_ConverterUnitMission( "Pion ALAT EvacuationSanitaire", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT EvacuationSanitaire" ), eMission_Pion_ALAT_EvacuationSanitaire ),
    T_ConverterUnitMission( "Pion ALAT ReconnaitreDansLaProfondeur", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT ReconnaitreDansLaProfondeur" ), eMission_Pion_ALAT_ReconnaitreDansLaProfondeur ),
    T_ConverterUnitMission( "Pion ALAT ReconnaitreContourEnnemi", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT ReconnaitreContourEnnemi" ), eMission_Pion_ALAT_ReconnaitreContourEnnemi ),
    T_ConverterUnitMission( "Pion ALAT DetruireNeutraliserDansZone", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT DetruireNeutraliserDansZone" ), eMission_Pion_ALAT_DetruireNeutraliserDansZone ),
    T_ConverterUnitMission( "Pion ALAT DetruireAeromobile", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT DetruireAeromobile" ), eMission_Pion_ALAT_DetruireAeromobile ),
    T_ConverterUnitMission( "Pion ALAT Freiner", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Freiner" ), eMission_Pion_ALAT_Freiner ),
    T_ConverterUnitMission( "Pion ALAT Jalonner", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Jalonner" ), eMission_Pion_ALAT_Jalonner ),
    T_ConverterUnitMission( "Pion ALAT Escorter", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Escorter" ), eMission_Pion_ALAT_Escorter ),
    T_ConverterUnitMission( "Pion ALAT Heliporter", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Heliporter" ), eMission_Pion_ALAT_Heliporter ),
    T_ConverterUnitMission( "Pion ALAT HeliporterHelitransporterPlotRavitaillement", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT HeliporterHelitransporterPlotRavitaillement" ), eMission_Pion_ALAT_HeliporterHelitransporterPlotRavitaillement ),
    T_ConverterUnitMission( "Pion ALAT Helitransporter", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Helitransporter" ), eMission_Pion_ALAT_Helitransporter ),
    T_ConverterUnitMission( "Pion ALAT EffectuerRechercheEtSauvetage", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT EffectuerRechercheEtSauvetage" ), eMission_Pion_ALAT_EffectuerRechercheEtSauvetage ),
    T_ConverterUnitMission( "Pion ALAT IMEX", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT IMEX" ), eMission_Pion_ALAT_IMEX ),
    T_ConverterUnitMission( "Pion ALAT Eclairer", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Eclairer" ), eMission_Pion_ALAT_Eclairer ),
    T_ConverterUnitMission( "Pion ALAT Surveiller", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Surveiller" ), eMission_Pion_ALAT_Surveiller ),
    T_ConverterUnitMission( "Pion ALAT AppuyerDirectAuContact", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT AppuyerDirectAuContact" ), eMission_Pion_ALAT_AppuyerDirectAuContact ),
    T_ConverterUnitMission( "Pion ALAT Couvrir", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Couvrir" ), eMission_Pion_ALAT_Couvrir ),
    T_ConverterUnitMission( "Pion ALAT Attaquer", QT_TRANSLATE_NOOP( "ENT", "Pion ALAT Attaquer" ), eMission_Pion_ALAT_Attaquer ),
    T_ConverterUnitMission( "Pion ASA DefendreSite", QT_TRANSLATE_NOOP( "ENT", "Pion ASA DefendreSite" ), eMission_Pion_ASA_DefendreSite ),
    T_ConverterUnitMission( "Pion ASA DefendreSiteAPartirPosition", QT_TRANSLATE_NOOP( "ENT", "Pion ASA DefendreSiteAPartirPosition" ), eMission_Pion_ASA_DefendreSiteAPartirPosition ),
    T_ConverterUnitMission( "Pion ASA DefendreZone", QT_TRANSLATE_NOOP( "ENT", "Pion ASA DefendreZone" ), eMission_Pion_ASA_DefendreZone ),
    T_ConverterUnitMission( "Pion ASA DefendreZoneAPartirPosition", QT_TRANSLATE_NOOP( "ENT", "Pion ASA DefendreZoneAPartirPosition" ), eMission_Pion_ASA_DefendreZoneAPartirPosition ),
    T_ConverterUnitMission( "Pion ASA Surveiller", QT_TRANSLATE_NOOP( "ENT", "Pion ASA Surveiller" ), eMission_Pion_ASA_Surveiller ),
    T_ConverterUnitMission( "Pion ASA DefendreUnites", QT_TRANSLATE_NOOP( "ENT", "Pion ASA DefendreUnites" ), eMission_Pion_ASA_DefendreUnites ),
    T_ConverterUnitMission( "Pion ASA MISTRAL DefendreSite", QT_TRANSLATE_NOOP( "ENT", "Pion ASA MISTRAL DefendreSite" ), eMission_Pion_ASA_MISTRAL_DefendreSite ),
    T_ConverterUnitMission( "Pion ASA MISTRAL DefendreSiteAPartirPosition", QT_TRANSLATE_NOOP( "ENT", "Pion ASA MISTRAL DefendreSiteAPartirPosition" ), eMission_Pion_ASA_MISTRAL_DefendreSiteAPartirPosition ),
    T_ConverterUnitMission( "Pion ASA MISTRAL DefendreZone", QT_TRANSLATE_NOOP( "ENT", "Pion ASA MISTRAL DefendreZone" ), eMission_Pion_ASA_MISTRAL_DefendreZone ),
    T_ConverterUnitMission( "Pion ASA MISTRAL DefendreZoneAPartirPosition", QT_TRANSLATE_NOOP( "ENT", "Pion ASA MISTRAL DefendreZoneAPartirPosition" ), eMission_Pion_ASA_MISTRAL_DefendreZoneAPartirPosition ),
    T_ConverterUnitMission( "Pion ASA MISTRAL Surveiller", QT_TRANSLATE_NOOP( "ENT", "Pion ASA MISTRAL Surveiller" ), eMission_Pion_ASA_MISTRAL_Surveiller ),
    T_ConverterUnitMission( "Pion ASS AssurerMiseEnOeuvre", QT_TRANSLATE_NOOP( "ENT", "Pion ASS AssurerMiseEnOeuvre" ), eMission_Pion_ASS_AssurerMiseEnOeuvre ),
    T_ConverterUnitMission( "Pion ASS AssurerMiseEnOeuvreSurPosition", QT_TRANSLATE_NOOP( "ENT", "Pion ASS AssurerMiseEnOeuvreSurPosition" ), eMission_Pion_ASS_AssurerMiseEnOeuvreSurPosition ),
    T_ConverterUnitMission( "Pion ASS ReconnaitreZoneDeploiement", QT_TRANSLATE_NOOP( "ENT", "Pion ASS ReconnaitreZoneDeploiement" ), eMission_Pion_ASS_ReconnaitreZoneDeploiement ),
    T_ConverterUnitMission( "Pion ASS AcquerirLanceursEni", QT_TRANSLATE_NOOP( "ENT", "Pion ASS AcquerirLanceursEni" ), eMission_Pion_ASS_AcquerirLanceursEni ),
    T_ConverterUnitMission( "Pion ASS EO AcquerirObjectifs", QT_TRANSLATE_NOOP( "ENT", "Pion ASS EO AcquerirObjectifs" ), eMission_Pion_ASS_EO_AcquerirObjectifs ),
    T_ConverterUnitMission( "Pion ASS EOP AcquerirObjectifs", QT_TRANSLATE_NOOP( "ENT", "Pion ASS EOP AcquerirObjectifs" ), eMission_Pion_ASS_EOP_AcquerirObjectifs ),
    T_ConverterUnitMission( "Pion ASS RATAC AcquerirObjectifs", QT_TRANSLATE_NOOP( "ENT", "Pion ASS RATAC AcquerirObjectifs" ), eMission_Pion_ASS_RATAC_AcquerirObjectifs ),
    T_ConverterUnitMission( "Pion ABC ReconnaitreAxe", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ReconnaitreAxe" ), eMission_Pion_ABC_ReconnaitreAxe ),
    T_ConverterUnitMission( "Pion ABC Soutenir", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Soutenir" ), eMission_Pion_ABC_Soutenir ),
    T_ConverterUnitMission( "Pion ABC Couvrir", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Couvrir" ), eMission_Pion_ABC_Couvrir ),
    T_ConverterUnitMission( "Pion ABC FlancGarder", QT_TRANSLATE_NOOP( "ENT", "Pion ABC FlancGarder" ), eMission_Pion_ABC_FlancGarder ),
    T_ConverterUnitMission( "Pion ABC Reduire", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Reduire" ), eMission_Pion_ABC_Reduire ),
    T_ConverterUnitMission( "Pion ABC Appuyer", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Appuyer" ), eMission_Pion_ABC_Appuyer ),
    T_ConverterUnitMission( "Pion ABC Fixer", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Fixer" ), eMission_Pion_ABC_Fixer ),
    T_ConverterUnitMission( "Pion ABC RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion ABC RenseignerSur" ), eMission_Pion_ABC_RenseignerSur ),
    T_ConverterUnitMission( "Pion ABC ControlerZone", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ControlerZone" ), eMission_Pion_ABC_ControlerZone ),
    T_ConverterUnitMission( "Pion ABC Surveiller", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Surveiller" ), eMission_Pion_ABC_Surveiller ),
    T_ConverterUnitMission( "Pion ABC Barrer", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Barrer" ), eMission_Pion_ABC_Barrer ),
    T_ConverterUnitMission( "Pion ABC Attaquer", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Attaquer" ), eMission_Pion_ABC_Attaquer ),
    T_ConverterUnitMission( "Pion ABC ReconnaitrePoint", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ReconnaitrePoint" ), eMission_Pion_ABC_ReconnaitrePoint ),
    T_ConverterUnitMission( "Pion ABC Eclairer", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Eclairer" ), eMission_Pion_ABC_Eclairer ),
    T_ConverterUnitMission( "Pion ABC Jalonner", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Jalonner" ), eMission_Pion_ABC_Jalonner ),
    T_ConverterUnitMission( "Pion ABC ContreAttaquer", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ContreAttaquer" ), eMission_Pion_ABC_ContreAttaquer ),
    T_ConverterUnitMission( "Pion ABC SemparerZone", QT_TRANSLATE_NOOP( "ENT", "Pion ABC SemparerZone" ), eMission_Pion_ABC_SemparerZone ),
    T_ConverterUnitMission( "Pion ABC ReleverUnite", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ReleverUnite" ), eMission_Pion_ABC_ReleverUnite ),
    T_ConverterUnitMission( "Pion ABC ArmerPIA", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ArmerPIA" ), eMission_Pion_ABC_ArmerPIA ),
    T_ConverterUnitMission( "Pion ABC OuvrirItineraire", QT_TRANSLATE_NOOP( "ENT", "Pion ABC OuvrirItineraire" ), eMission_Pion_ABC_OuvrirItineraire ),
    T_ConverterUnitMission( "Pion ABC Freiner", QT_TRANSLATE_NOOP( "ENT", "Pion ABC Freiner" ), eMission_Pion_ABC_Freiner ),
    T_ConverterUnitMission( "Pion ABC EscorterUnite", QT_TRANSLATE_NOOP( "ENT", "Pion ABC EscorterUnite" ), eMission_Pion_ABC_EscorterUnite ),
    T_ConverterUnitMission( "Pion ABC EscorterUnites", QT_TRANSLATE_NOOP( "ENT", "Pion ABC EscorterUnites" ), eMission_Pion_ABC_EscorterUnites ),
    T_ConverterUnitMission( "Pion ABC ArmerPointDeControle", QT_TRANSLATE_NOOP( "ENT", "Pion ABC ArmerPointDeControle" ), eMission_Pion_ABC_ArmerPointDeControle ),
    T_ConverterUnitMission( "Pion ABC OrganiserAccueilColonneRefugies", QT_TRANSLATE_NOOP( "ENT", "Pion ABC OrganiserAccueilColonneRefugies" ), eMission_Pion_ABC_OrganiserAccueilColonneRefugies ),
    T_ConverterUnitMission( "Pion ABC AppuyerSurPosition", QT_TRANSLATE_NOOP( "ENT", "Pion ABC AppuyerSurPosition" ), eMission_Pion_ABC_AppuyerSurPosition ),
    T_ConverterUnitMission( "Pion GEN RealiserFosseAC", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserFosseAC" ), eMission_Pion_GEN_RealiserFosseAC ),
    T_ConverterUnitMission( "Pion GEN RealiserAbattisMine", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserAbattisMine" ), eMission_Pion_GEN_RealiserAbattisMine ),
    T_ConverterUnitMission( "Pion GEN RealiserPointMine", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserPointMine" ), eMission_Pion_GEN_RealiserPointMine ),
    T_ConverterUnitMission( "Pion GEN RealiserBarricade", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserBarricade" ), eMission_Pion_GEN_RealiserBarricade ),
    T_ConverterUnitMission( "Pion GEN RealiserEboulement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserEboulement" ), eMission_Pion_GEN_RealiserEboulement ),
    T_ConverterUnitMission( "Pion GEN RealiserBouchonMine", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserBouchonMine" ), eMission_Pion_GEN_RealiserBouchonMine ),
    T_ConverterUnitMission( "Pion GEN DetruireRoute", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DetruireRoute" ), eMission_Pion_GEN_DetruireRoute ),
    T_ConverterUnitMission( "Pion GEN DetruirePont", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DetruirePont" ), eMission_Pion_GEN_DetruirePont ),
    T_ConverterUnitMission( "Pion GEN SecuriserItineraire", QT_TRANSLATE_NOOP( "ENT", "Pion GEN SecuriserItineraire" ), eMission_Pion_GEN_SecuriserItineraire ),
    T_ConverterUnitMission( "Pion GEN DegagerAbattisMine", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DegagerAbattisMine" ), eMission_Pion_GEN_DegagerAbattisMine ),
    T_ConverterUnitMission( "Pion GEN DegagerEboulement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DegagerEboulement" ), eMission_Pion_GEN_DegagerEboulement ),
    T_ConverterUnitMission( "Pion GEN DegagerRues", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DegagerRues" ), eMission_Pion_GEN_DegagerRues ),
    T_ConverterUnitMission( "Pion GEN ComblerEntonnoir", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ComblerEntonnoir" ), eMission_Pion_GEN_ComblerEntonnoir ),
    T_ConverterUnitMission( "Pion GEN ComblerFosseAC", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ComblerFosseAC" ), eMission_Pion_GEN_ComblerFosseAC ),
    T_ConverterUnitMission( "Pion GEN CreerContournement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN CreerContournement" ), eMission_Pion_GEN_CreerContournement ),
    T_ConverterUnitMission( "Pion GEN RealiserZoneMineeLineaire", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserZoneMineeLineaire" ), eMission_Pion_GEN_RealiserZoneMineeLineaire ),
    T_ConverterUnitMission( "Pion GEN OuvrirTrouee", QT_TRANSLATE_NOOP( "ENT", "Pion GEN OuvrirTrouee" ), eMission_Pion_GEN_OuvrirTrouee ),
    T_ConverterUnitMission( "Pion GEN Renforcer", QT_TRANSLATE_NOOP( "ENT", "Pion GEN Renforcer" ), eMission_Pion_GEN_Renforcer ),
    T_ConverterUnitMission( "Pion GEN FranchirEntonnoir", QT_TRANSLATE_NOOP( "ENT", "Pion GEN FranchirEntonnoir" ), eMission_Pion_GEN_FranchirEntonnoir ),
    T_ConverterUnitMission( "Pion GEN FranchirFosseAC", QT_TRANSLATE_NOOP( "ENT", "Pion GEN FranchirFosseAC" ), eMission_Pion_GEN_FranchirFosseAC ),
    T_ConverterUnitMission( "Pion GEN RealiserZoneMineeParDispersion", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserZoneMineeParDispersion" ), eMission_Pion_GEN_RealiserZoneMineeParDispersion ),
    T_ConverterUnitMission( "Pion GEN RealiserPostesTir", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserPostesTir" ), eMission_Pion_GEN_RealiserPostesTir ),
    T_ConverterUnitMission( "Pion GEN RealiserTravauxProtection", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserTravauxProtection" ), eMission_Pion_GEN_RealiserTravauxProtection ),
    T_ConverterUnitMission( "Pion GEN RetablirItineraire", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RetablirItineraire" ), eMission_Pion_GEN_RetablirItineraire ),
    T_ConverterUnitMission( "Pion GEN EquiperExploiter", QT_TRANSLATE_NOOP( "ENT", "Pion GEN EquiperExploiter" ), eMission_Pion_GEN_EquiperExploiter ),
    T_ConverterUnitMission( "Pion GEN ExecuterVariantement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ExecuterVariantement" ), eMission_Pion_GEN_ExecuterVariantement ),
    T_ConverterUnitMission( "Pion GEN ReconnaitreSiteFranchissement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ReconnaitreSiteFranchissement" ), eMission_Pion_GEN_ReconnaitreSiteFranchissement ),
    T_ConverterUnitMission( "Pion GEN ReconnaitreItineraire", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ReconnaitreItineraire" ), eMission_Pion_GEN_ReconnaitreItineraire ),
    T_ConverterUnitMission( "Pion GEN DeminerSiteFranchissement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DeminerSiteFranchissement" ), eMission_Pion_GEN_DeminerSiteFranchissement ),
    T_ConverterUnitMission( "Pion GEN ReconnaitreZone", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ReconnaitreZone" ), eMission_Pion_GEN_ReconnaitreZone ),
    T_ConverterUnitMission( "Pion GEN CreerPlateForme", QT_TRANSLATE_NOOP( "ENT", "Pion GEN CreerPlateForme" ), eMission_Pion_GEN_CreerPlateForme ),
    T_ConverterUnitMission( "Pion GEN CreerPiste", QT_TRANSLATE_NOOP( "ENT", "Pion GEN CreerPiste" ), eMission_Pion_GEN_CreerPiste ),
    T_ConverterUnitMission( "Pion GEN CreerAirePoser", QT_TRANSLATE_NOOP( "ENT", "Pion GEN CreerAirePoser" ), eMission_Pion_GEN_CreerAirePoser ),
    T_ConverterUnitMission( "Pion GEN NeutraliserEnginsExplosifs", QT_TRANSLATE_NOOP( "ENT", "Pion GEN NeutraliserEnginsExplosifs" ), eMission_Pion_GEN_NeutraliserEnginsExplosifs ),
    T_ConverterUnitMission( "Pion GEN ActiverObstacle", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ActiverObstacle" ), eMission_Pion_GEN_ActiverObstacle ),
    T_ConverterUnitMission( "Pion GEN AmeliorerMobilite", QT_TRANSLATE_NOOP( "ENT", "Pion GEN AmeliorerMobilite" ), eMission_Pion_GEN_AmeliorerMobilite ),
    T_ConverterUnitMission( "Pion GEN RealiserAireLogistique", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserAireLogistique" ), eMission_Pion_GEN_RealiserAireLogistique ),
    T_ConverterUnitMission( "Pion GEN DepolluerZone", QT_TRANSLATE_NOOP( "ENT", "Pion GEN DepolluerZone" ), eMission_Pion_GEN_DepolluerZone ),
    T_ConverterUnitMission( "Pion GEN AmenagerTerrainPoser", QT_TRANSLATE_NOOP( "ENT", "Pion GEN AmenagerTerrainPoser" ), eMission_Pion_GEN_AmenagerTerrainPoser ),
    T_ConverterUnitMission( "Pion GEN RealiserPlateFormeDeploiement", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserPlateFormeDeploiement" ), eMission_Pion_GEN_RealiserPlateFormeDeploiement ),
    T_ConverterUnitMission( "Pion GEN ReprendreMissionConstruction", QT_TRANSLATE_NOOP( "ENT", "Pion GEN ReprendreMissionConstruction" ), eMission_Pion_GEN_ReprendreMissionConstruction ),
    T_ConverterUnitMission( "Pion GEN RealiserZonePoserHelicoptere", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserZonePoserHelicoptere" ), eMission_Pion_GEN_RealiserZonePoserHelicoptere ),
    T_ConverterUnitMission( "Pion GEN AppuyerDebarquementTroupesGU", QT_TRANSLATE_NOOP( "ENT", "Pion GEN AppuyerDebarquementTroupesGU" ), eMission_Pion_GEN_AppuyerDebarquementTroupesGU ),
    T_ConverterUnitMission( "Pion GEN RealiserCampPrisonniers", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserCampPrisonniers" ), eMission_Pion_GEN_RealiserCampPrisonniers ),
    T_ConverterUnitMission( "Pion GEN RealiserCampRefugies", QT_TRANSLATE_NOOP( "ENT", "Pion GEN RealiserCampRefugies" ), eMission_Pion_GEN_RealiserCampRefugies ),
    T_ConverterUnitMission( "Pion INF ReconnaitreAxe", QT_TRANSLATE_NOOP( "ENT", "Pion INF ReconnaitreAxe" ), eMission_Pion_INF_ReconnaitreAxe ),
    T_ConverterUnitMission( "Pion INF Couvrir", QT_TRANSLATE_NOOP( "ENT", "Pion INF Couvrir" ), eMission_Pion_INF_Couvrir ),
    T_ConverterUnitMission( "Pion INF Reduire", QT_TRANSLATE_NOOP( "ENT", "Pion INF Reduire" ), eMission_Pion_INF_Reduire ),
    T_ConverterUnitMission( "Pion INF Appuyer", QT_TRANSLATE_NOOP( "ENT", "Pion INF Appuyer" ), eMission_Pion_INF_Appuyer ),
    T_ConverterUnitMission( "Pion INF Fixer", QT_TRANSLATE_NOOP( "ENT", "Pion INF Fixer" ), eMission_Pion_INF_Fixer ),
    T_ConverterUnitMission( "Pion INF ControlerSecteur", QT_TRANSLATE_NOOP( "ENT", "Pion INF ControlerSecteur" ), eMission_Pion_INF_ControlerSecteur ),
    T_ConverterUnitMission( "Pion INF SurveillerSecteur", QT_TRANSLATE_NOOP( "ENT", "Pion INF SurveillerSecteur" ), eMission_Pion_INF_SurveillerSecteur ),
    T_ConverterUnitMission( "Pion INF Barrer", QT_TRANSLATE_NOOP( "ENT", "Pion INF Barrer" ), eMission_Pion_INF_Barrer ),
    T_ConverterUnitMission( "Pion INF ReconnaitrePoint", QT_TRANSLATE_NOOP( "ENT", "Pion INF ReconnaitrePoint" ), eMission_Pion_INF_ReconnaitrePoint ),
    T_ConverterUnitMission( "Pion INF Eclairer", QT_TRANSLATE_NOOP( "ENT", "Pion INF Eclairer" ), eMission_Pion_INF_Eclairer ),
    T_ConverterUnitMission( "Pion INF Interdire", QT_TRANSLATE_NOOP( "ENT", "Pion INF Interdire" ), eMission_Pion_INF_Interdire ),
    T_ConverterUnitMission( "Pion INF Semparer", QT_TRANSLATE_NOOP( "ENT", "Pion INF Semparer" ), eMission_Pion_INF_Semparer ),
    T_ConverterUnitMission( "Pion INF PrendrePreciserContact", QT_TRANSLATE_NOOP( "ENT", "Pion INF PrendrePreciserContact" ), eMission_Pion_INF_PrendrePreciserContact ),
    T_ConverterUnitMission( "Pion INF Defendre", QT_TRANSLATE_NOOP( "ENT", "Pion INF Defendre" ), eMission_Pion_INF_Defendre ),
    T_ConverterUnitMission( "Pion INF DetruireEmbuscade", QT_TRANSLATE_NOOP( "ENT", "Pion INF DetruireEmbuscade" ), eMission_Pion_INF_DetruireEmbuscade ),
    T_ConverterUnitMission( "Pion INF ContreAttaquer", QT_TRANSLATE_NOOP( "ENT", "Pion INF ContreAttaquer" ), eMission_Pion_INF_ContreAttaquer ),
    T_ConverterUnitMission( "Pion INF Freiner", QT_TRANSLATE_NOOP( "ENT", "Pion INF Freiner" ), eMission_Pion_INF_Freiner ),
    T_ConverterUnitMission( "Pion INF FlancGarder", QT_TRANSLATE_NOOP( "ENT", "Pion INF FlancGarder" ), eMission_Pion_INF_FlancGarder ),
    T_ConverterUnitMission( "Pion INF OrganiserUnCheckPoint", QT_TRANSLATE_NOOP( "ENT", "Pion INF OrganiserUnCheckPoint" ), eMission_Pion_INF_OrganiserUnCheckPoint ),
    T_ConverterUnitMission( "Pion INF Harceler", QT_TRANSLATE_NOOP( "ENT", "Pion INF Harceler" ), eMission_Pion_INF_Harceler ),
    T_ConverterUnitMission( "Pion INF DetruireParCoupDeMain", QT_TRANSLATE_NOOP( "ENT", "Pion INF DetruireParCoupDeMain" ), eMission_Pion_INF_DetruireParCoupDeMain ),
    T_ConverterUnitMission( "Pion INF EscorterUnite", QT_TRANSLATE_NOOP( "ENT", "Pion INF EscorterUnite" ), eMission_Pion_INF_EscorterUnite ),
    T_ConverterUnitMission( "Pion INF EscorterUnites", QT_TRANSLATE_NOOP( "ENT", "Pion INF EscorterUnites" ), eMission_Pion_INF_EscorterUnites ),
    T_ConverterUnitMission( "Pion INF RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion INF RenseignerSur" ), eMission_Pion_INF_RenseignerSur ),
    T_ConverterUnitMission( "Pion INF Soutenir", QT_TRANSLATE_NOOP( "ENT", "Pion INF Soutenir" ), eMission_Pion_INF_Soutenir ),
    T_ConverterUnitMission( "Pion INF Securiser", QT_TRANSLATE_NOOP( "ENT", "Pion INF Securiser" ), eMission_Pion_INF_Securiser ),
    T_ConverterUnitMission( "Pion INF Recueillir", QT_TRANSLATE_NOOP( "ENT", "Pion INF Recueillir" ), eMission_Pion_INF_Recueillir ),
    T_ConverterUnitMission( "Pion INF RepondreAAggression", QT_TRANSLATE_NOOP( "ENT", "Pion INF RepondreAAggression" ), eMission_Pion_INF_RepondreAAggression ),
    T_ConverterUnitMission( "Pion INF AppuyerSurPosition", QT_TRANSLATE_NOOP( "ENT", "Pion INF AppuyerSurPosition" ), eMission_Pion_INF_AppuyerSurPosition ),
    T_ConverterUnitMission( "Pion LOG SeDeployer", QT_TRANSLATE_NOOP( "ENT", "Pion LOG SeDeployer" ), eMission_Pion_LOG_SeDeployer ),
    T_ConverterUnitMission( "Pion LOG DistribuerMoyens", QT_TRANSLATE_NOOP( "ENT", "Pion LOG DistribuerMoyens" ), eMission_Pion_LOG_DistribuerMoyens ),
    T_ConverterUnitMission( "Pion LOG TrierBlesses", QT_TRANSLATE_NOOP( "ENT", "Pion LOG TrierBlesses" ), eMission_Pion_LOG_TrierBlesses ),
    T_ConverterUnitMission( "Pion LOG TraiterBlesses", QT_TRANSLATE_NOOP( "ENT", "Pion LOG TraiterBlesses" ), eMission_Pion_LOG_TraiterBlesses ),
    T_ConverterUnitMission( "Pion LOG TrierEtTraiterBlesses", QT_TRANSLATE_NOOP( "ENT", "Pion LOG TrierEtTraiterBlesses" ), eMission_Pion_LOG_TrierEtTraiterBlesses ),
    T_ConverterUnitMission( "Pion LOG SoignerBlesses", QT_TRANSLATE_NOOP( "ENT", "Pion LOG SoignerBlesses" ), eMission_Pion_LOG_SoignerBlesses ),
    T_ConverterUnitMission( "Pion LOG PrendreEnCompteReactionsMentales", QT_TRANSLATE_NOOP( "ENT", "Pion LOG PrendreEnCompteReactionsMentales" ), eMission_Pion_LOG_PrendreEnCompteReactionsMentales ),
    T_ConverterUnitMission( "Pion LOG DecontaminerBlesses", QT_TRANSLATE_NOOP( "ENT", "Pion LOG DecontaminerBlesses" ), eMission_Pion_LOG_DecontaminerBlesses ),
    T_ConverterUnitMission( "Pion LOG Reparer", QT_TRANSLATE_NOOP( "ENT", "Pion LOG Reparer" ), eMission_Pion_LOG_Reparer ),
    T_ConverterUnitMission( "Pion LOG Evacuer", QT_TRANSLATE_NOOP( "ENT", "Pion LOG Evacuer" ), eMission_Pion_LOG_Evacuer ),
    T_ConverterUnitMission( "Pion LOG Convoyer", QT_TRANSLATE_NOOP( "ENT", "Pion LOG Convoyer" ), eMission_Pion_LOG_Convoyer ),
    T_ConverterUnitMission( "Pion LOG DistribuerRessources", QT_TRANSLATE_NOOP( "ENT", "Pion LOG DistribuerRessources" ), eMission_Pion_LOG_DistribuerRessources ),
    T_ConverterUnitMission( "Pion LOG ConstituerConvoi", QT_TRANSLATE_NOOP( "ENT", "Pion LOG ConstituerConvoi" ), eMission_Pion_LOG_ConstituerConvoi ),
    T_ConverterUnitMission( "Pion LOG Livrer", QT_TRANSLATE_NOOP( "ENT", "Pion LOG Livrer" ), eMission_Pion_LOG_Livrer ),
    T_ConverterUnitMission( "Pion LOG PreparerBascule", QT_TRANSLATE_NOOP( "ENT", "Pion LOG PreparerBascule" ), eMission_Pion_LOG_PreparerBascule ),
    T_ConverterUnitMission( "Pion LOG AppuyerMouvement", QT_TRANSLATE_NOOP( "ENT", "Pion LOG AppuyerMouvement" ), eMission_Pion_LOG_AppuyerMouvement ),
    T_ConverterUnitMission( "Pion LOG ReconnaitreItineraire", QT_TRANSLATE_NOOP( "ENT", "Pion LOG ReconnaitreItineraire" ), eMission_Pion_LOG_ReconnaitreItineraire ),
    T_ConverterUnitMission( "Pion LOG TransporterUnite", QT_TRANSLATE_NOOP( "ENT", "Pion LOG TransporterUnite" ), eMission_Pion_LOG_TransporterUnite ),
    T_ConverterUnitMission( "Pion LOG ReconnaitreZoneContaminee", QT_TRANSLATE_NOOP( "ENT", "Pion LOG ReconnaitreZoneContaminee" ), eMission_Pion_LOG_ReconnaitreZoneContaminee ),
    T_ConverterUnitMission( "Pion LOG AppuyerMouvementDansZone", QT_TRANSLATE_NOOP( "ENT", "Pion LOG AppuyerMouvementDansZone" ), eMission_Pion_LOG_AppuyerMouvementDansZone ),
    T_ConverterUnitMission( "Pion LOG MettreEnOeuvreZoneStationnement", QT_TRANSLATE_NOOP( "ENT", "Pion LOG MettreEnOeuvreZoneStationnement" ), eMission_Pion_LOG_MettreEnOeuvreZoneStationnement ),
    T_ConverterUnitMission( "Pion LOG AppuyerFranchissementSurUnPoint", QT_TRANSLATE_NOOP( "ENT", "Pion LOG AppuyerFranchissementSurUnPoint" ), eMission_Pion_LOG_AppuyerFranchissementSurUnPoint ),
    T_ConverterUnitMission( "Pion LOG BloquerZone", QT_TRANSLATE_NOOP( "ENT", "Pion LOG BloquerZone" ), eMission_Pion_LOG_BloquerZone ),
    T_ConverterUnitMission( "Pion LOG ReconnaitreZoneDeDeploiement", QT_TRANSLATE_NOOP( "ENT", "Pion LOG ReconnaitreZoneDeDeploiement" ), eMission_Pion_LOG_ReconnaitreZoneDeDeploiement ),
    T_ConverterUnitMission( "Pion LOG ArmerPIA", QT_TRANSLATE_NOOP( "ENT", "Pion LOG ArmerPIA" ), eMission_Pion_LOG_ArmerPIA ),
    T_ConverterUnitMission( "Pion LOG Surveiller", QT_TRANSLATE_NOOP( "ENT", "Pion LOG Surveiller" ), eMission_Pion_LOG_Surveiller ),
    T_ConverterUnitMission( "Pion Test Heliporter", QT_TRANSLATE_NOOP( "ENT", "Pion Test Heliporter" ), eMission_Pion_Test_Heliporter ),
    T_ConverterUnitMission( "Pion Test MoveTo", QT_TRANSLATE_NOOP( "ENT", "Pion Test MoveTo" ), eMission_Pion_Test_MoveTo ),
    T_ConverterUnitMission( "Pion Test CreateObject", QT_TRANSLATE_NOOP( "ENT", "Pion Test CreateObject" ), eMission_Pion_Test_CreateObject ),
    T_ConverterUnitMission( "Pion Test DestroyObject", QT_TRANSLATE_NOOP( "ENT", "Pion Test DestroyObject" ), eMission_Pion_Test_DestroyObject ),
    T_ConverterUnitMission( "Pion Test CreateBypass", QT_TRANSLATE_NOOP( "ENT", "Pion Test CreateBypass" ), eMission_Pion_Test_CreateBypass ),
    T_ConverterUnitMission( "Pion Test Fire", QT_TRANSLATE_NOOP( "ENT", "Pion Test Fire" ), eMission_Pion_Test_Fire ),
    T_ConverterUnitMission( "Pion Test ChangePosture", QT_TRANSLATE_NOOP( "ENT", "Pion Test ChangePosture" ), eMission_Pion_Test_ChangePosture ),
    T_ConverterUnitMission( "Pion Test PathFind", QT_TRANSLATE_NOOP( "ENT", "Pion Test PathFind" ), eMission_Pion_Test_PathFind ),
    T_ConverterUnitMission( "Pion Test FindPosition", QT_TRANSLATE_NOOP( "ENT", "Pion Test FindPosition" ), eMission_Pion_Test_FindPosition ),
    T_ConverterUnitMission( "Pion Test Reinforce", QT_TRANSLATE_NOOP( "ENT", "Pion Test Reinforce" ), eMission_Pion_Test_Reinforce ),
    T_ConverterUnitMission( "Pion PlastronEnnemi", QT_TRANSLATE_NOOP( "ENT", "Pion PlastronEnnemi" ), eMission_Pion_PlastronEnnemi ),
    T_ConverterUnitMission( "Pion FaireMouvement", QT_TRANSLATE_NOOP( "ENT", "Pion FaireMouvement" ), eMission_Pion_FaireMouvement ),
    T_ConverterUnitMission( "Pion Suivre", QT_TRANSLATE_NOOP( "ENT", "Pion Suivre" ), eMission_Pion_Suivre ),
    T_ConverterUnitMission( "Pion SeFaireDecontaminer", QT_TRANSLATE_NOOP( "ENT", "Pion SeFaireDecontaminer" ), eMission_Pion_SeFaireDecontaminer ),
    T_ConverterUnitMission( "Pion Franchir", QT_TRANSLATE_NOOP( "ENT", "Pion Franchir" ), eMission_Pion_Franchir ),
    T_ConverterUnitMission( "Pion Decrocher", QT_TRANSLATE_NOOP( "ENT", "Pion Decrocher" ), eMission_Pion_Decrocher ),
    T_ConverterUnitMission( "Pion Stationner", QT_TRANSLATE_NOOP( "ENT", "Pion Stationner" ), eMission_Pion_Stationner ),
    T_ConverterUnitMission( "Pion SeFaireTransporter", QT_TRANSLATE_NOOP( "ENT", "Pion SeFaireTransporter" ), eMission_Pion_SeFaireTransporter ),
    T_ConverterUnitMission( "Pion InterdireFranchissementPopulation", QT_TRANSLATE_NOOP( "ENT", "Pion InterdireFranchissementPopulation" ), eMission_Pion_InterdireFranchissementPopulation ),
    T_ConverterUnitMission( "Pion FilterPopulation", QT_TRANSLATE_NOOP( "ENT", "Pion FilterPopulation" ), eMission_Pion_FilterPopulation ),
    T_ConverterUnitMission( "Pion RenseignerSurPopulation", QT_TRANSLATE_NOOP( "ENT", "Pion RenseignerSurPopulation" ), eMission_Pion_RenseignerSurPopulation ),
    T_ConverterUnitMission( "Pion CanaliserPopulation", QT_TRANSLATE_NOOP( "ENT", "Pion CanaliserPopulation" ), eMission_Pion_CanaliserPopulation ),
    T_ConverterUnitMission( "Pion SecuriserPopulationDansZone", QT_TRANSLATE_NOOP( "ENT", "Pion SecuriserPopulationDansZone" ), eMission_Pion_SecuriserPopulationDansZone ),
    T_ConverterUnitMission( "Pion SecuriserZoneContreMenacesAsymetriques", QT_TRANSLATE_NOOP( "ENT", "Pion SecuriserZoneContreMenacesAsymetriques" ), eMission_Pion_SecuriserZoneContreMenacesAsymetriques ),
    T_ConverterUnitMission( "Pion Generique", QT_TRANSLATE_NOOP( "ENT", "Pion Generique" ), eMission_Pion_Generique ),
    T_ConverterUnitMission( "Pion MILICE Provoquer", QT_TRANSLATE_NOOP( "ENT", "Pion MILICE Provoquer" ), eMission_Pion_MILICE_Provoquer ),
    T_ConverterUnitMission( "Pion MILICE PrendreEtTenir", QT_TRANSLATE_NOOP( "ENT", "Pion MILICE PrendreEtTenir" ), eMission_Pion_MILICE_PrendreEtTenir ),
    T_ConverterUnitMission( "Pion MILICE DetruireEmbuscade", QT_TRANSLATE_NOOP( "ENT", "Pion MILICE DetruireEmbuscade" ), eMission_Pion_MILICE_DetruireEmbuscade ),
    T_ConverterUnitMission( "Pion JOINT MARINE Transporter", QT_TRANSLATE_NOOP( "ENT", "Pion JOINT MARINE Transporter" ), eMission_Pion_JOINT_MARINE_Transporter ),
    T_ConverterUnitMission( "Pion JOINT AIR EffectuerRaid", QT_TRANSLATE_NOOP( "ENT", "Pion JOINT AIR EffectuerRaid" ), eMission_Pion_JOINT_AIR_EffectuerRaid ),
    T_ConverterUnitMission( "Pion ASY SInfiltrer", QT_TRANSLATE_NOOP( "ENT", "Pion ASY SInfiltrer" ), eMission_Pion_ASY_SInfiltrer ),
    T_ConverterUnitMission( "Pion ASY CommettreAttentatContrePopulation", QT_TRANSLATE_NOOP( "ENT", "Pion ASY CommettreAttentatContrePopulation" ), eMission_Pion_ASY_CommettreAttentatContrePopulation ),
    T_ConverterUnitMission( "Pion ASY CommettreAttentatContreForcesArmees", QT_TRANSLATE_NOOP( "ENT", "Pion ASY CommettreAttentatContreForcesArmees" ), eMission_Pion_ASY_CommettreAttentatContreForcesArmees ),
    T_ConverterUnitMission( "Pion ASY CommettreAttentatContreInstallation", QT_TRANSLATE_NOOP( "ENT", "Pion ASY CommettreAttentatContreInstallation" ), eMission_Pion_ASY_CommettreAttentatContreInstallation ),
    T_ConverterUnitMission( "Pion ASY CommettreExactionsSurPopulationDansZone", QT_TRANSLATE_NOOP( "ENT", "Pion ASY CommettreExactionsSurPopulationDansZone" ), eMission_Pion_ASY_CommettreExactionsSurPopulationDansZone ),
    T_ConverterUnitMission( "Pion ASY Renseigner", QT_TRANSLATE_NOOP( "ENT", "Pion ASY Renseigner" ), eMission_Pion_ASY_Renseigner ),
    T_ConverterUnitMission( "Pion NBC AnimerUnPlotDeDecontamination", QT_TRANSLATE_NOOP( "ENT", "Pion NBC AnimerUnPlotDeDecontamination" ), eMission_Pion_NBC_AnimerUnPlotDeDecontamination ),
    T_ConverterUnitMission( "Pion NBC ReconnaitreItineraire", QT_TRANSLATE_NOOP( "ENT", "Pion NBC ReconnaitreItineraire" ), eMission_Pion_NBC_ReconnaitreItineraire ),
    T_ConverterUnitMission( "Pion NBC ReconnaitreZone", QT_TRANSLATE_NOOP( "ENT", "Pion NBC ReconnaitreZone" ), eMission_Pion_NBC_ReconnaitreZone ),
    T_ConverterUnitMission( "Pion NBC DecontaminerZone", QT_TRANSLATE_NOOP( "ENT", "Pion NBC DecontaminerZone" ), eMission_Pion_NBC_DecontaminerZone ),
    T_ConverterUnitMission( "Pion NBC ReconnaitreUnSiteROTA", QT_TRANSLATE_NOOP( "ENT", "Pion NBC ReconnaitreUnSiteROTA" ), eMission_Pion_NBC_ReconnaitreUnSiteROTA ),
    T_ConverterUnitMission( "Pion RENS ROHUM RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROHUM RenseignerSur" ), eMission_Pion_RENS_ROHUM_RenseignerSur ),
    T_ConverterUnitMission( "Pion RENS ROHUM SExfiltrer", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROHUM SExfiltrer" ), eMission_Pion_RENS_ROHUM_SExfiltrer ),
    T_ConverterUnitMission( "Pion RENS ROHUM OrienterGuider", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROHUM OrienterGuider" ), eMission_Pion_RENS_ROHUM_OrienterGuider ),
    T_ConverterUnitMission( "Pion RENS ROEM Appuyer", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROEM Appuyer" ), eMission_Pion_RENS_ROEM_Appuyer ),
    T_ConverterUnitMission( "Pion RENS ROEM RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROEM RenseignerSur" ), eMission_Pion_RENS_ROEM_RenseignerSur ),
    T_ConverterUnitMission( "Pion RENS ROEM MettreEnOeuvre", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROEM MettreEnOeuvre" ), eMission_Pion_RENS_ROEM_MettreEnOeuvre ),
    T_ConverterUnitMission( "Pion RENS RADINT RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion RENS RADINT RenseignerSur" ), eMission_Pion_RENS_RADINT_RenseignerSur ),
    T_ConverterUnitMission( "Pion RENS RADINT MettreEnOeuvre", QT_TRANSLATE_NOOP( "ENT", "Pion RENS RADINT MettreEnOeuvre" ), eMission_Pion_RENS_RADINT_MettreEnOeuvre ),
    T_ConverterUnitMission( "Pion RENS ROIM MettreEnOeuvre", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROIM MettreEnOeuvre" ), eMission_Pion_RENS_ROIM_MettreEnOeuvre ),
    T_ConverterUnitMission( "Pion RENS ROIM SDTI RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROIM SDTI RenseignerSur" ), eMission_Pion_RENS_ROIM_SDTI_RenseignerSur ),
    T_ConverterUnitMission( "Pion RENS ROIM CL289 RenseignerSur", QT_TRANSLATE_NOOP( "ENT", "Pion RENS ROIM CL289 RenseignerSur" ), eMission_Pion_RENS_ROIM_CL289_RenseignerSur ),
    T_ConverterUnitMission( "Pion TRANS EtablirGererLiaison", QT_TRANSLATE_NOOP( "ENT", "Pion TRANS EtablirGererLiaison" ), eMission_Pion_TRANS_EtablirGererLiaison ),

    T_ConverterUnitMission( "", "", (E_UnitMission)-1  )
};

ENT_Tr::T_ConverterPopulationMission ENT_Tr::populationMissionConverter_[] =
{
    T_ConverterPopulationMission( "Population FaireMouvement", QT_TRANSLATE_NOOP( "ENT", "Population FaireMouvement" ), eMission_Population_FaireMouvement ),
    T_ConverterPopulationMission( "Population Manifester", QT_TRANSLATE_NOOP( "ENT", "Population Manifester" ), eMission_Population_Manifester ),
    T_ConverterPopulationMission( "Population CommettreExactions", QT_TRANSLATE_NOOP( "ENT", "Population CommettreExactions" ), eMission_Population_CommettreExactions ),

    T_ConverterPopulationMission( "", "", (E_PopulationMission)-1  )
};

ENT_Tr::T_ConverterFragOrder ENT_Tr::fragOrderConverter_[] =
{
    T_ConverterFragOrder( "MettreTenueNBC", QT_TRANSLATE_NOOP( "ENT", "Mettre Tenue NBC" ), eOrdreConduite_MettreTenueNBC ),
    T_ConverterFragOrder( "EnleverTenueNBC", QT_TRANSLATE_NOOP( "ENT", "Enlever Tenue NBC" ), eOrdreConduite_EnleverTenueNBC ),
    T_ConverterFragOrder( "PasserEnSilenceRadio", QT_TRANSLATE_NOOP( "ENT", "Passer En Silence Radio" ), eOrdreConduite_PasserEnSilenceRadio ),
    T_ConverterFragOrder( "ArreterSilenceRadio", QT_TRANSLATE_NOOP( "ENT", "Arreter Silence Radio" ), eOrdreConduite_ArreterSilenceRadio ),
    T_ConverterFragOrder( "PasserEnSilenceRadar", QT_TRANSLATE_NOOP( "ENT", "Passer En Silence Radar" ), eOrdreConduite_PasserEnSilenceRadar ),
    T_ConverterFragOrder( "ArreterSilenceRadar", QT_TRANSLATE_NOOP( "ENT", "Arreter Silence Radar" ), eOrdreConduite_ArreterSilenceRadar ),
    T_ConverterFragOrder( "RecupererTransporteurs", QT_TRANSLATE_NOOP( "ENT", "Recuperer Transporteurs" ), eOrdreConduite_RecupererTransporteurs ),
    T_ConverterFragOrder( "AttendreSePoster", QT_TRANSLATE_NOOP( "ENT", "Attendre Se Poster" ), eOrdreConduite_AttendreSePoster ),
    T_ConverterFragOrder( "Poursuivre", QT_TRANSLATE_NOOP( "ENT", "Poursuivre" ), eOrdreConduite_Poursuivre ),
    T_ConverterFragOrder( "Interrompre", QT_TRANSLATE_NOOP( "ENT", "Interrompre" ), eOrdreConduite_Interrompre ),
    T_ConverterFragOrder( "ChangerAmbiance", QT_TRANSLATE_NOOP( "ENT", "Changer Ambiance" ), eOrdreConduite_ChangerAmbiance ),
    T_ConverterFragOrder( "ChangerReglesEngagement", QT_TRANSLATE_NOOP( "ENT", "Changer Regles Engagement" ), eOrdreConduite_ChangerReglesEngagement ),
    T_ConverterFragOrder( "ChangerReglesEngagementPopulation", QT_TRANSLATE_NOOP( "ENT", "Changer Regles Engagement Population" ), eOrdreConduite_ChangerReglesEngagementPopulation ),
    T_ConverterFragOrder( "Deboucher", QT_TRANSLATE_NOOP( "ENT", "Deboucher" ), eOrdreConduite_Deboucher ),
    T_ConverterFragOrder( "Decrocher", QT_TRANSLATE_NOOP( "ENT", "Decrocher" ), eOrdreConduite_Decrocher ),
    T_ConverterFragOrder( "AcquerirObjectif", QT_TRANSLATE_NOOP( "ENT", "Acquerir Objectif" ), eOrdreConduite_AcquerirObjectif ),
    T_ConverterFragOrder( "Brouiller", QT_TRANSLATE_NOOP( "ENT", "Brouiller" ), eOrdreConduite_Brouiller ),
    T_ConverterFragOrder( "ChangerPositionDebarquement", QT_TRANSLATE_NOOP( "ENT", "Changer Position Debarquement" ), eOrdreConduite_ChangerPositionDebarquement ),
    T_ConverterFragOrder( "Pion ChangerDePosition", QT_TRANSLATE_NOOP( "ENT", "Pion: Changer De Position" ), eOrdreConduite_Pion_ChangerDePosition ),
    T_ConverterFragOrder( "Pion Contourner", QT_TRANSLATE_NOOP( "ENT", "Pion: Contourner" ), eOrdreConduite_Pion_Contourner ),
    T_ConverterFragOrder( "Pion EnvoyerVehicule", QT_TRANSLATE_NOOP( "ENT", "Pion: Envoyer Vehicule" ), eOrdreConduite_Pion_EnvoyerVehicule ),
    T_ConverterFragOrder( "Pion ActiverObstacle", QT_TRANSLATE_NOOP( "ENT", "Pion: Activer Obstacle" ), eOrdreConduite_Pion_ActiverObstacle ),
    T_ConverterFragOrder( "Pion AppliquerFeux", QT_TRANSLATE_NOOP( "ENT", "Pion: Appliquer Feux" ), eOrdreConduite_Pion_AppliquerFeux ),
    T_ConverterFragOrder( "Pion ExecuterTir", QT_TRANSLATE_NOOP( "ENT", "Pion: Executer Tir" ), eOrdreConduite_Pion_ExecuterTir ),
    T_ConverterFragOrder( "Pion Emettre", QT_TRANSLATE_NOOP( "ENT", "Pion: Emettre" ), eOrdreConduite_Pion_Emettre ),
    T_ConverterFragOrder( "Pion StopperEmission", QT_TRANSLATE_NOOP( "ENT", "Pion: Stopper Emission" ), eOrdreConduite_Pion_StopperEmission ),
    T_ConverterFragOrder( "Pion RenforcerEnVSRAM", QT_TRANSLATE_NOOP( "ENT", "Pion: Renforcer En VSRAM" ), eOrdreConduite_Pion_RenforcerEnVSRAM ),
    T_ConverterFragOrder( "Pion TransfererVSRAM", QT_TRANSLATE_NOOP( "ENT", "Pion: Transferer VSRAM" ), eOrdreConduite_Pion_TransfererVSRAM ),
    T_ConverterFragOrder( "Pion ReprendreAuxOrdresVSRAM", QT_TRANSLATE_NOOP( "ENT", "Pion: Reprendre Aux Ordres VSRAM" ), eOrdreConduite_Pion_ReprendreAuxOrdresVSRAM ),
    T_ConverterFragOrder( "ModifierTempsBordeeMaintenance", QT_TRANSLATE_NOOP( "ENT", "Modifier Temps Bordee Maintenance" ), eOrdreConduite_ModifierTempsBordeeMaintenance ),
    T_ConverterFragOrder( "ModifierTempsBordeeSante", QT_TRANSLATE_NOOP( "ENT", "Modifier Temps Bordee Sante" ), eOrdreConduite_ModifierTempsBordeeSante ),
    T_ConverterFragOrder( "ModifierPrioritesReparations", QT_TRANSLATE_NOOP( "ENT", "Modifier Priorites Reparations" ), eOrdreConduite_ModifierPrioritesReparations ),
    T_ConverterFragOrder( "ModifierPrioritesBlesses", QT_TRANSLATE_NOOP( "ENT", "Modifier Priorites Blesses" ), eOrdreConduite_ModifierPrioritesBlesses ),
    T_ConverterFragOrder( "ModifierPrioritesTactiquesReparations", QT_TRANSLATE_NOOP( "ENT", "Modifier Priorites Tactiques Reparations" ), eOrdreConduite_ModifierPrioritesTactiquesReparations ),
    T_ConverterFragOrder( "ModifierPrioritesTactiquesBlesses", QT_TRANSLATE_NOOP( "ENT", "Modifier Priorites Tactiques Blesses" ), eOrdreConduite_ModifierPrioritesTactiquesBlesses ),
    T_ConverterFragOrder( "Automate RealiserVariantement", QT_TRANSLATE_NOOP( "ENT", "Automate: Realiser Variantement" ), eOrdreConduite_Automate_RealiserVariantement ),
    T_ConverterFragOrder( "Automate DesequiperSiteFranchissement", QT_TRANSLATE_NOOP( "ENT", "Automate: Desequiper Site Franchissement" ), eOrdreConduite_Automate_DesequiperSiteFranchissement ),
    T_ConverterFragOrder( "Automate ReagirFaceAEni", QT_TRANSLATE_NOOP( "ENT", "Automate: Reagir Face AEni" ), eOrdreConduite_Automate_ReagirFaceAEni ),
    T_ConverterFragOrder( "Automate AffecterPionAObstacle", QT_TRANSLATE_NOOP( "ENT", "Automate: Affecter Pion AObstacle" ), eOrdreConduite_Automate_AffecterPionAObstacle ),
    T_ConverterFragOrder( "Automate ReconnaitreZoneImplantation", QT_TRANSLATE_NOOP( "ENT", "Automate: Reconnaitre Zone Implantation" ), eOrdreConduite_Automate_ReconnaitreZoneImplantation ),
    T_ConverterFragOrder( "Automate TC2 GererMaterielAvantDeplacement", QT_TRANSLATE_NOOP( "ENT", "Automate: TC2 Gerer Materiel Avant Deplacement" ), eOrdreConduite_Automate_TC2_GererMaterielAvantDeplacement ),
    T_ConverterFragOrder( "Automate ActiverObstacle", QT_TRANSLATE_NOOP( "ENT", "Automate: Activer Obstacle" ), eOrdreConduite_Automate_ActiverObstacle ),
    T_ConverterFragOrder( "Automate TransfererRenforts", QT_TRANSLATE_NOOP( "ENT", "Automate: Transferer Renforts" ), eOrdreConduite_Automate_TransfererRenforts ),
    T_ConverterFragOrder( "Population ChangerAttitude", QT_TRANSLATE_NOOP( "ENT", "Population: Changer Attitude" ), eOrdreConduite_Population_ChangerAttitude ),

    T_ConverterFragOrder( "", "", (E_FragOrder)-1  )
};


//-----------------------------------------------------------------------------
// Name: ENT_Tr::InitTranslations
// Created: AGR
//-----------------------------------------------------------------------------
void ENT_Tr::InitTranslations()
{
        InitTr( TypeLocalisationConverter_, "ENT_Tr" );
    InitTr( GDH_QualificatifConverter_, "ENT_Tr" );
    InitTr( UnitIdentificationLevelConverter_, "ENT_Tr" );
    InitTr( FamilleDotationConverter_, "ENT_Tr" );
    InitTr( FamilleMunitionConverter_, "ENT_Tr" );
    InitTr( UnitNatureWeaponConverter_, "ENT_Tr" );
    InitTr( UnitNatureSpecializationConverter_, "ENT_Tr" );
    InitTr( UnitNatureQualifierConverter_, "ENT_Tr" );
    InitTr( UnitNatureCategoryConverter_, "ENT_Tr" );
    InitTr( UnitNatureMobilityConverter_, "ENT_Tr" );
    InitTr( UnitCapaciteMissionConverter_, "ENT_Tr" );
    InitTr( NatureLevelConverter_, "ENT_Tr" );
    InitTr( HumanWoundConverter_, "ENT_Tr" );
    InitTr( HumanRankConverter_, "ENT_Tr" );
    InitTr( TempsBordeeConverter_, "ENT_Tr" );
    InitTr( MissionALAT_PorteeActionConverter_, "ENT_Tr" );
    InitTr( MissionALAT_AmbianceMissionConverter_, "ENT_Tr" );
    InitTr( ModeDeploiementConverter_, "ENT_Tr" );
    InitTr( OrderErrorCodeConverter_, "ENT_Tr" );
    InitTr( OrderStateConverter_, "ENT_Tr" );
    InitTr( ObjectTypeConverter_, "ENT_Tr" );
    InitTr( MissionGenUrgenceConverter_, "ENT_Tr" );
    InitTr( MissionGenSousTypeObstacleConverter_, "ENT_Tr" );
    InitTr( ObjectErrorCodeConverter_, "ENT_Tr" );
    InitTr( MissionGenPrioriteConverter_, "ENT_Tr" );
    InitTr( ModeDeploiementSectionMistralConverter_, "ENT_Tr" );
    InitTr( TypeFosseACConverter_, "ENT_Tr" );
    InitTr( GenTypePontageConverter_, "ENT_Tr" );
    InitTr( MissionInfNatureObjectifConverter_, "ENT_Tr" );
    InitTr( AutomateOrderFormationConverter_, "ENT_Tr" );
    InitTr( CtrlErrorCodeConverter_, "ENT_Tr" );
    InitTr( MsgCtrlClientAnnouncementConverter_, "ENT_Tr" );
    InitTr( EtatSimConverter_, "ENT_Tr" );
    InitTr( PrecipitationTypeConverter_, "ENT_Tr" );
    InitTr( TypeLimaConverter_, "ENT_Tr" );
    InitTr( InfoContextErrorCodeConverter_, "ENT_Tr" );
    InitTr( TypeCRConverter_, "ENT_Tr" );
    InitTr( FireEffectTypeConverter_, "ENT_Tr" );
    InitTr( DiplomatieConverter_, "ENT_Tr" );
    InitTr( ChangeDiplomatieErrorCodeConverter_, "ENT_Tr" );
    InitTr( ChangeGroupeConnaissanceErrorCodeConverter_, "ENT_Tr" );
    InitTr( ChangeLiensLogistiquesErrorCodeConverter_, "ENT_Tr" );
    InitTr( ChangeAutomateErrorCodeConverter_, "ENT_Tr" );
    InitTr( CtrlErrorCodeLauncherConverter_, "ENT_Tr" );
    InitTr( LogMaintenanceTraitementEtatConverter_, "ENT_Tr" );
    InitTr( LogRavitaillementTraitementEtatConverter_, "ENT_Tr" );
    InitTr( MsgLogRavitaillementChangeQuotasAckConverter_, "ENT_Tr" );
    InitTr( MsgLogRavitaillementPousserFluxAckConverter_, "ENT_Tr" );
    InitTr( LogSanteTraitementEtatConverter_, "ENT_Tr" );
    InitTr( AutomateStateConverter_, "ENT_Tr" );
    InitTr( UnitPostureConverter_, "ENT_Tr" );
    InitTr( EtatRapForConverter_, "ENT_Tr" );
    InitTr( EtatCombatRencontreConverter_, "ENT_Tr" );
    InitTr( EtatOperationnelConverter_, "ENT_Tr" );
    InitTr( DisponibiliteAuTirConverter_, "ENT_Tr" );
    InitTr( RoeConverter_, "ENT_Tr" );
    InitTr( RoePopulationConverter_, "ENT_Tr" );
    InitTr( UnitFatigueConverter_, "ENT_Tr" );
    InitTr( UnitMoralConverter_, "ENT_Tr" );
    InitTr( UnitExperienceConverter_, "ENT_Tr" );
    InitTr( SetAutomateModeErrorCodeConverter_, "ENT_Tr" );
    InitTr( UnitAttrErrorCodeConverter_, "ENT_Tr" );
    InitTr( PopulationAttitudeConverter_, "ENT_Tr" );
    InitTr( PopulationAttrErrorCodeConverter_, "ENT_Tr" );
    InitTr( AmbianceMissionConverter_, "ENT_Tr" );
    InitTr( TypeMunitionTirIndirectConverter_, "ENT_Tr" );
    InitTr( ActionReagirFaceAEniConverter_, "ENT_Tr" );
    InitTr( TC2GererMaterielAvantDeplacementConverter_, "ENT_Tr" );

    InitTr( automataMissionConverter_ );
    InitTr( unitMissionConverter_ );
    InitTr( populationMissionConverter_ );
    InitTr( fragOrderConverter_ );
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
// Name: ENT_Tr::ConvertFromUnitNatureWeapon
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitNatureWeapon( E_UnitNatureWeapon nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitNatureWeaponConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitNatureSpecialization
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitNatureSpecialization( E_UnitNatureSpecialization nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitNatureSpecializationConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitNatureQualifier
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitNatureQualifier( E_UnitNatureQualifier nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitNatureQualifierConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitNatureCategory
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitNatureCategory( E_UnitNatureCategory nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitNatureCategoryConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitNatureMobility
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitNatureMobility( E_UnitNatureMobility nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitNatureMobilityConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitCapaciteMission
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitCapaciteMission( E_UnitCapaciteMission nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( UnitCapaciteMissionConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromTempsBordee
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTempsBordee( E_TempsBordee nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TempsBordeeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMissionALAT_PorteeAction
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMissionALAT_PorteeAction( E_MissionALAT_PorteeAction nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionALAT_PorteeActionConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMissionALAT_AmbianceMission
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMissionALAT_AmbianceMission( E_MissionALAT_AmbianceMission nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionALAT_AmbianceMissionConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromModeDeploiement
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromModeDeploiement( E_ModeDeploiement nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ModeDeploiementConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromObjectType
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromObjectType( E_ObjectType nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ObjectTypeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMissionGenUrgence
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMissionGenUrgence( E_MissionGenUrgence nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionGenUrgenceConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromObjectErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromObjectErrorCode( E_ObjectErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ObjectErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMissionGenPriorite
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMissionGenPriorite( E_MissionGenPriorite nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionGenPrioriteConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromModeDeploiementSectionMistral
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromModeDeploiementSectionMistral( E_ModeDeploiementSectionMistral nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ModeDeploiementSectionMistralConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTypeFosseAC
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTypeFosseAC( E_TypeFosseAC nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeFosseACConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromGenTypePontage
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromGenTypePontage( E_GenTypePontage nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( GenTypePontageConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMissionInfNatureObjectif
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMissionInfNatureObjectif( E_MissionInfNatureObjectif nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MissionInfNatureObjectifConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromCtrlErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromCtrlErrorCode( E_CtrlErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( CtrlErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromMsgCtrlClientAnnouncement
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromMsgCtrlClientAnnouncement( E_MsgCtrlClientAnnouncement nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( MsgCtrlClientAnnouncementConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromChangeGroupeConnaissanceErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromChangeGroupeConnaissanceErrorCode( E_ChangeGroupeConnaissanceErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ChangeGroupeConnaissanceErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromChangeLiensLogistiquesErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromChangeLiensLogistiquesErrorCode( E_ChangeLiensLogistiquesErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ChangeLiensLogistiquesErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromChangeAutomateErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromChangeAutomateErrorCode( E_ChangeAutomateErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ChangeAutomateErrorCodeConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromCtrlErrorCodeLauncher
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromCtrlErrorCodeLauncher( E_CtrlErrorCodeLauncher nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( CtrlErrorCodeLauncherConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromAutomateState
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomateState( E_AutomateState nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AutomateStateConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromDisponibiliteAuTir
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromDisponibiliteAuTir( E_DisponibiliteAuTir nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( DisponibiliteAuTirConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromSetAutomateModeErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromSetAutomateModeErrorCode( E_SetAutomateModeErrorCode nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( SetAutomateModeErrorCodeConverter_, nValue, nConverterType );
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
// Name: ENT_Tr::ConvertFromAmbianceMission
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAmbianceMission( E_AmbianceMission nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( AmbianceMissionConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTypeMunitionTirIndirect( E_TypeMunitionTirIndirect nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TypeMunitionTirIndirectConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromActionReagirFaceAEni
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromActionReagirFaceAEni( E_ActionReagirFaceAEni nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( ActionReagirFaceAEniConverter_, nValue, nConverterType );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromTC2GererMaterielAvantDeplacement
// Created: AGR
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromTC2GererMaterielAvantDeplacement( E_TC2GererMaterielAvantDeplacement nValue, ENT_Tr_ABC::E_Conversion nConverterType )
{
    return ENT_Tr::InverseFindInConverter( TC2GererMaterielAvantDeplacementConverter_, nValue, nConverterType );
}




//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromAutomataMission
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromAutomataMission( E_AutomataMission val, ENT_Tr_ABC::E_Conversion e )
{
   return ENT_Tr::InverseFindInConverter( automataMissionConverter_, val ,e );
}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromUnitMission
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromUnitMission( E_UnitMission val, ENT_Tr_ABC::E_Conversion e )
{
    return ENT_Tr::InverseFindInConverter( unitMissionConverter_, val ,e );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromPopulationMission
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromPopulationMission( E_PopulationMission val, ENT_Tr_ABC::E_Conversion e )
{
    return ENT_Tr::InverseFindInConverter( populationMissionConverter_, val ,e );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertFromFragOrder
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
const std::string& ENT_Tr::ConvertFromFragOrder( E_FragOrder val, ENT_Tr_ABC::E_Conversion e )
{
    return ENT_Tr::InverseFindInConverter( fragOrderConverter_, val, e );
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
// Name: ENT_Tr::ConvertToUnitNatureWeapon
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitNatureWeapon ENT_Tr::ConvertToUnitNatureWeapon( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitNatureWeaponConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitNatureSpecialization
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitNatureSpecialization ENT_Tr::ConvertToUnitNatureSpecialization( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitNatureSpecializationConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitNatureQualifier
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitNatureQualifier ENT_Tr::ConvertToUnitNatureQualifier( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitNatureQualifierConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitNatureCategory
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitNatureCategory ENT_Tr::ConvertToUnitNatureCategory( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitNatureCategoryConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitNatureMobility
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitNatureMobility ENT_Tr::ConvertToUnitNatureMobility( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitNatureMobilityConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitCapaciteMission
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitCapaciteMission ENT_Tr::ConvertToUnitCapaciteMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( UnitCapaciteMissionConverter_, strName );
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
// Name: ENT_Tr::ConvertToTempsBordee
// Created: AGR
//-----------------------------------------------------------------------------
E_TempsBordee ENT_Tr::ConvertToTempsBordee( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TempsBordeeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMissionALAT_PorteeAction
// Created: AGR
//-----------------------------------------------------------------------------
E_MissionALAT_PorteeAction ENT_Tr::ConvertToMissionALAT_PorteeAction( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionALAT_PorteeActionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMissionALAT_AmbianceMission
// Created: AGR
//-----------------------------------------------------------------------------
E_MissionALAT_AmbianceMission ENT_Tr::ConvertToMissionALAT_AmbianceMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionALAT_AmbianceMissionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToModeDeploiement
// Created: AGR
//-----------------------------------------------------------------------------
E_ModeDeploiement ENT_Tr::ConvertToModeDeploiement( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ModeDeploiementConverter_, strName );
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
// Name: ENT_Tr::ConvertToObjectType
// Created: AGR
//-----------------------------------------------------------------------------
E_ObjectType ENT_Tr::ConvertToObjectType( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ObjectTypeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMissionGenUrgence
// Created: AGR
//-----------------------------------------------------------------------------
E_MissionGenUrgence ENT_Tr::ConvertToMissionGenUrgence( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionGenUrgenceConverter_, strName );
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
// Name: ENT_Tr::ConvertToObjectErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ObjectErrorCode ENT_Tr::ConvertToObjectErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ObjectErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMissionGenPriorite
// Created: AGR
//-----------------------------------------------------------------------------
E_MissionGenPriorite ENT_Tr::ConvertToMissionGenPriorite( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionGenPrioriteConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToModeDeploiementSectionMistral
// Created: AGR
//-----------------------------------------------------------------------------
E_ModeDeploiementSectionMistral ENT_Tr::ConvertToModeDeploiementSectionMistral( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ModeDeploiementSectionMistralConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTypeFosseAC
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeFosseAC ENT_Tr::ConvertToTypeFosseAC( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeFosseACConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToGenTypePontage
// Created: AGR
//-----------------------------------------------------------------------------
E_GenTypePontage ENT_Tr::ConvertToGenTypePontage( const std::string& strName )
{
    return ENT_Tr::FindInConverter( GenTypePontageConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToMissionInfNatureObjectif
// Created: AGR
//-----------------------------------------------------------------------------
E_MissionInfNatureObjectif ENT_Tr::ConvertToMissionInfNatureObjectif( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MissionInfNatureObjectifConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAutomateOrderFormation
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomateOrderFormation ENT_Tr::ConvertToAutomateOrderFormation( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomateOrderFormationConverter_, strName );
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
// Name: ENT_Tr::ConvertToMsgCtrlClientAnnouncement
// Created: AGR
//-----------------------------------------------------------------------------
E_MsgCtrlClientAnnouncement ENT_Tr::ConvertToMsgCtrlClientAnnouncement( const std::string& strName )
{
    return ENT_Tr::FindInConverter( MsgCtrlClientAnnouncementConverter_, strName );
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
// Name: ENT_Tr::ConvertToChangeGroupeConnaissanceErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ChangeGroupeConnaissanceErrorCode ENT_Tr::ConvertToChangeGroupeConnaissanceErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ChangeGroupeConnaissanceErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToChangeLiensLogistiquesErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ChangeLiensLogistiquesErrorCode ENT_Tr::ConvertToChangeLiensLogistiquesErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ChangeLiensLogistiquesErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToChangeAutomateErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_ChangeAutomateErrorCode ENT_Tr::ConvertToChangeAutomateErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ChangeAutomateErrorCodeConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToCtrlErrorCodeLauncher
// Created: AGR
//-----------------------------------------------------------------------------
E_CtrlErrorCodeLauncher ENT_Tr::ConvertToCtrlErrorCodeLauncher( const std::string& strName )
{
    return ENT_Tr::FindInConverter( CtrlErrorCodeLauncherConverter_, strName );
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
// Name: ENT_Tr::ConvertToAutomateState
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomateState ENT_Tr::ConvertToAutomateState( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AutomateStateConverter_, strName );
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
// Name: ENT_Tr::ConvertToDisponibiliteAuTir
// Created: AGR
//-----------------------------------------------------------------------------
E_DisponibiliteAuTir ENT_Tr::ConvertToDisponibiliteAuTir( const std::string& strName )
{
    return ENT_Tr::FindInConverter( DisponibiliteAuTirConverter_, strName );
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
// Name: ENT_Tr::ConvertToSetAutomateModeErrorCode
// Created: AGR
//-----------------------------------------------------------------------------
E_SetAutomateModeErrorCode ENT_Tr::ConvertToSetAutomateModeErrorCode( const std::string& strName )
{
    return ENT_Tr::FindInConverter( SetAutomateModeErrorCodeConverter_, strName );
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
// Name: ENT_Tr::ConvertToAmbianceMission
// Created: AGR
//-----------------------------------------------------------------------------
E_AmbianceMission ENT_Tr::ConvertToAmbianceMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( AmbianceMissionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTypeMunitionTirIndirect
// Created: AGR
//-----------------------------------------------------------------------------
E_TypeMunitionTirIndirect ENT_Tr::ConvertToTypeMunitionTirIndirect( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TypeMunitionTirIndirectConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToActionReagirFaceAEni
// Created: AGR
//-----------------------------------------------------------------------------
E_ActionReagirFaceAEni ENT_Tr::ConvertToActionReagirFaceAEni( const std::string& strName )
{
    return ENT_Tr::FindInConverter( ActionReagirFaceAEniConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToTC2GererMaterielAvantDeplacement
// Created: AGR
//-----------------------------------------------------------------------------
E_TC2GererMaterielAvantDeplacement ENT_Tr::ConvertToTC2GererMaterielAvantDeplacement( const std::string& strName )
{
    return ENT_Tr::FindInConverter( TC2GererMaterielAvantDeplacementConverter_, strName );
}




//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToAutomataMission
// Created: AGR
//-----------------------------------------------------------------------------
E_AutomataMission ENT_Tr::ConvertToAutomataMission( const std::string& strName )
{
   return ENT_Tr::FindInConverter( automataMissionConverter_, strName );
}


//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToUnitMission
// Created: AGR
//-----------------------------------------------------------------------------
E_UnitMission ENT_Tr::ConvertToUnitMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( unitMissionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToPopulationMission
// Created: AGR
//-----------------------------------------------------------------------------
E_PopulationMission ENT_Tr::ConvertToPopulationMission( const std::string& strName )
{
    return ENT_Tr::FindInConverter( populationMissionConverter_, strName );
}

//-----------------------------------------------------------------------------
// Name: ENT_Tr::ConvertToFragOrder
// Created: AGR
//-----------------------------------------------------------------------------
E_FragOrder ENT_Tr::ConvertToFragOrder( const std::string& strName )
{
    return ENT_Tr::FindInConverter( fragOrderConverter_, strName );
}

