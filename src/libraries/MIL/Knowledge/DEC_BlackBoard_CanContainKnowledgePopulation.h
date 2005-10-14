// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulation.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:56 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulation.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgePopulation_h_
#define __DEC_BlackBoard_CanContainKnowledgePopulation_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_Population;
class MIL_KnowledgeGroup;
class MIL_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgePopulation
{
    MT_COPYNOTALLOWED( DEC_BlackBoard_CanContainKnowledgePopulation )

private:
    //! @name Types
    //@{
    typedef std::map< const MIL_Population*, DEC_Knowledge_Population* > T_KnowledgePopulationMap;
    typedef T_KnowledgePopulationMap::iterator                           IT_KnowledgePopulationMap;
    typedef T_KnowledgePopulationMap::const_iterator                     CIT_KnowledgePopulationMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     DEC_BlackBoard_CanContainKnowledgePopulation();
    ~DEC_BlackBoard_CanContainKnowledgePopulation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive& , const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_Population& CreateKnowledgePopulation ( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& populationPerceived );
    void                      DestroyKnowledgePopulation( DEC_Knowledge_Population& knowledge );
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_Population* GetKnowledgePopulation( const MIL_Population& associatedPopulation ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulation( UnaryFunction fct ) const
    {
        for( CIT_KnowledgePopulationMap itKnowledge = knowledgePopulationMap_.begin(); itKnowledge != knowledgePopulationMap_.end(); )
        {
            DEC_Knowledge_Population& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgePopulationMap knowledgePopulationMap_;
    //@}
};

#include "DEC_BlackBoard_CanContainKnowledgePopulation.inl"

#endif // __DEC_BlackBoard_CanContainKnowledgePopulation_h_
