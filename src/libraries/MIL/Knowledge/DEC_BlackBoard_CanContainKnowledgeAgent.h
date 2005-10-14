// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h $
// $Author: Nld $
// $Modtime: 20/04/05 19:27 $
// $Revision: 4 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeAgent.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeAgent_h_
#define __DEC_BlackBoard_CanContainKnowledgeAgent_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_Agent;
class MIL_Agent_ABC;
class MIL_KnowledgeGroup;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeAgent
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeAgent
{
    MT_COPYNOTALLOWED( DEC_BlackBoard_CanContainKnowledgeAgent )

public:
    //! @name Constructors/Destructor
    //@{
     DEC_BlackBoard_CanContainKnowledgeAgent();
    ~DEC_BlackBoard_CanContainKnowledgeAgent();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_Agent& CreateKnowledgeAgent ( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown );
    void                 DestroyKnowledgeAgent( DEC_Knowledge_Agent& knowledge );
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_Agent* GetKnowledgeAgentFromMosID( uint nID ) const;
    DEC_Knowledge_Agent* GetKnowledgeAgentFromDiaID( uint nID ) const;
    DEC_Knowledge_Agent* GetKnowledgeAgent( const MIL_Agent_ABC& agentKnown ) const;
    bool                 HasKnowledgeAgent( const MIL_Agent_ABC& agentKnown ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgent( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeAgentMap itKnowledge = knowledgeAgentMap_.begin(); itKnowledge != knowledgeAgentMap_.end(); )
        {
            DEC_Knowledge_Agent& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

    //! @name Types
    //@{
    typedef std::map< const MIL_Agent_ABC*, DEC_Knowledge_Agent* > T_KnowledgeAgentMap;
    typedef T_KnowledgeAgentMap::iterator                          IT_KnowledgeAgentMap;
    typedef T_KnowledgeAgentMap::const_iterator                    CIT_KnowledgeAgentMap;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, DEC_Knowledge_Agent* >   T_KnowledgeAgentIDMap;
    typedef T_KnowledgeAgentIDMap::iterator          IT_KnowledgeAgentIDMap;
    typedef T_KnowledgeAgentIDMap::const_iterator    CIT_KnowledgeAgentIDMap;
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeAgentMap   knowledgeAgentMap_;
    T_KnowledgeAgentIDMap knowledgeAgentFromMosIDMap_;
    T_KnowledgeAgentIDMap knowledgeAgentFromDiaIDMap_;
    //@}
};

#include "DEC_BlackBoard_CanContainKnowledgeAgent.inl"

#endif // __DEC_BlackBoard_CanContainKnowledgeAgent_h_
