// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAgentKnowledgeList_h_
#define __ParamAgentKnowledgeList_h_

#include "EntityListParameter.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class AgentKnowledgeConverter_ABC;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamAgentKnowledgeList
    @brief  ParamAgentKnowledgeList
    // $$$$ AGE 2006-03-14: factor these stuffs with single entity parameters
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledgeList : public EntityListParameter< kernel::AgentKnowledge_ABC >
                              , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledgeList( QObject* pParent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, kernel::Controller& controller
                 , kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
    virtual ~ParamAgentKnowledgeList();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void AddToMenu( kernel::ContextMenu& menu );
    virtual EntityParameter< kernel::AgentKnowledge_ABC >* CreateElement( const kernel::AgentKnowledge_ABC& potential );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::AgentKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    unsigned int count_;
    //@}
};

    }
}

#endif // __ParamAgentKnowledgeList_h_
