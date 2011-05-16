// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationCollision.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:28 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgePopulationCollision.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgePopulationCollision_h_
#define __DEC_BlackBoard_CanContainKnowledgePopulationCollision_h_

#include "MIL.h"
#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_PopulationCollision;
class MIL_Population;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgePopulationCollision
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgePopulationCollision : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< const MIL_Population*, DEC_Knowledge_PopulationCollision* > T_KnowledgePopulationCollisionMap;
    typedef T_KnowledgePopulationCollisionMap::iterator                           IT_KnowledgePopulationCollisionMap;
    typedef T_KnowledgePopulationCollisionMap::const_iterator                     CIT_KnowledgePopulationCollisionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgePopulationCollision();
    virtual ~DEC_BlackBoard_CanContainKnowledgePopulationCollision();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_PopulationCollision& CreateKnowledgePopulationCollision ( const MIL_Agent_ABC& agentColliding, MIL_Population& population );
    void                               DestroyKnowledgePopulationCollision( DEC_Knowledge_PopulationCollision& knowledge );
    //@}

    //! @name Queries
    //@{
    void                               GetKnowledgesPopulationCollision( T_KnowledgePopulationCollisionVector& container ) const;
    DEC_Knowledge_PopulationCollision* GetKnowledgePopulationCollision ( const MIL_Population& populationCollision ) const;
    bool HasCollision() const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulationCollision( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgePopulationCollisionMap itKnowledge = knowledgePopulationCollisionMap_.begin(); itKnowledge != knowledgePopulationCollisionMap_.end(); )
        {
            DEC_Knowledge_PopulationCollision& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgePopulationCollisionMap   knowledgePopulationCollisionMap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgePopulationCollision )

#endif // __DEC_BlackBoard_CanContainKnowledgePopulationCollision_h_
