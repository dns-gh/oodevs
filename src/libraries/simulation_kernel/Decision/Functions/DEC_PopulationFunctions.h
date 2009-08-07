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

#include "MIL.h"

class DEC_Decision_ABC;
class MIL_Population;
class MIL_Mission_ABC;

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

    // Effects
    static void SetAttitude    ( MIL_Population& callerPopulation, unsigned int attitudeId );
    static unsigned int  GetAttitude    ( const MIL_Population& callerPopulation );

    // Knowledge agents
    static int GetKnowledgeAgentRoePopulation ( uint agentId );

    // Knowledge objects
    static boost::shared_ptr<MT_Vector2D> GetKnowledgeObjectLocalisation 	( int knowledgeId);
    static bool IsKnowledgeObjectValid										( unsigned int nID );
    static std::vector< unsigned int > GetObjectsInZone						( const TER_Localisation* pZone, const std::vector< std::string >& parameters );
    static int DamageObject													( int knowledgeId, double damageFactor ) ;
    static float GetKnowledgeObjectDistance									( const MIL_Population& callerPopulation, int knowledgeId ) ;
    static boost::shared_ptr<MT_Vector2D> GetKnowledgeObjectClosestPoint 	( const MIL_Population& callerPopulation, int knowledgeId ) ;
    static int IsEnemy                         								( const MIL_Population& callerPopulation, int knowledgeId   );

    // Etat decisionnel
    static void NotifyDominationStateChanged( MIL_Population& callerPopulation, MT_Float dominationState );

    static std::string  GetSzName( MIL_Population& callerPopulation );
    //@}    
    
    static MIL_Mission_ABC* GetMission( DEC_Decision_ABC* pAgent );
    static void SetMission            ( DEC_Decision_ABC* object, MIL_Mission_ABC* mission );  

};

#endif // __DEC_PopulationFunctions_h_
