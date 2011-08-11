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

class MIL_AgentPion;
class DEC_Decision_ABC;
class PHY_DotationCategory;
class TER_Localisation;
class MT_Vector2D;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgePopulationFunctions
{
public:
    //! @name Functions
    //@{
    template< typename T > static std::pair< float, int > GetDominationState( const T& caller, unsigned int knowledgeId );
    template< typename T > static unsigned int GetAttitude( const T& caller, unsigned int knowledgeId );
                           static float GetDangerosity( const MIL_AgentPion& caller, int knowledgeId );
                           static void Recon( const MIL_AgentPion& caller, int knowledgeId );
    template< typename T > static bool IsRecon( const T& caller, unsigned int knowledgeId );
    template< typename T > static bool IsInZone( const T& caller, unsigned int knowledgeId, TER_Localisation* pLoc );
                           static bool IsPerceivedByAgent( const MIL_AgentPion& caller, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > GetConcentrationPosition( const MIL_AgentPion& caller, int knowledgeId, unsigned int concentrationId );
                           static unsigned int GetAllHumansInConcentration( const MIL_AgentPion& caller, int knowledgeId, unsigned int concentrationId );
                           static boost::shared_ptr< MT_Vector2D > ClosestPoint( const MIL_AgentPion& caller, int knowledgeId );
                           static void Secure( const MIL_AgentPion& caller, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > SecuringPoint ( const MIL_AgentPion& caller, int knowledgeId );
                           static bool IsEnemy( const MIL_AgentPion& caller, int knowledgeId );
                           static int Exterminate( MIL_AgentPion& caller, int knowledgeId, float surface, const PHY_DotationCategory* dotation );
                           static std::string GetCriticalIntelligence( const MIL_AgentPion& caller, int knowledgeId );
                           static bool HasFlow( const MIL_AgentPion& caller, int knowledgeId );
                           static void ChangeArmedIndividuals( const MIL_AgentPion& callerAgent, int knowledgeId, const double rArmedIndividuals );
                           static float GetCrowdAffinity( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static int GetCrowdAttitude( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static boost::shared_ptr< MT_Vector2D > GetFlowHead( const DEC_Decision_ABC& callerAgent, int knowledgeId );
                           static int GetClosestConcentration( const DEC_Decision_ABC& callerAgent, int knowledgeId, const MT_Vector2D& point, int refDistance );
    //@}
};

#include "DEC_KnowledgePopulationFunctions.inl"

#endif // __DEC_KnowledgePopulationFunctions_h_
