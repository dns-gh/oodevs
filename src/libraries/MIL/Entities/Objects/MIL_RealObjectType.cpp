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

#include "MIL_pch.h"

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
#include "Tools/MIL_Tools.h"
#include "Tools/MIL_IDManager.h"

MIL_RealObjectType::T_ObjectTypeMap     MIL_RealObjectType::objectTypes_;
MIL_RealObjectTypeFilter*               MIL_RealObjectType::pObjectTypesToAvoid_ = 0;

const MIL_RealObjectType& MIL_RealObjectType::fosseAntiChar_            = *new MIL_RealObjectType( "fosse anti char"           , eObjectTypeFosseAntiChar          , EnumObjectType::fosse_anti_char          , &MIL_FosseAntiChar              ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::abattis_                  = *new MIL_RealObjectType( "abattis"                   , eObjectTypeAbattis                , EnumObjectType::abattis                  , &MIL_Abattis                    ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::barricade_                = *new MIL_RealObjectType( "barricade"                 , eObjectTypeBarricade              , EnumObjectType::barricade                , &MIL_Barricade                  ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::bouchonMines_             = *new MIL_RealObjectType( "bouchon mines"             , eObjectTypeBouchonMines           , EnumObjectType::bouchon_mines            , &MIL_BouchonMines               ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::zoneMineeLineaire_        = *new MIL_RealObjectType( "zone minee lineaire"       , eObjectTypeZoneMineeLineaire      , EnumObjectType::zone_minee_lineaire      , &MIL_ZoneMineeLineaire          ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::zoneMineeParDispersion_   = *new MIL_RealObjectType( "zone minee par dispersion" , eObjectTypeZoneMineeParDispersion , EnumObjectType::zone_minee_par_dispersion, &MIL_ZoneMineeParDispersion     ::Create, eHate   ); 
const MIL_RealObjectType& MIL_RealObjectType::eboulement_               = *new MIL_RealObjectType( "eboulement"                , eObjectTypeEboulement             , EnumObjectType::eboulement               , &MIL_Eboulement                 ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::destructionRoute_         = *new MIL_RealObjectType( "destruction route"         , eObjectTypeDestructionRoute       , EnumObjectType::destruction_route        , &MIL_DestructionRoute           ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::destructionPont_          = *new MIL_RealObjectType( "destruction pont"          , eObjectTypeDestructionPont        , EnumObjectType::destruction_pont         , &MIL_DestructionPont            ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::pontFlottantContinu_      = *new MIL_RealObjectType( "pont flottant continu"     , eObjectTypePontFlottantContinu    , EnumObjectType::pont_flottant_continu    , &MIL_FloatingBridgeContinuous   ::Create, eIgnore );  
const MIL_RealObjectType& MIL_RealObjectType::pontFlottantDiscontinu_   = *new MIL_RealObjectType( "pont flottant discontinu"  , eObjectTypePontFlottantDiscontinu , EnumObjectType::pont_flottant_discontinu , &MIL_FloatingBridgeDiscontinuous::Create, eIgnore );  
const MIL_RealObjectType& MIL_RealObjectType::posteTir_                 = *new MIL_RealObjectType( "poste tir"                 , eObjectTypePosteTir               , EnumObjectType::poste_tir                , &MIL_PosteTir                   ::Create, eIgnore );  
const MIL_RealObjectType& MIL_RealObjectType::zoneProtegee_             = *new MIL_RealObjectType( "zone protegee"             , eObjectTypeZoneProtegee           , EnumObjectType::zone_protegee            , &MIL_ZoneProtegee               ::Create, eIgnore );  
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationCanon_    = *new MIL_RealObjectType( "zone implantation canon"   , eObjectTypeZoneImplantationCanon  , EnumObjectType::zone_implantation_canon  , &MIL_ZoneImplantationCanon      ::Create, eAvoid  );  
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationCOBRA_    = *new MIL_RealObjectType( "zone implantation cobra"   , eObjectTypeZoneImplantationCOBRA  , EnumObjectType::zone_implantation_cobra  , &MIL_ZoneImplantationCOBRA      ::Create, eAvoid  );  
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationLRM_      = *new MIL_RealObjectType( "zone implantation lrm"     , eObjectTypeZoneImplantationLRM    , EnumObjectType::zone_implantation_lrm    , &MIL_ZoneImplantationLRM        ::Create, eAvoid  );  
const MIL_RealObjectType& MIL_RealObjectType::siteFranchissement_       = *new MIL_RealObjectType( "site franchissement"       , eObjectTypeSiteFranchissement     , EnumObjectType::site_franchissement      , &MIL_SiteFranchissement         ::Create, eIgnore );  
const MIL_RealObjectType& MIL_RealObjectType::nuageNBC_                 = *new MIL_RealObjectType( "nuage nbc"                 , eObjectTypeNuageNBC               , EnumObjectType::nuage_nbc                , &MIL_NuageNBC                   ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::siteDecontamination_      = *new MIL_RealObjectType( "site decontamination"      , eObjectTypeSiteDecontamination    , EnumObjectType::site_decontamination     , &MIL_SiteDecontamination        ::Create, eAvoid  );  
const MIL_RealObjectType& MIL_RealObjectType::plotRavitaillement_       = *new MIL_RealObjectType( "plot ravitaillement"       , eObjectTypePlotRavitaillement     , EnumObjectType::plot_ravitaillement      , &MIL_PlotRavitaillement         ::Create, eAvoid  );  
const MIL_RealObjectType& MIL_RealObjectType::zoneBrouillageBrod_       = *new MIL_RealObjectType( "zone brouillage brod"      , eObjectTypeZoneBrouillageBrod     , EnumObjectType::zone_brouillage_brod     , &MIL_ZoneBrouillageBrod         ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::zoneBrouillageBromure_    = *new MIL_RealObjectType( "zone brouillage bromure"   , eObjectTypeZoneBrouillageBromure  , EnumObjectType::zone_brouillage_bromure  , &MIL_ZoneBrouillageBromure      ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::rota_                     = *new MIL_RealObjectType( "rota"                      , eObjectTypeRota                   , EnumObjectType::rota                     , &MIL_Rota                       ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::zoneNBC_                  = *new MIL_RealObjectType( "zone NBC"                  , eObjectTypeZoneNBC                , EnumObjectType::zone_nbc                 , &MIL_ZoneNBC                    ::Create, eHate   );  
const MIL_RealObjectType& MIL_RealObjectType::airePoser_                = *new MIL_RealObjectType( "aire poser"                , eObjectTypeAirePoser              , EnumObjectType::aire_poser               , &MIL_AirePoser                  ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::piste_                    = *new MIL_RealObjectType( "piste"                     , eObjectTypePiste                  , EnumObjectType::piste                    , &MIL_Piste                      ::Create, ePrefer );
const MIL_RealObjectType& MIL_RealObjectType::plateForme_               = *new MIL_RealObjectType( "plateforme"                , eObjectTypePlateForme             , EnumObjectType::plateforme               , &MIL_PlateForme                 ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::zoneMobiliteAmelioree_    = *new MIL_RealObjectType( "zone mobilite amelioree"   , eObjectTypeZoneMobiliteAmelioree  , EnumObjectType::zone_mobilite_amelioree  , &MIL_ZoneMobiliteAmelioree      ::Create, ePrefer );
const MIL_RealObjectType& MIL_RealObjectType::zonePoserHelicoptere_     = *new MIL_RealObjectType( "zone poser helicoptere"    , eObjectTypeZonePoserHelicoptere   , EnumObjectType::zone_poser_helicoptere   , &MIL_ZonePoserHelicoptere       ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::aireLogistique_           = *new MIL_RealObjectType( "aire logistique"           , eObjectTypeAireLogistique         , EnumObjectType::aire_logistique          , &MIL_AireLogistique             ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::campPrisonniers_          = *new MIL_RealObjectType( "camp prisonniers"          , eObjectTypeCampPrisonniers        , EnumObjectType::camp_prisonniers         , &MIL_CampPrisonniers            ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::campRefugies_             = *new MIL_RealObjectType( "camp refugies"             , eObjectTypeCampRefugies           , EnumObjectType::camp_refugies            , &MIL_CampRefugies               ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::itineraireLogistique_     = *new MIL_RealObjectType( "itineraire logistique"     , eObjectTypeItineraireLogistique   , EnumObjectType::itineraire_logistique    , &MIL_ItineraireLogistique       ::Create, eEnjoy  );
const MIL_RealObjectType& MIL_RealObjectType::posteControle_            = *new MIL_RealObjectType( "poste controle"            , eObjectTypePosteControle          , EnumObjectType::poste_controle           , &MIL_PosteControle              ::Create, ePrefer );
const MIL_RealObjectType& MIL_RealObjectType::terrainLargage_           = *new MIL_RealObjectType( "terrain largage"           , eObjectTypeTerrainLargage         , EnumObjectType::terrain_largage          , &MIL_TerrainLargage             ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::zoneForbiddenFire_        = *new MIL_RealObjectType( "zone interdite tir"        , eObjectTypeZoneForbiddenFire      , EnumObjectType::zone_interdite_tir       , &MIL_ZoneForbiddenFire          ::Create, eIgnore );
const MIL_RealObjectType& MIL_RealObjectType::zoneForbiddenMove_        = *new MIL_RealObjectType( "zone interdite mouvement"  , eObjectTypeZoneForbiddenMove      , EnumObjectType::zone_interdite_mouvement , &MIL_ZoneForbiddenMove          ::Create, eHate   );
const MIL_RealObjectType& MIL_RealObjectType::zoneImplantationMortier_  = *new MIL_RealObjectType( "zone implantation mortier" , eObjectTypeZoneImplantationMortier, EnumObjectType::zone_implantation_mortier, &MIL_ZoneImplantationMortier    ::Create, eAvoid  );
const MIL_RealObjectType& MIL_RealObjectType::installation_             = *new MIL_RealObjectType( "installation"              , eObjectTypeInstallation           , EnumObjectType::installation             , &MIL_Installation               ::Create, eIgnore );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_RealObjectType::Initialize( MIL_InputArchive& archive )
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

    pObjectTypesToAvoid_ = new MIL_RealObjectTypeFilter();

    archive.Section  ( "Objets" );
    archive.BeginList( "ObjetsReels" );

    while( archive.NextListElement() )
    {
        archive.Section( "Objet" );

        std::string strType;
        archive.ReadAttribute( "type", strType );

        const MIL_RealObjectType* pType = FindObjectType( strType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown object type", archive.GetContext() );

        const_cast< MIL_RealObjectType* >( pType )->Read( archive );
        
        archive.EndSection(); // Objet
    }
    // Post check
    for( CIT_ObjectTypeMap itType = objectTypes_.begin(); itType != objectTypes_.end(); ++itType )
        if( !itType->second->IsInitialized() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Object type '%s' not initialized", itType->second->GetName().c_str() ), archive.GetContext() );

    archive.EndList   (); // ObjetsReels
    archive.EndSection(); // Objets
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_RealObjectType::Terminate()
{
    delete pObjectTypesToAvoid_;
    pObjectTypesToAvoid_ = 0;
    objectTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_RealObjectType::MIL_RealObjectType( const std::string& strName, E_ObjectType nType, ASN1T_EnumObjectType nAsnID, T_ObjectInstanciator objectInstanciator, E_Behavior nBehavior )
    : nType_                           ( nType )
    , nAsnID_                          ( nAsnID )
    , objectInstanciator_              ( objectInstanciator )
    , strName_                         ( strName )
    , bCanBePrepared_                  ( false ) 
    , bCanBeMined_                     ( false )
    , bCanBeBypassed_                  ( false )
    , rAvoidanceDistance_              ( 0. )
    , rDefaultSpeedWhenBypassed_       ( 0. )
    , rDefaultSpeedWhenNotBypassed_    ( 0. )
    , nSpeedPolicy_                    ( eSpeedPolicy_Slowest )
    , rSpeedPolicyMaxSpeedAgentFactor_ ( 1. )
    , rMaxInteractionHeight_           ( 0. )
    , bInitialized_                    ( false )
    , attritions_                      (  )
    , pDotationCategoryForConstruction_( 0 )
    , pDotationCategoryForMining_      ( 0 )
    , pDefaultConsumptionMode_         ( 0 )
    , nNbrMaxAnimators_                ( 0 )
    , rExitingPopulationDensity_       ( std::numeric_limits< MT_Float >::max() )
    , pIDManager_                      ()
    , nBehavior_                       ( nBehavior )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeAttritions
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeAttritions( MIL_InputArchive& archive )
{
    attritions_.resize( PHY_Protection::GetProtections().size() );
    
    if ( !archive.Section( "Attritions", MIL_InputArchive::eNothing ) )
        return;

    const PHY_Protection::T_ProtectionMap& protections = PHY_Protection::GetProtections();
    for ( PHY_Protection::CIT_ProtectionMap itProtection = protections.begin(); itProtection != protections.end(); ++itProtection )
    {
        const PHY_Protection& protection = *itProtection->second;

        std::stringstream strSectionName;
        strSectionName << "ProtectionType" << protection.GetName();

        archive.Section( strSectionName.str() );

        assert( attritions_.size() > protection.GetID() );
        attritions_[ protection.GetID() ] = PHY_AttritionData( archive );

        archive.EndSection(); // ProtectionTypeXXX
    }
    archive.EndSection(); // Attritions
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeDotation
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeDotation( MIL_InputArchive& archive, const std::string& strSection, const PHY_DotationCategory*& pDotationCategory ) const
{
    pDotationCategory = 0;
    if ( !archive.Section( strSection, MIL_InputArchive::eNothing ) )
        return;

    std::string strDotationType;
    std::string strDotationCategory;
    
    archive.ReadAttribute( "type", strDotationType );
    archive.ReadAttribute( "categorie", strDotationCategory );

    const PHY_DotationType* pDotationType = PHY_DotationType::FindDotationType( strDotationType );
    if( !pDotationType )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation type", archive.GetContext() );
    pDotationCategory = pDotationType->FindDotationCategory( strDotationCategory );
    if( !pDotationCategory )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown dotation category", archive.GetContext() );

    archive.EndSection(); // strSection
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeDotations
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeDotations( MIL_InputArchive& archive )
{
if( !archive.Section( "Dotations", MIL_InputArchive::eNothing ) )
        return;

    InitializeDotation( archive, "Construction", pDotationCategoryForConstruction_ );
    InitializeDotation( archive, "Valorisation", pDotationCategoryForMining_       );

    archive.EndSection(); // Dotations
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializePlacementScores
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializePlacementScores( MIL_InputArchive& archive )
{
     // Placement scores
    if( !archive.BeginList( "PlacementScores", MIL_InputArchive::eNothing ) )
        return;

    while ( archive.NextListElement() )
    {
        std::string strTerrainType;

        archive.Section( "Terrain" );
        archive.ReadAttribute( "type", strTerrainType );

        TerrainData nLandType = MIL_Tools::ConvertLandType( strTerrainType );
        if( nLandType.Area() == 0xFF )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown land type", archive.GetContext() );

        uint nScore = 0;
        archive.Read( nScore );
        environmentScores_.push_back( std::make_pair( nLandType, nScore ) );
        archive.EndSection(); // Terrain
    }
    archive.EndList();
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::InitializeSpeedData
// Created: NLD 2005-02-17
// -----------------------------------------------------------------------------
void MIL_RealObjectType::InitializeSpeedData( MIL_InputArchive& archive )
{
    archive.ReadField( "VitesseParDefaut", rDefaultSpeedWhenNotBypassed_ );
    if ( rDefaultSpeedWhenNotBypassed_ >= 0. )
        rDefaultSpeedWhenNotBypassed_ = MIL_Tools::ConvertSpeedMosToSim( rDefaultSpeedWhenNotBypassed_ );
    else
        rDefaultSpeedWhenNotBypassed_ = std::numeric_limits< MT_Float >::max();

    archive.ReadField( "VitesseParDefautQuandContourne", rDefaultSpeedWhenBypassed_ );
    if ( rDefaultSpeedWhenBypassed_ >= 0. )
        rDefaultSpeedWhenBypassed_ = MIL_Tools::ConvertSpeedMosToSim( rDefaultSpeedWhenBypassed_  );
    else
        rDefaultSpeedWhenBypassed_ = std::numeric_limits< MT_Float >::max();

    std::string strSpeedPolicy;

    archive.Section( "ModeImpactSurVitessePion" );
    archive.ReadAttribute( "type", strSpeedPolicy );
    if( sCaseInsensitiveEqual()( strSpeedPolicy, "AuPlusLent" ) )
        nSpeedPolicy_ = eSpeedPolicy_Slowest;
    else if( sCaseInsensitiveEqual()( strSpeedPolicy, "VitesseMaxObjet" ) )
        nSpeedPolicy_ = eSpeedPolicy_ObjectMaxSpeed;
    else if( sCaseInsensitiveEqual()( strSpeedPolicy, "VitesseMaxAgent" ) )
    {
        nSpeedPolicy_ = eSpeedPolicy_AgentMaxSpeed;
        archive.ReadAttribute( "pourcentageVitesseMaxAgent", rSpeedPolicyMaxSpeedAgentFactor_, CheckValueBound( 0., 100. ) );
        rSpeedPolicyMaxSpeedAgentFactor_ /= 100.;
    }
    
    archive.EndSection(); // ModeImpactSurVitessePion
}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType::Read
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void MIL_RealObjectType::Read( MIL_InputArchive& archive )
{
    if( bInitialized_ ) 
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Object types are already initialized" );

    if( nBehavior_ == eHate || nBehavior_ == eAvoid )
        pObjectTypesToAvoid_->Set( *this );

    pIDManager_ = MIL_IDManager::FindObjectIDManager( strName_ );
    if( !pIDManager_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "Class ID of object '%s' is not initialized", strName_.c_str() ) );

    std::string strConsumptionMode;
    archive.ReadField( "ModeConsommationParDefaut", strConsumptionMode );
    pDefaultConsumptionMode_ = PHY_ConsumptionType::FindConsumptionType( strConsumptionMode );
    if( !pDefaultConsumptionMode_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown consumption type", archive.GetContext() );

    archive.ReadField( "PeutEtrePrepare"  , bCanBePrepared_ );
    archive.ReadField( "PeutEtreValorise" , bCanBeMined_    );
    archive.ReadField( "PeutEtreContourne", bCanBeBypassed_ );
    
    archive.ReadField( "HauteurMaxInteraction", rMaxInteractionHeight_ );

    rAvoidanceDistance_ = 100.; // $$$$ AGE 2005-03-30: 
    archive.ReadField( "DistanceEvitement", rAvoidanceDistance_, MIL_InputArchive::eNothing );
    rAvoidanceDistance_ = MIL_Tools::ConvertMeterToSim( rAvoidanceDistance_ );

    archive.ReadField( "NombreAnimateursMax"      , nNbrMaxAnimators_         , CheckValueGreaterOrEqual( 0  ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );
    archive.ReadField( "DensitePopulationSortante", rExitingPopulationDensity_, CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    InitializeSpeedData      ( archive );
    InitializePlacementScores( archive );
    InitializeAttritions     ( archive );
    InitializeDotations      ( archive );

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
// Name: MIL_RealObjectType::GetAttritionData
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
const PHY_AttritionData& MIL_RealObjectType::GetAttritionData( const PHY_Protection& protection ) const
{
    assert( attritions_.size() > protection.GetID() );
    return attritions_[ protection.GetID() ];
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
