// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __KnowledgeConverter_ABC_h_
#define __KnowledgeConverter_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class AgentKnowledge_ABC;

// =============================================================================
/** @class  KnowledgeConverter_ABC
    @brief  KnowledgeConverter_ABC
*/
// Created: MCO 2014-09-05
// =============================================================================
class KnowledgeConverter_ABC : private boost::noncopyable
{
public:
             KnowledgeConverter_ABC() {}
    virtual ~KnowledgeConverter_ABC() {}

    virtual const kernel::AgentKnowledge_ABC* FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) const = 0;
};

}

#endif // __KnowledgeConverter_ABC_h_
