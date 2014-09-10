// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_BlackBoard_CanContainKnowledgeObjectPerception.h $
// $Author: Jvt $
// $Modtime: 6/04/05 18:56 $
// $Revision: 3 $
// $Workfile: DEC_BlackBoard_CanContainKnowledgeObjectPerception.h $
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeObjectPerception_h_
#define __DEC_BlackBoard_CanContainKnowledgeObjectPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_ObjectPerception;
class MIL_Agent_ABC;
class MIL_Object_ABC;

// =============================================================================
/** @class  DEC_BlackBoard_CanContainKnowledgeObjectPerception
    @brief  The base class representing all the knowledges of an entity
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeObjectPerception : private boost::noncopyable
{

private:
    //! @name Types
    //@{
    typedef std::map< const MIL_Object_ABC*, DEC_Knowledge_ObjectPerception* > T_KnowledgeObjectPerceptionMap;
    typedef T_KnowledgeObjectPerceptionMap::iterator                               IT_KnowledgeObjectPerceptionMap;
    typedef T_KnowledgeObjectPerceptionMap::const_iterator                         CIT_KnowledgeObjectPerceptionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgeObjectPerception();
    virtual ~DEC_BlackBoard_CanContainKnowledgeObjectPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    DEC_Knowledge_ObjectPerception& CreateKnowledgeObjectPerception ( const MIL_Agent_ABC& agentPerceiving, MIL_Object_ABC& objectPerceived );
    void DestroyKnowledgeObjectPerception( DEC_Knowledge_ObjectPerception& knowledge );
    void Clear();
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_ObjectPerception* GetKnowledgeObjectPerception( const MIL_Object_ABC& associatedObject ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesObjectPerception( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeObjectPerceptionMap itKnowledge = knowledgeObjectPerceptionMap_.begin(); itKnowledge != knowledgeObjectPerceptionMap_.end(); )
        {
            DEC_Knowledge_ObjectPerception& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }

    template < class BinaryFunction, class Parameter >
    void ApplyOnKnowledgesObjectPerception( BinaryFunction& fct, Parameter param ) const
    {
        for( CIT_KnowledgeObjectPerceptionMap itKnowledge = knowledgeObjectPerceptionMap_.begin(); itKnowledge != knowledgeObjectPerceptionMap_.end(); )
        {
            DEC_Knowledge_ObjectPerception& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge, param );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeObjectPerceptionMap   knowledgeObjectPerceptionMap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeObjectPerception )

#endif // __DEC_BlackBoard_CanContainKnowledgeObjectPerception_h_
