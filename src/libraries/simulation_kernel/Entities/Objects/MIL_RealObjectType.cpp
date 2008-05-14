// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_RealObjectType.cpp $
// $Author: Age $
// $Modtime: 7/04/05 14:44 $
// $Revision: 24 $
// $Workfile: MIL_RealObjectType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_RealObjectType.h"

#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "MIL_RealObjectTypeFilter.h"

#include "MIL_FosseAntiChar.h"
#include "MIL_Abattis.h"
#include "MIL_Barricade.h"
#include "MIL_BouchonMines.h"
#include "MIL_ZoneMineeLineaire.h"
#include "MIL_ZoneMineeParDispersion.h"
#include "MIL_Eboulement.h"
#include "MIL_DestructionRoute.h"
#include "MIL_DestructionPont.h"
#include "MIL_FloatingBridgeContinuous.h"
#include "MIL_FloatingBridgeDiscontinuous.h"
#include "MIL_PosteTir.h"
#include "MIL_ZoneProtegee.h"
#include "MIL_ZoneImplantationCanon.h"
#include "MIL_ZoneImplantationCOBRA.h"
#include "MIL_ZoneImplantationLRM.h"
#include "MIL_SiteFranchissement.h"
#include "MIL_NuageNBC.h"
#include "MIL_SiteDecontamination.h"
#include "MIL_PlotRavitaillement.h"
#include "MIL_Rota.h"
#include "MIL_ZoneNBC.h"
#include "MIL_ZoneBrouillageBromure.h"
#include "MIL_ZoneBrouillageBrod.h"
#include "MIL_AirePoser.h"
#include "MIL_Piste.h"
#include "MIL_PlateForme.h"
#include "MIL_ZoneMobiliteAmelioree.h"
#include "MIL_ZonePoserHelicoptere.h"
#include "MIL_AireLogistique.h"
#include "MIL_CampPrisonniers.h"
#include "MIL_CampRefugies.h"
#include "MIL_ItineraireLogistique.h"
#include "MIL_PosteControle.h"
#include "MIL_TerrainLargage.h"
#include "MIL_ZoneForbiddenFire.h"
#include "MIL_ZoneForbiddenMove.h"
#include "MIL_ZoneImplantationMortier.h"
#include "MIL_Installation.h"
#include "MIL_Fire.h"
#include "MIL_EmergencyShelter.h"
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"
#include "xeumeuleu/xml.h"



MIL_RealObjectType::T_ObjectTypeMap     MIL_RealObjectType::objectTypes_;
MIL_RealObjectTypeFilter*               MIL_RealObjectType::pDangerousObjectTypes_ = 0;

const MIL_RealObjectType& MIL_RealObjectType::fosseAntiChar_            = *new MIL_RealObjectType( "fosse anti char"           , eObjectTypeFosseAntiChar          , EnumObjectType::fosse_anti_char          , &MIL_FosseAntiChar              ::Create );
const MIL_RealObjectType& MIL_RealObjectType::abattis_                  = *new MIL_RealObjectType( "abattis"                   , eObjectTypeAbattis                , EnumObjectType::abattis                  , &MIL_Abattis                    ::Create );
const MIL_RealObjectType& MIL_RealObjectType::barricade_                = *new MIL_RealObjectType( "barricade"                 , eObjectTypeBarricade              , EnumObjectType::barricade                , &MIL_Barricade                  ::Create );
const MIL_RealObjectType& MIL_RealObjectType::bouchonMines_             = *new MIL_RealObjectType( "bouchon mines"             , eObjectTypeBouchonMines           , EnumObjectType::bouchon_mines            , &MIL_BouchonMines               ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneMineeLineaire_        = *new MIL_RealObjectType( "zone minee lineaire"       , eObjectTypeZoneMineeLineaire      , EnumObjectType::zone_minee_lineaire      , &MIL_ZoneMineeLineaire          ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneMineeParDispersion_   = *new MIL_RealObjectType( "zone minee par dispersion" , eObjectTypeZoneMineeParDispersion , EnumObjectType::zone_minee_par_dispersion, &MIL_ZoneMineeParDispersion     ::Create );
const MIL_RealObjectType& MIL_RealObjectType::eboulement_               = *new MIL_RealObjectType( "eboulement"                , eObjectTypeEboulement             , EnumObjectType::eboulement               , &MIL_Eboulement                 ::Create );
const MIL_RealObjectType& MIL_RealObjectType::destructionRoute_         = *new MIL_RealObjectType( "destruction route"         , eObjectTypeDestructionRoute       , EnumObjectType::destruction_route        , &MIL_DestructionRoute           ::Create );
const MIL_RealObjectType& MIL_RealObjectType::destructionPont_          = *new MIL_RealObjectType( "destruction pont"          , eObjectTypeDestructionPont        , EnumObjectType::destruction_pont         , &MIL_DestructionPont            ::Create );
const MIL_RealObjectType& MIL_RealObjectType::pontFlottantContinu_      = *new MIL_RealObjectType( "pont flottant continu"     , eObjectTypePontFlottantContinu    , EnumObjectType::pont_flottant_continu    , &MIL_FloatingBridgeContinuous   ::Create );
const MIL_RealObjectType& MIL_RealObjectType::pontFlottantDiscontinu_   = *new MIL_RealObjectType( "pont flottant discontinu"  , eObjectTypePontFlottantDiscontinu , EnumObjectType::pont_flottant_discontinu , &MIL_FloatingBridgeDiscontinuous::Create );
const MIL_RealObjectType& MIL_RealObjectType::posteTir_                 = *new MIL_RealObjectType( "poste tir"                 , eObjectTypePosteTir               , EnumObjectType::poste_tir                , &MIL_PosteTir                   ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneProtegee_             = *new MIL_RealObjectType( "zone protegee"             , eObjectTypeZoneProtegee           , EnumObjectType::zone_protegee            , &MIL_ZoneProtegee               ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationCanon_    = *new MIL_RealObjectType( "zone implantation canon"   , eObjectTypeZoneImplantationCanon  , EnumObjectType::zone_implantation_canon  , &MIL_ZoneImplantationCanon      ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationCOBRA_    = *new MIL_RealObjectType( "zone implantation cobra"   , eObjectTypeZoneImplantationCOBRA  , EnumObjectType::zone_implantation_cobra  , &MIL_ZoneImplantationCOBRA      ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationLRM_      = *new MIL_RealObjectType( "zone implantation lrm"     , eObjectTypeZoneImplantationLRM    , EnumObjectType::zone_implantation_lrm    , &MIL_ZoneImplantationLRM        ::Create );
const MIL_RealObjectType& MIL_RealObjectType::siteFranchissement_       = *new MIL_RealObjectType( "site franchissement"       , eObjectTypeSiteFranchissement     , EnumObjectType::site_franchissement      , &MIL_SiteFranchissement         ::Create );
const MIL_RealObjectType& MIL_RealObjectType::nuageNBC_                 = *new MIL_RealObjectType( "nuage nbc"                 , eObjectTypeNuageNBC               , EnumObjectType::nuage_nbc                , &MIL_NuageNBC                   ::Create );
const MIL_RealObjectType& MIL_RealObjectType::siteDecontamination_      = *new MIL_RealObjectType( "site decontamination"      , eObjectTypeSiteDecontamination    , EnumObjectType::site_decontamination     , &MIL_SiteDecontamination        ::Create );
const MIL_RealObjectType& MIL_RealObjectType::plotRavitaillement_       = *new MIL_RealObjectType( "plot ravitaillement"       , eObjectTypePlotRavitaillement     , EnumObjectType::plot_ravitaillement      , &MIL_PlotRavitaillement         ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneBrouillageBrod_       = *new MIL_RealObjectType( "zone brouillage brod"      , eObjectTypeZoneBrouillageBrod     , EnumObjectType::zone_brouillage_brod     , &MIL_ZoneBrouillageBrod         ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneBrouillageBromure_    = *new MIL_RealObjectType( "zone brouillage bromure"   , eObjectTypeZoneBrouillageBromure  , EnumObjectType::zone_brouillage_bromure  , &MIL_ZoneBrouillageBromure      ::Create );
const MIL_RealObjectType& MIL_RealObjectType::rota_                     = *new MIL_RealObjectType( "rota"                      , eObjectTypeRota                   , EnumObjectType::rota                     , &MIL_Rota                       ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneNBC_                  = *new MIL_RealObjectType( "zone NBC"                  , eObjectTypeZoneNBC                , EnumObjectType::zone_nbc                 , &MIL_ZoneNBC                    ::Create );
const MIL_RealObjectType& MIL_RealObjectType::airePoser_                = *new MIL_RealObjectType( "aire poser"                , eObjectTypeAirePoser              , EnumObjectType::aire_poser               , &MIL_AirePoser                  ::Create );
const MIL_RealObjectType& MIL_RealObjectType::piste_                    = *new MIL_RealObjectType( "piste"                     , eObjectTypePiste                  , EnumObjectType::piste                    , &MIL_Piste                      ::Create );
const MIL_RealObjectType& MIL_RealObjectType::plateForme_               = *new MIL_RealObjectType( "plateforme"                , eObjectTypePlateForme             , EnumObjectType::plateforme               , &MIL_PlateForme                 ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneMobiliteAmelioree_    = *new MIL_RealObjectType( "zone mobilite amelioree"   , eObjectTypeZoneMobiliteAmelioree  , EnumObjectType::zone_mobilite_amelioree  , &MIL_ZoneMobiliteAmelioree      ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zonePoserHelicoptere_     = *new MIL_RealObjectType( "zone poser helicoptere"    , eObjectTypeZonePoserHelicoptere   , EnumObjectType::zone_poser_helicoptere   , &MIL_ZonePoserHelicoptere       ::Create );
const MIL_RealObjectType& MIL_RealObjectType::aireLogistique_           = *new MIL_RealObjectType( "aire logistique"           , eObjectTypeAireLogistique         , EnumObjectType::aire_logistique          , &MIL_AireLogistique             ::Create );
const MIL_RealObjectType& MIL_RealObjectType::campPrisonniers_          = *new MIL_RealObjectType( "camp prisonniers"          , eObjectTypeCampPrisonniers        , EnumObjectType::camp_prisonniers         , &MIL_CampPrisonniers            ::Create );
const MIL_RealObjectType& MIL_RealObjectType::campRefugies_             = *new MIL_RealObjectType( "camp refugies"             , eObjectTypeCampRefugies           , EnumObjectType::camp_refugies            , &MIL_CampRefugies               ::Create );
const MIL_RealObjectType& MIL_RealObjectType::itineraireLogistique_     = *new MIL_RealObjectType( "itineraire logistique"     , eObjectTypeItineraireLogistique   , EnumObjectType::itineraire_logistique    , &MIL_ItineraireLogistique       ::Create );
const MIL_RealObjectType& MIL_RealObjectType::posteControle_            = *new MIL_RealObjectType( "poste controle"            , eObjectTypePosteControle          , EnumObjectType::poste_controle           , &MIL_PosteControle              ::Create );
const MIL_RealObjectType& MIL_RealObjectType::terrainLargage_           = *new MIL_RealObjectType( "terrain largage"           , eObjectTypeTerrainLargage         , EnumObjectType::terrain_largage          , &MIL_TerrainLargage             ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneForbiddenFire_        = *new MIL_RealObjectType( "zone interdite tir"        , eObjectTypeZoneForbiddenFire      , EnumObjectType::zone_interdite_tir       , &MIL_ZoneForbiddenFire          ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneForbiddenMove_        = *new MIL_RealObjectType( "zone interdite mouvement"  , eObjectTypeZoneForbiddenMove      , EnumObjectType::zone_interdite_mouvement , &MIL_ZoneForbiddenMove          ::Create );
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationMortier_  = *new MIL_RealObjectType( "zone implantation mortier" , eObjectTypeZoneImplantationMortier, EnumObjectType::zone_implantation_mortier, &MIL_ZoneImplantationMortier    ::Create );
const MIL_RealObjectType& MIL_RealObjectType::installation_             = *new MIL_RealObjectType( "installation"              , eObjectTypeInstallation           , EnumObjectType::installation             , &MIL_Installation               ::Create );
const MIL_RealObjectType& MIL_RealObjectType::fire_                     = *new MIL_RealObjectType( "fire"                      , eObjectTypeFire                   , EnumObjectType::fire                     , &MIL_Fire                       ::Create );
const MIL_RealObjectType& MIL_RealObjectType::emergencyShelter_         = *new MIL_RealObjectType( "emergency shelter"         , eObjectTypeEmergencyShelter       , EnumObjectType::emergency_shelter        , &MIL_EmergencyShelter           ::Create );

// =============================================================================
// MANAGER
// =============================================================================

struct MIL_RealObjectType::LoadingWrapper
{
    void ReadObject( xml::xistream& xis )
    {
        MIL_RealObjectType::ReadObject( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_RealObjectType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing real object types" );

    objectTypes_[ fosseAntiChar_          .GetID() ] = &fosseAntiChar_          ;
    objectTypes_[ abattis_                .GetID() ] = &abattis_                ;
    objectTypes_[ barricade_              .GetID() ] = &barricade_              ;
    objectTypes_[ bouchonMines_           .GetID() ] = &bouchonMines_           ;
    objectTypes_[ zoneMineeLineaire_      .GetID() ] = &zoneMineeLineaire_      ;
    objectTypes_[ zoneMineeParDispersion_ .GetID() ] = &zoneMineeParDispersion_ ;
    objectTypes_[ eboulement_             .GetID() ] = &eboulement_             ;
    objectTypes_[ destructionRoute_       .GetID() ] = &destructionRoute_       ;
    objectTypes_[ destructionPont_        .GetID() ] = &destructionPont_        ;
    objectTypes_[ pontFlottantContinu_    .GetID() ] = &pontFlottantContinu_    ;
    objectTypes_[ pontFlottantDiscontinu_ .GetID() ] = &pontFlottantDiscontinu_ ;
    objectTypes_[ posteTir_               .GetID() ] = &posteTir_               ;
    objectTypes_[ zoneProtegee_           .GetID() ] = &zoneProtegee_           ;
    objectTypes_[ zoneImplantationCanon_  .GetID() ] = &zoneImplantationCanon_  ;
    objectTypes_[ zoneImplantationCOBRA_  .GetID() ] = &zoneImplantationCOBRA_  ;
    objectTypes_[ zoneImplantationLRM_    .GetID() ] = &zoneImplantationLRM_    ;
    objectTypes_[ siteFranchissement_     .GetID() ] = &siteFranchissement_     ;
    objectTypes_[ nuageNBC_               .GetID() ] = &nuageNBC_               ;
    objectTypes_[ siteDecontamination_    .GetID() ] = &siteDecontamination_    ;
    objectTypes_[ plotRavitaillement_     .GetID() ] = &plotRavitaillement_     ;
    objectTypes_[ zoneBrouillageBrod_     .GetID() ] = &zoneBrouillageBrod_     ;
    objectTypes_[ zoneBrouillageBromure_  .GetID() ] = &zoneBrouillageBromure_  ;
    objectTypes_[ rota_                   .GetID() ] = &rota_                   ;
    objectTypes_[ zoneNBC_                .GetID() ] = &zoneNBC_                ;
    objectTypes_[ airePoser_              .GetID() ] = &airePoser_              ;
    objectTypes_[ piste_                  .GetID() ] = &piste_                  ;
    objectTypes_[ plateForme_             .GetID() ] = &plateForme_             ;
    objectTypes_[ zoneMobiliteAmelioree_  .GetID() ] = &zoneMobiliteAmelioree_  ;
    objectTypes_[ zonePoserHelicoptere_   .GetID() ] = &zonePoserHelicoptere_   ;
    objectTypes_[ aireLogistique_         .GetID() ] = &aireLogistique_         ;
    objectTypes_[ campPrisonniers_        .GetID() ] = &campPrisonniers_        ;
    objectTypes_[ campRefugies_           .GetID() ] = &campRefugies_           ;
    objectTypes_[ itineraireLogistique_   .GetID() ] = &itineraireLogistique_   ;
    objectTypes_[ posteControle_          .GetID() ] = &posteControle_          ;
    objectTypes_[ terrainLargage_         .GetID() ] = &terrainLargage_         ;
    objectTypes_[ zoneForbiddenFire_      .GetID() ] = &zoneForbiddenFire_      ;
    objectTypes_[ zoneForbiddenMove_      .GetID() ] = &zoneForbiddenMove_      ;
    objectTypes_[ zoneImplantationMortier_.GetID() ] = &zoneImplantationMortier_;
    objectTypes_[ installation_           .GetID() ] = &installation_;
    objectTypes_[ fire_                   .GetID() ] = &fire_;
    objectTypes_[ emergencyShelter_       .GetID() ] = &emergencyShelter_;

    pDangerousObjectTypes_ = new MIL_RealObjectTypeFilter();
    LoadingWrapper loader;

    xis >> xml::start( "objects" )
            >> xml::start( "real-objects" )
            >> xml::list( "object", loader, &LoadingWrapper::ReadObject )
            >> xml::end()
        >> xml::end();

    // Post check
    for( CIT_ObjectTypeMap itType = objectTypes_.begin(); itType != objectTypes_.end(); ++itType )
        if( !itType->second->IsInitialized() )
            xis.error( "Object type '" + itType->second->GetName() + " ' not initialized" );

}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadObject
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ReadObject( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );

    const MIL_RealObjectType* pType = Find( strType );
    if( !pType )
        xis.error( "Unknown object type" );

    const_cast< MIL_RealObjectType* >( pType )->Read( xis );

}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_RealObjectType::Terminate()
{
    delete pDangerousObjectTypes_;
    pDangerousObjectTypes_ = 0;
    objectTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_RealObjectType::MIL_RealObjectType( const std::string& strName, E_ObjectType nType, ASN1T_EnumObjectType nAsnID, T_ObjectInstanciator objectInstanciator )
    : nType_                           ( nType )
    , nAsnID_                          ( nAsnID )
    , objectInstanciator_              ( objectInstanciator )
    , strName_                         ( strName )
    , bCanBeReservedObstacle_          ( false )
    , bCanBeMined_                     ( false )
    , bCanBeBypassed_                  ( false )
    , rAvoidanceDistance_              ( 0. )
    , rDefaultSpeedWhenBypassed_       ( 0. )
    , rDefaultSpeedWhenNotBypassed_    ( 0. )
    , nSpeedPolicy_                    ( eSpeedPolicy_Slowest )
    , rSpeedPolicyMaxSpeedAgentFactor_ ( 1. )
    , rMaxInteractionHeight_           ( 0. )
    , bInitialized_                    ( false )
    , pionAttritions_                  (  )
    , pDotationCategoryForConstruction_( 0 )
    , nNbrDotationForConstruction_     ( 0 )
    , pDotationCategoryForMining_      ( 0 )
    , nNbrDotationForMining_           ( 0 )
    , pDefaultConsumptionMode_         ( 0 )
    , nNbrMaxAnimators_                ( 0 )
    , rExitingPopulationDensity_       ( std::numeric_limits< MT_Float >::max() )
    , pIDManager_                      ()
    , rPopulationAttritionPH_          ( 0. )
    , rPopulationAttritionSurface_     ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializePionAttritionData
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializePionAttritionData( xml::xistream& xis )
{
    xis >> xml::list( "unit-attritions", *this, &MIL_RealObjectType::ListAttrition );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ListAttrition
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ListAttrition( xml::xistream& xis )
{
    xis >> xml::list( "unit-attrition", *this, &MIL_RealObjectType::ReadAttrition );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadAttrition
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ReadAttrition( xml::xistream& xis )
{
    pionAttritions_.resize( PHY_Protection::GetProtections().size() );
    const PHY_Protection::T_ProtectionMap& protections = PHY_Protection::GetProtections();
    std::string protectionType;

    xis >> xml::attribute( "protection", protectionType );

    PHY_Protection::CIT_ProtectionMap it = protections.find( protectionType );

    if( it == protections.end() )
        xis.error( "invalid protection name" );

    const PHY_Protection& protection = *it->second;

    assert( pionAttritions_.size() > protection.GetID() );
    pionAttritions_[ protection.GetID() ] = PHY_AttritionData( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializePopulationAttritionData
// Created: NLD 2006-04-24
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializePopulationAttritionData( xml::xistream& xis )
{
    xis >> xml::list( "population-attrition", *this, &MIL_RealObjectType::ReadPopulation );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadPopulation
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ReadPopulation( xml::xistream& xis )
{
    rPopulationAttritionPH_      = 0.;
    rPopulationAttritionSurface_ = 0.;

    xis >> xml::attribute( "surface", rPopulationAttritionSurface_ )
        >> xml::attribute( "ph", rPopulationAttritionPH_ );

    if( rPopulationAttritionPH_ <= 0. )
        xis.error( "population-attrition: ph <= 0" );
    if( rPopulationAttritionSurface_ <= 0. )
        xis.error( "population-attrition: surface <= 0" );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeDotation
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeDotation( xml::xistream& xis, const std::string& strSection, const PHY_DotationCategory*& pDotationCategory, uint& nDotationValue ) const
{
    xis >> xml::list( strSection, *this, &MIL_RealObjectType::ReadValorizationOrConstruction, nDotationValue, pDotationCategory );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadValorizationOrConstruction
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ReadValorizationOrConstruction( xml::xistream& xis, uint& nDotationValue, const PHY_DotationCategory*& pDotationCategory ) const
{
    std::string strDotationType;
    std::string strDotationCategory;

    xis >> xml::attribute( "category", strDotationType )
        >> xml::attribute( "dotation", strDotationCategory );

    const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
    if( !pDotationType )
        xis.error( "Unknown dotation type" );
    pDotationCategory = pDotationType->FindDotationCategory( strDotationCategory );
    if( !pDotationCategory )
        xis.error( "Unknown dotation category" );

    xis >> xml::attribute( "count", nDotationValue );

    if( nDotationValue < 0 )
        xis.error( "dotations: count < 0" );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeDotations
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeDotations( xml::xistream& xis )
{
    xis >> xml::list( "dotations", *this, &MIL_RealObjectType::ReadInitializeDotation );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadInitializeDotation
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ReadInitializeDotation( xml::xistream& xis )
{
    InitializeDotation( xis, "construction", pDotationCategoryForConstruction_, nNbrDotationForConstruction_ );
    InitializeDotation( xis, "valorization", pDotationCategoryForMining_      , nNbrDotationForMining_       );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializePlacementScores
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializePlacementScores( xml::xistream& xis )
{
     // Placement scores
    xis >> xml::optional()
            >> xml::start( "sensible-positions" )
                >> xml::list( "terrain", *this, &MIL_RealObjectType::ReadTerrain )
            >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ReadTerrain
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void MIL_RealObjectType::ReadTerrain( xml::xistream& xis )
{
    std::string strTerrainType;
    uint nScore = 0;

    xis >> xml::attribute( "terrain", strTerrainType )
        >> xml::attribute( "value", nScore );

    TerrainData nLandType = MIL_Tools::ConvertLandType( strTerrainType );
    if( nLandType.Area() == 0xFF )
        xis.error( "Unknown land type" );

    environmentScores_.push_back( std::make_pair( nLandType, nScore ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeSpeedData
// Created: NLD 2005-02-17
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeSpeedData( xml::xistream& xis )
{
    xis >> xml::attribute( "default-speed", rDefaultSpeedWhenNotBypassed_ )
        >> xml::attribute( "default-bypassed-speed", rDefaultSpeedWhenBypassed_ );

    if( rDefaultSpeedWhenNotBypassed_ >= 0. )
        rDefaultSpeedWhenNotBypassed_ = MIL_Tools::ConvertSpeedMosToSim( rDefaultSpeedWhenNotBypassed_ );
    else
        rDefaultSpeedWhenNotBypassed_ = std::numeric_limits< MT_Float >::max();

    if( rDefaultSpeedWhenBypassed_ >= 0. )
        rDefaultSpeedWhenBypassed_ = MIL_Tools::ConvertSpeedMosToSim( rDefaultSpeedWhenBypassed_  );
    else
        rDefaultSpeedWhenBypassed_ = std::numeric_limits< MT_Float >::max();

    std::string strSpeedPolicy;

    xis >> xml::attribute( "unit-speed-impact-mode", strSpeedPolicy );

    if( sCaseInsensitiveEqual()( strSpeedPolicy, "AuPlusLent" ) )
        nSpeedPolicy_ = eSpeedPolicy_Slowest;
    else if( sCaseInsensitiveEqual()( strSpeedPolicy, "VitesseMaxObjet" ) )
        nSpeedPolicy_ = eSpeedPolicy_ObjectMaxSpeed;
    else if( sCaseInsensitiveEqual()( strSpeedPolicy, "VitesseMaxAgent" ) )
    {
        nSpeedPolicy_ = eSpeedPolicy_AgentMaxSpeed;
        xis >> xml::attribute( "max-unit-percentage-speed", rSpeedPolicyMaxSpeedAgentFactor_ );
        if( rSpeedPolicyMaxSpeedAgentFactor_ < 0 || rSpeedPolicyMaxSpeedAgentFactor_ > 100 )
            xis.error( "max-unit-percentage-speed not in [0..100]" );
        rSpeedPolicyMaxSpeedAgentFactor_ /= 100.;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Read
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void MIL_RealObjectType::Read( xml::xistream& xis )
{
    if( bInitialized_ )
        xis.error( "Object types are already initialized" );

    pIDManager_ = MIL_IDManager::FindObjectIDManager( strName_ );
    if( !pIDManager_ )
        xis.error( "Class ID of object '" + strName_ + "' is not initialized" );

    bool bDangerous;
    xis >> xml::attribute( "dangerous", bDangerous );
    if( bDangerous )
        pDangerousObjectTypes_->Set( *this );

    std::string strConsumptionMode;
    xis >> xml::attribute( "default-consumption-mode", strConsumptionMode );
    pDefaultConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( strConsumptionMode );
    if( !pDefaultConsumptionMode_ )
        xis.error( "Unknown consumption type" );

    xis >> xml::attribute( "can-be-maneuver-obstacle", bCanBeReservedObstacle_ )
        >> xml::attribute( "can-be-developed", bCanBeMined_ )
        >> xml::attribute( "can-be-bypassed", bCanBeBypassed_ )
        >> xml::attribute( "max-interaction-height", rMaxInteractionHeight_ );

    rAvoidanceDistance_ = 100.; // $$$$ AGE 2005-03-30:
    xis >> xml::optional()
            >> xml::attribute( "avoid-distance", rAvoidanceDistance_ );
    rAvoidanceDistance_ = MIL_Tools::ConvertMeterToSim( rAvoidanceDistance_ );

    nNbrMaxAnimators_ = 0;
    rExitingPopulationDensity_ = 0;

    xis >> xml::optional()
            >> xml::attribute( "max-animating-units", nNbrMaxAnimators_ )
        >> xml::optional()
            >> xml::attribute( "population-density", rExitingPopulationDensity_ );

    if( nNbrMaxAnimators_ < 0 )
        xis.error( "max-animating-units < 0" );
    if( rExitingPopulationDensity_ < 0 )
        xis.error( "population-density < 0" );

    InitializeSpeedData              ( xis );
    InitializePlacementScores        ( xis );
    InitializePopulationAttritionData( xis );
    InitializePionAttritionData      ( xis );
    InitializeDotations              ( xis );

    bInitialized_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_RealObjectType::~MIL_RealObjectType()
{

}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::GetPionAttritionData
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
const PHY_AttritionData& MIL_RealObjectType::GetPionAttritionData( const PHY_Protection& protection ) const
{
    assert( pionAttritions_.size() > protection.GetID() );
    return pionAttritions_[ protection.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ComputePlacementScore
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
int MIL_RealObjectType::ComputePlacementScore( const MT_Vector2D&, const TerrainData& nPassability ) const
{
    int nScore = -1;
    for( CIT_EnvironmentScoreMap itEnvScore = environmentScores_.begin(); itEnvScore != environmentScores_.end(); ++itEnvScore )
    {
        if( nPassability.ContainsAll( itEnvScore->first ) )
        {
            if( nScore == -1 )
                nScore = 0;
            nScore += itEnvScore->second;
        }
    }
    return nScore;
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::ApplySpeedPolicy
// Created: NLD 2005-02-17
// -----------------------------------------------------------------------------
MT_Float MIL_RealObjectType::ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const
{
    assert( rAgentSpeedWithinObject      <= rAgentMaxSpeed );
    assert( rAgentSpeedWithinEnvironment <= rAgentMaxSpeed );

    switch( nSpeedPolicy_ )
    {
        case eSpeedPolicy_Slowest        : return std::min( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment );
        case eSpeedPolicy_AgentMaxSpeed  : return rAgentMaxSpeed * rSpeedPolicyMaxSpeedAgentFactor_;
        case eSpeedPolicy_ObjectMaxSpeed : return rAgentSpeedWithinObject;
    }
    return 0.;
}
