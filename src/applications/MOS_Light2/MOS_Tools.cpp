//*****************************************************************************
//
// $Created: NLD 2003-05-31 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Tools.cpp $
// $Author: Age $
// $Modtime: 27/04/05 18:51 $
// $Revision: 21 $
// $Workfile: MOS_Tools.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"
#include "MOS_Tools.h"

#include "MOS_Meteo_ABC.h"
#include "MOS_RawVisionData.h"
#include "MT_Tools/MT_Tools_Types.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Tools.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: AGE 2005-03-31
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_UnitPosture nPosture )
{
    QObject a;
    switch( nPosture )
    {
        case eUnitPosture_PostureMouvement:         return a.tr( "Mouvement" );
        case eUnitPosture_PostureMouvementDiscret:  return a.tr( "MouvementDiscret" );
        case eUnitPosture_PostureArret:             return a.tr( "Arret" );
        case eUnitPosture_PosturePosteReflexe:      return a.tr( "PosteReflexe" );
        case eUnitPosture_PosturePoste:             return a.tr( "Poste" );
        case eUnitPosture_PosturePosteAmenage:      return a.tr( "PosteAmenage" );
        case eUnitPosture_PosturePostePrepareGenie: return a.tr( "PostePrepareGenie" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nState 
    @return 
*/
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_TroopHealthState nState )
{
    QObject a;
    switch( nState )
    {
        case eTroopHealthStateTotal:              return a.tr( "Total" );
        case eTroopHealthStateOperational:        return a.tr( "Opérationnels" );
        case eTroopHealthStateDead:               return a.tr( "Morts" );
        case eTroopHealthStateWounded:            return a.tr( "Blessés" );
        case eTroopHealthStateMentalWounds:       return a.tr( "Blessés mentalement" );
        case eTroopHealthStateContaminated:       return a.tr( "Contaminés" );
        case eTroopHealthStateInTreatment:        return a.tr( "En traitement" );
        case eTroopHealthStateUsedForMaintenance: return a.tr( "Utilisés pour maintenance" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-04-15
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_FuncLimaType nType )
{
    QObject a;
    switch( nType )
    {
        case eLimaFuncLD:     return a.tr( "LD" );
        case eLimaFuncLCA:    return a.tr( "LCA" );
        case eLimaFuncLC:     return a.tr( "LC" );
        case eLimaFuncLI:     return a.tr( "LI" );
        case eLimaFuncLO:     return a.tr( "LO" );
        case eLimaFuncLCAR:   return a.tr( "LCAR" );
        case eLimaFuncLR:     return a.tr( "LR" );
        case eLimaFuncLDM:    return a.tr( "LDM" );
        case eLimaFuncLFM:    return a.tr( "LFM" );
        case eLimaFuncLIA:    return a.tr( "LIA" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_NatureAtlasType nType )
{
    QObject a;
    switch( nType )
    {
        case eNatureAtlas_Armored:          return a.tr( "Blinde" );
        case eNatureAtlas_ASS:              return a.tr( "ASS" );
        case eNatureAtlas_ASA:              return a.tr( "ASA" );
        case eNatureAtlas_PC:               return a.tr( "PC" );
        case eNatureAtlas_Log:              return a.tr( "Log" );
        case eNatureAtlas_Mortar:           return a.tr( "Mortier" );
        case eNatureAtlas_RocketLauncher:   return a.tr( "Lance roquette" );
        case eNatureAtlas_Vehicule:         return a.tr( "Vehicule" );
        case eNatureAtlas_PersonnelAPied:   return a.tr( "Personnel a pied" );
        case eNatureAtlas_Helicoptere:      return a.tr( "Helicoptere" );
        case eNatureAtlas_Indefini:         return a.tr( "Indefini" );
        case eNatureAtlas_None:             return a.tr( "Aucun" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nResult 
    @return 
*/
// Created: APE 2004-05-03
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_PerceptionResult nResult )
{
    QObject a;
    switch( nResult )
    {
        case eIdentification:  return a.tr( "Identification" );
        case eRecognition:     return a.tr( "Reconnaissance" );
        case eDetection:       return a.tr( "Detection" );
        case eNotSeen:         return a.tr( "Non vu" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( ASN1T_EnumTypeLocalisation nType )
{
    QObject a;
    switch( nType )
    {
        case EnumTypeLocalisation::circle:   return a.tr( "cercle" );
        case EnumTypeLocalisation::ellipse:  return a.tr( "ellipse" );
        case EnumTypeLocalisation::line:     return a.tr( "ligne" );
        case EnumTypeLocalisation::polygon:  return a.tr( "polygone" );
        case EnumTypeLocalisation::point:    return a.tr( "point" );
        case EnumTypeLocalisation::sector:   return a.tr( "secteur" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( ASN1T_EnumObjectType nType )
{
    QObject a;
    switch( nType )
    {
        case EnumObjectType::bouchon_mines:             return a.tr( "bouchon mines" );
        case EnumObjectType::zone_minee_lineaire:       return a.tr( "zone minee lineaire" );
        case EnumObjectType::zone_minee_par_dispersion: return a.tr( "zone minee par dispersion" );
        case EnumObjectType::fosse_anti_char:           return a.tr( "fosse anti char" );
        case EnumObjectType::abattis:                   return a.tr( "abattis" );
        case EnumObjectType::barricade:                 return a.tr( "baricade" );
        case EnumObjectType::eboulement:                return a.tr( "eboulement" );
        case EnumObjectType::destruction_route:         return a.tr( "destruction route" );
        case EnumObjectType::destruction_pont:          return a.tr( "destruction pont" );
        case EnumObjectType::pont_flottant:             return a.tr( "pont flottant" );
        case EnumObjectType::poste_tir:                 return a.tr( "poste tir" );
        case EnumObjectType::zone_protegee:             return a.tr( "zone protegee" );
        case EnumObjectType::zone_implantation_canon:   return a.tr( "zone implantation canon" );
        case EnumObjectType::zone_implantation_cobra:   return a.tr( "zone implantation cobra" );
        case EnumObjectType::zone_implantation_lrm:     return a.tr( "zone implantation lrm" );
        case EnumObjectType::site_franchissement:       return a.tr( "site franchissement" );
        case EnumObjectType::nuage_nbc:                 return a.tr( "nuage nbc" );
        case EnumObjectType::plot_ravitaillement:       return a.tr( "plot ravitaillement" );
        case EnumObjectType::zone_brouillage_brod:      return a.tr( "zone de brouillage brod" );
        case EnumObjectType::site_decontamination:      return a.tr( "site decontamination" );
        case EnumObjectType::rota:                      return a.tr( "rota" );
        case EnumObjectType::zone_nbc:                  return a.tr( "zone nbc" );
        case EnumObjectType::zone_brouillage_bromure:   return a.tr( "zone de brouillage bromure" );
        case EnumObjectType::aire_poser:                return a.tr( "aire poser" );
        case EnumObjectType::piste:                     return a.tr( "piste" );
        case EnumObjectType::plateforme:                return a.tr( "plateforme" );
        case EnumObjectType::zone_mobilite_amelioree:   return a.tr( "zone mobilite amelioree" );
        case EnumObjectType::zone_poser_helicoptere:    return a.tr( "zone poser helicoptere" );
        case EnumObjectType::aire_logistique:           return a.tr( "aire logistique" );
        case EnumObjectType::itineraire_logistique:     return a.tr( "itineraire logistique" );
        case EnumObjectType::camp_prisonniers:          return a.tr( "camp prisonniers" );
        case EnumObjectType::camp_refugies:             return a.tr( "camp refugies" );
        case EnumObjectType::poste_controle:            return a.tr( "poste controle" );
        case EnumObjectType::terrain_largage:           return a.tr( "terrain largage" );
        case EnumObjectType::zone_interdite_mouvement:  return a.tr( "zone interdite mouvement" );
        case EnumObjectType::zone_interdite_tir:        return a.tr( "zone interdite tir" );
        case EnumObjectType::zone_implantation_mortier: return a.tr( "zone implantation mortier" );

        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nUrgency 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( ASN1T_EnumMissionGenUrgence nUrgency )
{
    QObject a;
    switch( nUrgency )
    {
        case EnumMissionGenUrgence::serie_minimum:          return a.tr( "Urgence" );
        case EnumMissionGenUrgence::serie_complementaire:   return a.tr( "Complémentaire" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( ASN1T_EnumMissionGenSousTypeObstacle nType )
{
    QObject a;
    switch( nType )
    {
        case EnumMissionGenSousTypeObstacle::preliminaire:   return a.tr( "Préliminaire" );
        case EnumMissionGenSousTypeObstacle::de_manoeuvre:   return a.tr( "De manoeuvre" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nObject 
    @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( ASN1T_EnumMissionGenPriorite nObject )
{
    QObject a;
    switch( nObject )
    {
        case EnumMissionGenPriorite::prioritaire:     return a.tr( "Prioritaire" );
        case EnumMissionGenPriorite::normal:          return a.tr( "Normal" );
        case EnumMissionGenPriorite::peu_prioritaire: return a.tr( "Peu prioritaire" );
        default:
            assert( false );
            return "Unknown";
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nType 
    @return 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_ForceRatioState nType )
{
    QObject a;
    switch( nType )
    {
        case eForceRatioStateNone:        return a.tr( "Aucun" );
        case eForceRatioStateNeutral:     return a.tr( "Neutre" );
        case eForceRatioStateFavorable:   return a.tr( "Favorable" );
        case eForceRatioStateUnfavorable: return a.tr( "Defavorable" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nState 
    @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_RulesOfEngagementState nState )
{
    QObject a;
    switch( nState )
    {
        case eRoeStateNone              : return a.tr( "Aucun" );
        case eRoeStateFreeFire          : return a.tr( "Tir libre" );
        case eRoeStateRestrictedFire    : return a.tr( "Tir restreint" );
        case eRoeStateFireByOrder       : return a.tr( "Tir sur ordre" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
/** @param  nState 
    @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( E_CloseCombatState nState )
{
    QObject a;
    switch( nState )
    {
        case eCloseCombatStateNone            : return a.tr( "Aucun" );
        case eCloseCombatStateDodge           : return a.tr( "Evitement" );
        case eCloseCombatStateFixed           : return a.tr( "Fixé" );
        case eCloseCombatStateProceedMission  : return a.tr( "Continue mission" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
QString MOS_Tools::ToString( MOS_Meteo_ABC::E_WeatherType nType )
{
    QObject a;
    switch( nType )
    {
        case MOS_Meteo_ABC::eWeather_Clear:     return a.tr( "Dégagé" );
        case MOS_Meteo_ABC::eWeather_Storm:     return a.tr( "Tempête" );
        case MOS_Meteo_ABC::eWeather_Fog:       return a.tr( "Brouillard" );
        case MOS_Meteo_ABC::eWeather_Drizzle:   return a.tr( "Crachin" );
        case MOS_Meteo_ABC::eWeather_Rain:      return a.tr( "Pluie" );
        case MOS_Meteo_ABC::eWeather_Snow:      return a.tr( "Neige" );
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* MOS_Tools::ToString( const ASN1T_EnumInfoContextErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumInfoContextErrorCode::no_error                     : return "Aucune erreur";
        case EnumInfoContextErrorCode::error_invalid_id             : return "ID non valide";
        case EnumInfoContextErrorCode::error_invalid_geometry       : return "Géométrie non valide" ;
        case EnumInfoContextErrorCode::error_invalid_lima_function  : return "Lima non valide" ;
        case EnumInfoContextErrorCode::error_invalid_limit_or_lima_used : return "Lima ou limite non valide";
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* MOS_Tools::ToString( const ASN1T_EnumCtrlErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumCtrlErrorCode::no_error                 : return "No error";
        case EnumCtrlErrorCode::error_already_started    : return "Already started";
        case EnumCtrlErrorCode::error_not_started        : return "Not started";
        case EnumCtrlErrorCode::error_not_paused         : return "Not paused";
        case EnumCtrlErrorCode::error_already_paused     : return "Already paused";
        case EnumCtrlErrorCode::error_invalid_time_factor: return "Invalid time factor";

        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* MOS_Tools::ToString( const ASN1T_EnumOrderErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumOrderErrorCode::no_error                        : return "No error";
        case EnumOrderErrorCode::error_invalid_unit              : return "Invalid unit";
        case EnumOrderErrorCode::error_invalid_limit             : return "Invalid limit";
        case EnumOrderErrorCode::error_invalid_lima              : return "Invalid lima";
        case EnumOrderErrorCode::error_invalid_mission           : return "Invalid mission";
        case EnumOrderErrorCode::error_invalid_mission_parameters: return "Invalid mission parameters";
        case EnumOrderErrorCode::error_unit_cannot_receive_order : return "Unit cannot receive this order";
        case EnumOrderErrorCode::error_invalid_order_conduite    : return "Invalid ordre de conduite";
        case EnumOrderErrorCode::error_invalid_order_mission     : return "Invalid ordre de mission";
        case EnumOrderErrorCode::error_invalid_order_initial     : return "Invalid ordre initial";
        case EnumOrderErrorCode::error_invalid_order_conduite_parameters: return "Invalid ordre de conduite parameters";
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* MOS_Tools::ToString( const ASN1T_EnumUnitAttrErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumUnitAttrErrorCode::no_error              : return "No error";
        case EnumUnitAttrErrorCode::error_invalid_unit    : return "Invalid unit";
        case EnumUnitAttrErrorCode::error_automate_embraye: return "Error : automate embraye";

        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToString
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
const char* MOS_Tools::ToString( const ASN1T_EnumObjectErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumObjectErrorCode::no_error                  : return "No error";
        case EnumObjectErrorCode::error_invalid_object      : return "Invalid object";
        case EnumObjectErrorCode::error_invalid_id          : return "Invalid id";
        case EnumObjectErrorCode::error_invalid_camp        : return "Invalid camp";
        case EnumObjectErrorCode::error_invalid_localisation: return "Invalid localisation";
        case EnumObjectErrorCode::error_missing_specific_attributes: return "Missing specific parameters";
        case EnumObjectErrorCode::error_invalid_specific_attributes: return "Invalid specific parameters";

        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::ToSymbol
/** @param  strUnitType 
    @return 
    */
// Created: APE 2004-08-30
// -----------------------------------------------------------------------------
const char* MOS_Tools::ToSymbol( const std::string& strUnitType )
{
    if( strUnitType == "CompagnieInfanterie"                 ) return "4I";
    if( strUnitType == "SectionInfanteriePied"               ) return "3I";
    if( strUnitType == "SectionInfanterieMecaniseeRoue"      ) return "3H";
    if( strUnitType == "SectionInfanterieMecaniseeChenille"  ) return "3M";
    if( strUnitType == "SectionInfanterieAntichar"           ) return "3*";
    if( strUnitType == "SectionInfanterieMecaniseeAntichar"  ) return "3P";
    if( strUnitType == "EscadronBlinde"                      ) return "4A";
    if( strUnitType == "PelotonBlinde"                       ) return "3A";
    if( strUnitType == "PelotonBlindeRoue"                   ) return "3C";
    if( strUnitType == "PelotonBlindeReco"                   ) return "3R";
    if( strUnitType == "PelotonBlindeRecoRoue"               ) return "3'";
    if( strUnitType == "PatrouilleBlindeEclairInvest"        ) return "2J";
    if( strUnitType == "PelotonBlindeAppuiDirect"            ) return "3B";
    if( strUnitType == "CompagnieGenie"                      ) return "4E";
    if( strUnitType == "SectionGeniePied"                    ) return "3E";
    if( strUnitType == "SectionGenieMecanisee"               ) return "3D";
    if( strUnitType == "GroupeGenieAppui"                    ) return "2a";
    if( strUnitType == "GroupeGenieAppuiEQT"                 ) return "2b";
    if( strUnitType == "GroupeGenieAppuiTECH"                ) return "2c";
    if( strUnitType == "GroupeGenieAppuiOBST"                ) return "2d";
    if( strUnitType == "GroupeGenieAppuiPont"                ) return "2e";
    if( strUnitType == "CompagnieArtillerie"                 ) return "4#";
    if( strUnitType == "SectionArtillerie"                   ) return "3#";
    if( strUnitType == "SectionArtillerieBlinde"             ) return "3(";
    if( strUnitType == "SectionArtillerieReco"               ) return "3\xDA";
    if( strUnitType == "SectionArtillerieSolAir"             ) return "3:"; 
    if( strUnitType == "SectionSante"                        ) return "3F"; 
    if( strUnitType == "CompagnieSante"                      ) return "4F"; 
    if( strUnitType == "GroupeRecoALAT"                      ) return "3S"; 
    if( strUnitType == ""                                    ) return "?";
    return "?";
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::FromString
/** @param  szString 
    @param  nId 
    */
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void MOS_Tools::FromString( const char* szString, ASN1T_EnumObjectType& nId )
{
    std::string strObjectType( szString );
    nId = (ASN1T_EnumObjectType)-1;

    if( sCaseInsensitiveEqual()( strObjectType, "fosse anti char"          ) ) nId = EnumObjectType::fosse_anti_char;
    if( sCaseInsensitiveEqual()( strObjectType, "abattis"                  ) ) nId = EnumObjectType::abattis;
    if( sCaseInsensitiveEqual()( strObjectType, "barricade"                ) ) nId = EnumObjectType::barricade;
    if( sCaseInsensitiveEqual()( strObjectType, "bouchon mines"            ) ) nId = EnumObjectType::bouchon_mines;
    if( sCaseInsensitiveEqual()( strObjectType, "zone minee lineaire"      ) ) nId = EnumObjectType::zone_minee_lineaire;
    if( sCaseInsensitiveEqual()( strObjectType, "zone minee par dispersion") ) nId = EnumObjectType::zone_minee_par_dispersion;
    if( sCaseInsensitiveEqual()( strObjectType, "eboulement"               ) ) nId = EnumObjectType::eboulement;
    if( sCaseInsensitiveEqual()( strObjectType, "destruction route"        ) ) nId = EnumObjectType::destruction_route;
    if( sCaseInsensitiveEqual()( strObjectType, "destruction pont"         ) ) nId = EnumObjectType::destruction_pont;
    if( sCaseInsensitiveEqual()( strObjectType, "pont flottant"            ) ) nId = EnumObjectType::pont_flottant;
    if( sCaseInsensitiveEqual()( strObjectType, "poste tir"                ) ) nId = EnumObjectType::poste_tir;
    if( sCaseInsensitiveEqual()( strObjectType, "zone protegee"            ) ) nId = EnumObjectType::zone_protegee;
    if( sCaseInsensitiveEqual()( strObjectType, "zone implantation canon"  ) ) nId = EnumObjectType::zone_implantation_canon;
    if( sCaseInsensitiveEqual()( strObjectType, "zone implantation cobra"  ) ) nId = EnumObjectType::zone_implantation_cobra;
    if( sCaseInsensitiveEqual()( strObjectType, "zone implantation lrm"    ) ) nId = EnumObjectType::zone_implantation_lrm;
    if( sCaseInsensitiveEqual()( strObjectType, "site franchissement"      ) ) nId = EnumObjectType::site_franchissement;
    if( sCaseInsensitiveEqual()( strObjectType, "plot ravitaillement"      ) ) nId = EnumObjectType::plot_ravitaillement;
    if( sCaseInsensitiveEqual()( strObjectType, "nuage nbc"                ) ) nId = EnumObjectType::nuage_nbc;
    if( sCaseInsensitiveEqual()( strObjectType, "site decontamination"     ) ) nId = EnumObjectType::site_decontamination;
    if( sCaseInsensitiveEqual()( strObjectType, "zone brouillage brod"     ) ) nId = EnumObjectType::zone_brouillage_brod;
    if( sCaseInsensitiveEqual()( strObjectType, "rota"                     ) ) nId = EnumObjectType::rota;
    if( sCaseInsensitiveEqual()( strObjectType, "zone nbc"                 ) ) nId = EnumObjectType::zone_nbc;
    if( sCaseInsensitiveEqual()( strObjectType, "zone brouillage bromure"  ) ) nId = EnumObjectType::zone_brouillage_bromure;
    if( sCaseInsensitiveEqual()( strObjectType, "aire poser"               ) ) nId = EnumObjectType::aire_poser;
    if( sCaseInsensitiveEqual()( strObjectType, "piste"                    ) ) nId = EnumObjectType::piste;
    if( sCaseInsensitiveEqual()( strObjectType, "plateforme"              ) ) nId = EnumObjectType::plateforme;
    if( sCaseInsensitiveEqual()( strObjectType, "zone mobilite amelioree"  ) ) nId = EnumObjectType::zone_mobilite_amelioree;
    if( sCaseInsensitiveEqual()( strObjectType, "zone poser helicoptere"   ) ) nId = EnumObjectType::zone_poser_helicoptere;
    if( sCaseInsensitiveEqual()( strObjectType, "aire logistique"          ) ) nId = EnumObjectType::aire_logistique;
    if( sCaseInsensitiveEqual()( strObjectType, "itineraire logistique"    ) ) nId = EnumObjectType::itineraire_logistique;
    if( sCaseInsensitiveEqual()( strObjectType, "camp prisonniers"         ) ) nId = EnumObjectType::camp_prisonniers;
    if( sCaseInsensitiveEqual()( strObjectType, "camp refugies"            ) ) nId = EnumObjectType::camp_refugies;
    if( sCaseInsensitiveEqual()( strObjectType, "poste controle"           ) ) nId = EnumObjectType::poste_controle;
    if( sCaseInsensitiveEqual()( strObjectType, "terrain largage"           ) ) nId = EnumObjectType::terrain_largage;
    if( sCaseInsensitiveEqual()( strObjectType, "zone interdite mouvement" ) ) nId = EnumObjectType::zone_interdite_mouvement;
    if( sCaseInsensitiveEqual()( strObjectType, "zone interdite tir"        ) ) nId = EnumObjectType::zone_interdite_tir;
    if( sCaseInsensitiveEqual()( strObjectType, "zone implantation mortier" ) ) nId = EnumObjectType::zone_implantation_mortier;
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::FromString
/** @param  szString 
    @param  nId 
    */
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void MOS_Tools::FromString( const char* szString, ASN1T_EnumTypeLocalisation& nId )
{
    std::string strType( szString );
    nId = (ASN1T_EnumTypeLocalisation)-1;

    if( strType == "cercle" )       nId = EnumTypeLocalisation::circle;
    if( strType == "ellipse" )      nId = EnumTypeLocalisation::ellipse;
    if( strType == "ligne" )        nId = EnumTypeLocalisation::line;
    if( strType == "polygone" )     nId = EnumTypeLocalisation::polygon;
    if( strType == "point" )        nId = EnumTypeLocalisation::point;
    if( strType == "secteur" )      nId = EnumTypeLocalisation::sector;

    if( nId == (ASN1T_EnumTypeLocalisation)-1 )
    {
        assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_Tools::PointNearLine
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
bool MOS_Tools::PointNearLine( const MT_Vector2D& point, const T_PointVector& line, MT_Float rMinDist, bool bLineLoop )
{
    assert( ! line.empty() );

    if( line.size() == 1 )
        return point.SquareDistance( line.front() ) < (rMinDist * rMinDist);

    for( uint n = 0; n < line.size() - 1; ++n )
    {
        MT_Line tmpLine( line[n], line[n+1] );
        if( tmpLine.IsInside( point, rMinDist ) )
            return true;
    }

    if( bLineLoop )
    {
        MT_Line tmpLine( line[line.size()-1], line[0] );
        if( tmpLine.IsInside( point, rMinDist ) )
            return true;
    }

    return false;
}

MOS_Tools::converter< E_NatureAtlasType > MOS_Tools::natureAtlasConverter_[] = 
{
    converter< E_NatureAtlasType >( "Blinde"          , eNatureAtlas_Armored         ),
    converter< E_NatureAtlasType >( "ASS"             , eNatureAtlas_ASS             ),
    converter< E_NatureAtlasType >( "ASA"             , eNatureAtlas_ASA             ),
    converter< E_NatureAtlasType >( "PC"              , eNatureAtlas_PC              ),
    converter< E_NatureAtlasType >( "Log"             , eNatureAtlas_Log             ),
    converter< E_NatureAtlasType >( "Mortier"         , eNatureAtlas_Mortar          ),
    converter< E_NatureAtlasType >( "Lance roquette"  , eNatureAtlas_RocketLauncher  ),
    converter< E_NatureAtlasType >( "Vehicule"        , eNatureAtlas_Vehicule        ),
    converter< E_NatureAtlasType >( "Personnel a pied", eNatureAtlas_PersonnelAPied  ),
    converter< E_NatureAtlasType >( "Helicoptere"     , eNatureAtlas_Helicoptere     ),
    converter< E_NatureAtlasType >( "Indefini"        , eNatureAtlas_Indefini        ),
    converter< E_NatureAtlasType >( "Aucun"           , eNatureAtlas_None            ),
    converter< E_NatureAtlasType >( ""                , (E_NatureAtlasType)-1        )
};

MOS_Tools::converter< E_PerceptionResult > MOS_Tools::perceptionLevelConverter_[] = 
{
    converter< E_PerceptionResult >( "Identification", eIdentification       ),
        converter< E_PerceptionResult >( "Reconnaissance", eRecognition          ),
        converter< E_PerceptionResult >( "Detection"     , eDetection            ),
        converter< E_PerceptionResult >( "Non vu"        , eNotSeen              ),
        converter< E_PerceptionResult >( ""              , (E_PerceptionResult)-1 )
};

MOS_Tools::converter< E_WeatherType > MOS_Tools::weatherConverter_[] =
{
    converter< E_WeatherType >( "PasDePrecipitation", eWeatherTypeNone            ),
        converter< E_WeatherType >( "TempeteDeSable"    , eWeatherTypeRainOrSnowStorm ),
        converter< E_WeatherType >( "Brouillard"        , eWeatherTypeFog             ),
        converter< E_WeatherType >( "Crachin"           , eWeatherTypeDrizzle         ),
        converter< E_WeatherType >( "Pluie"             , eWeatherTypeRain            ),
        converter< E_WeatherType >( "Neige"             , eWeatherTypeSnow            ),
        converter< E_WeatherType >( "Fumigene"          , eWeatherTypeSmoke           ),
        converter< E_WeatherType >( ""                  , (E_WeatherType)-1 )
};

MOS_Tools::converter< E_LightingType > MOS_Tools::lightingConverter_[] =
{
    converter< E_LightingType >( "JourSansNuage"         , eLightingJourSansNuage          ),
        converter< E_LightingType >( "JourPeuNuageux"        , eLightingJourPeuNuageux         ),
        converter< E_LightingType >( "JourMoyennementNuageux", eLightingJourMoyennementNuageux ),
        converter< E_LightingType >( "JourAssezNuageux"      , eLightingJourAssezNuageux       ),
        converter< E_LightingType >( "JourTresNuageux"       , eLightingJourTresNuageux        ),
        converter< E_LightingType >( "NuitPleineLune"        , eLightingNuitPleineLune         ),
        converter< E_LightingType >( "NuitTroisQuartDeLune"  , eLightingNuitTroisQuartDeLune   ),
        converter< E_LightingType >( "NuitDemiLune"          , eLightingNuitDemiLune           ),
        converter< E_LightingType >( "NuitQuartDeLune"       , eLightingNuitQuartDeLune        ),
        converter< E_LightingType >( "NuitNouvelleLune"      , eLightingNuitNouvelleLune       ),
        converter< E_LightingType >( "Eclairant"             , eLightingEclairant              ),
        converter< E_LightingType >( ""                      , (E_LightingType)-1 )
};

MOS_Tools::converter< MOS_RawVisionData::E_VisionObject > MOS_Tools::environnementConverter_[] =
{
    converter< MOS_RawVisionData::E_VisionObject >( "Sol"   , MOS_RawVisionData::eVisionGround ),
    converter< MOS_RawVisionData::E_VisionObject >( "Vide"  , MOS_RawVisionData::eVisionEmpty  ),
    converter< MOS_RawVisionData::E_VisionObject >( "Foret" , MOS_RawVisionData::eVisionForest ),
    converter< MOS_RawVisionData::E_VisionObject >( "Urbain", MOS_RawVisionData::eVisionUrban  ),
    converter< MOS_RawVisionData::E_VisionObject >( ""      , (MOS_RawVisionData::E_VisionObject)-1 )
};

MOS_Tools::converter< ASN1T_EnumDiplomatie > MOS_Tools::diplomatyConverter_[] =
{
    converter< ASN1T_EnumDiplomatie >( "inconnu", EnumDiplomatie::inconnu ),
    converter< ASN1T_EnumDiplomatie >( "ami"    , EnumDiplomatie::ami  ),
    converter< ASN1T_EnumDiplomatie >( "neutre" , EnumDiplomatie::neutre ),
    converter< ASN1T_EnumDiplomatie >( "ennemi" , EnumDiplomatie::ennemi  ),
    converter< ASN1T_EnumDiplomatie >( ""       , (ASN1T_EnumDiplomatie)-1 )
};

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertEnvironementType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertEnvironementType( MOS_RawVisionData::E_VisionObject nObject )
{
    return InverseFindInConverter( environnementConverter_, nObject );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertWeatherType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertWeatherType( E_WeatherType nWeatherType )
{
    return InverseFindInConverter( weatherConverter_, nWeatherType );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertLightingType
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertLightingType( E_LightingType nLightingType )
{
    return InverseFindInConverter( lightingConverter_, nLightingType );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::GetLightingFromName
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
E_LightingType MOS_Tools::GetLightingFromName( const std::string& sName )
{
    return FindInConverter( lightingConverter_, sName );
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertNatureAtlas
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertNatureAtlas( E_NatureAtlasType n  )
{
    return InverseFindInConverter( natureAtlasConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertPerceptionLevel
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertPerceptionLevel( E_PerceptionResult n )
{
    return InverseFindInConverter( perceptionLevelConverter_, n );     
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::ConvertDiplomaty
/** @return .
*/
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
std::string MOS_Tools::ConvertDiplomaty( ASN1T_EnumDiplomatie n )
{
    return InverseFindInConverter( diplomatyConverter_, n ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_Tools::GetDiplomatyFromName
/** @return 
*/
// Created: APE 2004-10-29
// -----------------------------------------------------------------------------
ASN1T_EnumDiplomatie MOS_Tools::GetDiplomatyFromName( const std::string& sName )
{
    return FindInConverter( diplomatyConverter_, sName );
}
