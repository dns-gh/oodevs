// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeFunctions.h $
// $Author: Nld $
// $Modtime: 11/05/05 16:03 $
// $Revision: 7 $
// $Workfile: DEC_KnowledgeFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeFunctions_h_
#define __DEC_KnowledgeFunctions_h_

#include "MIL.h"
#include "Knowledge/DEC_Knowledge_Def.h"

class DEC_Decision_ABC;
class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeFunctions
{
public:
    //! @name Rapport de force
    //@{
    static MT_Float GetRapForLocal     ( const MIL_AgentPion& callerAgent );

    static T_ConstKnowledgeAgentVector GetDetectedAgentsInFuseau        ( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetDetectedAgentsInZone          ( const MIL_AgentPion& callerAgent, const TER_Localisation* area );
    static T_ConstKnowledgeAgentVector GetAgentsAttacking               ( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetDangerousEnemies              ( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceived        ( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceivedInFuseau( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInZone           ( const MIL_AgentPion& callerAgent, TER_Localisation* pZone );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInFuseau         ( const MIL_AgentPion& callerAgent );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInCircle         ( const MIL_AgentPion& callerAgent, const MT_Vector2D* pCenter, float radius );

    template< typename T > static std::vector< boost::shared_ptr< TER_Localisation > > SortAccordingToUnloadedEnemies( const T& caller, const std::vector< boost::shared_ptr< TER_Localisation > >& locations );
    template< typename T > static std::vector< boost::shared_ptr< TER_Localisation > > SortAccordingToLoadedEnemies  ( const T& caller, const std::vector< boost::shared_ptr< TER_Localisation > >& locations );

    template< typename T > static T_ConstKnowledgeAgentVector GetLivingEnemiesPerceivedByPion( const T& caller, const DEC_Decision_ABC* perceiver );

    static T_ConstKnowledgeAgentVector GetNearbyRefugees                ( const MIL_AgentPion& callerAgent, MT_Float radius );
    static T_ConstKnowledgeAgentVector GetNearbySurrenderedAgents       ( const MIL_AgentPion& callerAgent, MT_Float radius );
    static bool EnemyPresenceInCircle( const MIL_AgentPion& callerAgent, const MT_Vector2D* center, MT_Float radius );
    template< typename T > static T_ConstKnowledgeAgentVector GetFriendsInZone( const T& caller, const TER_Localisation* location );
    template< typename T > static void ShareKnowledgesWith      ( const T& caller, DEC_Decision_ABC* receiver, float minutes );
    template< typename T > static void ShareKnowledgesInZoneWith( const T& caller, DEC_Decision_ABC* receiver, const MT_Vector2D* center, float radius );
       
    static boost::shared_ptr< DEC_Knowledge_Object > GetClosestObject      ( const MIL_AgentPion& callerAgent, const std::string& type );
    static boost::shared_ptr< DEC_Knowledge_Object > GetClosestFriendObject( const MIL_AgentPion& callerAgent, const std::string& type );
    static T_KnowledgeObjectDiaIDVector GetObjectsColliding( const MIL_AgentPion& callerAgent );
    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsInCircle( const T& caller, const MT_Vector2D* pCenter, MT_Float rRadius, const std::vector< std::string >& filters );
    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsInZone  ( const T& caller, const TER_Localisation* pLoc, const std::vector< std::string >& parameters );
    template< typename T > static T_KnowledgeObjectDiaIDVector GetObjectsInFuseau( const T& caller, const std::string& type );

    static T_KnowledgePopulationDiaIDVector GetPopulationsColliding( const MIL_AgentPion& callerAgent );
    static T_KnowledgePopulationDiaIDVector GetPopulationsAttacking( const MIL_AgentPion& callerAgent );
    template< typename T > static std::vector< unsigned int > GetPopulations( const T& caller );
    //@}

    //! @name Tools
    //@{
    template< typename T, typename B > static float ComputeEnemiesRatio( const T& caller, const B& boundaries, bool unloaded );
    //@}
};

#include "DEC_KnowledgeFunctions.inl"

#endif // __DEC_KnowledgeFunctions_h_

