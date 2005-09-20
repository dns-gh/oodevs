// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Nature.cpp $
// $Author: Age $
// $Modtime: 15/04/05 16:23 $
// $Revision: 4 $
// $Workfile: MOS_Nature.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Nature.h"
#include "MT_Tools/MT_ScipioException.h"

// -----------------------------------------------------------------------------
// Name: MOS_Nature constructor
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
MOS_Nature::MOS_Nature( MOS_InputArchive& archive )
    : level_( eNatureLevel_None )
    , weapon_( eUnitNatureWeapon_None )
    , specialization_( eUnitNatureSpecialization_None )
    , qualifier_( eUnitNatureQualifier_None )
    , category_( eUnitNatureCategory_None )
    , mobility_( eUnitNatureMobility_None )
    , capacity_( eUnitCapaciteMission_CivilianLawEnforcement )
{
    if( ! archive.Section( "Nature", MOS_InputArchive::eNothing ) )
        return;
    Read( archive, "Niveau",          std::ptr_fun( ConvertToNatureLevel ),              level_ );
    Read( archive, "Arme",            std::ptr_fun( ConvertToUnitNatureWeapon ),         weapon_ );
    Read( archive, "Specialisation",  std::ptr_fun( ConvertToUnitNatureSpecialization ), specialization_ );
    Read( archive, "Qualificatif",    std::ptr_fun( ConvertToUnitNatureQualifier ),      qualifier_ );
    Read( archive, "Categorie",       std::ptr_fun( ConvertToUnitNatureCategory ),       category_ );
    Read( archive, "TypeMobilite",    std::ptr_fun( ConvertToUnitNatureMobility ),       mobility_ );
    Read( archive, "CapaciteMission", std::ptr_fun( ConvertToUnitCapaciteMission ),      capacity_ );
    archive.EndSection(); // nature

    symbol_      = Symbol( *this );
    levelSymbol_ = LevelSymbol( *this );
}

// $$$$ AGE 2005-04-08: Try to organize symbols

// -----------------------------------------------------------------------------
// Name: MOS_Nature destructor
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
MOS_Nature::~MOS_Nature()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::Read
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
template< typename Enum, typename Solver >
void MOS_Nature::Read( MOS_InputArchive& archive, const std::string& field, const Solver& solver, Enum& result )
{
    std::string strValue;
    if( ! archive.Section( field, MOS_InputArchive::eNothing ) )
        return;
    archive.ReadAttribute( "type", strValue );
    archive.EndSection(); // field
    result = (Enum) solver( strValue );
}

namespace
{
    unsigned int Find( const std::string& strName, const std::string names[], unsigned int nMax )
    {
        for( unsigned int i = 0; i < nMax; ++i )
            if( !::stricmp( names[ i ].c_str(), strName.c_str() ) )
                return i;
        return 0;
    };
};

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToNatureLevel
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToNatureLevel( const std::string& strName )
{
    static std::string names[] = { "none", "o", "oo", "ooo", "i", "ii", "iii", "x", "xx", "xxx", "xxxx", "xxxxx" };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToUnitNatureWeapon
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToUnitNatureWeapon( const std::string& strName )
{
    static std::string names[] = {
        "None",
        "Antichar",
        "Administration",
        "Defense aerienne",
        "Centre soutien operations aeriennes",
        "Blinde",
        "Aviation",
        "Cavalerie",
        "Genie",
        "Deminage",
        "Soutien artillerie",
        "Artillerie",
        "Infanterie",
        "Guerre information",
        "Forces securite interne",
        "Execution loi",
        "Soutien atterrissage",
        "Logistique",
        "Reparation",
        "Manoeuvre",
        "Medical",
        "Renseignement militaire",
        "Missile",
        "NBC",
        "Inconnue",
        "Non specifiee",
        "Reconnaissance",
        "Transmissions",
        "Ravitaillement",
        "Tactical air control party",
        "Transport"
    };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToUnitNatureSpecialization
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToUnitNatureSpecialization( const std::string& strName )
{
    static std::string names[] = {
        "None",
        "Aerien",
        "Attaque aerienne",
        "Aeroporte",
        "Blinde",
        "Compose",
        "Construction",
        "Dentiste",
        "Guerre electronique",
        "Finance",
        "Voilure fixe",
        "Terrestre",
        "Helicoptere",
        "Jag",
        "Mecanise",
        "Police militaire",
        "Motorise",
        "Naval",
        "Service du personnel",
        "Courrier",
        "Psychologique",
        "Chemin de fer",
        "Religieux",
        "Riverine",
        "Security police",
        "Patrouille cotiere",
        "Chirurgie",
        "Veterinaire",
        "Decollage atterissage vertical court",
    };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToUnitNatureQualifier
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToUnitNatureQualifier( const std::string& strName )
{
    static std::string names[] = {
        "None",
        "Lourd",
        "Leger",
        "Moyen",
    };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToUnitNatureCategory
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToUnitNatureCategory( const std::string& strName )
{
    static std::string names[] = {
        "None",
        "Combat",
        "Soutien logistique",
        "Combat support",
        "Inconnu",
    };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToUnitNatureMobility
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToUnitNatureMobility( const std::string& strName )
{
    static std::string names[] = {
        "None",
        "Aeroporte",
        "Aerien compose",
        "Aerien voilure fixe",
        "Aerien plus leger que air",
        "Aerien ailes rotatives",
        "Aerien amphibie",
        "Animal monte",
        "Demonte",
        "Terrestre chemin de fer",
        "Terrestre auto propulse",
        "Terrestre chenille",
        "Terrestre remorque",
        "Terrestre roues",
        "Maritime sous marin",
        "Maritime surface",
        "Inconnu",
        
    };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::ConvertToUnitCapaciteMission
// Created: AGE 2005-04-15
// -----------------------------------------------------------------------------
unsigned int MOS_Nature::ConvertToUnitCapaciteMission( const std::string& strName )
{
    static std::string names[] = {
        "Civilian law enforcement",
        "Command operations",
        "Couverture aerienne",
        "Joint intelligence",
        "Morale, welfare recreation",
        "Reglage electronique",
        "Remplacement",
        "Service mortuaire",
        "Signals intelligence (SIGINT)",
        "Theatre missile defence",
        "Utilitaire",
        "Aeroporte",
        "Affaires publiques",
        "Affaires publiques, communication",
        "Affaires publiques, joint information",
        "Alpin",
        "Amphibie",
        "Arctique",
        "Artillerie - reconnaissance",
        "Attaque",
        "Attaque aerienne",
        "C2",
        "Chemin de fer",
        "Ciblage",
        "Contre espionnage",
        "Eclairage",
        "Entretien",
        "Entretien, materiel",
        "Entretien, missile",
        "Entretien, opto-electronique",
        "Espionnage",
        "Espionnage militaire, aerial exploitation",
        "Espionnage militaire, operation",
        "Espionnage militaire, tactical exploitation",
        "Evacuation sanitaire",
        "Finance",
        "Genie, combat",
        "Genie, construction",
        "Genie, construction navale",
        "Guerre electronique",
        "Guerre electronique, interception",
        "Guerre electronique, brouillage",
        "Guerre electronique, detection",
        "Interrogation",
        "Main d'oeuvre",
        "Maintien de la paix",
        "Marine",
        "Mine de contre-mesure",
        "NBC, biologique",
        "NBC, chimique",
        "NBC, chimique, fumee",
        "NBC, decontamination",
        "NBC, decontamination chimique",
        "NBC, nucleaire",
        "Non renseigne",
        "Observation",
        "Observation, capteur",
        "Observation, longue portee",
        "Observation, meteo",
        "Observation, module terrestre",
        "Purification de l'eau",
        "Ravitaillement (class I)",
        "Ravitaillement (class II)",
        "Ravitaillement (class III aviation)",
        "Ravitaillement (class III)",
        "Ravitaillement (class IV)",
        "Ravitaillement (class V)",
        "Ravitaillement (eau)",
        "Ravitaillement, blanchisserie/bain",
        "Recherche & sauvetage",
        "Reconnaissance",
        "Reconnaissance aerienne tactique",
        "Recuperation",
        "Service du personnel",
        "Service juridique",
        "Support aerien rapproche",
        "Transmissions, forward communications",
        "Transmissions, node centre",
        "Transmissions, node, large extension",
        "Transmissions, node, small extension",
        "Transmissions, radio relay",
        "Transmissions, radio tactical satellite",
        "Transmissions, radio teletype",
        "Transmissions, support",
        "Transmissions, radio",
        "Transport, allocation des mouvements",
        "Transport, APOD/APOE",
        "Transport, SPOD/SPOE",
        "Transport, missile",
        "Verrouillage de cible",
        "Verrouillage de cible, flash",
        "Verrouillage de cible, radar",
        "Verrouillage de cible, sonore",
    };
    static unsigned int nNames = sizeof( names ) / sizeof( std::string );
    return Find( strName, names, nNames );
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LevelSymbol
// Created: AGE 2005-04-08
// -----------------------------------------------------------------------------
char MOS_Nature::LevelSymbol( const MOS_Nature& n )
{
    switch( n.level_ )
    {
        case eNatureLevel_o:     return 'Z';
        case eNatureLevel_oo:    return 'Y';
        case eNatureLevel_ooo:   return 'X';
        case eNatureLevel_i:     return 'W';
        case eNatureLevel_ii:    return 'V';
        case eNatureLevel_iii:   return 'U';
        case eNatureLevel_x:     return 'a';
        case eNatureLevel_xx:    return 'a';
        case eNatureLevel_xxx:   return 'a';
        case eNatureLevel_xxxx:  return 'a';
        case eNatureLevel_xxxxx: return 'a';
    };
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::Symbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::Symbol( const MOS_Nature& n )
{
    switch( n.category_ )
    {
        case eUnitNatureCategory_Combat:            return CombatSymbol( n );
        case eUnitNatureCategory_SoutienLogistique: return LogSymbol( n );
        case eUnitNatureCategory_CombatSupport:     return SupportSymbol( n );
    };
    return 'b';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatSymbol( const MOS_Nature& n )
{
    switch( n.weapon_ )
    {
        case eUnitNatureWeapon_DefenseAerienne:       return CombatAirDefenseSymbol( n );
        case eUnitNatureWeapon_ForcesSecuriteInterne: return CombatSecuritySymbol( n );
        case eUnitNatureWeapon_Blinde:                return CombatArmoredSymbol( n );
        case eUnitNatureWeapon_Antichar:              return CombatAntiCharSymbol( n );
        case eUnitNatureWeapon_Aviation:              return CombatAviationSymbol( n );
        case eUnitNatureWeapon_Infanterie:            return CombatInfantrySymbol( n );
        case eUnitNatureWeapon_Genie:                 return CombatGenieSymbol( n );
        case eUnitNatureWeapon_Artillerie:            return CombatArtillerySymbol( n );
        case eUnitNatureWeapon_Reconnaissance:        return CombatReconSymbol( n );
        case eUnitNatureWeapon_Missile:               return CombatMissileSymbol( n );
    };
    return 'c';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LogSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::LogSymbol( const MOS_Nature& n )
{
    switch( n.weapon_ )
    {
        case eUnitNatureWeapon_Administration: return LogAdministrativeSymbol( n );
        case eUnitNatureWeapon_Medical:        return LogMedicalSymbol( n );
        case eUnitNatureWeapon_Ravitaillement: return LogSupplySymbol( n );
        case eUnitNatureWeapon_Transport:      return LogTransportSymbol( n );
        case eUnitNatureWeapon_Reparation:     return LogReparationSymbol( n );
    };
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportSymbol( const MOS_Nature& n )
{
    switch( n.weapon_ )
    {
        case eUnitNatureWeapon_Nbc:                    return SupportNbcSymbol( n );
        case eUnitNatureWeapon_RenseignementMilitaire: return SupportRensSymbol( n );
        case eUnitNatureWeapon_ExecutionLoi:           return SupportLawSymbol( n );
        case eUnitNatureWeapon_Transmissions:          return SupportTransSymbol( n );
        case eUnitNatureWeapon_GuerreInformation:      return SupportInfWarSymbol( n );
        case eUnitNatureWeapon_SoutienAtterrissage:    return SupportLandingSymbol( n );
        case eUnitNatureWeapon_Deminage:               return SupportMineSweepSymbol( n );
        case eUnitNatureWeapon_Manoeuvre:              return SupportManoeuvreSymbol( n );
    };
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatAirDefenseSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatAirDefenseSymbol( const MOS_Nature& n )
{
    return 'd';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatSecuritySymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatSecuritySymbol( const MOS_Nature& n )
{
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatArmoredSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatArmoredSymbol( const MOS_Nature& n )
{
    if( n.mobility_ == eUnitNatureMobility_TerrestreRoues )
        return 'j';
    if( n.mobility_ == eUnitNatureMobility_TerrestreChenille )
        return 'e';
    if( n.capacity_ == eUnitCapaciteMission_Eclairage )
        return 'k';
    return 'e';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatAntiCharSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatAntiCharSymbol( const MOS_Nature& n )
{
    if( n.specialization_ == eUnitNatureSpecialization_Blinde )
        return 'm';
    if( n.mobility_ == eUnitNatureMobility_TerrestreRoues )
        return 'P';
    return 'l';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatAviationSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatAviationSymbol( const MOS_Nature& n )
{
    if( n.mobility_ == eUnitNatureMobility_AerienVoilureFixe )
        return 'o';
    if( n.mobility_ == eUnitNatureMobility_AerienAilesRotatives )
        return 'p';
    if( n.specialization_ == eUnitNatureSpecialization_DecollageAtterissageVerticalCourt )
        return 'a';
    return 'n';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatInfantrySymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatInfantrySymbol( const MOS_Nature& n )
{
    if( n.specialization_ == eUnitNatureSpecialization_Mecanise )
        return 'r';
    return 'q';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatGenieSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatGenieSymbol( const MOS_Nature& n )
{
    if( n.capacity_ == eUnitCapaciteMission_GenieCombat )
        return CombatGenieCombatSymbol( n );
    if( n.capacity_ == eUnitCapaciteMission_GenieConstruction )
        return CombatGenieConstructionSymbol( n );
    return 'Q';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatGenieCombatSymbol
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
char MOS_Nature::CombatGenieCombatSymbol( const MOS_Nature& n )
{
    if( n.specialization_ == eUnitNatureSpecialization_Mecanise
     || n.mobility_       == eUnitNatureMobility_TerrestreChenille )
        return 't';
    if( n.specialization_ == eUnitNatureSpecialization_Motorise )
        return 'u';
    return 'R';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatGenieConstructionSymbol
// Created: AGE 2005-04-14
// -----------------------------------------------------------------------------
char MOS_Nature::CombatGenieConstructionSymbol( const MOS_Nature& n )
{
    return 'Q'; // $$$$ AGE 2005-04-14:
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatArtillerySymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatArtillerySymbol( const MOS_Nature& n )
{
    if( n.capacity_ == eUnitCapaciteMission_ArtillerieReconnaissance
     || n.capacity_ == eUnitCapaciteMission_Reconnaissance )
        return 'x';
    if( n.capacity_ == eUnitCapaciteMission_VerrouillageDeCible
     || n.capacity_ == eUnitCapaciteMission_VerrouillageDeCibleFlash )
        return 'S';
    if( n.capacity_ == eUnitCapaciteMission_VerrouillageDeCibleRadar )
        return 'T';
    if( n.capacity_ == eUnitCapaciteMission_VerrouillageDeCibleSonore )
        return 'a';
    if( n.mobility_ == eUnitNatureMobility_TerrestreAutoPropulse )
        return '1';
    return 'v';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatReconSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatReconSymbol( const MOS_Nature& n )
{
    return 'w';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::CombatMissileSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::CombatMissileSymbol( const MOS_Nature& n )
{
    return 'y';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LogAdministrativeSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::LogAdministrativeSymbol( const MOS_Nature& n )
{
    return '2';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LogMedicalSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::LogMedicalSymbol( const MOS_Nature& n )
{
    if( n.specialization_ == eUnitNatureSpecialization_Psychologique )
        return 'A';
    return 'z';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LogSupplySymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::LogSupplySymbol( const MOS_Nature& n )
{
    if( n.capacity_ == eUnitCapaciteMission_RavitaillementClassi )
        return 'C';
    if( n.capacity_ == eUnitCapaciteMission_RavitaillementClassiii )
        return 'D';
    if( n.capacity_ == eUnitCapaciteMission_RavitaillementClassIv )
        return 'E';
    if( n.capacity_ == eUnitCapaciteMission_RavitaillementClassv )
        return 'F';
    return 'B';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LogTransportSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::LogTransportSymbol( const MOS_Nature& n )
{
    return 'G';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::LogReparationSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::LogReparationSymbol( const MOS_Nature& n )
{
    return 'H';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportNbcSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportNbcSymbol( const MOS_Nature& n )
{
    if( n.capacity_ == eUnitCapaciteMission_NbcBiologique )
        return 'M';
    if( n.capacity_ == eUnitCapaciteMission_NbcChimique )
        return 'J';
    if( n.capacity_ == eUnitCapaciteMission_NbcDecontamination
     || n.capacity_ == eUnitCapaciteMission_NbcDecontaminationChimique )
        return 'K';
    if( n.capacity_ == eUnitCapaciteMission_NbcNucleaire )
        return 'L';
    return 'I';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportRensSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportRensSymbol( const MOS_Nature& n )
{
    if( n.specialization_ == eUnitNatureSpecialization_GuerreElectronique )
        return SupportElecWarSymbol( n );
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportLawSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportLawSymbol( const MOS_Nature& n )
{
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportTransSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportTransSymbol( const MOS_Nature& n )
{
    return '3';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportInfWarSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportInfWarSymbol( const MOS_Nature& n )
{
    return '4';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportLandingSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportLandingSymbol( const MOS_Nature& n )
{
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportMineSweepSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportMineSweepSymbol( const MOS_Nature& n )
{
    return 'a';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportManoeuvreSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportManoeuvreSymbol( const MOS_Nature& n )
{
    return 'N';
}

// -----------------------------------------------------------------------------
// Name: MOS_Nature::SupportElecWarSymbol
// Created: AGE 2005-04-07
// -----------------------------------------------------------------------------
char MOS_Nature::SupportElecWarSymbol( const MOS_Nature& n )
{
    if( n.capacity_ == eUnitCapaciteMission_GuerreElectroniqueInterception )
        return '6';
    if( n.capacity_ == eUnitCapaciteMission_GuerreElectroniqueBrouillage )
        return '7';
    if( n.capacity_ == eUnitCapaciteMission_GuerreElectroniqueDetection )
        return '8';
    return '5';
}
