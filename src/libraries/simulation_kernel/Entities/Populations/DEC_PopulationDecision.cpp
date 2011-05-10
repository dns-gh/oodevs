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
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPion.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionUrbanDestruction.h"
#include <boost/serialization/vector.hpp>
#include <boost/lambda/lambda.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_PopulationDecision )

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_PopulationDecision::DEC_PopulationDecision( MIL_Population& population, unsigned int gcPause, unsigned int gcMult )
    : DEC_Decision( population, gcPause, gcMult )
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
    SetModel( pEntity_->GetType().GetModel() );
    StartDefaultBehavior();
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
    InitBrain( model.GetScriptFile(), model.GetName(), model.GetIncludePath(), "none", model.IsMasalife() );
    diaType_ = model.GetDIAType();
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
// Name: DEC_RolePion_Decision::RegisterUserArchetypeFunctions
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterUserArchetypeFunctions ( directia::brain::Brain& brain )
{
    // Knowledge objects
    brain[ "DEC_IsValidKnowledgeObject" ] = &DEC_PopulationFunctions::IsKnowledgeObjectValid;

    // Former szName_, mission_, automate_:
    brain[ "DEC_GetSzName" ] = &DEC_PopulationFunctions::GetSzName;
    brain[ "DEC_GetRawMission" ] = &DEC_PopulationFunctions::GetMission;

    // Time
    brain[ "DEC_GetTimeInSeconds" ] = &DEC_MiscFunctions::GetTimeInSeconds;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationDecision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_PopulationDecision::RegisterUserFunctions( directia::brain::Brain& brain )
{
    // Knowledge objects
    brain[ "DEC_ObjectKnowledgesInZone" ] =
        boost::function<  std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_PopulationFunctions::GetObjectsInZone, boost::cref( GetPopulation() ), _1, _2 ) );
    brain[ "DEC_ObjectKnowledgesInCircle" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( double, const std::vector< std::string >& ) >( boost::bind( &DEC_PopulationFunctions::GetObjectsInCircle, boost::ref( GetPopulation() ), _1, _2 ) );

    // Actions
    brain[ "DEC__StopAction" ] =
        boost::function< unsigned int ( unsigned int ) >( boost::bind( &DEC_ActionFunctions::StopAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_PauseAction" ] =
        boost::function< void ( unsigned int ) >( boost::bind( &DEC_ActionFunctions::SuspendAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_ReprendAction" ] =
        boost::function< void ( unsigned int ) >( boost::bind( &DEC_ActionFunctions::ResumeAction< MIL_Population >, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC__StartDeplacement" ] =
        boost::function< unsigned int( MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionMove, MT_Vector2D* >, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC__StartTirSurPions" ] =
        boost::function< unsigned int( float ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPions, float >, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC__StartTirSurPion" ] =
        boost::function< unsigned int( float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, unsigned int >, boost::ref( GetPopulation() ), _1, _2 ) );
    brain[ "DEC_StartTirSurPion" ] =
        boost::function< unsigned int( float, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, DEC_Decision_ABC* >, boost::ref( GetPopulation() ), _1, _2 ) );
    brain[ "DEC_DetruireBlocUrbain" ] =
        boost::function< void( UrbanObjectWrapper* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_Population_ActionUrbanDestruction, UrbanObjectWrapper* >, boost::ref( GetPopulation() ), _1 ) );
   brain[ "DEC_EtatBlocUrbain" ] =
        boost::function< float( UrbanObjectWrapper* )>( boost::bind( &DEC_UrbanObjectFunctions::GetStateUrbanBlock, _1 ) );

    // Self
    brain[ "DEC_GetPosition" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >() >( boost::bind( &DEC_PopulationFunctions::GetBarycenter, boost::cref( GetPopulation() ) ) );

    // Knowledge agents
    brain[ "DEC_ConnaissanceAgent_RoePopulation" ] =
        boost::function< int ( int ) > ( boost::bind(&DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation, _1 ) );
    brain[ "DEC_Connaissances_PionsPrenantAPartie" ] =
            boost::function< std::vector<unsigned int>() >(boost::bind(&DEC_PopulationKnowledge::GetPionsAttacking, boost::cref( GetPopulation().GetKnowledge() ) ) );
    brain[ "DEC_Connaissances_PionsSecurisant" ] =
            boost::function< std::vector<unsigned int>() >(boost::bind(&DEC_PopulationKnowledge::GetPionsSecuring, boost::cref( GetPopulation().GetKnowledge() ) ) );

    // Knowledge objects
    brain[ "DEC_ConnaissanceObjet_Localisation" ] = &DEC_PopulationFunctions::GetKnowledgeObjectLocalisation;
    brain[ "DEC_ConnaissanceObjet_Degrader" ] = &DEC_PopulationFunctions::DamageObject;
    brain[ "DEC_ConnaissanceObjet_Distance" ] =
            boost::function< float ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_PopulationFunctions::GetKnowledgeObjectDistance, boost::cref( GetPopulation() ), _1 ) );
    brain[ "DEC_ConnaissanceObjet_PointPlusProche" ] =
            boost::function< boost::shared_ptr< MT_Vector2D > ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint, boost::cref( GetPopulation() ) ,_1 ) );
    brain[ "DEC_ConnaissanceObjet_EstEnnemi" ] =
            boost::function< int ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_PopulationFunctions::IsEnemy, boost::ref( GetPopulation() ), _1 ) );

    // Debug
    brain[ "DEC_DebugAffichePoint"   ] =
            boost::function< void ( const MT_Vector2D* ) > (boost::bind(&DEC_MiscFunctions::DebugDrawPoint< MIL_Population >, boost::cref( GetPopulation()) , _1 ) );
    brain[ "DEC_DebugAffichePoints"  ] =
            boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind(&DEC_MiscFunctions::DebugDrawPoints< MIL_Population >, boost::cref( GetPopulation()), _1  ) );
    brain[ "DEC_Debug" ] =
            boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug< MIL_Population > , boost::cref( GetPopulation()) , "Population" , _1  ) );
    brain[ "DEC_Trace" ] =
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace< MIL_Population >, boost::cref( GetPopulation() ), _1 ) );
    brain[ "DEC_DecisionalState" ] =
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_PopulationFunctions::DecisionalState, boost::cref( GetPopulation() ), _1, _2 ) );

    // Effects
    brain[ "DEC_Population_RalentissementPion_ChangeVitesse" ] =
        boost::function< void ( double ) >(boost::bind( &MIL_Population::SetPionMaxSpeed, boost::ref( GetPopulation() ), _1) );
    brain[ "DEC_Population_RalentissementPion_VitesseParDefaut" ] =
        boost::bind( &MIL_Population::ResetPionMaxSpeed,  boost::ref( GetPopulation() ) );
    brain[ "DEC_Population_ChangerAttitude" ] =
        boost::function< void ( int ) >(boost::bind( &DEC_PopulationFunctions::SetAttitude, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_Population_Attitude" ] =
        boost::function< int() >(boost::bind( &DEC_PopulationFunctions::GetAttitude, boost::ref( GetPopulation() ) ) );
    brain[ "DEC_Population_Positions" ] =
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >() >(boost::bind( &DEC_PopulationFunctions::GetCurrentLocations, boost::cref( GetPopulation() ) ) );

    // Move
    brain[ "DEC_IsPointInUrbanBlockTrafficable" ] = boost::function< bool( MT_Vector2D&, bool ) >( boost::lambda::constant( true ) ); // $$$$ _RC_ LGY 2010-12-27: Pour une population, un point dans un block urbain est toujours accessible?
    brain[ "DEC_Agent_NiveauInstallation" ] = boost::bind( &DEC_PopulationFunctions::GetMovingState, boost::ref( GetPopulation() ) );
    brain[ "DEC_HasFlow" ] = boost::function< bool() >( boost::bind( &DEC_PopulationFunctions::HasFlow, boost::ref( GetPopulation() ) ) );
    brain[ "DEC_Population_HasReachedBlockBorder" ] = boost::function< bool( const UrbanObjectWrapper* ) >( boost::bind( &DEC_PopulationFunctions::HasReachedBlockBorder, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_Population_HasReachedDestination" ] = boost::function< bool( const MT_Vector2D* ) >( boost::bind( &DEC_PopulationFunctions::HasReachedDestination, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_Population_HasReachedDestinationCompletely" ] = boost::function< bool( const MT_Vector2D* ) >( boost::bind( &DEC_PopulationFunctions::HasReachedDestinationCompletely, boost::ref( GetPopulation() ), _1 ) );

    // Etats decisionnel
    brain[ "DEC_Population_ChangeEtatDomination" ] =
        boost::function< void( double ) >( boost::bind( &DEC_PopulationFunctions::NotifyDominationStateChanged, boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_Population_Morts" ] =
        boost::function< unsigned int() >( boost::bind( &MIL_Population::GetDeadHumans, boost::ref( GetPopulation() ) ) );

    // Representations
    brain[ "DEC_GetOrdersCategory" ] =
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( GetPopulation() ) );
    brain[ "DEC_GetPointsCategory" ] =
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( GetPopulation() ) );
    brain[ "DEC_RemoveFromOrdersCategory" ] =
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_DeleteRepresentation" ] =
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPopulation() ), _1 ) );
    brain[ "DEC_RemoveFromPointsCategory" ] =
        boost::function< void( boost::shared_ptr< DEC_PathPoint > )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( GetPopulation() ), _1 ) );

    // Former szName_, mission_, automate_:
    brain[ "DEC_SetMission" ] =
        boost::function< void ( DEC_Decision_ABC*, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_PopulationFunctions::SetMission, _1, _2 ) );
    brain[ "DEC_FinMission" ] = boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_Population >, boost::ref( GetPopulation() ) );

    //Security
    brain[ "DEC_GetUrbanBlockAngriness" ] =
        boost::function< double() >( boost::bind( &DEC_PopulationFunctions::GetUrbanBlockAngriness, boost::ref( GetPopulation() ) ) );
    brain[ "DEC_ReintegrateUrbanBlock" ] = boost::function< void() >( boost::bind( &DEC_PopulationFunctions::ReintegrateUrbanBlock, boost::ref( GetPopulation() ) ) );
}

/*
// -----------------------------------------------------------------------------
// Fonctions appelées seulement dans DEC_RolePion_Decision
// -----------------------------------------------------------------------------

int DEC_PopulationDecision::GeteEtatDecPrudence() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatDecPrudence(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatNbc() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatNbc(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatDestruction() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatDestruction(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatFeu() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatFeu(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatAmbiance() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatAmbiance(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatRadio() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatRadio(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatRadar() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatRadar(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatDeplacement() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatDeplacement(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatOrdreCoordination() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatOrdreCoordination(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteConsigneTir() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteConsigneTir(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteConsigneTirPopulation() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteConsigneTirPopulation(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatSoutien() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatSoutien(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteEtatSituationEnnemi() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteEtatSituationEnnemi(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteTypeContact() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteTypeContact(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

int DEC_PopulationDecision::GeteNiveauAction() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

void DEC_PopulationDecision::SeteNiveauAction(int value )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}
*/


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
void DEC_PopulationDecision::RegisterSelf( directia::brain::Brain& brain, bool isMasalife, const std::string& groupName )
{
    brain[ "myself" ] = (DEC_Decision_ABC*)this;
    if( isMasalife )
        brain[ "InitMeCrowd" ](  brain[ "integration.ontology.types.body" ], brain[ "myself" ], groupName );
}