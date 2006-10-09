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
}

class AgentKnowledge;
class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ParamAgentKnowledge
    @brief  ParamAgentKnowledge
*/
// Created: AGE 2006-03-14
// =============================================================================
class ParamAgentKnowledge : public EntityParameter< AgentKnowledge >
                          , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAgentKnowledge( QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& agent );
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
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name Member data
    //@{
    AgentKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& agent_;
    //@}
};

#endif // __ParamAgentKnowledge_h_
