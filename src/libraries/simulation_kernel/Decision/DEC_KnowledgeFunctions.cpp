// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeFunctions.cpp $
// $Author: Nld $
// $Modtime: 23/03/05 18:22 $
// $Revision: 7 $
// $Workfile: DEC_KnowledgeFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeFunctions.h"
#include "Brain.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/DisasterCapacity.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_Logger.h"
#include "Urban/MIL_UrbanCache.h"

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAllAgentsInZone
// Created: FAI 2014-04-03
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetAllAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent->GetPion().GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetAllAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetDetectedAgentsInZone( knowledges, callerAgent.GetOrderManager().GetFuseau() );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDetectedAgentsInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDetectedAgentsInZone( const MIL_AgentPion& callerAgent, const TER_Localisation* area )
{
    if( !area )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetDetectedAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetEnemyAgentsInZone
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetEnemyAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area )
{
    T_ConstKnowledgeAgentVector knowledges;
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = callerAgent->GetPion().GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetEnemyAgentsInZone( knowledges, *area );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttacking
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetAgentsAttacking( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector attackers;
    callerAgent.GetKnowledge().GetAgentsAttacking( attackers );
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetAgentsAttackingAlly
// Created: DDA 2010-06-02
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector  DEC_KnowledgeFunctions::GetAgentsAttackingAlly( const DEC_Decision_ABC* agentAlly )
{
    if( !agentAlly )
        throw MASA_EXCEPTION( "invalid parameter." );
    return DEC_KnowledgeFunctions::GetAgentsAttacking( agentAlly->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetEnemiesAttacking
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetEnemiesAttacking( const DEC_Decision_ABC* callerAgent )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector attackers;
    std::set< boost::shared_ptr< DEC_Knowledge_Agent > > buffer;
    const std::vector< MIL_Automate* >& allies = callerAgent->GetPion().GetKnowledgeGroup()->GetAutomates();
    for( auto it = allies.begin(); it != allies.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        const std::vector< MIL_AgentPion* >& pions = (*it)->GetPions();
        for( auto pit = pions.begin(); pit != pions.end(); ++pit )
        {
            T_ConstKnowledgeAgentVector enemies = DEC_KnowledgeFunctions::GetAgentsAttacking( **pit );
            for( auto cit = enemies.begin(); cit != enemies.end(); ++cit )
            {
                if( buffer.insert( *cit ).second )
                    attackers.push_back( *cit );
            }
        }
    }
    return attackers;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeFunctions::GetNearestToFriend
// Created: LDC 2011-06-24
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeFunctions::GetNearestToFriend( const DEC_Decision_ABC* callerAgent, T_ConstKnowledgeAgentVector units )
{
    if( !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );
    double minSquareDistance = std::numeric_limits< double >::max();
    boost::shared_ptr< DEC_Knowledge_Agent > result;
    const std::vector< MIL_Automate* >& allies = callerAgent->GetPion().GetKnowledgeGroup()->GetAutomates();
    for( std::vector< MIL_Automate* >::const_iterator it = allies.begin(); it != allies.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        const std::vector< MIL_AgentPion* >& pions = (*it)->GetPions();
        for( std::vector< MIL_AgentPion* >::const_iterator pit = pions.begin(); pit != pions.end(); ++pit )
        {
            if( !(*pit) )
                throw MASA_EXCEPTION( "invalid parameter." );
            const MT_Vector2D& allyPosition = (*pit)->Get< PHY_RoleInterface_Location >().GetPosition();
            for( auto uit = units.begin(); uit != units.end(); ++uit )
            {
                if( !(*uit) )
                    throw MASA_EXCEPTION( "invalid parameter." );
                const MT_Vector2D& uPosition = (*uit)->GetPosition();
                double squareDistance = allyPosition.SquareDistance( uPosition );
                if( squareDistance < minSquareDistance )
                {
                    result = *uit;
                    minSquareDistance = squareDistance;
                }
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDangerousEnemies
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetDangerousEnemies( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetKnowledge().GetDangerousEnemies();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceived
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceived( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledge().GetLivingEnemiesPerceived( knowledges );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    callerAgent.GetKnowledge().GetLivingEnemiesPerceivedInZone( knowledges, callerAgent.GetOrderManager().GetFuseau() );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInZone
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone )
{
    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetLivingEnemiesInZone( knowledges, *pZone );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetWoundedUnitsInZone
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetWoundedUnitsInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone )
{
    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg)
        bbKg->GetWoundedUnitsInZone( knowledges, *pZone );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetCiviliansInZone
// Created: LGY 2012-12-20
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetCiviliansInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone )
{
    if( !pZone )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg)
        bbKg->GetCiviliansInZone( knowledges, *pZone );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau( const MIL_AgentPion& callerAgent )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetLivingEnemiesInZone( knowledges, callerAgent.GetOrderManager().GetFuseau() );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* pCenter, float radius )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetLivingEnemiesInCircle( knowledges, *pCenter, radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle
// Created: LGY 2012-12-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* pCenter, float radius )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg)
        bbKg->GetWoundedOrDeadUnitsInCircle( knowledges, *pCenter, radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObservableKnowledge
// Created: MGD 2010-02-09
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetObservableKnowledge( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    auto bbKg = pion.GetKnowledgeGroup()->GetKnowledge();
    if( !bbKg )
        return;

    //Agents
    knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ), bbKg->GetEnemies(), true );

    //Object
    T_KnowledgeObjectVector objectsKn;
    bbKg->GetKnowledgeObjectContainer().GetObjects( objectsKn );

    knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.object" ), objectsKn, true );

    //Urban
    T_UrbanObjectVector blocks;
    pion.GetArmy().GetKnowledge().GetUrbanObjects( blocks );

    knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.urbanBlock" ), blocks, true );

}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlock
// Created: DDA 2010-03-15
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetUrbanBlock( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Urban
    T_UrbanObjectVector blocks;
    pion.GetArmy().GetKnowledge().GetUrbanObjects( blocks );
    knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.urbanBlock" ), blocks, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockInCircle
// Created: DDA 2010-03-16
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlockInCircle( boost::shared_ptr< MT_Vector2D > center, float radius )
{
    if( !center )
        throw MASA_EXCEPTION( "invalid parameter." );
    //Urban
    T_UrbanObjectVector result;
    MIL_AgentServer::GetWorkspace().GetUrbanCache().GetListWithinCircle( *center, radius, result );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockForPosition
// Created: DDA 2011-06-20
// -----------------------------------------------------------------------------
const MIL_UrbanObject_ABC* DEC_KnowledgeFunctions::GetUrbanBlockForPosition( const MIL_AgentPion& pion, boost::shared_ptr< MT_Vector2D >& point )
{
    if( !point )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_UrbanObjectVector blocks;
    pion.GetArmy().GetKnowledge().GetUrbanObjects( blocks );
    for( T_UrbanObjectVector::iterator it = blocks.begin(); it != blocks.end(); ++it )
        if( ( *it ) && ( *it )->GetLocalisation().IsInside( *point ) )
            return ( *it );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockInZone
// Created: BCI 2011-03-01
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlockInZone( const MIL_AgentPion& pion, TER_Localisation* pLocalisation )
{
    T_UrbanObjectVector result;
    if( pLocalisation )
    {
        T_UrbanObjectVector blocks;
        pion.GetArmy().GetKnowledge().GetUrbanObjects( blocks );
        for( T_UrbanObjectVector::iterator it = blocks.begin(); it != blocks.end(); ++it )
            if( ( *it ) && pLocalisation->Contains( ( *it )->GetLocalisation() ) )
                result.push_back( *it );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetUrbanBlockInOrIntersectZone
// Created: GGE 2014-08-04
// -----------------------------------------------------------------------------
T_UrbanObjectVector DEC_KnowledgeFunctions::GetUrbanBlockInOrIntersectZone( const MIL_AgentPion& pion, TER_Localisation* pLocalisation )
{
    T_UrbanObjectVector result;
    if( pLocalisation )
    {
        T_UrbanObjectVector blocks;
        pion.GetArmy().GetKnowledge().GetUrbanObjects( blocks );
        for( T_UrbanObjectVector::iterator it = blocks.begin(); it != blocks.end(); ++it )
            if( ( *it ) && pLocalisation->IsIntersecting( ( *it )->GetLocalisation() ) )
                result.push_back( *it );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDestroyableKnowledge
// Created: MGD 2010-02-10
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetDestroyableKnowledge( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Agents //@TODO Add private tools function
    auto bbKg = pion.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ),bbKg->GetEnemies(), true );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetIdentifiableKnowledge
// Created: PSN & GGE 2010-03-25
// -----------------------------------------------------------------------------
void DEC_KnowledgeFunctions::GetIdentifiableKnowledge( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    //Agents //@TODO Add private tools function
    auto bbKg = pion.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ),bbKg->GetEnemies(), true );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbyRefugees
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbyRefugees( const MIL_AgentPion& callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetRefugeesInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbyTerrorists
// Created: DDA 2011-08-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbyTerrorists( const MIL_AgentPion& callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetTerroristsInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetNearbySurrenderedAgents
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetNearbySurrenderedAgents( const MIL_AgentPion& callerAgent, double radius )
{
    T_ConstKnowledgeAgentVector knowledges;
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetSurrenderedAgentsInCircle( knowledges, callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), radius );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::EnemyPresenceInCircle
// Created: NLD 2006-04-14
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::EnemyPresenceInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* center, double radius )
{
    if( !center )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        return bbKg->EnemyPresenceInCircle( *center, radius );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetRapForLocal
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
double DEC_KnowledgeFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent )
{
    return callerAgent.GetKnowledge().GetRapForLocalValue();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestObject( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    std::vector< std::string > typeList;
    typeList.push_back( type );
    const MIL_ObjectFilter filter( typeList );
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->GetClosestObject( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), filter );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetClosestFriendObject
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > DEC_KnowledgeFunctions::GetClosestFriendObject( const MIL_Agent_ABC& callerAgent, const std::string& type )
{
    std::vector< std::string > typeList;
    typeList.push_back( type );
    const MIL_ObjectFilter filter( typeList );
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->GetClosestFriendObject( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), filter, callerAgent.GetArmy() );
    return boost::shared_ptr< DEC_Knowledge_Object >();
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsColliding
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsColliding( const MIL_AgentPion& callerAgent )
{
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent.GetKnowledge().GetObjectsColliding( objectsColliding );

    return objectsColliding;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsCollidingFromType
// Created: GGE 2012-03-06
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsCollidingFromType( const MIL_AgentPion& callerAgent, const std::string& objectType )
{
    T_KnowledgeObjectDiaIDVector result;
    T_KnowledgeObjectDiaIDVector objectsColliding;
    callerAgent.GetKnowledge().GetObjectsColliding( objectsColliding );
    for( auto it = objectsColliding.begin(); it != objectsColliding.end(); ++it )
    {
        if( *it )
        {
            const MIL_ObjectType_ABC& type = (*it)->GetType();
            if( type.GetName() == objectType )
                result.push_back( *it );
        }
    }

    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetCollidingDisasters
// Created: LGY 2012-11-27
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetCollidingDisasters( const MIL_AgentPion& callerAgent )
{
    T_KnowledgeObjectDiaIDVector results;
    T_KnowledgeObjectDiaIDVector disasters = GetDisasters( callerAgent );

    for( auto it = disasters.begin(); it != disasters.end(); ++it )
        if( *it )
            if( (*it)->GetLocalisation().IsInside( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() ) )
                results.push_back( *it );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetDisasters
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetDisasters( const MIL_AgentPion& callerAgent )
{
    T_KnowledgeObjectDiaIDVector disasters;
    T_KnowledgeObjectDiaIDVector objects;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjects( objects );
    for( auto it = objects.begin(); it != objects.end(); ++it )
        if( *it )
        {
            const MIL_ObjectType_ABC& type = ( *it )->GetType();
            if( type.GetCapacity< DisasterCapacity >() )
                disasters.push_back( *it );
        }
    return disasters;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsWithCapacityInZone
// Created: JSR 2012-04-17
// -----------------------------------------------------------------------------
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsWithCapacityInZone( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const TER_Localisation* pLoc )
{
    if( !pLoc || !callerAgent )
        throw MASA_EXCEPTION( "invalid parameter." );

    T_KnowledgeObjectDiaIDVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsWithCapacityInZone( knowledges, capacity, *pLoc );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::IsPositionInsideObjectOfType
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::IsPositionInsideObjectOfType( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const MT_Vector2D* pCenter )
{
    if( !callerAgent || !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = callerAgent->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->IsPositionInsideObjectOfType( capacity, *pCenter );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsColliding
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
T_KnowledgePopulationDiaIDVector DEC_KnowledgeFunctions::GetPopulationsColliding( const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector populationsColliding;
    callerAgent.GetKnowledge().GetPopulationsColliding( populationsColliding );

    return populationsColliding;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulationsAttacking
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
T_KnowledgePopulationDiaIDVector DEC_KnowledgeFunctions::GetPopulationsAttacking( const MIL_AgentPion& callerAgent )
{
    T_KnowledgePopulationDiaIDVector attackers;
    callerAgent.GetKnowledge().GetPopulationsAttacking( attackers );

    return attackers;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::IsPopulationAttacking
// Created: DDA 2011-05-03
// -----------------------------------------------------------------------------
bool DEC_KnowledgeFunctions::IsPopulationAttacking( const MIL_AgentPion& callerAgent, int knowledgeId )
{
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( knowledgeId );
        if( pKnowledge )
            return callerAgent.GetKnowledge().IsPopulationAttacking( *pKnowledge );
    }
    return false;
}

boost::shared_ptr< DEC_Knowledge_Agent > DEC_KnowledgeFunctions::GetAgentKnowledge(
        int callerId, int knowledgeId )
{
    const auto& entities = MIL_AgentServer::GetWorkspace().GetEntityManager();
    if( MIL_AgentPion* caller = entities.FindAgentPion( callerId ) )
    {
        return caller->GetKnowledge().ResolveKnowledgeAgent( knowledgeId );
    }
    return boost::shared_ptr< DEC_Knowledge_Agent >();
}
