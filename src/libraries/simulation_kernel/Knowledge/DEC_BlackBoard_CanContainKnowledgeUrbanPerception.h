// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_BlackBoard_CanContainKnowledgeUrbanPerception_h_
#define __DEC_BlackBoard_CanContainKnowledgeUrbanPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_Def.h"

class DEC_Knowledge_UrbanPerception;
class MIL_Agent_ABC;
class UrbanObjectWrapper;

// =============================================================================
// @class  DEC_BlackBoard_CanContainKnowledgeUrban
// Created: MGD 2009-12-07
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeUrbanPerception : private boost::noncopyable
{
private:
    //! @name Types
    //@{
    typedef std::map< unsigned, boost::shared_ptr< DEC_Knowledge_UrbanPerception > > T_KnowledgeUrbanPerceptionMap;
    typedef T_KnowledgeUrbanPerceptionMap::iterator                                 IT_KnowledgeUrbanPerceptionMap;
    typedef T_KnowledgeUrbanPerceptionMap::const_iterator                          CIT_KnowledgeUrbanPerceptionMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_BlackBoard_CanContainKnowledgeUrbanPerception();
    virtual ~DEC_BlackBoard_CanContainKnowledgeUrbanPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< DEC_Knowledge_UrbanPerception > CreateKnowledgeUrbanPerception ( const MIL_Agent_ABC& agentPerceiving, const UrbanObjectWrapper& objectPerceived );
    void DestroyKnowledgeUrbanPerception( DEC_Knowledge_UrbanPerception& knowledge );
    //@}

    //! @name Queries
    //@{
    boost::shared_ptr< DEC_Knowledge_UrbanPerception > GetKnowledgeUrbanPerception( const UrbanObjectWrapper& associatedUrban ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesUrbanPerception( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeUrbanPerceptionMap itKnowledge = knowledgeUrbanPerceptionMap_.begin(); itKnowledge != knowledgeUrbanPerceptionMap_.end(); )
        {
            DEC_Knowledge_UrbanPerception& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }

    template < class BinaryFunction, class Parameter >
    void ApplyOnKnowledgesUrbanPerception( BinaryFunction& fct, Parameter param ) const
    {
        for( CIT_KnowledgeUrbanPerceptionMap itKnowledge = knowledgeUrbanPerceptionMap_.begin(); itKnowledge != knowledgeUrbanPerceptionMap_.end(); )
        {
            DEC_Knowledge_UrbanPerception& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge, param );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    T_KnowledgeUrbanPerceptionMap knowledgeUrbanPerceptionMap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeUrbanPerception )

#endif // __DEC_BlackBoard_CanContainKnowledgeUrbanPerception_h_
