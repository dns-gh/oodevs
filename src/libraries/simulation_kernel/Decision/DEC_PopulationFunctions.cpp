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


// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::DecisionalState( const MIL_Population& callerPopulation, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().mutable_source()->mutable_crowd()->set_id( callerPopulation.GetID() );
    msg().set_key( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetAttitude( MIL_Population& callerPopulation, unsigned int attitudeId )
{
    const MIL_PopulationAttitude* pAttitude = MIL_PopulationAttitude::Find( attitudeId );
    assert( pAttitude );
    callerPopulation.SetAttitude( *pAttitude );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetAttitude
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
unsigned int DEC_PopulationFunctions::GetAttitude( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetAttitude().GetID() ;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetUrbanDestructionState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetUrbanDestructionState( MIL_Population& callerPopulation, bool state )
{
    callerPopulation.SetUrbanDestructionState( state );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetUrbanDestructionState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::GetUrbanDestructionState( MIL_Population& callerPopulation )
{
    return callerPopulation.GetUrbanDestructionState();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetDemonstrationState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetDemonstrationState( MIL_Population& callerPopulation, bool state )
{
    callerPopulation.SetDemonstrationState( state );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetDemonstrationState
// Created: NPT 2013-06-05
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::GetDemonstrationState( MIL_Population& callerPopulation )
{
    return callerPopulation.GetDemonstrationState();
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
std::vector< boost::shared_ptr< TER_Localisation > > DEC_PopulationFunctions::GetCurrentLocations( const MIL_Population& callerPopulation )
{
    std::vector< boost::shared_ptr< TER_Localisation > > locations;
    PopulationVisitor visitor( locations );

    callerPopulation.Apply( visitor );
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
// Name: DEC_PopulationFunctions::IsAgentInside
// Created: DDA 2011-05-17
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::IsAgentInside( const MIL_Population& caller, DEC_Decision_ABC* pAgent )
{
    if( !pAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_AgentPion& pion = pAgent->GetPion() ;
    return pion.Get< PHY_RoleInterface_Population >().HasCollisionWithCrowd( caller );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetObjectsInZone
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_Knowledge_Object > > DEC_PopulationFunctions::GetObjectsInZone( const MIL_Population& caller, const TER_Localisation* pZone, const std::vector< std::string >& parameters )
{
    typedef std::vector< boost::shared_ptr< DEC_Knowledge_Object > >::iterator IT_KnowledgeObject;

    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( parameters );
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > knowledges; //T_KnowledgeObjectDiaIDVector

    auto knowledgeGroups = caller.GetArmy().GetKnowledgeGroups();
    for( auto it = knowledgeGroups.begin(); it != knowledgeGroups.end(); ++it )
    {
        if( it->second->IsJammed() )
            continue;
        auto bbKg = it->second->GetKnowledge();
        if( bbKg )
        {
            std::vector< boost::shared_ptr< DEC_Knowledge_Object > > knowledgesTmp; //T_KnowledgeObjectDiaIDVector
            bbKg->GetKnowledgeObjectContainer().GetObjectsInZone( knowledgesTmp, filter, *pZone );
            for( auto it = knowledgesTmp.begin(); it != knowledgesTmp.end(); ++it )
                if( *it && (*it)->IsValid() )
                    knowledges.push_back( *it );
        }
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Object > > DEC_PopulationFunctions::GetObjectsInCircle
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_Knowledge_Object > > DEC_PopulationFunctions::GetObjectsInCircle( const MIL_Population& caller, double radius, const std::vector< std::string >& parameters )
{
    TER_Localisation circle( *caller.GetBarycenter(), radius );
    return GetObjectsInZone( caller, &circle, parameters );
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
bool DEC_PopulationFunctions::HasFlow( MIL_Population& population )
{
    return population.HasFlow();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetMovingState
// Created: LGY 2010-12-27
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetMovingState( MIL_Population& population )
{
    return HasFlow( population ) ? PHY_Posture::mouvement_.GetID() : PHY_Posture::poste_.GetID();
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
float DEC_PopulationFunctions::GetKnowledgeObjectDistance( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return std::numeric_limits< float >::max();
    return (float)callerPopulation.GetDistanceTo( pKnowledge->GetLocalisation() ) ;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return boost::shared_ptr<MT_Vector2D>();
    else
    {
        MT_Vector2D* pPoint = new MT_Vector2D( callerPopulation.GetClosestPoint( pKnowledge->GetLocalisation() ) );
        return boost::shared_ptr<MT_Vector2D>( pPoint );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::IsEnemy( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge )
{
    if( !( pKnowledge && pKnowledge->IsValid() ) )
        return int( eTristate_DontKnow );
    return int( pKnowledge->IsAnEnemy( callerPopulation.GetArmy() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::NotifyDominationStateChanged( MIL_Population& callerPopulation, double dominationState )
{
    callerPopulation.GetDecision().NotifyDominationStateChanged( dominationState );
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
boost::shared_ptr< MT_Vector2D > DEC_PopulationFunctions::GetBarycenter( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetBarycenter();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetPopulationAngriness
// Created: BCI 2011-03-18
// -----------------------------------------------------------------------------
double DEC_PopulationFunctions::GetUrbanBlockAngriness( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetUrbanBlockAngriness();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::ReintegrateUrbanBlock
// Created: BCI 2011-03-21
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::ReintegrateUrbanBlock( MIL_Population& callerPopulation )
{
    callerPopulation.GetOrderManager().CancelMission();
    callerPopulation.ChangeComposition( 0, 0, 0, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasReachedDestination
// Created: NLD 2011-03-21
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasReachedDestination( const MIL_Population& callerPopulation, const MT_Vector2D* destination )
{
    if( !destination )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerPopulation.HasReachedDestination( *destination );
}
// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasReachedBlockBorder
// Created: DDA 2011-04-04
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasReachedBlockBorder( const MIL_Population& callerPopulation, const MIL_UrbanObject_ABC* pUrbanKnowledge )
{
    if( !pUrbanKnowledge )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerPopulation.HasReachedBlockBorder( pUrbanKnowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::HasReachedDestinationCompletely
// Created: NLD 2011-03-21
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::HasReachedDestinationCompletely( const MIL_Population& callerPopulation, const MT_Vector2D* destination )
{
    if( !destination )
        throw MASA_EXCEPTION( "invalid parameter." );
    return callerPopulation.HasReachedDestinationCompletely( *destination );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetActualNumber
// Created: LMT 2011-05-27
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetActualNumber( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetHealthyHumans();
}
// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetContaminatedHumans
// Created: LMT 2011-11-18
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetContaminatedHumans( const MIL_Population& callerPopulation )
{
    return callerPopulation.GetContaminatedHumans();
}
