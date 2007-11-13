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

class DIA_Call_ABC;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Fuseau;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeFunctions
{
public:
    //! @name Rapport de force
    //@{
    static void GetRapForLocal         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetRapForGlobal        ( DIA_Call_ABC& call, const MIL_Automate&  callerAgent );

    static void GetDetectedAgentsInFuseau        ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetDetectedAgentsInZone          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetAgentsAttacking               ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetDangerousEnemies              ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
//    static void GetLivingTerroristsPerceived     ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLivingEnemiesPerceived        ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLivingEnemiesPerceivedInFuseau( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLivingEnemiesPerceivedInZone  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLivingEnemiesInZone           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLivingEnemiesInFuseau         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLivingEnemiesInCircle         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    template< typename T > static void ComputeFuseauUnloadedEnemiesRatio    ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void ComputeFuseauLoadedEnemiesRatio      ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void ComputeLocationUnloadedEnemiesRatio  ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void ComputeLocationLoadedEnemiesRatio    ( DIA_Call_ABC& call, const T& caller );

//    template< typename T > static void ComputeFuseauUnloadedEnemyIntelligenceRatio  ( DIA_Call_ABC& call, const T& caller );
//    template< typename T > static void ComputeFuseauLoadedEnemyIntelligenceRatio    ( DIA_Call_ABC& call, const T& caller );
//    template< typename T > static void ComputeLocationUnloadedEnemyIntelligenceRatio( DIA_Call_ABC& call, const T& caller );
//    template< typename T > static void ComputeLocationLoadedEnemyIntelligencesRatio ( DIA_Call_ABC& call, const T& caller );

    template< typename T > static void SortFuseauxAccordingToUnloadedEnemies( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void SortFuseauxAccordingToLoadedEnemies  ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetLivingEnemiesPerceivedByPion      ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetDangerousEnemiesInZoneOfPion      ( DIA_Call_ABC& call, const T& caller );

    static void GetNearbyRefugees                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetNearbySurrenderedAgents       ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void EnemyPresenceInCircle            ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    template< typename T > static void GetFriendsInZone         ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void ShareKnowledgesWith      ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void ShareKnowledgesInZoneWith( DIA_Call_ABC& call, const T& caller );
       
    static void GetClosestObject       ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );    
    static void GetClosestFriendObject ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );    
    static void GetObjectsColliding    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    template< typename T > static void GetObjects        ( DIA_Call_ABC& call, const T& caller );   
    template< typename T > static void GetObjectsInCircle( DIA_Call_ABC& call, const T& caller );   
    template< typename T > static void GetObjectsInZone  ( DIA_Call_ABC& call, const T& caller );   
    template< typename T > static void GetObjectsInFuseau( DIA_Call_ABC& call, const T& caller );   

    static void GetPopulationsColliding( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPopulationsAttacking( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    template< typename T > static void GetPopulations           ( DIA_Call_ABC& call, const T& caller );
    //@}

    //! @name Tools
    //@{
    template< typename T, typename U > static float _ComputeZoneEnemiesRatio          ( const T& caller, const U& zone, bool unloaded );
//    template< typename T, typename U > static float _ComputeZoneEnemyIntelligenceRatio( const T& caller, const U& zone, bool unloaded );
    //@}
};

#include "DEC_KnowledgeFunctions.inl"

#endif // __DEC_KnowledgeFunctions_h_

