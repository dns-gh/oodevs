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

// =============================================================================
/** @class  ParamAgentKnowledge
    @brief  ParamAgentKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledge : public EntityParameter< kernel::AgentKnowledge_ABC >
                          , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledge( QObject* pParent, const kernel::OrderParameter& parameter, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
             ParamAgentKnowledge( QObject* pParent, const kernel::OrderParameter& parameter, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent, const kernel::AgentKnowledge_ABC& potential );
    virtual ~ParamAgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgentKnowledge( const ParamAgentKnowledge& );            //!< Copy constructor
    ParamAgentKnowledge& operator=( const ParamAgentKnowledge& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    kernel::AgentKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

#endif // __ParamAgentKnowledge_h_
