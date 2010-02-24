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

class DEC_Decision_ABC;
class DEC_Knowledge_Urban;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_KnowledgeUrbanFunctions
    @brief  DEC_KnowledgeUrbanFunctions
*/
// Created: SLG 2010-02-01
// =============================================================================
class DEC_KnowledgeUrbanFunctions
{

public:
    //! @name Functions
    //@{
    static int GetCurrentPerceptionLevel( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    static boost::shared_ptr< MT_Vector2D > GetCurrentBarycenter( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    static float GetTrafficability       ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge );
    //@}
};

#endif // __DEC_KnowledgeUrbanFunctions_h_
