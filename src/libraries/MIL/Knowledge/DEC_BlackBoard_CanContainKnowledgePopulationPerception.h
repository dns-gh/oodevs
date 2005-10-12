// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:56 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulationPerception.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgePopulationPerception_h_
#define __DEC_BlackBoard_CanContainKnowledgePopulationPerception_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_PopulationPerception;
class MIL_AgentPion;
class MIL_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgePopulationPerception
{
    MT_COPYNOTALLOWED( DEC_BlackBoard_CanContainKnowledgePopulationPerception )

private:
    //! @name Types
    //@{
    typedef std::map< const MIL_Population*, DEC_Knowledge_PopulationPerception* > T_KnowledgePopulationPerceptionMap;
    typedef T_KnowledgePopulationPerceptionMap::iterator                           IT_KnowledgePopulationPerceptionMap;
    typedef T_KnowledgePopulationPerceptionMap::const_iterator                     CIT_KnowledgePopulationPerceptionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     DEC_BlackBoard_CanContainKnowledgePopulationPerception();
    ~DEC_BlackBoard_CanContainKnowledgePopulationPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive& , const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_PopulationPerception& CreateKnowledgePopulationPerception ( const MIL_AgentPion& agentPerceiving, MIL_Population& populationPerceived );
    void                                DestroyKnowledgePopulationPerception( DEC_Knowledge_PopulationPerception& knowledge );
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_PopulationPerception* GetKnowledgePopulationPerception( const MIL_Population& associatedPopulation ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulationPerception( UnaryFunction fct ) const
    {
        for( CIT_KnowledgePopulationPerceptionMap itKnowledge = knowledgePopulationPerceptionMap_.begin(); itKnowledge != knowledgePopulationPerceptionMap_.end(); )
        {
            DEC_Knowledge_PopulationPerception& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgePopulationPerceptionMap knowledgePopulationPerceptionMap_;
    //@}
};

#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.inl"

#endif // __DEC_BlackBoard_CanContainKnowledgePopulationPerception_h_
