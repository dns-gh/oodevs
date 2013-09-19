// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __DEC_KnowledgeObjectFunctions_h_
#define __DEC_KnowledgeObjectFunctions_h_

class DEC_Decision_ABC;
class DEC_Knowledge_Object;
class MIL_Agent_ABC;
class MIL_Entity_ABC;
class MT_Vector2D;
class PHY_DotationCategory;
class TER_Localisation;
class DEC_Knowledge_Agent;
class MIL_Population;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeObjectFunctions
{
public:
    //! @name Functions
    //@{
    static float GetSiteFranchissementWidth( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static std::string GetType( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static std::string GetName( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static const TER_Localisation* GetLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsInAvoidanceArea( boost::shared_ptr< MT_Vector2D > point, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsNearEffectArea( boost::shared_ptr< MT_Vector2D > point, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, double distance );
    static int IsBypassed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int IsMined( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int IsFullMined( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int IsBreached( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int IsConstructed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsObstacleActivated( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int GetActivationTime( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsActivableObstacle( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsKnowledgeValid( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool MustBeMined( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static int QueueForDecontamination( MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int QueueUnitForDecontamination( DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int QueueKnowledgeForDecontamination(  boost::shared_ptr< DEC_Knowledge_Agent > pAgent, boost::shared_ptr< DEC_Knowledge_Object > pObject );
    static int PopulationQueueForDecontamination( MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool CanBeAnimated( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static float GetConstructionLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static float GetBurningLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static float GetValorizationLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static float GetAnimationLevel( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static bool CanBeOccupied( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static void DecontaminateZone( const MIL_Agent_ABC& callerAgent, const TER_Localisation* location );
    static int DamageObject( MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float factor, const PHY_DotationCategory* dotation );

    static bool CanBeBypassed( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static void SetExitingPopulationDensity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density );
    static void SetExitingPopulationDensityInPercentage( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density );
    static void ResetExitingPopulationDensity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static void Recon( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsRecon( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static int EquipLogisticRoute( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsStockSupplied( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static int IsAnEnemy( const MIL_Entity_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int IsAFriend( const MIL_Entity_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int GetCurrentPerceptionLevel( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static float EstimatedWorkTime( MIL_Agent_ABC& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool CanBeValorized( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool HasCapacity( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::string& capacity );
    static void BuildInstantaneously( const DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static bool IsUndergroundNetworkExit( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int ActivateUndergroundNetworkExit( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int DeactivateUndergroundNetworkExit( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static double EstimatedUndergroundTime( const DEC_Decision_ABC& callerAgent,boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int HideInUndergroundNetwork( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pExit );
    static int GetOutFromUndergroundNetwork( DEC_Decision_ABC& callerAgent );
    static bool EnterAndExitInSameUndergroundNetwork( boost::shared_ptr< DEC_Knowledge_Object > pEnter,boost::shared_ptr< DEC_Knowledge_Object > pExit );
    //@}
};

#endif // __DEC_KnowledgeObjectFunctions_h_
