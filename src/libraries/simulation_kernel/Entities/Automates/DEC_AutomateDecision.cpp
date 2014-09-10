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
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
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
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_AutomateDecision )

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision( MIL_Automate& automate, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : DEC_Decision            ( automate, gcMult, gcPause, logger )
    , nRulesOfEngagementState_( eRoeStateFreeFire )
    , pRoePopulation_         ( PHY_RoePopulation::Default() )
    , nCloseCombatState_      ( eCloseCombatStateNone )
    , nOperationalState_      ( eOpStateOperational )
    , fuseau_                 ( 0 )
    , ePhaseMission_          ( 0 )
    , eEtatEchelon_           ( 0 )
    , eEtatDec_               ( 0 )
    , eEtatLima_              ( 0 )
    , rDestruction_           ( 0 )
    , bStateHasChanged_       ( true )
    , bOrdreAttendre_         ( false )
    , bOrdrePoursuivre_       ( false )
    , bOrdreRalentir_         ( false )
    , bOrdreDecrocher_        ( false )
    , bOrdreTenirSurLR_       ( false )
    , bOrdreTenir_            ( false )
{
    const DEC_Model_ABC& model = automate.GetType().GetModel();
    try
    {
        SetModel( model );
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( tools::GetExceptionMsg( e ) );
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
    unsigned int nRoePopulationID;
    file >> boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this );
    file >> pEntity_;
    file >> nRulesOfEngagementState_;
    file >> nCloseCombatState_;
    file >> nOperationalState_;
    file >> nRoePopulationID;
    pRoePopulation_ = PHY_RoePopulation::Find( nRoePopulationID );

    unsigned int nID;
    file >> nID;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pEntity_ );
    unsigned int nRoePopulationID = pRoePopulation_->GetID();
    unsigned id = pEntity_->GetType().GetID();
    file << boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this );
    file << pEntity_;
    file << nRulesOfEngagementState_;
    file << nCloseCombatState_;
    file << nOperationalState_;
    file << nRoePopulationID;
    file << id;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::EndCleanStateAfterCrash()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetGroupName
// Created: LDC 2011-09-01
// -----------------------------------------------------------------------------
std::string DEC_AutomateDecision::GetGroupName()
{
    return GetAutomate().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_Automate_Decision::RegisterUserArchetypeFunctions
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterUserArchetypeFunctions ( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_GetSzName", &DEC_MiscFunctions::GetName );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterUserFunctions( sword::Brain& brain )
{
// Accessors
    brain.RegisterFunction( "DEC_Automate_PionsAvecPC", boost::bind( &DEC_AutomateFunctions::GetPionsWithPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsSansPC", boost::bind( &DEC_AutomateFunctions::GetPionsWithoutPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPC", boost::bind( &DEC_AutomateFunctions::GetPionPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_AutomatesSubordonnes", boost::bind( &DEC_AutomateFunctions::GetAutomates, boost::cref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsMelee", boost::bind( &DEC_AutomateFunctions::GetPionsMelee, boost::cref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsGenie", boost::bind( &DEC_AutomateFunctions::GetPionsGenie, boost::cref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_AutomateSuperieur_EstEmbraye", boost::bind( &DEC_AutomateFunctions::IsParentAutomateEngaged, boost::cref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsAvecPCCommunication", boost::bind( &DEC_AutomateFunctions::GetCommunicationPionsWithPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsSansPCCommunication", boost::bind( &DEC_AutomateFunctions::GetCommunicationPionsWithoutPC, boost::ref( GetAutomate() ) ) );

    // State
    brain.RegisterFunction( "DEC_Automate_EstEmbraye", boost::bind( &DEC_AutomateFunctions::IsEngaged, this ) );
    brain.RegisterFunction( "DEC_Automate_ROE", boost::bind( &DEC_AutomateFunctions::GetRulesOfEngagementState, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_ChangeEtatROE",
        std::function< void( int ) >( boost::bind( &DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_ChangeEtatROEPopulation",
        std::function< void( int ) >( boost::bind( &DEC_AutomateFunctions::NotifyRulesOfEngagementPopulationStateChanged, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_ROEPopulation", boost::bind( &DEC_AutomateFunctions::GetRoePopulation, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_isLogistic", boost::bind( &DEC_AutomateFunctions::IsLogistic, boost::ref( GetAutomate() ) ) );

    // Debug
    brain.RegisterFunction( "DEC_DecisionalState",
        std::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AutomateFunctions::DecisionalState, boost::ref( GetAutomate() ), _1, _2 ) ) );

    brain.RegisterFunction( "DEC_DebugAffichePoint"  ,
            std::function< void ( const MT_Vector2D* ) > ( boost::bind( &DEC_MiscFunctions::DebugDrawPoint, boost::cref( GetAutomate() ), _1  ) ) );
    brain.RegisterFunction( "DEC_DebugAffichePoints" ,
            std::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints, boost::cref( GetAutomate() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Debug",
            std::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug, boost::cref( GetAutomate() ) , "Automate" , _1  ) ) );
    brain.RegisterFunction( "DEC_Trace",
        std::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace, boost::cref( GetAutomate() ), _1 ) ) );

    // Connaissance
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesAvec",
        std::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesDansZoneAvec",
        std::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Knowledges_ObjectsInCircle",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2, _3, false ) ) );
    brain.RegisterFunction( "DEC_Knowledges_AllObjectsInCircle",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3, true ) ) );
    brain.RegisterFunction( "DEC_ObjectKnowledgesInZone",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_ObjetsDansFuseau",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion",
        std::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_Populations", boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_Automate >, boost::cref( GetAutomate() ) ) );

    // Geometry
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeAutomatLocalisationBarycenterInFuseau, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometry_SplitLocalisation",
        std::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, const MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitAutomatLocalisationInParts, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_DecoupeFuseauEnTroncons",
        std::function< std::vector< boost::shared_ptr< TER_Localisation > >( const double ) >( boost::bind( &DEC_GeometryFunctions::SplitAutomatLocalisationInSections, boost::ref( GetAutomate() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionObstacle",
        std::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, double) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePositionForAutomat, boost::cref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArrivee",
                            boost::bind( &DEC_GeometryFunctions::ComputeAutomatDestPoint, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointDepart",
                            boost::bind( &DEC_GeometryFunctions::ComputeAutomatStartPoint, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansFuseau",
        std::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInAutomatFuseau, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALima",
        std::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeAutomatLima, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALimaDansFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( unsigned int, double, const MIL_Fuseau* ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionsParRapportALima",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( unsigned int, double, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::ComputePointsBeforeLima, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_StartCalculLignesAvantEtArriere",
        std::function< DEC_FrontAndBackLinesComputer* ( const std::vector< DEC_Decision_ABC* >& ) >( boost::bind( &DEC_GeometryFunctions::StartComputingFrontAndBackLines, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_StartCalculAutomateLignesAvantEtArriere",
        std::function< DEC_FrontAndBackLinesComputer* ( const std::vector< DEC_Decision_ABC* >& ) >( boost::bind( &DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerAutomateDistanceLigneArriere",
        std::function< float ( DEC_FrontAndBackLinesComputer*, DEC_Decision_ABC* )> ( boost::bind( &DEC_GeometryFunctions::ComputeDistanceAutomatFromBackLine, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArriveePourPion",
        std::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC* ) >( boost::bind( &DEC_GeometryFunctions::ComputeDestPointForPion, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInAutomatFuseau, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInAutomatFuseau, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerRetardSurObjectifs",
        std::function< float( const MIL_Fuseau*, const std::vector< DEC_Decision_ABC* >&, const std::vector< DEC_Objective* >& ) >( boost::bind( &DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_AdvanceAlongFuseau",
        std::function< double( DEC_Decision_ABC* ) >( boost::bind( &DEC_GeometryFunctions::ComputeAdvanceAlongFuseau, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_GetPointsLimas",
        std::function< std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >(int, int) >( boost::bind( &DEC_GeometryFunctions::GetPointsOnLimasInFuseau, boost::ref( GetAutomate() ), _1, _2 ) ) );

    // Orders
    brain.RegisterFunction( "DEC_NouvelleMission",
                            boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_FinMission",
                            boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_GetLima",
        std::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_GetLimasFromType",
        std::function< std::vector< unsigned int >( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLimasFromType< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ProchaineLimaHoraireNonFlagee",
            std::function< MIL_LimaOrder* () >( boost::bind( &DEC_OrdersFunctions::GetNextScheduledLima< MIL_Automate >, boost::ref( GetAutomate() ) ) ) );
    brain.RegisterFunction( "DEC_Fuseau",
            boost::bind( &DEC_OrdersFunctions::GetFuseau, boost::cref( *this ) ) );
    brain.RegisterFunction( "DEC_SetMissionLimaFlag",
            std::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::AutomateSetMissionLimaFlag, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_GetMissionLimaFlag",
            std::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_SetMissionLimaFlagHoraire",
            std::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignMissionCrowdParameter",
            std::function< void( boost::shared_ptr< MIL_Mission_ABC >, const std::string&, int ) >( boost::bind( &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter, this, _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_AssignMissionCrowdListParameter",
            std::function< boost::shared_ptr<MIL_MissionParameter_ABC>( int ) >( boost::bind( &MIL_MissionParameterFactory::CreatePopulationKnowledge, this, _1 ) ) );

    // MRT / conduite
    brain.RegisterFunction( "DEC_MRT_CreerMissionPion",
        std::function< boost::shared_ptr< MIL_Mission_ABC > ( DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_OrdersFunctions::MRT_CreatePionMission, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_MRT_Valide",
                            boost::bind( &DEC_OrdersFunctions::MRT_Validate, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_MRT_AffecteFuseaux",
        std::function< void( std::vector< DEC_Decision_ABC* > ) >( boost::bind( &DEC_OrdersFunctions::MRT_AffectFuseaux, boost::ref( GetAutomate() ), _1 ) ) );
   brain.RegisterFunction( "DEC_CDT_CreerMissionPion",
        std::function< boost::shared_ptr< MIL_Mission_ABC >( DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CDT_CreatePionMission, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_CDT_DonnerMissionPion_Mission",
        std::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GivePionMission, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_CreerMissionAutomate",
        std::function< boost::shared_ptr< MIL_Mission_ABC > (DEC_Decision_ABC*, const std::string&)> ( boost::bind( &DEC_OrdersFunctions::CreateAutomateMission, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_DonnerMissionAutomate_Mission",
            std::function< void( boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::GiveAutomateMission , _1, boost::ref( GetAutomate() ) ) ) );
    brain.RegisterFunction( "DEC_DonnerMissionAutomate",
            std::function< void( boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::GiveAutomateMissionToAutomat , _1, boost::ref( GetAutomate() ) ) ) );
    brain.RegisterFunction( "DEC_DecouperFuseau",
            std::function<std::list<MIL_Fuseau*> (unsigned int ) >( boost::bind( &DEC_OrdersFunctions::SplitFuseau  , boost::ref( GetAutomate() ), _1 ) ) );

    // Accesseurs sur les pions
    brain.RegisterFunction( "DEC_Automate_PionEstContamine",
        std::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionContaminated, this, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionEstEmpoisonne",
        std::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionIntoxicated, this, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionEstNeutralise",
        std::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionNeutralized, this, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionEstTransporte",
        std::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionTransported, this, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionRelevePion",
        std::function< bool( DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::MakePionRelievePion, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutReleverPion",
        std::function< bool( const DEC_Decision_ABC*, const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::CanPionRelievePion, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_EstPointDansFuseauPion",
        std::function< bool( MT_Vector2D*, DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPointInPionFuseau , boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutConstruireObjet",
        std::function< bool( const DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_AutomateFunctions::CanPionConstructObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutConstruireObjetAvecLocalisation",
        std::function< bool( const DEC_Decision_ABC*, const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AutomateFunctions::CanPionConstructObjectWithLocalisation, boost::cref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutConstruireContournementObjet",
        std::function< bool( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AutomateFunctions::CanPionBypassObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutDetruireObjet",
        std::function< bool( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AutomateFunctions::CanPionDestroyObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutDevaloriserObjet",
        std::function< bool( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AutomateFunctions::CanPionDemineObject, boost::cref( GetAutomate() ), _1, _2 ) ) );

    // Logistique
    brain.RegisterFunction( "DEC_Automate_PcDeTC2",
        boost::bind( &DEC_LogisticFunctions::AutomateGetPcTC2, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_DemandeDeRavitaillement",
        boost::bind( &DEC_LogisticFunctions::AutomateRequestSupply, boost::ref( GetAutomate() ) ) );

    // Objects
    brain.RegisterFunction( "DEC_CreerObjetSansDelais",
        std::function< int( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicCreateObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_MagicGetOrCreateObject",
        std::function< int( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicGetOrCreateObject, boost::cref( GetAutomate() ), _1, _2 ) ) );

    // Populations
    brain.RegisterFunction( "DEC_KnowledgePopulation_Domination",
        std::function< std::pair< float, int > (int) > ( boost::bind ( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_Automate >, boost::cref( GetAutomate() ) ,_1 ) )  );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_EstDansZone",
            std::function< bool (unsigned int , TER_Localisation*) > ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsInZone  < MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_KnowledgePopulation_GetBarycenter",
        std::function< boost::shared_ptr< MT_Vector2D >( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetBarycenter< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Crowd_GetHumansFromAllTypes",
        std::function< std::vector< unsigned int >( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetHumansFromAllTypes< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );

    // Representations
    brain.RegisterFunction( "DEC_GetOrdersCategory",
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_GetPointsCategory",
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_RemoveFromOrdersCategory",
        std::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DeleteRepresentation",
        std::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_RemoveFromPointsCategory",
        std::function< void( boost::shared_ptr< DEC_PathPoint > )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetAutomate() ), _1 ) ) );
    
    brain.RegisterFunction( "DEC_KnowledgeAgent_IsPerceptionLevelMax", std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceptionLevelMax, boost::cref( GetAutomate() ), _1 ) ) );

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
    msg().set_meeting_engagement( sword::EnumMeetingEngagementStatus( nCloseCombatState_ ) );
    msg().set_operational_state ( sword::EnumOperationalStatus( nOperationalState_ ) );
    msg().set_roe               ( sword::RulesOfEngagement::Value( nRulesOfEngagementState_ ) );
    msg().set_decisional_model  ( model_->GetName() );
    msg().set_brain_debug       ( brainDebug_ );
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
// Modified: NMI 2013-09-03
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_AutomateDecision::GetDecAutomate()
{
    return this;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetName
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
std::string DEC_AutomateDecision::GetName() const
{
    return pEntity_->GetName();
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
// Name: DEC_Automate_Decision::GetOrderManager
// Created: NMI 2013-11-15
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC& DEC_AutomateDecision::GetOrderManager() const
{
    return GetAutomate().GetOrderManager();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetKnowledgeGroup
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > DEC_AutomateDecision::GetKnowledgeGroup() const
{
    return pEntity_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterSelf( sword::Brain& brain )
{
    brain.GetScriptRef( "myself" ) = (DEC_Decision_ABC*)this;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterSpecific
// Created: JSR 2013-02-18
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName )
{
    if( isMasalife )
        brain.GetScriptRef( "InitMeAutomat" )(
            brain.GetScriptRef( "integration.ontology.types.body" ),
            brain.GetScriptRef( "myself" ),
            groupName );
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
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Setzone
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Setzone( TER_Localisation* /*value*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
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

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState state )
{
    if( nRulesOfEngagementState_ != state )
    {
        nRulesOfEngagementState_ = state;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::NotifyRulesOfEngagementPopulationStateChanged
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::NotifyRulesOfEngagementPopulationStateChanged( const PHY_RoePopulation& roe )
{
    if( roe != *pRoePopulation_ )
    {
        pRoePopulation_   = &roe;
        bStateHasChanged_  = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetRoePopulation
// Created: LGY 2012-06-22
// -----------------------------------------------------------------------------
const PHY_RoePopulation& DEC_AutomateDecision::GetRoePopulation() const
{
    return *pRoePopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::AddReconnoiteredPoint
// Created: LMT 2011-08-16
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::AddReconnoiteredPointBy( DEC_Decision_ABC* pPion, MT_Vector2D& point )
{
    listReconnoitringPoint_[point] = pPion;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::PointIsReconnoiteredByMeOrNoOne
// Created: LMT 2011-08-16
// -----------------------------------------------------------------------------
bool DEC_AutomateDecision::PointIsReconnoiteredByMeOrNoOne( DEC_Decision_ABC* pPion,MT_Vector2D& point ) const
{
    std::map< MT_Vector2D, DEC_Decision_ABC* >::const_iterator it = listReconnoitringPoint_.find( point );
    if( it != listReconnoitringPoint_.end() )
    {
        if( it->second != pPion )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Reload
// Created:AHC 2012-01-24
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Reload( bool force, bool doInitBrain )
{
    DEC_Decision< MIL_Automate >::Reload( force, doInitBrain );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::GetRulesOfEngagementState
// Created: LGY 2012-06-26
// -----------------------------------------------------------------------------
int DEC_AutomateDecision::GetRulesOfEngagementState() const
{
    return nRulesOfEngagementState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::ActivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::ActivateBrainDebug( bool activate )
{
    DEC_Decision< MIL_Automate >::ActivateBrainDebug( activate );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::PostStartMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::PostStartMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::PostStopMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::PostStopMission()
{
    // NOTHING
}
