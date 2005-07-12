// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AgentKnowledgeSynthetizer.h $
// $Author: Jvt $
// $Modtime: 24/03/05 13:11 $
// $Revision: 5 $
// $Workfile: DEC_KS_AgentKnowledgeSynthetizer.h $
//
// *****************************************************************************

#ifndef __DEC_KS_AgentKnowledgeSynthetizer_h_
#define __DEC_KS_AgentKnowledgeSynthetizer_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class DEC_Knowledge_AgentPerception;
class DEC_Knowledge_Agent;

class MIL_KnowledgeGroup;
class MIL_Agent_ABC;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_KS_AgentKnowledgeSynthetizer
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used :
                - To synthetize the subordinate's perceptions of the agents 
                  into knowledges for a GTIA
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_AgentKnowledgeSynthetizer : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_AgentKnowledgeSynthetizer ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_AgentKnowledgeSynthetizer( DEC_KnowledgeBlackBoard& blackBoard, const MIL_KnowledgeGroup& knowledgeGroup );
    virtual ~DEC_KS_AgentKnowledgeSynthetizer();
    //@}
    
    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    //@}

private:
    //! @name Preparation
    //@{
    void PrepareKnowledgeAgent    ( DEC_Knowledge_Agent& knowledge );
    void ExtrapolateKnowledgeAgent( DEC_Knowledge_Agent& knowledge );
    //@}

    //! @name Update
    //@{
    void                 UpdateKnowledgesFromAgentPerception( const DEC_Knowledge_AgentPerception& perception );
    DEC_Knowledge_Agent& GetKnowledgeToUpdate               ( MIL_Agent_ABC& agentKnown ) const;
    //@}

private:
    //! @name Members
    //@{
    const MIL_KnowledgeGroup* pKnowledgeGroup_;
    //@}
};

#include "DEC_KS_AgentKnowledgeSynthetizer.inl"

#endif // __DEC_KS_AgentKnowledgeSynthetizer_h_
