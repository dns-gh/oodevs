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
#include <tools/Map.h>
#include <boost/shared_ptr.hpp>

class DEC_Knowledge_Population;
class MIL_KnowledgeGroup;
class MIL_Population;
class KnowledgesVisitor_ABC;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgePopulation : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef tools::Map< const MIL_Population*, boost::shared_ptr< DEC_Knowledge_Population > > T_KnowledgePopulationMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgePopulation( MIL_KnowledgeGroup* group );
             DEC_BlackBoard_CanContainKnowledgePopulation();
    virtual ~DEC_BlackBoard_CanContainKnowledgePopulation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive& , const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_Population& CreateKnowledgePopulation ( const MIL_KnowledgeGroup& group, MIL_Population& populationPerceived );
    void DestroyKnowledgePopulation( DEC_Knowledge_Population& knowledge );
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    void Merge( const DEC_BlackBoard_CanContainKnowledgePopulation& subGroup );
    //@}

    //! @name Queries
    //@{
    boost::shared_ptr< DEC_Knowledge_Population > GetKnowledgePopulation( const MIL_Population& associatedPopulation ) const;
    boost::shared_ptr< DEC_Knowledge_Population > GetKnowledgePopulationFromID( unsigned int nID ) const;
    const T_KnowledgePopulationMap& GetKnowledgePopulations() const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulation( UnaryFunction& fct ) const
    {
        for( auto it = knowledgePopulationMap_.begin(); it != knowledgePopulationMap_.end(); )
        {
            DEC_Knowledge_Population& knowledge = *it->second;
            ++it;
            fct( knowledge );
        }
    }
    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulation( UnaryFunction& fct, int currentTimeStep ) const
    {
        for( auto itKnowledge = knowledgePopulationMap_.begin(); itKnowledge != knowledgePopulationMap_.end(); )
        {
            DEC_Knowledge_Population& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge, currentTimeStep );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    MIL_KnowledgeGroup* pKnowledgeGroup_;
    T_KnowledgePopulationMap knowledgePopulationMap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgePopulation )

#endif // __DEC_BlackBoard_CanContainKnowledgePopulation_h_
