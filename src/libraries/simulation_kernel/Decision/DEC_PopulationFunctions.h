// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_PopulationFunctions.h $
// $Author: Jvt $
// $Modtime: 11/05/05 10:42 $
// $Revision: 17 $
// $Workfile: DEC_PopulationFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationFunctions_h_
#define __DEC_PopulationFunctions_h_

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class MIL_Population;
class MIL_Mission_ABC;
class TER_Localisation;
class MT_Vector2D;
class DEC_Knowledge_Object;
class MIL_UrbanObject_ABC;
class DEC_Knowledge_Agent;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_PopulationFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    // State
    static boost::shared_ptr< MT_Vector2D > GetBarycenter( const DEC_Decision_ABC* callerPopulation );
    static int GetHealthyHumans( const DEC_Decision_ABC* callerPopulation );
    static int GetContaminatedHumans( const DEC_Decision_ABC* callerPopulation );

    // Effects
    static void ResetPionMaxSpeed( DEC_Decision_ABC* callerPopulation );
    static void SetPionMaxSpeed( DEC_Decision_ABC* callerPopulation, double speed );
    static void SetAttitude( DEC_Decision_ABC* callerPopulation, unsigned int attitudeId );
    static unsigned int GetAttitude( const DEC_Decision_ABC* callerPopulation );
    static std::vector< boost::shared_ptr< TER_Localisation > > GetCurrentLocations ( const DEC_Decision_ABC* callerPopulation );
    static double GetUrbanBlockAngriness( const DEC_Decision_ABC* callerPopulation );
    static void ReintegrateUrbanBlock( DEC_Decision_ABC* callerPopulation );
    static void HealWounded( DEC_Decision_ABC* callerPopulation );
    static void SetUrbanDestructionState( DEC_Decision_ABC* callerPopulation, bool state );
    static bool GetUrbanDestructionState( DEC_Decision_ABC* callerPopulation );
    static void SetDemonstrationState( DEC_Decision_ABC* callerPopulation, bool state );
    static bool GetDemonstrationState( DEC_Decision_ABC* callerPopulation );

    // Knowledge agents
    static int GetKnowledgeAgentRoePopulation ( unsigned int agentId );

    // Knowledge objects
    static const TER_Localisation* GetKnowledgeObjectLocalisation( boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static std::vector< boost::shared_ptr< DEC_Knowledge_Object > > GetObjectsInCircle( const DEC_Decision_ABC* caller, double radius, const std::vector< std::string >& parameters );
    static int DamageObject( boost::shared_ptr< DEC_Knowledge_Object > knowledge, double damageFactor ) ;
    static float GetKnowledgeObjectDistance( const DEC_Decision_ABC* callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > knowledge ) ;
    static boost::shared_ptr<MT_Vector2D> GetKnowledgeObjectClosestPoint( const DEC_Decision_ABC* callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > knowledge ) ;
    static int IsEnemy( const DEC_Decision_ABC* callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static bool HasFlow( const DEC_Decision_ABC* population );
    static std::vector< unsigned int > GetPionsAttacking( const DEC_Decision_ABC* callerPopulation );
    static std::vector< unsigned int > GetPionsSecuring( const DEC_Decision_ABC* callerPopulation );

    // Move
    static bool HasReachedDestination( const DEC_Decision_ABC* callerPopulation, const MT_Vector2D* destination );
    static bool HasReachedBlockBorder( const DEC_Decision_ABC* callerPopulation, const MIL_UrbanObject_ABC* pUrbanKnowledge );
    static bool HasReachedDestinationCompletely( const DEC_Decision_ABC* callerPopulation, const MT_Vector2D* destination );

    // Etat decisionnel
    static void NotifyDominationStateChanged( DEC_Decision_ABC* callerPopulation, double dominationState );
    static unsigned int GetDeadHumans( const DEC_Decision_ABC* callerPopulation );
    static std::string  GetSzName( MIL_Population& callerPopulation );
    static double GetDominationState( DEC_Decision_ABC& callerPopulation );
    //@}

    static void SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission );
};

#endif // __DEC_PopulationFunctions_h_
