// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentCommunications_h_
#define __AgentCommunications_h_

#include "EntityCommunications.h"

// =============================================================================
/** @class  AgentCommunications
    @brief  AgentCommunications
*/
// Created: SBO 2006-10-09
// =============================================================================
class AgentCommunications : public EntityCommunications
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~AgentCommunications();
    //@}
};

#endif // __AgentCommunications_h_
