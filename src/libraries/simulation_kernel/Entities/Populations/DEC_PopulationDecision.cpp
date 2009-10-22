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
#include "MT_Tools/MT_CrashHandler.h"
#include "Network/NET_ASN_Messages.h"
#include "DIA/DIA_Script_Exception.h"
#include "DIA/DIA_Internal_Exception.h"

#include "Decision/Functions/DEC_PopulationFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPion.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPions.h"


BOOST_CLASS_EXPORT_GUID( DEC_PopulationDecision, "DEC_PopulationDecision" )

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision( MIL_Population& population )
    : DEC_Decision             ( population )
    , rDominationState_        ( 0. )
    , rLastDominationState_    ( 0. )
    , bStateHasChanged_        ( true )
{
    const DEC_Model_ABC& model = population.GetType().GetModel();
    try
    {
        SetModel( model );
        //GetVariable( nDIANameIdx_    ).SetValue( population.GetName() ); // $$$$ LDC: FIXME Use member data
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision()
    : DEC_Decision             () 
    , rDominationState_        ( 0. )
    , rLastDominationState_    ( 0. )
    , bStateHasChanged_        ( true )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_PopulationDecision::~DEC_PopulationDecision()
{
    MT_DELETEOWNED( missionBehaviorParameters_.GetParameters() );
    missionBehaviorParameters_.GetParameters().clear();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> pEntity_
         >> rDominationState_
         >> rLastDominationState_;
   
    const DEC_Model_ABC& model = pEntity_->GetType().GetModel();
    
    try
    {
        SetModel( model );
    }
    catch( DIA_Internal_Exception& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.GetExceptionMessage() );
    }

    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pEntity_ );
    
    file << pEntity_
         << rDominationState_
         << rLastDominationState_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::EndCleanStateAfterCrash
// Created: NLD 2005-06-23
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::EndCleanStateAfterCrash()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterUserFunctions( directia::Brain& brain )
{
    // Actions
    brain.RegisterFunction( "DEC_StopAction",
        boost::function< PHY_Action_ABC* (PHY_Action_ABC*) >( boost::bind( &DEC_ActionFunctions::StopAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_PauseAction",
        boost::function< void (PHY_Action_ABC*) >( boost::bind( &DEC_ActionFunctions::SuspendAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ReprendAction",
        boost::function< void (PHY_Action_ABC*) >( boost::bind( &DEC_ActionFunctions::ResumeAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartDeplacement",
        boost::function< PHY_Action_ABC*( MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionMove, MT_Vector2D* >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartTirSurPions",
        boost::function< PHY_Action_ABC*( float ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPions, float >, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartTirSurPion",
        boost::function< PHY_Action_ABC*( float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, unsigned int >, boost::ref( GetPopulation() ), _1, _2 ) ) );

    // Knowledge agents
    brain.RegisterFunction( "DEC_ConnaissanceAgent_RoePopulation",
        boost::function< int ( int ) > ( boost::bind(&DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation, _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PionsPrenantAPartie",
    		boost::function< std::vector<unsigned int>() >(boost::bind(&DEC_PopulationKnowledge::GetPionsAttacking, boost::cref( GetPopulation().GetKnowledge() ) ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PionsSecurisant",
    		boost::function< std::vector<unsigned int>() >(boost::bind(&DEC_PopulationKnowledge::GetPionsSecuring, boost::cref( GetPopulation().GetKnowledge() ) ) ) );

    // Knowledge objects
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Localisation",
        boost::function< boost::shared_ptr< MT_Vector2D > ( int ) > ( boost::bind(&DEC_PopulationFunctions::GetKnowledgeObjectLocalisation , _1) ) );
    brain.RegisterFunction( "DEC_IsValidKnowledgeObject",       &DEC_PopulationFunctions::IsKnowledgeObjectValid         );
    brain.RegisterFunction( "DEC_ObjectKnowledgesInZone",            &DEC_PopulationFunctions::GetObjectsInZone               );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Degrader",
    		boost::function< int ( int , float )> ( boost::bind(&DEC_PopulationFunctions::DamageObject, _1, _2)      ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Distance",
    		boost::function< float ( int )> ( boost::bind(&DEC_PopulationFunctions::GetKnowledgeObjectDistance, boost::cref(GetPopulation()), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PointPlusProche",
    		boost::function< boost::shared_ptr< MT_Vector2D > ( int ) > ( boost::bind (&DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint, boost::cref( GetPopulation() ) , _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstEnnemi",
    		boost::function< int ( int ) > ( boost::bind(&DEC_PopulationFunctions::IsEnemy, boost::ref( GetPopulation() ), _1 ) ) );

    // Debug
    brain.RegisterFunction( "DEC_DebugAffichePoint"  ,
    		boost::function< void ( const MT_Vector2D* ) > (boost::bind(&DEC_MiscFunctions::DebugDrawPoint< MIL_Population >, boost::cref( GetPopulation()) , _1 ) ) );
    brain.RegisterFunction( "DEC_DebugAffichePoints" ,
    		boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind(&DEC_MiscFunctions::DebugDrawPoints< MIL_Population >, boost::cref( GetPopulation()), _1  ) ) );
    brain.RegisterFunction( "DEC_Debug",
    		boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug< MIL_Population > , boost::cref( GetPopulation()) , "Population" , _1  ) ) );
    brain.RegisterFunction( "DEC_Trace",
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace< MIL_Population >, boost::cref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DecisionalState",
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_PopulationFunctions::DecisionalState, boost::cref( GetPopulation() ), _1, _2 ) ) );

    // RC
    brain.RegisterFunction( "DEC_RC1",
        boost::function< void ( int, int ) >( boost::bind( &DEC_MiscFunctions::Report< MIL_Population >, boost::ref( GetPopulation() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_RC_AgentKnowledge",
        boost::function< void ( int, int, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledge< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_DotationType",
        boost::function< void ( int, int, const PHY_DotationCategory* ) >( boost::bind( &DEC_MiscFunctions::ReportDotationType< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_EquipmentType",
        boost::function< void ( int, int, const PHY_ComposanteTypePion* ) >( boost::bind( &DEC_MiscFunctions::ReportEquipmentType< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Float",
        boost::function< void ( int, int, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloat< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Float_Float",
        boost::function< void ( int, int, float, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloatFloat< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Id",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportId< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_ObjectKnowledge",
        boost::function< void ( int, int, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MiscFunctions::ReportObjectKnoweldge< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion",
        boost::function< void ( int, int, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPion< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Automate",
        boost::function< void ( int, int, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionAutomate< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_TirPion",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportTirPion< MIL_Population >, boost::ref( GetPopulation() ), _1, _2, _3 ) ) );

    // Effects
    brain.RegisterFunction( "DEC_Population_RalentissementPion_ChangeVitesse",
        boost::function< void ( MT_Float ) >(boost::bind( &MIL_Population::SetPionMaxSpeed, boost::ref( GetPopulation() ), _1) ) );
    brain.RegisterFunction( "DEC_Population_RalentissementPion_VitesseParDefaut",
    	boost::bind( &MIL_Population::ResetPionMaxSpeed,  boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_Population_ChangerAttitude",
        boost::function< void ( int ) >(boost::bind( &DEC_PopulationFunctions::SetAttitude, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_Attitude",
        boost::function< int() >(boost::bind( &DEC_PopulationFunctions::GetAttitude, boost::ref( GetPopulation() ) ) ) );

    // Etats decisionnel
    brain.RegisterFunction( "DEC_Population_ChangeEtatDomination", 
        boost::function<void (MT_Float)>(boost::bind(&DEC_PopulationFunctions::NotifyDominationStateChanged, boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Population_Morts",                
        boost::function<float()>( boost::bind(&MIL_Population::GetNbrDeadHumans, boost::ref(GetPopulation()) ) ) );

    // Representations
    brain.RegisterFunction( "DEC_GetOrdersCategory",
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_GetPointsCategory",
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetPopulation() ) ) );
    brain.RegisterFunction( "DEC_RemoveFromOrdersCategory",
        boost::function< void ( MIL_FragOrder* ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DeleteRepresentation",
        boost::function< void ( MIL_FragOrder* ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPopulation() ), _1 ) ) );
    brain.RegisterFunction( "DEC_RemoveFromPointsCategory",
        boost::function< void( DEC_PathPoint* )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetPopulation() ), _1 ) ) );
    
    // Former szName_, mission_, automate_:
    brain.RegisterFunction( "DEC_GetSzName",             &DEC_PopulationFunctions::GetSzName       );
    brain.RegisterFunction( "DEC_GetRawMission",         &DEC_PopulationFunctions::GetMission            );
    brain.RegisterFunction( "DEC_SetMission",
        boost::function< void ( DEC_Decision_ABC*, MIL_Mission_ABC* ) >( boost::bind( &DEC_PopulationFunctions::SetMission, _1, _2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StartMissionBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();

    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::StopMissionBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendFullState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendFullState( NET_ASN_MsgPopulationUpdate& msg ) const
{
    msg().m.etat_dominationPresent = 1;
    msg().etat_domination          = (uint)( rDominationState_ * 100. );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::SendChangedState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::SendChangedState( NET_ASN_MsgPopulationUpdate& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
MT_Float DEC_PopulationDecision::GetDominationState() const
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
void DEC_PopulationDecision::NotifyDominationStateChanged( MT_Float rValue )
{
    assert( rValue >= 0. && rValue <= 1. );
    if( rDominationState_ == rValue )
        return;

    rDominationState_ = rValue;

    static const MT_Float rDeltaPercentageForNetwork = 0.05;
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
DEC_AutomateDecision* DEC_PopulationDecision::GetDecAutomate() const
{
    assert( false );
    throw std::runtime_error( "DEC_GetAutomate unexpected on population" );
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
// Name: DEC_PopulationDecision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterSelf( directia::Brain& brain )
{
    brain.RegisterObject( "myself", (DEC_Decision_ABC*)this );
}
