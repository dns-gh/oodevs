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
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Tools.h"

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DecisionalState
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::DecisionalState( const MIL_Population& callerPopulation, const std::string& key, const std::string& value )
{
    NET_ASN_MsgDecisionalState msg;
    msg().oid   = callerPopulation.GetID();
    msg().key   = key.c_str();
    msg().value = value.c_str();
    msg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetAttitude
// Created: SBO 2005-11-23
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetAttitude( MIL_Population& callerPopulation, uint attitudeId )
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

// =============================================================================
// KNOWLEDGE AGENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation
// Created: NLD 2005-12-01
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation( unsigned int agentId )
{
    const MIL_AgentPion* pAgent = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( agentId );
    assert( pAgent  );
    return pAgent->GetRole< DEC_RolePion_Decision >().GetRoePopulation().GetID();
}

// =============================================================================
// KNOWLEDGE OBJECTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetObjectsInZone
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
std::vector< unsigned int > DEC_PopulationFunctions::GetObjectsInZone( const TER_Localisation* pZone, const std::vector< std::string >& parameters )
{
    assert( pZone );

    std::vector< unsigned int > knowledges;

    MIL_ObjectFilter                filter( parameters );
    TER_Object_ABC::T_ObjectVector  objects;
    TER_World::GetWorld().GetObjectManager().GetListWithinLocalisation( *pZone, objects );
    for( TER_Object_ABC::CIT_ObjectVector it = objects.begin(); it != objects.end(); ++it )
    {
        MIL_Object_ABC& object = static_cast< MIL_Object_ABC& >( **it );

        if( !object().CanBePerceived() || !filter.Test( object.GetType() ) )
            continue;
        knowledges.push_back( object.GetID() );
    }

    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectLocalisation
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_PopulationFunctions::GetKnowledgeObjectLocalisation  ( int knowledgeId)
{
    const MIL_Object_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( knowledgeId );
    if( !( pObject && (*pObject)().CanBePerceived() ) )
        return boost::shared_ptr<MT_Vector2D>();
    else
        return boost::shared_ptr<MT_Vector2D>((MT_Vector2D *)&pObject->GetLocalisation()) ;//, &DEC_Tools::GetTypeLocalisation(), 1 );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsKnowledgeObjectValid
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
bool DEC_PopulationFunctions::IsKnowledgeObjectValid( unsigned int nID )
{
    const MIL_Object_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( nID );
    return ( pObject && (*pObject)().CanBePerceived() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::DamageObject
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::DamageObject( int knowledgeId, double damageFactor )
{
	MIL_Object_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia(knowledgeId);
    if( !( pObject && (*pObject)().CanBePerceived() ) )
    {
        return  (int)eQueryInvalid ;
    }

    (*pObject)().Destroy( damageFactor );
    return (int)eQueryValid;
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectDistance
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
float DEC_PopulationFunctions::GetKnowledgeObjectDistance( const MIL_Population& callerPopulation, int knowledgeId )
{
    const MIL_Object_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( knowledgeId );
    if( !( pObject && (*pObject)().CanBePerceived() ) )
        return 0.0f;
    else
        return (float)callerPopulation.GetDistanceTo( pObject->GetLocalisation() ) ;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint
// Created: SBO 2005-12-13
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint(const MIL_Population& callerPopulation, int knowledgeId )
{
    const MIL_Object_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia( knowledgeId );
    if( !( pObject && (*pObject)().CanBePerceived() ) )
    	return boost::shared_ptr<MT_Vector2D>();
    else
    {
        MT_Vector2D* pPoint = new MT_Vector2D( callerPopulation.GetClosestPoint( pObject->GetLocalisation() ) );
        return boost::shared_ptr<MT_Vector2D>(pPoint);
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::IsEnemy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
int DEC_PopulationFunctions::IsEnemy(const MIL_Population& callerPopulation, int knowledgeId  )
{
	const MIL_Object_ABC* pObject = DEC_FunctionsTools::GetPopulationKnowledgeObjectFromDia(knowledgeId);
    if( !( pObject && (*pObject)().CanBePerceived() ) )
        return 0;
    else
        return (int)callerPopulation.GetArmy().IsAnEnemy( pObject->GetArmy() ) ;
}

// =============================================================================
// ETAT DECISIONNEL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::NotifyDominationStateChanged
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::NotifyDominationStateChanged( MIL_Population& callerPopulation, MT_Float dominationState)
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
// Name: DEC_PopulationFunctions::GetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
MIL_Mission_ABC* DEC_PopulationFunctions::GetMission( DEC_Decision_ABC* pAgent )
{
    return pAgent->GetMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::SetMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_PopulationFunctions::SetMission( DEC_Decision_ABC* object, MIL_Mission_ABC* mission )
{
    object->SetMission( mission );
}
