// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgeFactory_ABC_h_
#define __AgentKnowledgeFactory_ABC_h_

#include "ASN_Types.h"

class AgentKnowledge;
class KnowledgeGroup_ABC;

// =============================================================================
/** @class  AgentKnowledgeFactory_ABC
    @brief  AgentKnowledgeFactory_ABC
*/
// Created: AGE 2006-02-15
// =============================================================================
class AgentKnowledgeFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgeFactory_ABC() {};
    virtual ~AgentKnowledgeFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual AgentKnowledge* CreateAgentKnowledge( const KnowledgeGroup_ABC& group, const ASN1T_MsgUnitKnowledgeCreation& message ) = 0;
    //@}
};

#endif // __AgentKnowledgeFactory_ABC_h_
