// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __DEC_KnowledgeFunctions_h_
#define __DEC_KnowledgeFunctions_h_

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_Knowledge_Def.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_Sharing.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Tools/MIL_Tools.h"
#include "MIL_Time_ABC.h"

class DEC_Decision_ABC;
class MIL_AgentPion;
class MIL_Agent_ABC;
class TER_Localisation;
class MT_Vector2D;

namespace sword
{
    class Brain;
}

namespace directia
{
namespace tools
{
namespace binders
{
    class ScriptRef;
}
}
}

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeFunctions
{
public:
    //! @name Rapport de force
    //@{
    static double GetRapForLocal( const MIL_AgentPion& callerAgent );

	static T_ConstKnowledgeAgentVector GetAllAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetDetectedAgentsInFuseau( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetDetectedAgentsInZone( const MIL_AgentPion& callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetEnemyAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetAgentsAttacking( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetAgentsAttackingAlly( const DEC_Decision_ABC* agentAlly );
    static T_ConstKnowledgeAgentVector GetEnemiesAttacking( const DEC_Decision_ABC* callerAgent );
    static boost::shared_ptr< DEC_Knowledge_Agent > GetNearestToFriend( const DEC_Decision_ABC* callerAgent, T_ConstKnowledgeAgentVector units );
    static T_ConstKnowledgeAgentVector GetDangerousEnemies( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceived( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceivedInFuseau( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetWoundedUnitsInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetCiviliansInZone( const MIL_AgentPion& callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInFuseau( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* pCenter, float radius );
    static T_ConstKnowledgeAgentVector GetWoundedOrDeadUnitsInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* pCenter, float radius );

    static void GetObservableKnowledge( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table );
    static void GetUrbanBlock( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table );
    static T_UrbanObjectVector GetUrbanBlockInCircle( boost::shared_ptr< MT_Vector2D > center, float radius );
    static T_UrbanObjectVector GetUrbanBlockInZone( const MIL_AgentPion& pion, TER_Localisation* );
    static const MIL_UrbanObject_ABC* GetUrbanBlockForPosition( const MIL_AgentPion& pion, boost::shared_ptr< MT_Vector2D >& point );
    static void GetDestroyableKnowledge( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table );
    static void GetIdentifiableKnowledge( sword::Brain& brain, const MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table );

    template< typename T > static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceivedByPion( const T& caller, const DEC_Decision_ABC* perceiver );

    static T_ConstKnowledgeAgentVector GetNearbyRefugees( const MIL_AgentPion& callerAgent, double radius );
    static T_ConstKnowledgeAgentVector GetNearbyTerrorists( const MIL_AgentPion& callerAgent, double radius );
    static T_ConstKnowledgeAgentVector GetNearbySurrenderedAgents( const MIL_AgentPion& callerAgent, double radius );
    static bool EnemyPresenceInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* center, double radius );
    template< typename T > static T_ConstKnowledgeAgentVector GetFriendsInZone( const T& caller, const TER_Localisation* location );
    template< typename T > static void ShareKnowledgesWith( const T& caller, DEC_Decision_ABC* receiver, float minutes );
    template< typename T > static void ShareKnowledgesInZoneWith( const T& caller, DEC_Decision_ABC* receiver, const MT_Vector2D* center, float radius );

    static boost::shared_ptr< DEC_Knowledge_Object > GetClosestObject( const MIL_Agent_ABC& callerAgent, const std::string& type );
    static boost::shared_ptr< DEC_Knowledge_Object > GetClosestFriendObject( const MIL_Agent_ABC& callerAgent, const std::string& type );
    static T_KnowledgeObjectDiaIDVector GetObjectsColliding( const MIL_AgentPion& callerAgent );
    static T_KnowledgeObjectDiaIDVector GetObjectsCollidingFromType( const MIL_AgentPion& callerAgent, const std::string& objectType );
    static T_KnowledgeObjectDiaIDVector GetCollidingDisasters( const MIL_AgentPion& callerAgent );
    static T_KnowledgeObjectDiaIDVector GetDisasters( const MIL_AgentPion& callerAgent );

    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsInCircle( const T& caller, const MT_Vector2D* pCenter, double rRadius, const std::vector< std::string >& filters, bool nonActivatedObstacles );
    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsInZone( const T& caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters );
    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsIntersectingInZone( const T& caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters );
    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsInFuseau( const T& caller, const std::string& type );
    static T_KnowledgeObjectDiaIDVector GetObjectsWithCapacityInZone( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const TER_Localisation* pLoc );
    static bool IsPositionInsideObjectOfType( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const MT_Vector2D* pCenter );

    static T_KnowledgePopulationDiaIDVector GetPopulationsColliding( const MIL_AgentPion& callerAgent );
    static T_KnowledgePopulationDiaIDVector GetPopulationsAttacking( const MIL_AgentPion& callerAgent );
    static bool IsPopulationAttacking( const MIL_AgentPion& callerAgent, int knowledgeId );
    template< typename T > static std::vector< unsigned int > GetPopulations( const T& caller );
    static boost::shared_ptr< DEC_Knowledge_Agent > GetAgentKnowledge(
            int callerId, int knowledgeId );
    //@}

    //! @name Tools
    //@{
    template< typename T, typename B > static float ComputeEnemiesRatio( const T& caller, const B& boundaries, bool unloaded );
    //@}
};

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion
// Created: NLD 2006-04-18
// -----------------------------------------------------------------------------
template< typename T >
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion( const T& caller, const DEC_Decision_ABC* perceiver )
{
    if( !perceiver )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MIL_AgentPion& source = perceiver->GetPion();
    T_ConstKnowledgeAgentVector sourceKnowledge;
    source.GetKnowledge().GetLivingEnemiesPerceived( sourceKnowledge );
    T_ConstKnowledgeAgentVector results;
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< MIL_KnowledgeGroup > sourceKnowledgeGroup = source.GetKnowledgeGroup();
        bbKg->TranslateKnowledges( sourceKnowledge, sourceKnowledgeGroup, results );
    }
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeFunctions::ShareKnowledgesWith( const T& caller, DEC_Decision_ABC* receiver, float minutes )
{
    if( !receiver )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = receiver->GetAutomate().GetKnowledgeGroup()->GetKnowledge();
    if( !bbKg )
        return;
    const unsigned int sharingTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep() + unsigned int( MIL_Tools::ConvertMinutesToSim( minutes ) );
    boost::shared_ptr< MIL_KnowledgeGroup > callerKnowledgeGroup = caller.GetKnowledgeGroup();
    bbKg->GetKsSharing().ShareFromSource( callerKnowledgeGroup, sharingTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
template< typename T >
void DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith( const T& caller, DEC_Decision_ABC* receiver, const MT_Vector2D* center, float radius )
{
    if( !receiver )
        throw MASA_EXCEPTION( "invalid parameter." );
    auto bbKg = receiver->GetAutomate().GetKnowledgeGroup()->GetKnowledge();
    if( !bbKg )
        return;
    boost::shared_ptr< MIL_KnowledgeGroup > callerKnowledgeGroup = caller.GetKnowledgeGroup();
    bbKg->GetKsSharing().ShareFromSource( callerKnowledgeGroup, MIL_Time_ABC::GetTime().GetCurrentTimeStep(), *center, MIL_Tools::ConvertMeterToSim( radius ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInCircle
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T >
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInCircle( const T& caller, const MT_Vector2D* pCenter, double rRadius, const std::vector< std::string >& filters, bool nonActivatedObstacles )
{
    if( !pCenter )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( filters );
    T_KnowledgeObjectDiaIDVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = caller.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsInCircle( knowledges, filter, *pCenter, rRadius, nonActivatedObstacles );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInZone
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T >
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInZone( const T& caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters )
{
    if( !pLoc )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( parameters );
    T_KnowledgeObjectDiaIDVector knowledges;
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        T_KnowledgeObjectDiaIDVector knowledgesTmp;
        bbKg->GetKnowledgeObjectContainer().GetObjectsInZone( knowledgesTmp, filter, *pLoc );
        knowledges.insert( knowledges.end(), knowledgesTmp.begin(), knowledgesTmp.end() );
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsIntersectingInZone
// Created: ABR 2013-01-18
// -----------------------------------------------------------------------------
template< typename T >
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsIntersectingInZone( const T& caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters )
{
    if( !pLoc )
        throw MASA_EXCEPTION( "invalid parameter." );
    MIL_ObjectFilter filter( parameters );
    T_KnowledgeObjectDiaIDVector knowledges;
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = caller.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        container->GetObjectsIntersectingInZone( knowledges, filter, *pLoc );
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetObjectsInFuseau
// Created: NLD 2006-05-05
// -----------------------------------------------------------------------------
template< typename T >
T_KnowledgeObjectDiaIDVector DEC_KnowledgeFunctions::GetObjectsInFuseau( const T& caller, const std::string& type )
{
    std::vector< std::string > types;
    types.push_back( type );
    MIL_ObjectFilter filter( types );
    T_KnowledgeObjectDiaIDVector knowledges;
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        T_KnowledgeObjectDiaIDVector knowledgesTmp;
        bbKg->GetKnowledgeObjectContainer().GetObjectsInZone( knowledgesTmp, filter, caller.GetOrderManager().GetFuseau() );
        knowledges.insert( knowledges.end(), knowledgesTmp.begin(), knowledgesTmp.end() );
    }
    return knowledges;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetFriendsInZone
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
template< typename T >
T_ConstKnowledgeAgentVector DEC_KnowledgeFunctions::GetFriendsInZone( const T& caller, const TER_Localisation* location )
{
    if( !location )
        throw MASA_EXCEPTION( "invalid parameter." );
    T_ConstKnowledgeAgentVector results;
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetFriendsInZone( results, *location );
    return results;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeFunctions::GetPopulations
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
template< typename T >
std::vector< unsigned int > DEC_KnowledgeFunctions::GetPopulations( const T& caller )
{
    std::vector< unsigned int > results;
    auto bbKg = caller.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
        bbKg->GetPopulations( results );
    return results;
}

#endif // __DEC_KnowledgeFunctions_h_

