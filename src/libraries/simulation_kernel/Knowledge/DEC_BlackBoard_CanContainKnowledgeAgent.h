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
class KnowledgesVisitor_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeAgent : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Agent_ABC*, boost::shared_ptr< DEC_Knowledge_Agent > > T_KnowledgeAgentMap;
    typedef T_KnowledgeAgentMap::iterator                                              IT_KnowledgeAgentMap;
    typedef T_KnowledgeAgentMap::const_iterator                                        CIT_KnowledgeAgentMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgeAgent( const MIL_KnowledgeGroup& knowledgeGroup );
             DEC_BlackBoard_CanContainKnowledgeAgent();
    virtual ~DEC_BlackBoard_CanContainKnowledgeAgent();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_Agent& CreateKnowledgeAgent ( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown );
    void                 DestroyKnowledgeAgent( DEC_Knowledge_Agent& knowledge );
    void                 SaveAllCurrentKnowledgeAgent();
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    //@}

    //! @name Queries
    //@{
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgentFromID( unsigned int nID ) const;
    boost::shared_ptr< DEC_Knowledge_Agent > GetKnowledgeAgent      ( const MIL_Agent_ABC& agentKnown ) const;
    bool                 HasKnowledgeAgent      ( const MIL_Agent_ABC& agentKnown ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgent( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeAgentMap itKnowledge = realAgentMap_.begin(); itKnowledge != realAgentMap_.end(); )
        {
            DEC_Knowledge_Agent& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    template < class UnaryFunction >
    void ApplyOnPreviousKnowledgesAgent( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeAgentMap itKnowledge = previousAgentMap_.begin(); itKnowledge != previousAgentMap_.end(); )
        {
            DEC_Knowledge_Agent& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    template < class BinaryFunction >
    void ApplyOnKnowledgesAgent( BinaryFunction& fct, int currentTimeStep ) const
    {
        for( CIT_KnowledgeAgentMap itKnowledge = realAgentMap_.begin(); itKnowledge != realAgentMap_.end(); )
        {
            DEC_Knowledge_Agent& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge, currentTimeStep );
        }
    }
    //@}

    //! @name Queries
    //@{
    const T_KnowledgeAgentVector& GetDetected         ();
    const T_KnowledgeAgentVector& GetFriends          ();
    const T_KnowledgeAgentVector& GetEnemies          ();
    const T_KnowledgeAgentVector& GetMilitias         ();
    const T_KnowledgeAgentVector& GetTerrorists       ();
    const T_KnowledgeAgentVector& GetRefugees         ();
    const T_KnowledgeAgentVector& GetSurrenderedAgents();
    //@}

private:
    //! @name Tools
    //@{
    void UpdateQueriesCache();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, boost::shared_ptr< DEC_Knowledge_Agent > > T_KnowledgeAgentIDMap;
    typedef T_KnowledgeAgentIDMap::iterator                            IT_KnowledgeAgentIDMap;
    typedef T_KnowledgeAgentIDMap::const_iterator                      CIT_KnowledgeAgentIDMap;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_KnowledgeGroup*   pKnowledgeGroup_;
          T_KnowledgeAgentMap   realAgentMap_;
          T_KnowledgeAgentMap   previousAgentMap_;
          T_KnowledgeAgentIDMap unitKnowledgeFromIDMap_;
    //@}

    //! @name Queries caches
    //@{
    unsigned int nLastCacheUpdateTick_;

    T_KnowledgeAgentVector detectedContainer_;
    T_KnowledgeAgentVector friendsContainer_;
    T_KnowledgeAgentVector enemiesContainer_;
    T_KnowledgeAgentVector militiasContainer_;
    T_KnowledgeAgentVector terroristsContainer_;
    T_KnowledgeAgentVector refugeesContainer_;
    T_KnowledgeAgentVector surrenderedAgentsContainer_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeAgent )

#endif // __DEC_BlackBoard_CanContainKnowledgeAgent_h_
