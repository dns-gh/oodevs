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
#include <map>

class DEC_Knowledge_UrbanPerception;
class MIL_Agent_ABC;
class MIL_UrbanObject_ABC;

// =============================================================================
// @class  DEC_BlackBoard_CanContainKnowledgeUrban
// Created: MGD 2009-12-07
// =============================================================================
class DEC_BlackBoard_CanContainKnowledgeUrbanPerception : private boost::noncopyable
{
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
    DEC_Knowledge_UrbanPerception* CreateKnowledgeUrbanPerception ( const MIL_Agent_ABC& agentPerceiving, const MIL_UrbanObject_ABC& objectPerceived );
    void Clear();
    //@}

    //! @name Queries
    //@{
    DEC_Knowledge_UrbanPerception* GetKnowledgeUrbanPerception( const MIL_UrbanObject_ABC& associatedUrban ) const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesUrbanPerception( UnaryFunction& fct ) const
    {
        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            fct( *it->second );
    }

    template < class BinaryFunction, class Parameter >
    void ApplyOnKnowledgesUrbanPerception( BinaryFunction& fct, Parameter param ) const
    {
        for( auto it = perceptions_.begin(); it != perceptions_.end(); ++it )
            fct( *it->second, param );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, DEC_Knowledge_UrbanPerception* > perceptions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_BlackBoard_CanContainKnowledgeUrbanPerception )

#endif // __DEC_BlackBoard_CanContainKnowledgeUrbanPerception_h_
