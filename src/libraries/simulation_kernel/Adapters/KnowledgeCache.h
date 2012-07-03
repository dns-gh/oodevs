// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_KNOWLEDGE_CACHE_H
#define SWORD_KNOWLEDGE_CACHE_H

#include "simulation_terrain/TER_Localisation.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class DEC_Knowledge_Object;

namespace sword
{
// =============================================================================
/** @class  KnowledgeCache
    @brief  Knowledge cache
*/
// Created: MCO 2012-02-02
// =============================================================================
class KnowledgeCache
{
public:
    //! @name Member data
    //@{
    std::vector< TER_Localisation > geometrySignatures_;
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > objectsToAvoid_;
    //@}
};

}

#endif // SWORD_KNOWLEDGE_CACHE_H
