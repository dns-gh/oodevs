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

#include "Knowledge/DEC_Knowledge_Def.h"

class DEC_Decision_ABC;
class MIL_AgentPion;
class MIL_UrbanObject_ABC;
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
    static void Register( sword::Brain& brain );

    //! @name Rapport de force
    //@{
    static double GetRapForLocal( const DEC_Decision_ABC* callerAgent );

    static T_ConstKnowledgeAgentVector GetAllAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetDetectedAgentsInFuseau( const DEC_Decision_ABC* callerAgent );
    static T_ConstKnowledgeAgentVector GetDetectedAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetEnemyAgentsInZone( const DEC_Decision_ABC* callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetAgentsAttacking( const DEC_Decision_ABC* callerAgent );
    static T_ConstKnowledgeAgentVector GetAgentsAttackingAlly( const DEC_Decision_ABC* agentAlly );
    static T_ConstKnowledgeAgentVector GetEnemiesAttacking( const DEC_Decision_ABC* callerAgent );
    static boost::shared_ptr< DEC_Knowledge_Agent > GetNearestToFriend( const DEC_Decision_ABC* callerAgent, T_ConstKnowledgeAgentVector units );
    static T_ConstKnowledgeAgentVector GetDangerousEnemies( const DEC_Decision_ABC* callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceived( const DEC_Decision_ABC* callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceivedInFuseau( const DEC_Decision_ABC* callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInZone( const DEC_Decision_ABC* callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetWoundedUnitsInZone( const DEC_Decision_ABC* callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetCiviliansInZone( const DEC_Decision_ABC* callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInFuseau( const DEC_Decision_ABC* callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInCircle( const DEC_Decision_ABC* callerAgent, const MT_Vector2D* pCenter, float radius );
    static T_ConstKnowledgeAgentVector GetWoundedOrDeadUnitsInCircle( const DEC_Decision_ABC* callerAgent, const MT_Vector2D* pCenter, float radius );
    static T_ConstKnowledgeAgentVector GetEnemies( const DEC_Decision_ABC* agent );

    static T_UrbanObjectVector GetUrbanBlocks( DEC_Decision_ABC* agent );
    static T_UrbanObjectVector GetUrbanBlockInCircle( boost::shared_ptr< MT_Vector2D > center, float radius );
    static T_UrbanObjectVector GetUrbanBlockInZone( const DEC_Decision_ABC* pion, TER_Localisation* );
    static T_UrbanObjectVector GetUrbanBlockInOrIntersectZone( const DEC_Decision_ABC* pion, TER_Localisation* );
    static const MIL_UrbanObject_ABC* GetUrbanBlockForPosition( const DEC_Decision_ABC* pion, boost::shared_ptr< MT_Vector2D >& point );

    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceivedByPion( const DEC_Decision_ABC* caller, const DEC_Decision_ABC* perceiver );

    static T_ConstKnowledgeAgentVector GetNearbyRefugees( const DEC_Decision_ABC* callerAgent, double radius );
    static T_ConstKnowledgeAgentVector GetNearbyTerrorists( const DEC_Decision_ABC* callerAgent, double radius );
    static T_ConstKnowledgeAgentVector GetNearbySurrenderedAgents( const DEC_Decision_ABC* callerAgent, double radius );
    static bool EnemyPresenceInCircle( const DEC_Decision_ABC* callerAgent, const MT_Vector2D* center, double radius );
    static T_ConstKnowledgeAgentVector GetFriendsInZone( const DEC_Decision_ABC* caller, const TER_Localisation* location );
    static void ShareKnowledgesWith( const DEC_Decision_ABC* caller, DEC_Decision_ABC* receiver, float minutes );
    static void ShareKnowledgesInZoneWith( const DEC_Decision_ABC* caller, DEC_Decision_ABC* receiver, const MT_Vector2D* center, float radius );

    static boost::shared_ptr< DEC_Knowledge_Object > GetClosestObject( const DEC_Decision_ABC* callerAgent, const std::string& type );
    static boost::shared_ptr< DEC_Knowledge_Object > GetClosestFriendObject( const DEC_Decision_ABC* callerAgent, const std::string& type );
    static T_KnowledgeObjectVector GetObjects( const DEC_Decision_ABC* agent );
    static T_KnowledgeObjectDiaIDVector GetObjectsColliding( const DEC_Decision_ABC* callerAgent );
    static T_KnowledgeObjectDiaIDVector GetObjectsCollidingFromType( const DEC_Decision_ABC* callerAgent, const std::string& objectType );
    static T_KnowledgeObjectDiaIDVector GetCollidingDisasters( const DEC_Decision_ABC* callerAgent );
    static T_KnowledgeObjectDiaIDVector GetDisasters( const DEC_Decision_ABC* callerAgent );

    static T_KnowledgeObjectDiaIDVector GetObjectsInCircle( const DEC_Decision_ABC* caller, const MT_Vector2D* pCenter, double rRadius, const std::vector< std::string >& filters, bool nonActivatedObstacles );
    static T_KnowledgeObjectDiaIDVector GetObjectsInZone( const DEC_Decision_ABC* caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters );
    static T_KnowledgeObjectDiaIDVector GetObjectsIntersectingInZone( const DEC_Decision_ABC* caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters );
    static T_KnowledgeObjectDiaIDVector GetObjectsInFuseau( const DEC_Decision_ABC* caller, const std::string& type );
    static T_KnowledgeObjectDiaIDVector GetObjectsWithCapacityInZone( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const TER_Localisation* pLoc );
    static bool IsPositionInsideObjectOfType( const DEC_Decision_ABC* callerAgent, const std::string& capacity, const MT_Vector2D* pCenter );

    static T_KnowledgePopulationDiaIDVector GetPopulationsColliding( const DEC_Decision_ABC* callerAgent );
    static T_KnowledgePopulationDiaIDVector GetPopulationsAttacking( const DEC_Decision_ABC* callerAgent );
    static bool IsPopulationAttacking( const DEC_Decision_ABC* callerAgent, int knowledgeId );
    static std::vector< unsigned int > GetPopulations( const DEC_Decision_ABC* caller );
    static boost::shared_ptr< DEC_Knowledge_Agent > GetAgentKnowledge( int callerId, int knowledgeId );
    //@}
};

#endif // __DEC_KnowledgeFunctions_h_

