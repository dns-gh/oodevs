// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:31 $
// $Revision: 20 $
// $Workfile: DEC_PopulationDecision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PopulationDecision.h"
#include "MIL_PopulationType.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_PopulationFunctions.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMoveAlong.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPion.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionBattle.h"
#include "Entities/Populations/Actions/PHY_Population_ActionUrbanDestruction.h"
#include "protocol/ClientSenders.h"
#include <boost/lambda/lambda.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_PopulationDecision )

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision( MIL_Population& population, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : DEC_Decision( population, gcPause, gcMult, logger )
    , rDominationState_    ( 0. )
    , rLastDominationState_( 0. )
    , bStateHasChanged_    ( true )
{
    const DEC_Model_ABC& model = population.GetType().GetModel();
    SetModel( model );
    //GetVariable( nDIANameIdx_    ).SetValue( population.GetName() ); // $$$$ LDC: FIXME Use member data
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_PopulationDecision::~DEC_PopulationDecision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Decision< MIL_Population  > >( *this );
    file >> pEntity_
         >> rDominationState_
         >> rLastDominationState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pEntity_ );
    file << boost::serialization::base_object< DEC_Decision< MIL_Population  > >( *this )
         << pEntity_
         << rDominationState_
         << rLastDominationState_;
}

void DEC_PopulationDecision::SetModel( const DEC_Model_ABC& model )
{
    model_ = &model;
    InitBrain( model.GetScriptFile(), model.GetDIAType(), model.GetIncludePath(),
               GetGroupName(), model.IsMasalife(), false, model.GetIntegrationDir() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::EndCleanStateAfterCrash()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetGroupName
// Created: LDC 2011-09-01
// -----------------------------------------------------------------------------
std::string DEC_PopulationDecision::GetGroupName()
{
    return "none";
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterUserArchetypeFunctions
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterUserArchetypeFunctions ( sword::Brain& brain )
{
    // Knowledge objects
    brain.RegisterFunction( "DEC_IsValidKnowledgeObject", &DEC_PopulationFunctions::IsKnowledgeObjectValid );

    // Former szName_, mission_, automate_:
    brain.RegisterFunction( "DEC_GetSzName", &DEC_PopulationFunctions::GetSzName );
    brain.RegisterFunction( "DEC_GetRawMission", &DEC_PopulationFunctions::GetMission );

    // Time
    brain.RegisterFunction( "DEC_GetTimeInSeconds", &DEC_MiscFunctions::GetTimeInSeconds );

    brain.RegisterFunction( "DEC_GetDomination", &DEC_PopulationFunctions::GetDominationState );

    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurPion", &DEC_KnowledgeAgentFunctions::GetDangerosityOnPion );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurConnaissance", &DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge );
    brain.RegisterFunction( "DEC_Agent_RapportDeForceLocal", &DEC_AgentFunctions::GetRapForLocalAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterUserFunctions( sword::Brain& brain )
{
    // Knowledge objects
    brain.RegisterFunction( "DEC_ObjectKnowledgesInZone",
        boost::function<  std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_PopulationFunctions::GetObjectsInZone, boost::cref( GetPopulation() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ObjectKnowledgesInCircle",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( double, const std::vector< std::string >& ) >( boost::bind( &DEC_PopulationFunctions::GetObjectsInCircle, boost::ref( GetPopulation() ), _1, _2 ) ) );

    // Actions
    brain.RegisterFunction( "DEC__StopAction",
        boost::function< unsigned int ( unsigned int ) >( boost::bind( &DEC_ActionFunctions::StopAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_PauseAction",
        boost::function< void ( unsigned int ) >( boost::bind( &DEC_ActionFunctions::SuspendAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ReprendAction",
        boost::function< void ( unsigned int ) >( boost::bind( &DEC_ActionFunctions::ResumeAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartDeplacement",
        boost::function< unsigned int( MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionMove, MT_Vector2D* >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_StartDeplacementItineraire",
        boost::function< unsigned int( std::vector< boost::shared_ptr< MT_Vector2D > > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionMoveAlong, std::vector< boost::shared_ptr< MT_Vector2D > > >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartTirSurPions",
        boost::function< unsigned int( float ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPions, float >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartTirSurPion",
        boost::function< unsigned int( float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, unsigned int >, boost::ref( GetPopulation() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_StartTirSurPion",
        boost::function< unsigned int( float, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, DEC_Decision_ABC* >, boost::ref( GetPopulation() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_DetruireBlocUrbain",
        boost::function< unsigned int( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionUrbanDestruction, MIL_UrbanObject_ABC* >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_EtatBlocUrbain",
        boost::function< float( MIL_UrbanObject_ABC* )>( boost::bind( &DEC_UrbanObjectFunctions::GetStateUrbanBlock, _1 ) ) );
    brain.RegisterFunction( "DEC_StartAgresserFoule",
        boost::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionBattle >, boost::ref( GetPopulation() ) ) ) );

    // Self
    brain.RegisterFunction( "DEC_GetPosition",
        boost::function< boost::shared_ptr< MT_Vector2D >() >( boost::bind( &DEC_PopulationFunctions::GetBarycenter, boost::cref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_GetNombrePersonne",
        boost::function< int () >( boost::bind( &DEC_PopulationFunctions::GetActualNumber, boost::cref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_GetNombrePersonneContaminee",
        boost::function< int () >( boost::bind( &DEC_PopulationFunctions::GetContaminatedHumans, boost::cref( GetPopulation() ) ) ) );

    // Agents
    brain.RegisterFunction( "DEC_Agent_EstDansFoule",
        boost::function< bool(  DEC_Decision_ABC* ) >( boost::bind( &DEC_PopulationFunctions::IsAgentInside, boost::ref( GetPopulation() ), _1 ) ) );

    // Orders
    brain.RegisterFunction( "DEC_AssignMissionCrowdParameter",
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC >, const std::string&, int ) >( boost::bind( &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter, this, _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_AssignMissionCrowdListParameter",
        boost::function< boost::shared_ptr<MIL_MissionParameter_ABC>( int ) >( boost::bind( &MIL_MissionParameterFactory::CreatePopulationKnowledge, this, _1 ) ) );

    // Knowledge agents
    brain.RegisterFunction( "DEC_ConnaissanceAgent_RoePopulation",
        boost::function< int ( int ) > ( boost::bind(&DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation, _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PionsPrenantAPartie",
            boost::function< std::vector<unsigned int>() >(boost::bind(&DEC_PopulationKnowledge::GetPionsAttacking, boost::cref( GetPopulation().GetKnowledge() ) ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PionsSecurisant",
            boost::function< std::vector<unsigned int>() >(boost::bind(&DEC_PopulationKnowledge::GetPionsSecuring, boost::cref( GetPopulation().GetKnowledge() ) ) ) );

    // Knowledge objects
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Localisation", &DEC_PopulationFunctions::GetKnowledgeObjectLocalisation );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Degrader", &DEC_PopulationFunctions::DamageObject );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Distance",
            boost::function< float ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_PopulationFunctions::GetKnowledgeObjectDistance, boost::cref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PointPlusProche",
            boost::function< boost::shared_ptr< MT_Vector2D > ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint, boost::cref( GetPopulation() ) ,_1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstEnnemi",
            boost::function< int ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_PopulationFunctions::IsEnemy, boost::ref( GetPopulation() ), _1 ) ) );

    // Debug
    brain.RegisterFunction( "DEC_DebugAffichePoint"  ,
            boost::function< void ( const MT_Vector2D* ) > (boost::bind(&DEC_MiscFunctions::DebugDrawPoint, boost::cref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DebugAffichePoints" ,
            boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind(&DEC_MiscFunctions::DebugDrawPoints, boost::cref( GetPopulation() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Debug",
            boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug, boost::cref( GetPopulation() ) , "Population" , _1  ) ) );
    brain.RegisterFunction( "DEC_Trace",
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace, boost::cref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DecisionalState",
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_PopulationFunctions::DecisionalState, boost::cref( GetPopulation() ), _1, _2 ) ) );

    // Effects
    brain.RegisterFunction( "DEC_Population_RalentissementPion_ChangeVitesse",
        boost::function< void ( double ) >(boost::bind( &MIL_Population::SetPionMaxSpeed, boost::ref( GetPopulation() ), _1) ) );
    brain.RegisterFunction( "DEC_Population_RalentissementPion_VitesseParDefaut",
        boost::bind( &MIL_Population::ResetPionMaxSpeed,  boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_Population_ChangerAttitude",
        boost::function< void ( int ) >(boost::bind( &DEC_PopulationFunctions::SetAttitude, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_Attitude",
        boost::function< int() >(boost::bind( &DEC_PopulationFunctions::GetAttitude, boost::ref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_Population_Positions",
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >() >(boost::bind( &DEC_PopulationFunctions::GetCurrentLocations, boost::cref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_Population_ChangeUrbanDestructionState",
        boost::function< void ( bool ) >(boost::bind( &DEC_PopulationFunctions::SetUrbanDestructionState, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_UrbanDestructionState",
        boost::function< bool() >(boost::bind( &DEC_PopulationFunctions::GetUrbanDestructionState, boost::ref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_Population_ChangeDemonstrationState",
        boost::function< void ( bool ) >(boost::bind( &DEC_PopulationFunctions::SetDemonstrationState, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_DemonstrationState",
        boost::function< bool() >(boost::bind( &DEC_PopulationFunctions::GetDemonstrationState, boost::ref( GetPopulation() ) ) ) );

    // Move
    brain.RegisterFunction( "DEC_Agent_NiveauInstallation", boost::bind( &DEC_PopulationFunctions::GetMovingState, boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_HasFlow", boost::function< bool() >( boost::bind( &DEC_PopulationFunctions::HasFlow, boost::ref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_Population_HasReachedBlockBorder", boost::function< bool( const MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_PopulationFunctions::HasReachedBlockBorder, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_HasReachedDestination", boost::function< bool( const MT_Vector2D* ) >( boost::bind( &DEC_PopulationFunctions::HasReachedDestination, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_HasReachedDestinationCompletely", boost::function< bool( const MT_Vector2D* ) >( boost::bind( &DEC_PopulationFunctions::HasReachedDestinationCompletely, boost::ref( GetPopulation() ), _1 ) ) );

    // Etats decisionnel
    brain.RegisterFunction( "DEC_Population_ChangeEtatDomination",
        boost::function< void( double ) >( boost::bind( &DEC_PopulationFunctions::NotifyDominationStateChanged, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_Morts",
        boost::function< unsigned int() >( boost::bind( &MIL_Population::GetDeadHumans, boost::ref( GetPopulation() ) ) ) );

    // Representations
    brain.RegisterFunction( "DEC_GetOrdersCategory",
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_GetPointsCategory",
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_RemoveFromOrdersCategory",
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DeleteRepresentation",
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_RemoveFromPointsCategory",
        boost::function< void( boost::shared_ptr< DEC_PathPoint > )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetPopulation() ), _1 ) ) );

    // Former szName_, mission_, automate_:
    brain.RegisterFunction( "DEC_SetMission",
        boost::function< void ( DEC_Decision_ABC*, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_PopulationFunctions::SetMission, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_FinMission", boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_Population >, boost::ref( GetPopulation() ) ) );

    //Security
    brain.RegisterFunction( "DEC_GetUrbanBlockAngriness",
        boost::function< double() >( boost::bind( &DEC_PopulationFunctions::GetUrbanBlockAngriness, boost::ref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_ReintegrateUrbanBlock", boost::function< void() >( boost::bind( &DEC_PopulationFunctions::ReintegrateUrbanBlock, boost::ref( GetPopulation() ) ) ) );
    brain.RegisterFunction( "DEC_Population_HealWounded",
        boost::function< void() >( boost::bind( &MIL_Population::HealWounded, boost::ref( GetPopulation() ) ) ) );

    // nbc
    brain.RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontamination",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::PopulationQueueForDecontamination, boost::ref( GetPopulation() ), _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior();
    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    const std::string& strBehavior = mission->GetType().GetDIABehavior();
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendFullState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendFullState( client::CrowdUpdate& msg ) const
{
    msg().set_domination( static_cast< unsigned int >( rDominationState_ * 100. ) );
    msg().set_decisional_model( model_->GetName() );
    msg().set_brain_debug( brainDebug_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendChangedState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendChangedState( client::CrowdUpdate& msg )
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
double DEC_PopulationDecision::GetDominationState() const
{
    return rDominationState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetPopulation
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_Population& DEC_PopulationDecision::GetPopulation() const
{
    assert( pEntity_ );
    return *pEntity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::NotifyDominationStateChanged( double rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rDominationState_ == rValue )
        return;

    rDominationState_ = rValue;

    static const double rDeltaPercentageForNetwork = 0.05;
    if( fabs( rLastDominationState_ - rDominationState_ ) > rDeltaPercentageForNetwork || rDominationState_ == 0. || rDominationState_ == 1. )
    {
        bStateHasChanged_ = true;
        rLastDominationState_ = rDominationState_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Clean
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::HasStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
bool DEC_PopulationDecision::HasStateChanged() const
{
    return bStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDecAutomate
// Created: LDC 2009-04-10
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_PopulationDecision::GetDecAutomate()
{
    throw MASA_EXCEPTION( "DEC_GetAutomate unexpected on population" );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_PopulationDecision::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetOrderManager
// Created: NMI 2013-11-15
// -----------------------------------------------------------------------------
MIL_OrderManager_ABC& DEC_PopulationDecision::GetOrderManager() const
{
    return GetPopulation().GetOrderManager();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterSelf( sword::Brain& brain )
{
    brain.GetScriptRef( "myself" ) = (DEC_Decision_ABC*)this;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterSpecific
// Created: JSR 2013-02-18
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName )
{
    if( isMasalife )
        brain.GetScriptRef( "InitMeCrowd" )(
            brain.GetScriptRef( "integration.ontology.types.body" ),
            brain.GetScriptRef( "myself" ),
            groupName );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::Reload
// Created: AHC 2012-01-25
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::Reload( bool doInitBrain )
{
    DEC_Decision< MIL_Population >::Reload( doInitBrain );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::ActivateBrainDebug
// Created: SLI 2013-06-21
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::ActivateBrainDebug( bool activate )
{
    DEC_Decision< MIL_Population >::ActivateBrainDebug( activate );
    bStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::PostStartMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::PostStartMission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::PostStopMission
// Created: MMC 2013-10-15
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::PostStopMission()
{
    // NOTHING
}

