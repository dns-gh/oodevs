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
#include <tools/Map.h>

class DEC_Knowledge_PopulationPerception;
class MIL_Agent_ABC;
class MIL_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgePopulationPerception : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgePopulationPerception();
    virtual ~DEC_BlackBoard_CanContainKnowledgePopulationPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive& , const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_PopulationPerception& CreateKnowledgePopulationPerception ( const MIL_Agent_ABC& agentPerceiving, MIL_Population& populationPerceived );
    void DestroyKnowledgePopulationPerception( DEC_Knowledge_PopulationPerception& knowledge );
    void Clear();
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_PopulationPerception* GetKnowledgePopulationPerception( const MIL_Population& associatedPopulation ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulationPerception( UnaryFunction& fct ) const
    {
        for( auto it = perceptions_.begin(); it != perceptions_.end(); )
        {
            DEC_Knowledge_PopulationPerception& knowledge = *it->second;
            ++it;
            fct( knowledge );
        }
    }

    template < class BinaryFunction, class Parameter >
    void ApplyOnKnowledgesPopulationPerception( BinaryFunction& fct, Parameter param ) const
    {
        for( auto it = perceptions_.begin(); it != perceptions_.end(); )
        {
            DEC_Knowledge_PopulationPerception& knowledge = *it->second;
            ++it;
            fct( knowledge, param );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    tools::Map< const MIL_Population*, DEC_Knowledge_PopulationPerception* > perceptions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgePopulationPerception )

#endif // __DEC_BlackBoard_CanContainKnowledgePopulationPerception_h_
