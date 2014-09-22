// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RolePion_Decision.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Entities/Agents/Actions/Moving/PHY_ActionMove.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.h"

BOOST_CLASS_EXPORT_IMPLEMENT( sword::legacy::RolePion_Decision )

using namespace sword::legacy;

namespace sword
{
namespace legacy
{
    template< typename Archive >
    void save_construct_data( Archive& archive, const sword::legacy::RolePion_Decision* role, const unsigned int /*version*/ )
    {
        archive << role->pEntity_;
        archive << role->gcPause_;
        archive << role->gcMult_;
        archive << role->logger_;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, sword::legacy::RolePion_Decision* role, const unsigned int /*version*/ )
    {
        MIL_AgentPion* pion;
        unsigned int gcPause;
        unsigned int gcMult;
        sword::DEC_Logger* logger;
        archive >> pion;
        archive >> gcPause;
        archive >> gcMult;
        archive >> logger;
        ::new( role )sword::legacy::RolePion_Decision( *pion, gcPause, gcMult, logger );
    }
}
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision constructor
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
RolePion_Decision::RolePion_Decision( MIL_AgentPion& pion, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : DEC_RolePion_Decision( pion, gcPause, gcMult, logger )
{
    RegisterFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision destructor
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
RolePion_Decision::~RolePion_Decision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::Reload
// Created: SLI 2012-07-16
// -----------------------------------------------------------------------------
void RolePion_Decision::Reload( bool force, bool doInitBrain )
{
    DEC_RolePion_Decision::Reload( force, doInitBrain );
    RegisterFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::load
// Created: SLI 2012-07-17
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Decision::load( Archive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< DEC_RolePion_Decision >( *this );
    RegisterFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::save
// Created: SLI 2012-07-17
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Decision::save( Archive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< DEC_RolePion_Decision >( *this );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterControlActions
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterControlActions()
{
    RegisterFunction( "DEC__StopAction",
        std::function< unsigned int( unsigned int) >( boost::bind( &DEC_ActionFunctions::StopAction< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_PauseAction",
        std::function< void( unsigned int ) >( boost::bind( &DEC_ActionFunctions::SuspendAction< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ReprendAction",
        std::function< void( unsigned int ) >( boost::bind( &DEC_ActionFunctions::ResumeAction < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_EtatAction",
        std::function< unsigned int( unsigned int ) >( boost::bind( &DEC_ActionFunctions::GetActionState < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterActions
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterActions()
{
    RegisterFunction( "DEC_StartDeplacement",
        std::function< unsigned int( boost::shared_ptr< DEC_Path_ABC > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMove, boost::shared_ptr< DEC_Path_ABC > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartMovementSuspended",
        std::function< unsigned int( boost::shared_ptr< DEC_Path_ABC > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMove, boost::shared_ptr< DEC_Path_ABC >, bool >, boost::ref( GetPion() ), _1, true ) ) );
    RegisterFunction( "DEC_Orientate",
        std::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_ActionFunctions::Orientate, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartTirDirect",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePion, boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    RegisterFunction( "DEC_StartTirDirectDebarques",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionUsingOnlyLoadable, boost::shared_ptr< DEC_Knowledge_Agent >, double, int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartTirDirectTransporteurs",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionUsingOnlyCarrier, boost::shared_ptr< DEC_Knowledge_Agent >, double, int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC__StartTirDirectSurComposantesMajeures",
        std::function< unsigned int( int, boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionOnMajorComposantes, int, boost::shared_ptr< DEC_Knowledge_Agent >, double, int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    RegisterFunction( "DEC__StartTirSurPopulation",
        std::function< unsigned int( unsigned int, const std::string& ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePopulation, unsigned int, const std::string& >, boost::ref( GetPion() ), _1, _2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterPath
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterPath()
{
    RegisterFunction( "DEC_CreerItineraire",
        std::function< boost::shared_ptr< DEC_Path_ABC >( MT_Vector2D*, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPoint, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_CreerItineraireBM",
        std::function< boost::shared_ptr< DEC_Path_ABC >( boost::shared_ptr< MT_Vector2D >, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPointBM, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_CreerItineraireListe",
        std::function< boost::shared_ptr< DEC_Path_ABC >( std::vector< boost::shared_ptr< MT_Vector2D > >, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPointList, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Itineraire_EstEnMouvementSur",
        std::function< bool ( const DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::IsMovingOnPath, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Itineraire_ExtrapolerPosition" ,
        std::function< boost::shared_ptr< MT_Vector2D >( const double, bool ) >( boost::bind( &DEC_PathFunctions::ExtrapolatePosition, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_GetNextObjectOnPath",
        std::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( boost::shared_ptr< DEC_Knowledge_Object >, float, const std::vector< std::string >& ) >(
        boost::bind( &DEC_PathFunctions::GetNextObjectOnPath, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_GetNextObjectOnPathWithBypassed",
        std::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( boost::shared_ptr< DEC_Knowledge_Object >, float, const std::vector< std::string >& ) >(
        boost::bind( &DEC_PathFunctions::GetNextObjectOnPathWithBypassed, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Itineraire_Etat",
        std::function< int ( DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::GetPathState, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Itineraire_DernierPoint",
        std::function< boost::shared_ptr< MT_Vector2D >( const DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::GetLastPointOfPath, boost::ref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterRepresentations
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterRepresentations()
{
    RegisterFunction( "DEC_GetPointsCategory", boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_RemoveFromPointsCategory",
        std::function< void( boost::shared_ptr< DEC_PathPoint > )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterPerception
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterPerception()
{
    RegisterFunction( "DEC_Perception_ActiverCoupsDeSonde", boost::bind( &DEC_PerceptionFunctions::EnableCoupDeSonde, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_DesactiverCoupsDeSonde", boost::bind( &DEC_PerceptionFunctions::DisableCoupDeSonde, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_ActiverSenseursSurDecision", boost::bind( &DEC_PerceptionFunctions::EnablePerceptionUponRequest, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_DesactiverSenseursSurDecision", boost::bind( &DEC_PerceptionFunctions::DisablePerceptionUponRequest, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_ActiverSenseurs", boost::bind( &DEC_PerceptionFunctions::EnableSensors, boost::ref( GetPion() ), true ) );
    RegisterFunction( "DEC_Perception_DesactiverSenseurs", boost::bind( &DEC_PerceptionFunctions::EnableSensors, boost::ref( GetPion() ), false ) );
    RegisterFunction( "DEC_Perception_ActiverRadar", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadar, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverRadar", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRadar, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_ActiverRadarSurLocalisation", std::function< int( int, const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadarOnLocalisation, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Perception_ActiverRadarSurPointPtr", std::function< int( int, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadarOnPointPtr, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverRadarSurLocalisation", std::function< void( int, int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRadarOnLocalisation, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Perception_ActiverPerceptionTirsIndirect", std::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableFlyingShellDetection, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverPerceptionTirsIndirect", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableFlyingShellDetection, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_ActiverReconnaissanceLocalisation", std::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionLocalisation, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_ActivateLocationProgressiveRecce", std::function< int( const TER_Localisation*, float ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionLocalisation, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverReconnaissanceLocalisation", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionLocalisation, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_ActiverReconnaissanceDansBlocUrbain", std::function< int( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionUrbanBlock, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverReconnaissanceDansBlocUrbain", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionUrbanBlock, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_ActiverDetectionObjetLocalisation", std::function< int( const TER_Localisation*, const MT_Vector2D*, double ) >( boost::bind( &DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation, boost::ref( *(DEC_Decision_ABC*)this ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverDetectionObjetLocalisation", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_ActiverReconnaissancePoint", std::function< int( MT_Vector2D*, double, double ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionPoint, boost::ref( *(DEC_Decision_ABC*)this ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverReconnaissancePoint", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionPoint, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_VisionVerrouilleeSurDirection", std::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModeDirection, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_VisionVerrouilleeSurPoint", std::function< void( const MT_Vector2D* ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModePoint, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_VisionVerrouilleeSurPointPtr", std::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModePointPtr, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_VisionNormale", boost::bind( &DEC_PerceptionFunctions::SetVisionModeNormal, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_ActiverObserveurTir", boost::bind( &DEC_PerceptionFunctions::EnableFireObserver, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_DesactiverObserveurTir", boost::bind( &DEC_PerceptionFunctions::DisableFireObserver, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_PointEstVisible", std::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_PerceptionFunctions::IsPointVisible, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_IdentifierToutesUnitesDansZone", std::function< void( const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::IdentifyAllAgentsInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetPerception", std::function< double( boost::shared_ptr< MT_Vector2D >, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::GetPerception, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Perception_ActiverModeEnregistrement", boost::bind( &DEC_PerceptionFunctions::EnableRecordMode, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_DesactiverModeEnregistrement", boost::bind( &DEC_PerceptionFunctions::DisableRecordMode, boost::ref( GetPion() ) ) );
    // ALAT
    RegisterFunction( "DEC_ALAT_ActiverReconnaissance", std::function< void( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecoAlat, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ALAT_DesactiverReconnaissance", boost::bind( &DEC_PerceptionFunctions::DisableRecoAlat, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_ALAT_ReconnaissanceNonVuTerminee", boost::bind( &DEC_PerceptionFunctions::HasNoDelayedPeceptions, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_ActiverSurveillance", std::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableSurveillanceLocalisation, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Perception_DesactiverSurveillance", std::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableSurveillanceLocalisation, boost::ref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterFire
// Created: MCO 2012-05-10
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterFire()
{
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUnite",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemy, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float, const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinPourTirerSurUnite",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToFireOnEnemy, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourEtreTireParUnite",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirer",
        std::function< float( float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFire, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxTirIndirect",
        std::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFire, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinTirIndirect",
        std::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToIndirectFire, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions",
        std::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions",
        std::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition",
        boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFire, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Tir_LancerFumigeneSurConnaissance", // $$$$ MCO 2012-06-25: should be an action
        std::function< double( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_MunitionPourTirIndirect",
        std::function< const PHY_DotationCategory* ( int, const MT_Vector2D* ) >( boost::bind( &DEC_FireFunctions::GetMunitionForIndirectFire, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Pion_InterdireMunition",
        std::function< void( const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_FireFunctions::ForbidAmmunition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Pion_AutoriserMunition",
        std::function< void( const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_FireFunctions::AllowAmmunition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Pion_AutoriserToutesMunitions",
        boost::bind( &DEC_FireFunctions::AllowAllAmmunitions, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Tir_PorteeTheoriqueMaxTirIndirect",
        std::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetTheoricMaxRangeToIndirectFire, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeTheoriqueMinTirIndirect",
        std::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetTheoricMinRangeToIndirectFire, boost::ref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterKnowledge
// Created: MCO 2012-05-14
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterKnowledge()
{
    RegisterFunction( "DEC_ConnaissanceAgent_AttritionPotentielle",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetPotentialAttrition, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_Dangerosite",
        std::function< float( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosity, boost::cref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterAgentKnowledgeFunctions
// Created: MCO 2012-05-15
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterAgentKnowledgeFunctions()
{
    RegisterFunction( "DEC_RapportDeForceLocal", boost::bind( &DEC_KnowledgeFunctions::GetRapForLocal, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesDangereuses", boost::bind( &DEC_KnowledgeFunctions::GetDangerousEnemies, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_ConnaissanceBlocUrbain_RapForLocal",
        std::function< float( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetRapForLocal, boost::cref( GetPion() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterFunctions
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterFunctions()
{
    RegisterControlActions();
    RegisterActions();
    RegisterPath();
    RegisterRepresentations();
    RegisterPerception();
    RegisterFire();
    RegisterKnowledge();
    RegisterAgentKnowledgeFunctions();
}
