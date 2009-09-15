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
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Messages.h"
#include "MT_Tools/MT_CrashHandler.h"

#include "Decision/Functions/DEC_AutomateFunctions.h"
#include "Decision/Functions/DEC_OrdersFunctions.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_KnowledgeFunctions.h"
#include "Decision/Functions/DEC_KnowledgeAgentFunctions.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/Functions/DEC_KnowledgePopulationFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
#include "Decision/Functions/DEC_IntelligenceFunctions.h"
#include "Decision/Functions/DEC_CommunicationFunctions.h"

#include <boost/bind.hpp>

BOOST_CLASS_EXPORT_GUID( DEC_AutomateDecision, "DEC_AutomateDecision" )

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision( MIL_Automate& automate )
    : DEC_Decision             ( automate )
    , nRulesOfEngagementState_ ( eRoeStateNone         )
    , nCloseCombatState_       ( eCloseCombatStateNone )
    , nOperationalState_       ( eOpStateOperational   )
    , bStateHasChanged_        ( true                  )    
    , ePhaseMission_           ( 0                     )
    , eEtatLima_               ( 0                     )
    , eEtatDec_                ( 0                     )
    , rDestruction_            ( 0                     )
    , eEtatEchelon_            ( 0                     )
    , bOrdreAttendre_          ( false                 )
    , bOrdrePoursuivre_        ( false                 )
    , bOrdreRalentir_          ( false                 )
    , bOrdreDecrocher_         ( false                 )
    , bOrdreTenirSurLR_        ( false                 )
    , bOrdreTenir_             ( false                 )
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
// Name: DEC_AutomateDecision constructor
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
DEC_AutomateDecision::DEC_AutomateDecision()
    : DEC_Decision             (                      ) 
    , nRulesOfEngagementState_ ( eRoeStateNone         )
    , nCloseCombatState_       ( eCloseCombatStateNone )
    , nOperationalState_       ( eOpStateOperational   )
    , bStateHasChanged_        ( true                  )   
    , ePhaseMission_           ( 0                     )
    , eEtatLima_               ( 0                     )
    , eEtatDec_                ( 0                     )
    , rDestruction_            ( 0                     )
    , eEtatEchelon_            ( 0                     )
    , bOrdreAttendre_          ( false                 )
    , bOrdrePoursuivre_        ( false                 )
    , bOrdreRalentir_          ( false                 )
    , bOrdreDecrocher_         ( false                 )
    , bOrdreTenirSurLR_        ( false                 )
    , bOrdreTenir_             ( false                 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_AutomateDecision::~DEC_AutomateDecision()
{
    StopDefaultBehavior();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Decision< MIL_Automate > >( *this )
		 >> pEntity_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_;
    
    assert( pEntity_ );
    
    uint nID;
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
void DEC_AutomateDecision::save( MIL_CheckPointOutArchive& file, const uint ) const
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

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::EndCleanStateAfterCrash()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterUserFunctions( directia::Brain& brain )
{
// Accessors
    brain.RegisterFunction( "DEC_Automate_PionsAvecPC", boost::bind( &DEC_AutomateFunctions::GetPionsWithPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.RegisterFunction( "DEC_Automate_PionsSansPC", boost::bind( &DEC_AutomateFunctions::GetPionsWithoutPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPC", boost::bind( &DEC_AutomateFunctions::GetPionPC, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPCDeAutomate", 
        boost::function< DEC_Decision_ABC*( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::GetPionPCOfAutomate, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_AutomatesSubordonnes", boost::bind( &DEC_AutomateFunctions::GetAutomates, boost::cref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_AutomateSuperieur_EstEmbraye", boost::bind( &DEC_AutomateFunctions::IsParentAutomateEngaged, boost::cref( GetAutomate() ) ) );

    brain.RegisterFunction( "DEC_Automate_PionsDeAutomateSansPC", 
        boost::function< std::vector< DEC_Decision_ABC* >( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionsDeAutomateAvecPC", 
        boost::function< std::vector< DEC_Decision_ABC* >( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::GetAutomatPionsWithPC, _1 ) ) );

    // State
    brain.RegisterFunction( "DEC_Automate_EstEmbraye", boost::bind( &DEC_AutomateFunctions::IsEngaged, this ) );
    
    // Debug
    brain.RegisterFunction( "DEC_DecisionalState",
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AutomateFunctions::DecisionalState, boost::ref( GetAutomate() ), _1, _2 ) ) );

    brain.RegisterFunction( "DEC_DebugAffichePoint"  ,
    		boost::function< void ( const MT_Vector2D* ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoint< MIL_Automate >, boost::ref( GetAutomate()), _1  ) ) );
    brain.RegisterFunction( "DEC_DebugAffichePoints" ,
    		boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints< MIL_Automate >, boost::ref( GetAutomate()), _1  ) ) );
    brain.RegisterFunction( "DEC_Debug",
    		boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug< MIL_Automate > , boost::ref( GetAutomate()) , "Automate" , _1  ) ) );
    brain.RegisterFunction( "DEC_Trace",
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );


    // Objets
    brain.RegisterFunction( "DEC_ActiverObjet", &DEC_ObjectFunctions::ActivateObject< MIL_Automate > );

    // Connaissance
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Verrouiller",
        boost::function< int( DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::Lock, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Deverrouiller",
        boost::function< void( DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::Unlock, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Position",
        boost::function< const MT_Vector2D*( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetPosition, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstEnVol",
        boost::function< bool( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsFlying, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstValide",
        boost::function< bool( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsKnowledgeValid, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstMort",
        boost::function< bool( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsDead, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstPrisonnier",
        boost::function< bool( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions ::IsPrisoner, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurPion",
        boost::function< float( const DEC_Knowledge_Agent*, const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosityOnPion, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurConnaissance",
        boost::function< float( const DEC_Knowledge_Agent*, const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EtatOps", 
        boost::function< float( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions ::GetOperationalState, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDetruitTactique",
        boost::function< bool( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetMajorOperationalState, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_NiveauPerceptionMax",
        boost::function< int( const DEC_Knowledge_Agent* ) >( boost::bind( &DEC_KnowledgeAgentFunctions ::GetMaxPerceptionLevelForKnowledgeGroup, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstValide",
        boost::function< bool (unsigned int) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsKnowledgeValid< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif",
        boost::function< bool( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstObstacleDeManoeuvre", 
        boost::function< bool( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsReservedObstacle< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstContourne", 
        boost::function< int( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsBypassed< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Localisation",
        boost::function< const TER_Localisation*( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetLocalisation< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Type", 
        boost::function< std::string( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetType< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_LargeurSiteFranchissement", 
        boost::function< float( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstReconnu",
        boost::function< bool( int ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsRecon< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesAvec",
        boost::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesDansZoneAvec",
        boost::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Knowledges_ObjectsInCircle",
        boost::function< std::vector< unsigned int >( const MT_Vector2D*, MT_Float, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_ObjectKnowledgesInZone",
        boost::function< std::vector< unsigned int >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_ObjetsDansFuseau", 
        boost::function< std::vector< unsigned int >( const std::string ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion",
        boost::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_Automate >, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_Populations", boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_Automate >, boost::cref( GetAutomate() ) ) );
    
    // Intelligence
    brain.RegisterFunction( "DEC_Rens_PourcentageEnnemisDebarquesDansZone",
        boost::function< float( const TER_Localisation* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeUnloadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_PourcentageEnnemisEmbarquesDansZone",
        boost::function< float( const TER_Localisation* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeLoadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_PourcentageEnnemisDebarquesDansFuseau",
        boost::function< float( const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeFuseauUnloadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_PourcentageEnnemisEmbarquesDansFuseau", 
        boost::function< float( const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeFuseauLoadedEnemiesRatio, boost::cref( GetAutomate() ), _1 ) ) );
    
    brain.RegisterFunction( "DEC_Rens_TrierZonesSelonPresenceEnnemisDebarques",
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const std::vector< boost::shared_ptr< TER_Localisation > >& ) >( boost::bind( &DEC_IntelligenceFunctions::SortZonesAccordingToUnloadedEnemies, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_TrierFuseauxSelonPresenceEnnemisDebarques", 
        boost::function< std::vector< boost::shared_ptr< MIL_Fuseau > >( const std::vector< boost::shared_ptr< MIL_Fuseau > >& ) >( boost::bind( &DEC_IntelligenceFunctions::SortAccordingToUnloadedEnemies, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_TrierFuseauxSelonPresenceEnnemisEmbarques", 
        boost::function< std::vector< boost::shared_ptr< MIL_Fuseau > >( const std::vector< boost::shared_ptr< MIL_Fuseau > >& ) >( boost::bind( &DEC_IntelligenceFunctions::SortAccordingToLoadedEnemies, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_AmiSurFlanc",
        boost::function< bool( const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::IsFriendOnFlank, boost::cref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Rens_CalculerDirectionCouverture",
        boost::function< boost::shared_ptr< MT_Vector2D >( const MT_Vector2D*, const MIL_Fuseau* ) >( boost::bind( &DEC_IntelligenceFunctions::ComputeCoverDirection, boost::cref( GetAutomate() ), _1, _2 ) ) );

    // RCS
    brain.RegisterFunction( "DEC_RC1",
        boost::function< void ( int, int ) >( boost::bind( &DEC_MiscFunctions::Report< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_RC_AgentKnowledge",
        boost::function< void ( int, int, DEC_Knowledge_Agent* ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledge< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_DotationType",
        boost::function< void ( int, int, const PHY_DotationCategory* ) >( boost::bind( &DEC_MiscFunctions::ReportDotationType< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_EquipmentType",
        boost::function< void ( int, int, const PHY_ComposanteTypePion* ) >( boost::bind( &DEC_MiscFunctions::ReportEquipmentType< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Float",
        boost::function< void ( int, int, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloat< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Float_Float",
        boost::function< void ( int, int, float, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloatFloat< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Id",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportId< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_ObjectKnowledge",
        boost::function< void ( int, int, DEC_Knowledge_Object* ) >( boost::bind( &DEC_MiscFunctions::ReportObjectKnoweldge< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion",
        boost::function< void ( int, int, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPion< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Automate",
        boost::function< void ( int, int, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionAutomate< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_PopulationKnowledge",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportPopulationKnowledge< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_TirPion",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportTirPion< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );

    // Geometry       
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometry_SplitLocalisation",
        boost::function<  std::vector< boost::shared_ptr< TER_Localisation > >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInParts< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_DecoupeFuseauEnTroncons",
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const MT_Float ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSections< MIL_Automate >, boost::ref( GetAutomate() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionObstacle",
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, MT_Float) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePosition< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArrivee",
                            boost::bind( &DEC_GeometryFunctions::ComputeDestPoint< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointDepart",
                            boost::bind( &DEC_GeometryFunctions::ComputeStartPoint< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansFuseau",
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALima",
        boost::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLima< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALimaDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( uint, MT_Float, const MIL_Fuseau* ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionsParRapportALima",
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( uint, MT_Float, uint ) >( boost::bind( &DEC_GeometryFunctions::ComputePointsBeforeLima, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_PositionsParRapportALocalisation",
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const std::vector< DEC_Decision_ABC* >&, TER_Localisation*, MT_Vector2D*, MT_Float ) >( boost::bind( &DEC_GeometryFunctions ::ComputeLocalisationPointsForPionsInFuseau, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_StartCalculLignesAvantEtArriere",
        boost::function< DEC_FrontAndBackLinesComputer* ( const std::vector< DEC_Decision_ABC* >& ) >( boost::bind( &DEC_GeometryFunctions::StartComputingFrontAndBackLines, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_StartCalculAutomateLignesAvantEtArriere",
        boost::function< DEC_FrontAndBackLinesComputer* ( const std::vector< DEC_Decision_ABC* >& ) >( boost::bind( &DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_StopCalculLignesAvantEtArriere",                        &DEC_GeometryFunctions::StopComputingFrontAndBackLines                                   );
    brain.RegisterFunction( "DEC_Geometrie_CalculerDistanceLigneAvant",                            &DEC_GeometryFunctions::ComputeDistanceFromFrontLine                                     );
    brain.RegisterFunction( "DEC_Geometrie_CalculerAutomateDistanceLigneAvant",                    &DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine                              );
    brain.RegisterFunction( "DEC_Geometrie_CalculerDistanceLigneArriere",                          &DEC_GeometryFunctions::ComputeDistanceFromBackLine                                      );
    brain.RegisterFunction( "DEC_Geometrie_CalculerAutomateDistanceLigneArriere",
        boost::function< float ( DEC_FrontAndBackLinesComputer*, DEC_Decision_ABC* )> ( boost::bind( &DEC_GeometryFunctions::ComputeDistanceAutomatFromBackLine, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArriveePourPion",
        boost::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC* ) >( boost::bind( &DEC_GeometryFunctions::ComputeDestPointForPion, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );  
    brain.RegisterFunction( "DEC_Geometrie_PionDevant",                                            &DEC_GeometryFunctions::GetFrontestPion );
    brain.RegisterFunction( "DEC_Geometrie_PionDerriere",                                          &DEC_GeometryFunctions::ComputeBackestAgent );
    brain.RegisterFunction( "DEC_Geometrie_CalculerRetard",                                        &DEC_GeometryFunctions::ComputeAutomatDelayFromSchedule                                  );
    brain.RegisterFunction( "DEC_Geometrie_CalculerRetardSurObjectifs",
        boost::function< float( const MIL_Fuseau*, const std::vector< DEC_Decision_ABC* >&, const std::vector< DEC_Objective* >& ) >( boost::bind( &DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives< MIL_Automate >, boost::ref( GetAutomate() ), _1, _2, _3 ) ) );

    // Orders
    brain.RegisterFunction( "DEC_NouvelleMission",
                            boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_FinMission",
                            boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_Automate >, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_GetLima",
        boost::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ProchaineLimaHoraireNonFlagee",
    		boost::function< unsigned int () >( boost::bind( &DEC_OrdersFunctions::GetNextScheduledLima< MIL_Automate >, boost::ref( GetAutomate() ) ) ) );
    brain.RegisterFunction( "DEC_Fuseau",
    		boost::bind( &DEC_OrdersFunctions::GetFuseau< MIL_Automate>, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_SetMissionLimaFlag",
    		boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::AutomateSetMissionLimaFlag, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_GetMissionLimaFlag",
    		boost::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_Automate >, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_SetMissionLimaFlagHoraire",
    		boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag, boost::ref( GetAutomate() ), _1, _2 ) ) );

    // MRT / conduite
    brain.RegisterFunction( "DEC_MRT_CreerMissionPion",
        boost::function< MIL_Mission_ABC* ( DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_OrdersFunctions::MRT_CreatePionMission, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_MRT_Valide",
                            boost::bind( &DEC_OrdersFunctions::MRT_Validate, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_MRT_AffecteFuseaux",
        boost::function< void( std::vector< DEC_Decision_ABC* > ) >( boost::bind( &DEC_OrdersFunctions::MRT_AffectFuseaux, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_CDT_CreerMissionPion",
        boost::function< MIL_Mission_ABC*( DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CDT_CreatePionMission, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_CDT_DonnerMissionPion_Mission",
        boost::function< void( MIL_Mission_ABC* pMission ) >( boost::bind( &DEC_OrdersFunctions::CDT_GivePionMission, boost::ref( GetAutomate() ), _1 ) ) );
    brain.RegisterFunction( "DEC_CreerMissionAutomate",
    	boost::function< MIL_Mission_ABC* (DEC_Decision_ABC*, const std::string&)> ( boost::bind( &DEC_OrdersFunctions::CreateAutomateMission, boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_DonnerMissionAutomate",
    		boost::function< void( MIL_AutomateMission*)>( boost::bind( &DEC_OrdersFunctions::GiveAutomateMission , _1, boost::ref( GetAutomate() ) ) ) );
    brain.RegisterFunction( "DEC_AssignerFuseauAMissionAutomate",
    		boost::function< void( MIL_Fuseau* , MIL_AutomateMission*)>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToAutomateMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignerDirectionAMissionAutomate",
    		boost::function< void (MT_Vector2D* , MIL_AutomateMission* ) >( boost::bind( &DEC_OrdersFunctions::AssignDirectionToAutomateMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_DecouperFuseau",
    		boost::function<std::list<MIL_Fuseau*> (unsigned int ) >( boost::bind( &DEC_OrdersFunctions::SplitFuseau  , boost::ref( GetAutomate() ), _1 ) ) );

    // Pion management
    brain.RegisterFunction( "DEC_Pion_ChangeAutomate", 
        boost::function< bool( DEC_Decision_ABC*, const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::PionChangeAutomate, _1, _2 ) ) );
    
    // Accesseurs sur les pions   
    brain.RegisterFunction( "DEC_Automate_PionEstContamine",
        boost::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionContaminated, this, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionEstNeutralise",
        boost::function< bool ( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPionNeutralized, this, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionEstNeutralise",
        &DEC_AutomateDecision::IsPionNeutralized );
    brain.RegisterFunction( "DEC_Automate_PionPosition", 
        boost::function< boost::shared_ptr< MT_Vector2D >( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::GetPionPosition, _1 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionRelevePion",
        boost::function< bool( DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::MakePionRelievePion, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutReleverPion",
        boost::function< bool( const DEC_Decision_ABC*, const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::CanPionRelievePion, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_EstPointDansFuseauPion",
        boost::function< bool( MT_Vector2D*, DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::IsPointInPionFuseau , boost::ref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_CalculerPositionParRapportALimaPourPion",
        boost::function< boost::shared_ptr< MT_Vector2D >( int, float, const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::ComputePointBeforeLimaForPion, _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Automate_CalculerPointProcheLocalisationDansFuseauPourPion",
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation*, const DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutConstruireObjet",
        boost::function< bool( const DEC_Decision_ABC*, const std::string& ) >( boost::bind( &DEC_AutomateFunctions::CanPionConstructObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutConstruireContournementObjet",
        boost::function< bool( const DEC_Decision_ABC*, int ) >( boost::bind( &DEC_AutomateFunctions::CanPionBypassObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionPeutDetruireObjet",
        boost::function< bool( const DEC_Decision_ABC*, int ) >( boost::bind( &DEC_AutomateFunctions::CanPionDestroyObject, boost::cref( GetAutomate() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Automate_PionTempsPourParcourirDistanceEnLigneDroite",
        boost::function< float( const DEC_Decision_ABC*, float ) >( boost::bind( &DEC_AutomateFunctions::PionTimeToMoveDistance, _1, _2 ) ) );

    // Logistique
    brain.RegisterFunction( "DEC_Automate_TC2",
        boost::bind( &DEC_LogisticFunctions::AutomateGetTC2, boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_DemandeDeRavitaillement", &DEC_LogisticFunctions::AutomateRequestSupply );

    // Objects
    brain.RegisterFunction( "DEC_CreerObjetSansDelais",    &DEC_ObjectFunctions::MagicCreateObject < MIL_Automate > );
    brain.RegisterFunction( "DEC_DetruireObjetSansDelais",
    		boost::function< void (int) > (boost::bind( &DEC_ObjectFunctions::MagicDestroyObject< MIL_Automate >, boost::ref( GetAutomate()), _1 ) ) );

    // Populations
    brain.RegisterFunction( "DEC_ConnaissancePopulation_Domination",
    		boost::function< float (int) > ( boost::bind ( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_Automate >, boost::cref( GetAutomate() ) ,_1 ) ) ) ;
    brain.RegisterFunction( "DEC_ConnaissancePopulation_EstDansZone",
    		boost::function< bool (unsigned int , TER_Localisation*) > ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsInZone  < MIL_Automate >, boost::cref( GetAutomate() ), _1, _2 ) ) );

    // Representations
    brain.RegisterFunction( "DEC_GetOrdersCategory",
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_GetPointsCategory",
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetAutomate() ) ) );
    brain.RegisterFunction( "DEC_RemoveFromOrdersCategory" , &DEC_MiscFunctions::RemoveFromOrdersCategory   );
    brain.RegisterFunction( "DEC_DeleteRepresentation" , &DEC_MiscFunctions::DeleteOrderRepresentation );
    brain.RegisterFunction( "DEC_RemoveFromPointsCategory",
        boost::function< void( DEC_PathPoint* )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetAutomate() ), _1 ) ) );
        
    // Former szName_, mission_, automate_:
    brain.RegisterFunction( "DEC_GetRawMission", 
        boost::function< MIL_Mission_ABC*( DEC_Decision_ABC* ) >( boost::bind( &DEC_AutomateFunctions::GetMission, _1 ) ) );
    brain.RegisterFunction( "DEC_SetMission",
        boost::function< void( DEC_Decision_ABC*, MIL_Mission_ABC* )>( boost::bind( &DEC_AutomateFunctions::SetMission, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_GetSzName",             &DEC_MiscFunctions::GetName             );
    brain.RegisterFunction( "DEC_GetAutomate",           &DEC_MiscFunctions::GetAutomate         );
    brain.RegisterFunction( "DEC_GetDirectionEnnemi",    &DEC_MiscFunctions::GetDirectionEnnemi  );
    brain.RegisterFunction( "DEC_Copie_DirectionDanger", &DEC_MiscFunctions::CopyDirectionDanger );

    DEC_CommunicationFunctions::Register( brain );

    pEntity_->GetType().RegisterFunctions( brain, GetAutomate() );
}

// =============================================================================
// UPDATE
// =============================================================================

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionMrtBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );    

    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionMrtBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionMrtBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseMRT );
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StartMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StartMissionConduiteBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );

    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::StopMissionConduiteBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::StopMissionConduiteBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior( MIL_MissionType_ABC::ePhaseCDT );
    StopMission( strBehavior );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendFullState( NET_ASN_MsgAutomatAttributes& msg ) const
{
    msg().m.rapport_de_forcePresent    = 1;
    msg().m.roePresent                 = 1;
    msg().m.combat_de_rencontrePresent = 1;
    msg().m.etat_operationnelPresent   = 1;

    msg().rapport_de_force      = (ASN1T_EnumForceRatioStatus)eForceRatioStateNone;
    msg().combat_de_rencontre   = (ASN1T_EnumMeetingEngagementStatus)nCloseCombatState_;
    msg().etat_operationnel     = (ASN1T_EnumOperationalStatus)nOperationalState_;
    msg().roe                   = (ASN1T_EnumRoe)nRulesOfEngagementState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::SendChangedState( NET_ASN_MsgAutomatAttributes& msg ) const
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
// Name: DEC_AutomateDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::RegisterSelf( directia::Brain& brain )
{
    brain.RegisterObject( "myself", (DEC_Decision_ABC*)this );
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
// -----------------------------------------------------------------------------
MIL_Fuseau* DEC_AutomateDecision::Getfuseau() const
{
    throw;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_AutomateDecision::Setfuseau
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_AutomateDecision::Setfuseau( MIL_Fuseau* /*value*/ )
{
    throw;
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
