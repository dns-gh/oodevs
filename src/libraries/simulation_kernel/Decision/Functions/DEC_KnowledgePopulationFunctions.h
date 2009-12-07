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

#include "MIL.h"

class MIL_AgentPion;

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
                           static float GetDangerosity    ( const MIL_AgentPion& caller, int knowledgeId);
                           static void Recon             ( const MIL_AgentPion& caller, int knowledgeId );
    template< typename T > static bool IsRecon           ( const T& caller, unsigned int knowledgeId );
    template< typename T > static bool IsInZone          ( const T& caller, unsigned int knowledgeId, TER_Localisation* pLoc );
                           static bool IsPerceivedByAgent( const MIL_AgentPion& caller, int knowledgeId );
                           static boost::shared_ptr<MT_Vector2D> ClosestPoint      ( const MIL_AgentPion& caller, int knowledgeId );
                           static void Secure            ( const MIL_AgentPion& caller, int knowledgeId );
                           static boost::shared_ptr<MT_Vector2D> SecuringPoint ( const MIL_AgentPion& caller, int knowledgeId );
                           static bool IsEnemy           ( const MIL_AgentPion& caller, int knowledgeId );
                           static int Exterminate		 ( const MIL_AgentPion& caller, int knowledgeId, float surface );
    //@}
};

#include "DEC_KnowledgePopulationFunctions.inl"

#endif // __DEC_KnowledgePopulationFunctions_h_
