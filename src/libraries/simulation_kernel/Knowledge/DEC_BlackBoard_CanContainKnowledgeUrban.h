// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************
#ifndef __DEC_BlackBoard_CanContainKnowledgeUrban_h_
#define __DEC_BlackBoard_CanContainKnowledgeUrban_h_

#include "MIL.h"

class DEC_Knowledge_Urban;
class MIL_Army_ABC;
class MIL_UrbanObject_ABC;

// =============================================================================
// @class  DEC_BlackBoard_CanContainKnowledgeUrban
// Created: MGD 2009-11-26
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeUrban : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< unsigned int, boost::shared_ptr< DEC_Knowledge_Urban > > T_KnowledgeUrbanMap;
    typedef T_KnowledgeUrbanMap::iterator                                     IT_KnowledgeUrbanMap;
    typedef T_KnowledgeUrbanMap::const_iterator                              CIT_KnowledgeUrbanMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_BlackBoard_CanContainKnowledgeUrban( const MIL_Army_ABC& knowledgeGroup );
    virtual ~DEC_BlackBoard_CanContainKnowledgeUrban();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Finalize();
    boost::shared_ptr< DEC_Knowledge_Urban > CreateKnowledgeUrban( const MIL_Army_ABC& army, const MIL_UrbanObject_ABC& object );
    void DestroyKnowledgeUrban( DEC_Knowledge_Urban& knowledge );
    //@}

    //! @name Queries
    //@{
    boost::shared_ptr< DEC_Knowledge_Urban > GetKnowledgeUrban( const MIL_UrbanObject_ABC& object ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesUrban( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeUrbanMap itKnowledge = urbanMapFromConcrete_.begin(); itKnowledge != urbanMapFromConcrete_.end(); )
        {
            boost::shared_ptr< DEC_Knowledge_Urban > knowledge = itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesUrbanRef( UnaryFunction& fct ) const
    {
        for( CIT_KnowledgeUrbanMap itKnowledge = urbanMapFromConcrete_.begin(); itKnowledge != urbanMapFromConcrete_.end(); )
        {
            DEC_Knowledge_Urban& knowledge = *itKnowledge->second;
            ++itKnowledge;
            fct( knowledge );
        }
    }
    //@}

private:

    //! @name CheckPoints
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_BlackBoard_CanContainKnowledgeUrban* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_BlackBoard_CanContainKnowledgeUrban* role, const unsigned int /*version*/ );
    //@}

    //! @name Member data
    //@{
    const MIL_Army_ABC& army_;
    T_KnowledgeUrbanMap urbanMapFromConcrete_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeUrban )

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::save_construct_data
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
template< typename Archive >
inline void save_construct_data( Archive& archive, const DEC_BlackBoard_CanContainKnowledgeUrban* blackboard, const unsigned int /*version*/ )
{
    const MIL_Army_ABC* const army = &blackboard->army_;
    archive << army;
}

// -----------------------------------------------------------------------------
// Name: DEC_BlackBoard_CanContainKnowledgeUrban::load_construct_data
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
template< typename Archive >
inline void load_construct_data( Archive& archive, DEC_BlackBoard_CanContainKnowledgeUrban* blackboard, const unsigned int /*version*/ )
{
    MIL_Army_ABC* army;
    archive >> army;
    ::new( blackboard )DEC_BlackBoard_CanContainKnowledgeUrban( *army );
}

#endif // __DEC_BlackBoard_CanContainKnowledgeUrban_h_
