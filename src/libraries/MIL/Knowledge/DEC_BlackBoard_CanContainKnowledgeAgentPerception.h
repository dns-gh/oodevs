// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.h $
// $Author: Jvt $
// $Modtime: 6/04/05 19:10 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgentPerception.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeAgentPerception_h_
#define __DEC_BlackBoard_CanContainKnowledgeAgentPerception_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_AgentPerception;
class MIL_AgentPion;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeAgentPerception
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeAgentPerception
{
    MT_COPYNOTALLOWED( DEC_BlackBoard_CanContainKnowledgeAgentPerception )

public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Agent_ABC*, DEC_Knowledge_AgentPerception* > T_KnowledgeAgentPerceptionMap;
    typedef T_KnowledgeAgentPerceptionMap::iterator                          IT_KnowledgeAgentPerceptionMap;
    typedef T_KnowledgeAgentPerceptionMap::const_iterator                    CIT_KnowledgeAgentPerceptionMap;

    typedef std::set< DEC_Knowledge_AgentPerception* >      T_KnowledgeAgentPerceptionSet;
    typedef T_KnowledgeAgentPerceptionSet::iterator         IT_KnowledgeAgentPerceptionSet;
    typedef T_KnowledgeAgentPerceptionSet::const_iterator   CIT_KnowledgeAgentPerceptionSet;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     DEC_BlackBoard_CanContainKnowledgeAgentPerception();
    ~DEC_BlackBoard_CanContainKnowledgeAgentPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_AgentPerception& CreateKnowledgeAgentPerception ( const MIL_AgentPion& agentPerceiving, MIL_Agent_ABC& agentPerceived );
    void                           DestroyKnowledgeAgentPerception( DEC_Knowledge_AgentPerception& knowledge );
    //@}

    //! @name Events
    //@{
    void NotifyKnowledgeAgentPerceptionIsAttacker   ( DEC_Knowledge_AgentPerception& knowledge );
    void NotifyKnowledgesAgentPerceptionNotAttackers();
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_AgentPerception* GetKnowledgeAgentPerception         ( const MIL_Agent_ABC& associatedAgent ) const;
    void                           GetAttackersKnowledgeAgentPerception( T_KnowledgeAgentPerceptionVector& container ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgentPerception( UnaryFunction fct ) const
    {
        for( CIT_KnowledgeAgentPerceptionMap itKnowledge = knowledgeAgentPerceptionMap_.begin(); itKnowledge != knowledgeAgentPerceptionMap_.end(); )
        {
            DEC_Knowledge_AgentPerception& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }           
    }
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeAgentPerceptionMap knowledgeAgentPerceptionMap_;
    T_KnowledgeAgentPerceptionSet attackerSet_;
    //@}
};

#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.inl"

#endif // __DEC_BlackBoard_CanContainKnowledgeAgentPerception_h_
