// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgePopulationFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 11:38 $
// $Revision: 11 $
// $Workfile: DEC_KnowledgePopulationFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgePopulationFunctions_h_
#define __DEC_KnowledgePopulationFunctions_h_

namespace sword
{
    class Brain;
}

class DEC_Decision_ABC;
class PHY_DotationCategory;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgePopulationFunctions
{
public:
    static void Register( sword::Brain& brain );

    //! @name Functions
    //@{
    static std::pair< float, int > GetDominationState( const DEC_Decision_ABC* caller, unsigned int knowledgeId );
    static boost::shared_ptr< MT_Vector2D > GetBarycenter( const DEC_Decision_ABC* caller, unsigned int knowledgeId );
    static unsigned int GetAttitude( const DEC_Decision_ABC* caller, unsigned int knowledgeId );
    static double GetDangerosity( const DEC_Decision_ABC* caller, int knowledgeId );
    static void Recon( const DEC_Decision_ABC* caller, int knowledgeId );
    static bool IsRecon( const DEC_Decision_ABC* caller, unsigned int knowledgeId );
    static bool IsInZone( const DEC_Decision_ABC* caller, unsigned int knowledgeId, TER_Localisation* pLoc );
    static bool IsPerceivedByAgent( const DEC_Decision_ABC* caller, int knowledgeId );
    static boost::shared_ptr< MT_Vector2D > GetConcentrationPosition( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId );
    static unsigned int GetAllHumansInConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId );
    static unsigned int GetAllHumans( const DEC_Decision_ABC& callerAgent, int knowledgeId );
    static boost::shared_ptr< MT_Vector2D > ClosestPoint( const DEC_Decision_ABC* caller, int knowledgeId );
    static void Secure( const DEC_Decision_ABC* caller, int knowledgeId );
    static boost::shared_ptr< MT_Vector2D > SecuringPoint ( const DEC_Decision_ABC* caller, int knowledgeId );
    static bool IsEnemy( const DEC_Decision_ABC* caller, int knowledgeId );
    static int Exterminate( DEC_Decision_ABC* caller, int knowledgeId, float surface, const PHY_DotationCategory* dotation );
    static std::string GetCriticalIntelligence( const DEC_Decision_ABC* caller, int knowledgeId );
    static bool IsFlowKnown( const DEC_Decision_ABC& caller, int knowledgeId );;
    static void ChangeArmedIndividuals( const DEC_Decision_ABC* callerAgent, int knowledgeId, const double rArmedIndividuals );
    static float GetCrowdAffinity( const DEC_Decision_ABC* callerAgent, int knowledgeId );
    static int GetCrowdAttitude( const DEC_Decision_ABC* callerAgent, int knowledgeId );
    static void StartHidingInCrowd( DEC_Decision_ABC* callerAgent, int knowledgeId );
    static void StopHidingInCrowd( DEC_Decision_ABC* callerAgent, int knowledgeId );
    static std::vector< DEC_Decision_ABC* > GetAgentsHiddenInCrowd( DEC_Decision_ABC* callerAgent, int knowledgeId );
    static boost::shared_ptr< MT_Vector2D > GetFlowHead( const DEC_Decision_ABC& callerAgent, int knowledgeId );
    static int GetClosestConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, const MT_Vector2D& point, int refDistance );
    static void Lock( const DEC_Decision_ABC* callerAgent, int knowledgeId );
    static void Unlock( const DEC_Decision_ABC* callerAgent, int knowledgeId );
    static unsigned int ExtractWoundedFromCrowd( const DEC_Decision_ABC* callerPion, unsigned int knowledgeId, const MT_Vector2D* position );
    static unsigned int ExtractDeadFromCrowd( const DEC_Decision_ABC* caller, unsigned int knowledgeId, const MT_Vector2D* position );
    static std::vector< unsigned int > GetHumansFromAllTypes( const DEC_Decision_ABC* caller, int knowledgeId );
    static bool HealWoundedHumans( const DEC_Decision_ABC* caller, int knowledgeId );
    static bool CanLoadCrowdConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, unsigned int concentrationId );
    static bool IsValid( const DEC_Decision_ABC& callerAgent, int knowledgeId );
    static DEC_Decision_ABC* GetBrain( const DEC_Decision_ABC& callerAgent, int knowledgeId );
    //@}
};

#endif // __DEC_KnowledgePopulationFunctions_h_
