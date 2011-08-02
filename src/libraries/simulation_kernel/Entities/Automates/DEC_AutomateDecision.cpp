// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/DEC_AutomateDecision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_AutomateDecision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_AutomateDecision.h"
#include "MIL_AutomateType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Decision/DEC_AutomateFunctions.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgePopulationFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_IntelligenceFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "MT_Tools/MT_ScipioException.h"
#include <boost/serialization/vector.hpp>
#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_AutomateDecision )

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision( MIL_Automate& automate, unsigned int gcPause, unsigned int gcMult )
    : DEC_Decision            ( automate, gcMult, gcPause )
    , nRulesOfEngagementState_( eRoeStateNone )
    , nCloseCombatState_      ( eCloseCombatStateNone )
    , nOperationalState_      ( eOpStateOperational )
    , rDestruction_           ( 0 )
    , bStateHasChanged_       ( true )
    , bOrdreAttendre_         ( false )
    , bOrdrePoursuivre_       ( false )
    , bOrdreRalentir_         ( false )
    , bOrdreDecrocher_        ( false )
    , bOrdreTenirSurLR_       ( false )
    , bOrdreTenir_            ( false )
    , fuseau_                 ( 0 )
    , eEtatEchelon_           ( 0 )
    , eEtatDec_               ( 0 )
    , eEtatLima_              ( 0 )
{
    const DEC_Model_ABC& model = automate.GetType().GetModel();
    try
    {
        SetModel( model );
    }
    catch( std::runtime_error& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.what() );
    }
    StartDefaultBehavior(); 
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_AutomateDecision::~DEC_AutomateDecision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this )
         >> pEntity_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_;
    unsigned int nID;
    file >> nID;
    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( nID );
    assert( pType );
    const DEC_Model_ABC& model = pType->GetModel();
    try
    {
        SetModel( model );
    }
    catch( std::runtime_error& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.what() );
    }
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pEntity_ );
    unsigned id = pEntity_->GetType().GetID();
    file << boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this )
         << pEntity_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << id;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::EndCleanStateAfterCrash()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Automate_Decision::RegisterUserArchetypeFunctions
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterUserArchetypeFunctions ( directia::brain::Brain& brain )
{
    // Objets
    brain[ "DEC_ActiverObjet" ] = &DEC_ObjectFunctions::ActivateObject;

    // Connaissance
    brain[ "DEC_IsValidKnowledgeObject" ] = &DEC_KnowledgeObjectFunctions::IsKnowledgeValid;
    brain[ "DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif" ] = &DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated;
    brain[ "DEC_ConnaissanceObjet_EstObstacleDeManoeuvre" ] = &DEC_KnowledgeObjectFunctions::IsReservedObstacle;
    brain[ "DEC_ConnaissanceObjet_EstContourne" ] = &DEC_KnowledgeObjectFunctions::IsBypassed;
    brain[ "DEC_ConnaissanceObjet_Localisation" ] = &DEC_KnowledgeObjectFunctions::GetLocalisation;
    brain[ "DEC_ConnaissanceObjet_Type" ] = &DEC_KnowledgeObjectFunctions::GetType;
    brain[ "DEC_ConnaissanceObjet_LargeurSiteFranchissement" ] = &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth;
    brain[ "DEC_ConnaissanceObjet_EstReconnu" ] = &DEC_KnowledgeObjectFunctions::IsRecon;

    // Geometry
    brain[ "DEC_Geometrie_StopCalculLignesAvantEtArriere" ] = &DEC_GeometryFunctions::StopComputingFrontAndBackLines;
    brain[ "DEC_Geometrie_CalculerDistanceLigneAvant" ] = &DEC_GeometryFunctions::ComputeDistanceFromFrontLine;
    brain[ "DEC_Geometrie_CalculerAutomateDistanceLigneAvant" ] = &DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine;
    brain[ "DEC_Geometrie_CalculerDistanceLigneArriere" ] = &DEC_GeometryFunctions::ComputeDistanceFromBackLine;
    brain[ "DEC_Geometrie_PionDevant" ] = &DEC_GeometryFunctions::GetFrontestPion;
    brain[ "DEC_Geometrie_PionDerriere" ] = &DEC_GeometryFunctions::ComputeBackestAgent;
    brain[ "DEC_Geometrie_CalculerRetard" ] = &DEC_GeometryFunctions::ComputeAutomatDelayFromSchedule;

    // Accesseurs sur les pions
    brain.Register( "DEC_Automate_PionEstNeutralise", &DEC_AutomateDecision::IsPionNeutralized );
    brain[ "DEC_Automate_CalculerPointProcheLocalisationDansFuseauPourPion" ] = &DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau;
    brain[ "DEC_Automate_GetEfficacite" ] = &DEC_AutomateFunctions::GetPionEfficiency;
    brain[ "DEC_Tir_PorteeMaxPourTirer" ] = &DEC_FireFunctions::GetMaxRangeToFireDecision;

    // Objects
    brain[ "DEC_DetruireObjetSansDelais" ] = &DEC_ObjectFunctions::MagicDestroyObject;

    // Former szName_, mission_, automate_:
    brain[ "DEC_GetSzName" ] = &DEC_MiscFunctions::GetName;
    brain[ "DEC_GetAutomate" ] = &DEC_MiscFunctions::GetAutomate;
    brain[ "DEC_GetDirectionEnnemi" ] = &DEC_MiscFunctions::GetDirectionEnnemi;
    brain[ "DEC_Copie_DirectionDanger_Mission" ] = &DEC_MiscFunctions::CopyDirectionDanger;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterUserFunctions( directia::brain::Brain& brain )
{
// Accessors
    brain[ "DEC_Automate_PionsAvecPC" ] = boost::bind( &DEC_AutomateFunctions::GetPionsWithPC, boost::ref( GetAutomate() ) );
    brain[ "DEC_Automate_PionsSansPC" ] = boost::bind( &DEC_AutomateFunctions::GetPionsWithoutPC, boost::ref( GetAutomate() ) );
    brain[ "DEC_Automate_PionPC" ] = boost::bind( &DEC_AutomateFunctions::GetPionPC, boost::ref( GetAutomate() ) );
    brain[ "DEC_Automate_PionPCDeAutomate" ] =
        boost::function< DEC_Decision_ABC*( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::GetPionPCOfAutomate, _1 ) );
    brain[ "DEC_Automate_AutomatesSubordonnes" ] = boost::bind( &DEC_AutomateFunctions::GetAutomates, boost::cref( GetAutomate() ) );
    brain[ "DEC_Automate_PionsMelee" ] = boost::bind( &DEC_AutomateFunctions::GetPionsMelee, boost::cref( GetAutomate() ) );
    brain[ "DEC_AutomateSuperieur_EstEmbraye" ] = boost::bind( &DEC_AutomateFunctions::IsParentAutomateEngaged, boost::cref( GetAutomate() ) );

    // State
    brain[ "DEC_Automate_EstEmbraye" ] = boost::bind( &DEC_AutomateFunctions::IsEngaged, this );

    // Debug
    brain[ "DEC_DecisionalState" ] =
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AutomateFunctions::DecisionalState, boost::ref( GetAutomate() ), _1, _2 ) );

    brain[ "DEC_DebugAffichePoint"   ] =
            boost::function< void ( const MT_Vector2D* ) > ( boost::bind( &DEC_MiscFunctions::DebugDrawPoint< MIL_Automate >, boost::ref( GetAutomate()), _1  ) );
    brain[ "DEC_DebugAffichePoints"  ] =
            boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints< MIL_Automate >, boost::ref( GetAutomate()), _1  ) );
    brain[ "DEC_Debug" ] =
            boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug< MIL_Automate > , boost::ref( GetAutomate()) , "Automate" , _1  ) );
    brain[ "DEC_Trace" ] =
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );

    // Connaissance
    brain[ "DEC_Connaissances_PartageConnaissancesAvec" ] =
        boost::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Connaissances_PartageConnaissancesDansZoneAvec" ] =
        boost::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2, _3 ) );
    brain[ "DEC_Knowledges_ObjectsInCircle" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2, _3 ) );
    brain[ "DEC_ObjectKnowledgesInZone" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Connaissances_ObjetsDansFuseau" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion" ] =
        boost::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Connaissances_Populations" ] = boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_Automate >, boost::cref( GetAutomate() ) );


    // Intelligence
    brain[ "DEC_Rens_PourcentageEnnemisDebarquesDansZone" ] =
        boost::function< float( const TER_Localisation* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeUnloadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_PourcentageEnnemisEmbarquesDansZone" ] =
        boost::function< float( const TER_Localisation* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeLoadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_PourcentageEnnemisDebarquesDansFuseau" ] =
        boost::function< float( const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeFuseauUnloadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_PourcentageEnnemisEmbarquesDansFuseau" ] =
        boost::function< float( const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeFuseauLoadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) );


    brain[ "DEC_Rens_TrierZonesSelonPresenceEnnemisDebarques" ] =
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const std::vector< boost::shared_ptr< TER_Localisation > >& ) >( boost::bind( &DEC_IntelligenceFunctions::SortZonesAccordingToUnloadedEnemies, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_TrierFuseauxSelonPresenceEnnemisDebarques" ] =
        boost::function< std::vector< boost::shared_ptr< MIL_Fuseau > >( const std::vector< boost::shared_ptr< MIL_Fuseau > >& ) >( boost::bind( &DEC_IntelligenceFunctions::SortAccordingToUnloadedEnemies, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_TrierFuseauxSelonPresenceEnnemisEmbarques" ] =
        boost::function< std::vector< boost::shared_ptr< MIL_Fuseau > >( const std::vector< boost::shared_ptr< MIL_Fuseau > >& ) >( boost::bind( &DEC_IntelligenceFunctions::SortAccordingToLoadedEnemies, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_AmiSurFlanc" ] =
        boost::function< bool( const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::IsFriendOnFlank, boost::cref( GetAutomate() ), _1 ) );
    brain[ "DEC_Rens_CalculerDirectionCouverture" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const MT_Vector2D*, const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeCoverDirection, boost::cref( GetAutomate() ), _1, _2 ) );

    // Geometry
    brain[ "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometry_SplitLocalisation" ] =
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInParts< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Geometrie_DecoupeFuseauEnTroncons" ] =
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const double ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSections< MIL_Automate >, boost::ref( GetAutomate() ), _1  ) );
    brain[ "DEC_Geometrie_CalculerPositionObstacle" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, double) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePosition< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_CalculerPointArrivee" ] =
                            boost::bind( &DEC_GeometryFunctions::ComputeDestPoint< MIL_Automate >, boost::ref( GetAutomate() ) );
    brain[ "DEC_Geometrie_CalculerPointDepart" ] =
                            boost::bind( &DEC_GeometryFunctions::ComputeStartPoint< MIL_Automate >, boost::ref( GetAutomate() ) );
    brain[ "DEC_Geometrie_EstPointDansFuseau" ] =
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerPositionParRapportALima" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLima< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Geometrie_CalculerPositionParRapportALimaDansFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( unsigned int, double, const MIL_Fuseau* ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_CalculerPositionsParRapportALima" ] =
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( unsigned int, double, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::ComputePointsBeforeLima, boost::ref( GetAutomate() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_StartCalculLignesAvantEtArriere" ] =
        boost::function< DEC_FrontAndBackLinesComputer* ( const std::vector< DEC_Decision_ABC* >& ) >( boost::bind( &DEC_GeometryFunctions::StartComputingFrontAndBackLines, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_StartCalculAutomateLignesAvantEtArriere" ] =
        boost::function< DEC_FrontAndBackLinesComputer* ( const std::vector< DEC_Decision_ABC* >& ) >( boost::bind( &DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerAutomateDistanceLigneArriere" ] =
        boost::function< float ( DEC_FrontAndBackLinesComputer*, DEC_Decision_ABC* )> ( boost::bind( &DEC_GeometryFunctions::ComputeDistanceAutomatFromBackLine, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Geometrie_CalculerPointArriveePourPion" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC* ) >( boost::bind( &DEC_GeometryFunctions::ComputeDestPointForPion, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerRetardSurObjectifs" ] =
        boost::function< float( const MIL_Fuseau*, const std::vector< DEC_Decision_ABC* >&, const std::vector< DEC_Objective* >& ) >( boost::bind( &DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_AdvanceAlongFuseau" ] =
        boost::function< double( DEC_Decision_ABC* ) >( boost::bind( &DEC_GeometryFunctions::ComputeAdvanceAlongFuseau, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_Geometrie_GetPointsLimas" ] =
        boost::function< std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >(int, int) >( boost::bind( &DEC_GeometryFunctions::GetPointsOnLimasInFuseau, boost::ref( GetAutomate() ), _1, _2 ) );

    // Orders
    brain[ "DEC_NouvelleMission" ] =
                            boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_Automate >, boost::ref( GetAutomate() ) );
    brain[ "DEC_FinMission" ] =
                            boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_Automate >, boost::ref( GetAutomate() ) );
    brain[ "DEC_GetLima" ] =
        boost::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_ProchaineLimaHoraireNonFlagee" ] =
            boost::function< MIL_LimaOrder* () >( boost::bind( &DEC_OrdersFunctions::GetNextScheduledLima< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain[ "DEC_Fuseau" ] =
            boost::bind( &DEC_OrdersFunctions::GetFuseau< MIL_Automate>, boost::ref( GetAutomate() ) );
    brain[ "DEC_SetMissionLimaFlag" ] =
            boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::AutomateSetMissionLimaFlag, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_GetMissionLimaFlag" ] =
            boost::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_Automate >, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_SetMissionLimaFlagHoraire" ] =
            boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag, boost::ref( GetAutomate() ), _1, _2 ) );

    // MRT / conduite
    brain[ "DEC_MRT_CreerMissionPion" ] =
        boost::function< boost::shared_ptr< MIL_Mission_ABC > ( DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_OrdersFunctions::MRT_CreatePionMission, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_MRT_Valide" ] =
                            boost::bind( &DEC_OrdersFunctions::MRT_Validate, boost::ref( GetAutomate() ) );
    brain[ "DEC_MRT_AffecteFuseaux" ] =
        boost::function< void( std::vector< DEC_Decision_ABC* > ) >( boost::bind( &DEC_OrdersFunctions::MRT_AffectFuseaux, boost::ref( GetAutomate() ), _1 ) );
   brain[ "DEC_CDT_CreerMissionPion" ] =
        boost::function< boost::shared_ptr< MIL_Mission_ABC >( DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CDT_CreatePionMission, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_CDT_DonnerMissionPion_Mission" ] =
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GivePionMission, boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_CreerMissionAutomate" ] =
        boost::function< boost::shared_ptr< MIL_Mission_ABC > (DEC_Decision_ABC*, const std::string&)> ( boost::bind( &DEC_OrdersFunctions::CreateAutomateMission, boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_DonnerMissionAutomate_Mission" ] =
            boost::function< void( boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::GiveAutomateMission , _1, boost::ref( GetAutomate() ) ) );
    brain[ "DEC_DonnerMissionAutomate" ] =
            boost::function< void( boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::GiveAutomateMissionToAutomat , _1, boost::ref( GetAutomate() ) ) );
    brain[ "DEC_DecouperFuseau" ] =
            boost::function<std::list<MIL_Fuseau*> (unsigned int ) >( boost::bind( &DEC_OrdersFunctions::SplitFuseau  , boost::ref( GetAutomate() ), _1 ) );

    // Accesseurs sur les pions
    brain[ "DEC_Automate_PionEstContamine" ] =
        boost::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionContaminated, this, _1 ) );
    brain[ "DEC_Automate_PionEstNeutralise" ] =
        boost::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionNeutralized, this, _1 ) );
    brain[ "DEC_Automate_PionRelevePion" ] =
        boost::function< bool( DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::MakePionRelievePion, boost::cref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Automate_PionPeutReleverPion" ] =
        boost::function< bool( const DEC_Decision_ABC*, const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::CanPionRelievePion, boost::cref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Automate_EstPointDansFuseauPion" ] =
        boost::function< bool( MT_Vector2D*, DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPointInPionFuseau , boost::ref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Automate_PionPeutConstruireObjet" ] =
        boost::function< bool( const DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_AutomateFunctions::CanPionConstructObject, boost::cref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Automate_PionPeutConstruireContournementObjet" ] =
        boost::function< bool( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AutomateFunctions::CanPionBypassObject, boost::cref( GetAutomate() ), _1, _2 ) );
    brain[ "DEC_Automate_PionPeutDetruireObjet" ] =
        boost::function< bool( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AutomateFunctions::CanPionDestroyObject, boost::cref( GetAutomate() ), _1, _2 ) );

    // Logistique
    brain[ "DEC_Automate_PcDeTC2" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateGetPcTC2, boost::ref( GetAutomate() ) );
    brain[ "DEC_DemandeDeRavitaillement" ] =
        boost::bind( &DEC_LogisticFunctions::AutomateRequestSupply, boost::ref( GetAutomate() ) );

    // Objects
    brain[ "DEC_CreerObjetSansDelais" ] =
        boost::function< void( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicCreateObject < MIL_Automate >, boost::ref( GetAutomate() ), _1, _2 ) );

    // Populations
    brain[ "DEC_KnowledgePopulation_Domination" ] =
        boost::function< std::pair< float, int > (int) > ( boost::bind ( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_Automate >, boost::cref( GetAutomate() ) ,_1 ) ) ;
    brain[ "DEC_ConnaissancePopulation_EstDansZone" ] =
            boost::function< bool (unsigned int , TER_Localisation*) > ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsInZone  < MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) );

    // Representations
    brain[ "DEC_GetOrdersCategory" ] =
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( GetAutomate() ) );
    brain[ "DEC_GetPointsCategory" ] =
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetAutomate() ) );
    brain[ "DEC_RemoveFromOrdersCategory" ] =
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_DeleteRepresentation" ] =
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetAutomate() ), _1 ) );
    brain[ "DEC_RemoveFromPointsCategory" ] =
        boost::function< void( boost::shared_ptr< DEC_PathPoint > )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetAutomate() ), _1 ) );

    // Former szName_, mission_, automate_:

    pEntity_->GetType().RegisterFunctions( brain, GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionMrtBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );

    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionMrtBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionConduiteBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionConduiteBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendFullState( client::AutomatAttributes& msg ) const
{
    msg().set_force_ratio       ( sword::ForceRatio_Value( eForceRatioStateNone ) );
    msg().set_meeting_engagement( sword::EnumMeetingEngagementStatus( nCloseCombatState_ ) );
    msg().set_operational_state ( sword::EnumOperationalStatus( nOperationalState_ ) );
    msg().set_roe               ( sword::RulesOfEngagement::Value( nRulesOfEngagementState_ ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendChangedState( client::AutomatAttributes& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetDecAutomate
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_AutomateDecision::GetDecAutomate() const
{
    assert( false );
    throw std::runtime_error( "Unexpected call to DEC_GetAutomate on automate" );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetName
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
std::string DEC_AutomateDecision::GetName() const
{
    assert( false );
    throw std::runtime_error( "Unexpected call to DEC_GetSzName on automate" );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::HasStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetAutomate
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Automate& DEC_AutomateDecision::GetAutomate() const
{
    assert( pEntity_ );
    return *pEntity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetKnowledgeGroup
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup& DEC_AutomateDecision::GetKnowledgeGroup() const
{
    return pEntity_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterSelf( directia::brain::Brain& brain, bool isMasalife, const std::string& groupName )
{
    brain[ "myself" ] = (DEC_Decision_ABC*)this;
    if( isMasalife )
    {
        brain[ "InitMeAutomat" ](  brain[ "integration.ontology.types.body" ], brain[ "myself" ], groupName );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatPhaseMission() const
{
    return ePhaseMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatPhaseMission( int value )
{
    ePhaseMission_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatLima() const
{
    return eEtatLima_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatLima( int value )
{
    eEtatLima_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatDec() const
{
    return eEtatDec_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatDec( int value )
{
    eEtatDec_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetrDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
float DEC_AutomateDecision::GetrDestruction() const
{
    return rDestruction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetrDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetrDestruction( float value )
{
    rDestruction_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GeteEtatEchelon() const
{
    return eEtatEchelon_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SeteEtatEchelon( int value )
{
    eEtatEchelon_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Getfuseau
// Created: LDC 2009-05-19
// $$$$ LDC USed only for ada, i.e. in the ge40 pion. Should be removed and rewritten.
// -----------------------------------------------------------------------------
const MIL_Fuseau* DEC_AutomateDecision::Getfuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Setfuseau
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Setfuseau( MIL_Fuseau* value )
{
    fuseau_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Getzone
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
TER_Localisation* DEC_AutomateDecision::Getzone() const
{
    throw;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Setzone
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Setzone( TER_Localisation* /*value*/ )
{
    throw;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreAttendre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreAttendre() const
{
    return bOrdreAttendre_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreAttendre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreAttendre( bool value )
{
    bOrdreAttendre_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdrePoursuivre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdrePoursuivre() const
{
    return bOrdrePoursuivre_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdrePoursuivre
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdrePoursuivre( bool value )
{
    bOrdrePoursuivre_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreRalentir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreRalentir() const
{
    return bOrdreRalentir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreRalentir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreRalentir( bool value )
{
    bOrdreRalentir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreDecrocher() const
{
    return bOrdreDecrocher_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreDecrocher( bool value )
{
    bOrdreDecrocher_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreTenirSurLR() const
{
    return bOrdreTenirSurLR_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreTenirSurLR( bool value )
{
    bOrdreTenirSurLR_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::GetbOrdreTenir() const
{
    return bOrdreTenir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SetbOrdreTenir( bool value )
{
    bOrdreTenir_ = value;
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_AutomateDecision::GetPionsWithoutPC
// Created: LDC 2009-06-15
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_AutomateDecision::GetPionsWithPC()
{
    return DEC_AutomateFunctions::GetPionsWithPC( GetAutomate() );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::IsPionNeutralized
// Created: LDC 2009-06-15
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::IsPionNeutralized( DEC_Decision_ABC* pPion )
{
    return DEC_AutomateFunctions::IsPionNeutralized( this, pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& DEC_AutomateDecision::GetDIAType() const
{
    return DEC_Decision< MIL_Automate >::GetDIAType();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::IsAutomateEngaged
// Created: HBD 2010-05-28
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::IsAutomateEngaged() const
{
    return pEntity_->IsEngaged();
}
