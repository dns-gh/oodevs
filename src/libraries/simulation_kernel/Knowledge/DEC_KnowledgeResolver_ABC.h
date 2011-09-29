// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_KnowledgeResolver_ABC_h_
#define __DEC_KnowledgeResolver_ABC_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace sword
{
    class ObjectKnowledgeId;
    class CrowdKnowledgeId;
    class UnitKnowledgeId;
}

class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class MIL_Object_ABC;
class MIL_Population;
class MIL_Agent_ABC;

// =============================================================================
/** @class  DEC_KnowledgeResolver_ABC
    @brief  DEC_KnowledgeResolver_ABC
*/
// Created: NLD 2006-11-22
// =============================================================================
class DEC_KnowledgeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KnowledgeResolver_ABC() {}
    virtual ~DEC_KnowledgeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const sword::UnitKnowledgeId&  asn ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( const MIL_Agent_ABC& agent ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Agent > ResolveKnowledgeAgent( unsigned int nID ) const = 0;

    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const sword::ObjectKnowledgeId& asn ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int nID ) const = 0;
    virtual boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int nID ) const = 0;

    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const sword::CrowdKnowledgeId& asn ) const = 0;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( const MIL_Population& population ) const = 0;
    virtual DEC_Knowledge_Population* ResolveKnowledgePopulation( unsigned int nID ) const = 0;
    //@}
};

#endif // __DEC_KnowledgeResolver_ABC_h_
