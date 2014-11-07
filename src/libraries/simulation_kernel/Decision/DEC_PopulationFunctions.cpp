// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AutomateFunctions.cpp $
// $Author: Jvt $
// $Modtime: 11/05/05 11:03 $
// $Revision: 20 $
// $Workfile: DEC_AutomateFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeFunctions.h"
#include "DEC_PopulationFunctions.h"
#include "DEC_FunctionsTools.h"
#include "Entities/MIL_Army.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Network/NET_Publisher_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/QueryValidity.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_AgentServer.h"
#include "protocol/ClientSenders.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"


void DEC_PopulationFunctions::ResetPionMaxSpeed( DEC_Decision_ABC* callerPopulation )
{
    callerPopulation->GetPopulation().ResetPionMaxSpeed();
}

void DEC_PopulationFunctions::SetPionMaxSpeed( DEC_Decision_ABC* callerPopulation, double speed )
{
    callerPopulation->GetPopulation().SetPionMaxSpeed( speed );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetAttitude( DEC_Decision_ABC* callerPopulation, unsigned int attitudeId )
{
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( attitudeId );
    assert( pAttitude );
    callerPopulation->GetPopulation().SetAttitude( *pAttitude );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetAttitude
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
unsigned int DEC_PopulationFunctions::GetAttitude( const DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetAttitude().GetID() ;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetUrbanDestructionState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetUrbanDestructionState( DEC_Decision_ABC* callerPopulation, bool state )
{
    callerPopulation->GetPopulation().SetUrbanDestructionState( state );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetUrbanDestructionState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::GetUrbanDestructionState( DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetUrbanDestructionState();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetDemonstrationState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetDemonstrationState( DEC_Decision_ABC* callerPopulation, bool state )
{
    callerPopulation->GetPopulation().SetDemonstrationState( state );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetDemonstrationState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::GetDemonstrationState( DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetDemonstrationState();
}

namespace
{
    class PopulationVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        explicit PopulationVisitor( std::vector< boost::shared_ptr< TER_Localisation > >& locations )
            : locations_ ( locations )
        {
            // NOTHING
        }

        void Visit( const MIL_PopulationElement_ABC& element )
        {
            boost::shared_ptr< TER_Localisation > location( new TER_Localisation( element.GetLocation() ) );
            locations_.push_back( location );
        }
    private:
        std::vector< boost::shared_ptr< TER_Localisation > >& locations_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetCurrentLocations
// Created: JCR 2010-09-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< TER_Localisation > > DEC_PopulationFunctions::GetCurrentLocations( const DEC_Decision_ABC* callerPopulation )
{
    std::vector< boost::shared_ptr< TER_Localisation > > locations;
    PopulationVisitor visitor( locations );

    callerPopulation->GetPopulation().Apply( visitor );
    return locations;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation( unsigned int agentId )
{
    const MIL_AgentPion* pAgent = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( agentId );
    if( !pAgent  )
        throw MASA_EXCEPTION( "invalid parameter." );
    return pAgent->GetRole< DEC_RolePion_Decision >().GetRoePopulation().GetID();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > > DEC_PopulationFunctions::GetObjectsInCircle
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_Knowledge_Object > > DEC_PopulationFunctions::GetObjectsInCircle( const DEC_Decision_ABC* caller, double radius, const std::vector< std::string >& parameters )
{
    TER_Localisation circle( *caller->GetPopulation().GetBarycenter(), radius );
    return DEC_KnowledgeFunctions::GetObjectsInZone( caller, &circle, parameters );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectLocalisation
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
const TER_Localisation* DEC_PopulationFunctions::GetKnowledgeObjectLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        return &pKnowledge->GetLocalisation();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasFlow
// Created: LGY 2010-12-27
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasFlow( const DEC_Decision_ABC* population )
{
    return population->GetPopulation().HasFlow();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DamageObject
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::DamageObject( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, double damageFactor )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return  (int)eQueryInvalid;
    MIL_Object_ABC* pObject = pKnowledge->GetObjectKnown();
    if( !pObject )
        return  (int)eQueryInvalid;
    (*pObject)().Destroy( damageFactor );
    return (int)eQueryValid;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectDistance
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
float DEC_PopulationFunctions::GetKnowledgeObjectDistance( const DEC_Decision_ABC* callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return std::numeric_limits< float >::max();
    return (float)callerPopulation->GetPopulation().GetDistanceTo( pKnowledge->GetLocalisation() ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint( const DEC_Decision_ABC* callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return boost::shared_ptr<MT_Vector2D>();
    else
    {
        MT_Vector2D* pPoint = new MT_Vector2D( callerPopulation->GetPopulation().GetClosestPoint( pKnowledge->GetLocalisation() ) );
        return boost::shared_ptr<MT_Vector2D>( pPoint );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::IsEnemy( const DEC_Decision_ABC* callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return int( eTristate_DontKnow );
    return int( pKnowledge->IsAnEnemy( callerPopulation->GetPopulation().GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::NotifyDominationStateChanged( DEC_Decision_ABC* callerPopulation, double dominationState )
{
    callerPopulation->GetPopulation().GetDecision().NotifyDominationStateChanged( dominationState );
}

unsigned int DEC_PopulationFunctions::GetDeadHumans( const DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetDeadHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetSzName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_PopulationFunctions::GetSzName( MIL_Population& callerPopulation )
{
    return callerPopulation.GetDecision().GetName();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetDominationState
// Created: LDC 2011-05-20
// -----------------------------------------------------------------------------
double DEC_PopulationFunctions::GetDominationState( DEC_Decision_ABC& callerPopulation )
{
    DEC_PopulationDecision* pop = dynamic_cast< DEC_PopulationDecision* >( &callerPopulation );
    return pop ? pop->GetDominationState() : 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission )
{
    if( !object )
        throw MASA_EXCEPTION( "invalid parameter." );
    object->SetMission( mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetBarycenter
// Created: MGD 2010-09-28
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PopulationFunctions::GetBarycenter( const DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetBarycenter();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetPopulationAngriness
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
double DEC_PopulationFunctions::GetUrbanBlockAngriness( const DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetUrbanBlockAngriness();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::ReintegrateUrbanBlock
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::ReintegrateUrbanBlock( DEC_Decision_ABC* callerPopulation )
{
    MIL_Population& population = callerPopulation->GetPopulation();
    population.GetOrderManager().CancelMission();
    population.ChangeComposition( 0, 0, 0, 0 );
}

void DEC_PopulationFunctions::HealWounded( DEC_Decision_ABC* callerPopulation )
{
    callerPopulation->GetPopulation().HealWounded();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasReachedDestination
// Created: NLD 2011-03-21
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasReachedDestination( const DEC_Decision_ABC* callerPopulation, const MT_Vector2D* destination )
{
    if( !destination )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerPopulation->GetPopulation().HasReachedDestination( *destination );
}
// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasReachedBlockBorder
// Created: DDA 2011-04-04
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasReachedBlockBorder( const DEC_Decision_ABC* callerPopulation, const MIL_UrbanObject_ABC* pUrbanKnowledge )
{
    if( !pUrbanKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerPopulation->GetPopulation().HasReachedBlockBorder( pUrbanKnowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasReachedDestinationCompletely
// Created: NLD 2011-03-21
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasReachedDestinationCompletely( const DEC_Decision_ABC* callerPopulation, const MT_Vector2D* destination )
{
    if( !destination )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerPopulation->GetPopulation().HasReachedDestinationCompletely( *destination );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetHealthyHumans
// Created: LMT 2011-05-27
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetHealthyHumans( const DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetHealthyHumans();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetContaminatedHumans
// Created: LMT 2011-11-18
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetContaminatedHumans( const DEC_Decision_ABC* callerPopulation )
{
    return callerPopulation->GetPopulation().GetContaminatedHumans();
}

std::vector< unsigned int > DEC_PopulationFunctions::GetPionsAttacking( const DEC_Decision_ABC* agent )
{
    return agent->GetPopulation().GetKnowledge().GetPionsAttacking();
}

std::vector< unsigned int > DEC_PopulationFunctions::GetPionsSecuring( const DEC_Decision_ABC* agent )
{
    return agent->GetPopulation().GetKnowledge().GetPionsSecuring();
}
