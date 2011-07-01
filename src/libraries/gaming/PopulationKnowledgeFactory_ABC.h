// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledgeFactory_ABC_h_
#define __PopulationKnowledgeFactory_ABC_h_

#include "protocol/Protocol.h"

namespace kernel
{
    class KnowledgeGroup_ABC;
    class PopulationKnowledge_ABC;
}

// =============================================================================
/** @class  PopulationKnowledgeFactory_ABC
    @brief  PopulationKnowledgeFactory_ABC
*/
// Created: AGE 2006-02-27
// =============================================================================
class PopulationKnowledgeFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgeFactory_ABC() {};
    virtual ~PopulationKnowledgeFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::PopulationKnowledge_ABC* CreatePopulationKnowledge( const kernel::KnowledgeGroup_ABC& group, const sword::CrowdKnowledgeCreation& message ) = 0;
    //@}
};

#endif // __PopulationKnowledgeFactory_ABC_h_
