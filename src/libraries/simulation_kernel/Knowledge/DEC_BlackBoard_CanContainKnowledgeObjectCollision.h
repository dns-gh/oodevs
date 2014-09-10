// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectCollision.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:28 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectCollision.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeObjectCollision_h_
#define __DEC_BlackBoard_CanContainKnowledgeObjectCollision_h_

#include "MIL.h"
#include "DEC_Knowledge_Def.h"
#include <tools/Map.h>

class DEC_Knowledge_ObjectCollision;
class MIL_Object_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeObjectCollision
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeObjectCollision : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgeObjectCollision();
    virtual ~DEC_BlackBoard_CanContainKnowledgeObjectCollision();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_ObjectCollision& CreateKnowledgeObjectCollision ( const MIL_Agent_ABC& agentColliding, MIL_Object_ABC& objectCollision );
    void DestroyKnowledgeObjectCollision( DEC_Knowledge_ObjectCollision& knowledge );
    void Clear();
    //@}

    //! @name Queries
    //@{
    void                           GetKnowledgesObjectCollision( T_KnowledgeObjectCollisionVector& container ) const;
    DEC_Knowledge_ObjectCollision* GetKnowledgeObjectCollision ( const MIL_Object_ABC& objectCollision ) const;

    template< class UnaryFunction >
    void ApplyOnKnowledgesObjectCollision( UnaryFunction& fct ) const
    {
        for( auto it = collisions_.begin(); it != collisions_.end(); )
        {
            DEC_Knowledge_ObjectCollision& knowledge = *it->second;
            ++it;
            fct( knowledge );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    tools::Map< const MIL_Object_ABC*, DEC_Knowledge_ObjectCollision* > collisions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeObjectCollision )

#endif // __DEC_BlackBoard_CanContainKnowledgeObjectCollision_h_
