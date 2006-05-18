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

class AgentKnowledge;
class Agent;
class Agent_ABC;
class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamAgentKnowledge
    @brief  ParamAgentKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledge : public EntityParameter< AgentKnowledge >
                          , public ContextMenuObserver_ABC< Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledge( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu, AgentKnowledgeConverter_ABC& converter, const Agent_ABC& agent );
    virtual ~ParamAgentKnowledge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAgentKnowledge( const ParamAgentKnowledge& );            //!< Copy constructor
    ParamAgentKnowledge& operator=( const ParamAgentKnowledge& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Agent& entity, QPopupMenu& menu );
    //@}

    //! @name Member data
    //@{
    AgentKnowledgeConverter_ABC& converter_;
    const Agent& agent_;
    //@}
};

#endif // __ParamAgentKnowledge_h_
