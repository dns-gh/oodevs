// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DEC_KnowledgeUrbanFunctions_h_
#define __DEC_KnowledgeUrbanFunctions_h_

#include "Knowledge/DEC_Knowledge_Def.h"

class DEC_Decision_ABC;
class DEC_Knowledge_Urban;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_KnowledgeUrbanFunctions
    @brief  DEC Knowledge urban functions
*/
// Created: SLG 2010-02-01
// =============================================================================
class DEC_KnowledgeUrbanFunctions
{
public:
    //! @name Functions
    //@{
    static float GetCurrentRecceProgress( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    static boost::shared_ptr< MT_Vector2D > GetCurrentBarycenter( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    static float GetPathfindCost ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    static float GetRapForLocal( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    static double GetPerception( const MIL_AgentPion& callerAgent, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget );
    static T_ConstKnowledgeAgentVector GetLivingEnemiesInBU( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    //@}
};

#endif // __DEC_KnowledgeUrbanFunctions_h_
