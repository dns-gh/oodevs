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

class DEC_Decision_ABC;
class MIL_Population;
class MIL_Mission_ABC;
class TER_Localisation;
class MT_Vector2D;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_PopulationFunctions
{
public:
    //! @name Functions
    //@{
    // Test / debug
    static void DecisionalState( const MIL_Population& callerPopulation, const std::string& key, const std::string& value );

    // State
    static boost::shared_ptr< MT_Vector2D > GetBarycenter( const MIL_Population& callerPopulation );

    // Effects
    static void SetAttitude( MIL_Population& callerPopulation, unsigned int attitudeId );
    static unsigned int GetAttitude( const MIL_Population& callerPopulation );
    static std::vector< boost::shared_ptr< TER_Localisation > > GetCurrentLocations ( const MIL_Population& callerPopulation );
    static double GetUrbanBlockAngriness( const MIL_Population& callerPopulation );
    static void ReintegrateUrbanBlock( MIL_Population& callerPopulation );

    // Knowledge agents
    static int GetKnowledgeAgentRoePopulation ( unsigned int agentId );

    // Knowledge objects
    static const TER_Localisation* GetKnowledgeObjectLocalisation( boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static bool IsKnowledgeObjectValid( boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static std::vector< boost::shared_ptr< DEC_Knowledge_Object > > GetObjectsInZone( const MIL_Population& caller, const TER_Localisation* pZone, const std::vector< std::string >& parameters );
    static std::vector< boost::shared_ptr< DEC_Knowledge_Object > > GetObjectsInCircle( const MIL_Population& caller, double radius, const std::vector< std::string >& parameters );
    static int DamageObject( boost::shared_ptr< DEC_Knowledge_Object > knowledge, double damageFactor ) ;
    static float GetKnowledgeObjectDistance( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > knowledge ) ;
    static boost::shared_ptr<MT_Vector2D> GetKnowledgeObjectClosestPoint( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > knowledge ) ;
    static int IsEnemy( const MIL_Population& callerPopulation, boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static int GetMovingState( MIL_Population& population );
    static bool HasFlow( MIL_Population& population );

    // Etat decisionnel
    static void NotifyDominationStateChanged( MIL_Population& callerPopulation, double dominationState );
    static std::string  GetSzName( MIL_Population& callerPopulation );
    //@}

    static boost::shared_ptr< MIL_Mission_ABC > GetMission( DEC_Decision_ABC* pAgent );
    static void SetMission( DEC_Decision_ABC* object, boost::shared_ptr< MIL_Mission_ABC > mission );

};

#endif // __DEC_PopulationFunctions_h_
