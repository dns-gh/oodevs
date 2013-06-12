// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentKnowledge_h_
#define __ParamAgentKnowledge_h_

#include "EntityParameter.h"

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class AgentKnowledge_ABC;
    class AgentKnowledgeConverter_ABC;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamAgentKnowledge
    @brief  ParamAgentKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledge : public EntityParameter< kernel::Agent_ABC >
                          , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledge( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamAgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& knowledge, kernel::ContextMenu& menu );
    //@}
};

    }
}

#endif // __ParamAgentKnowledge_h_
