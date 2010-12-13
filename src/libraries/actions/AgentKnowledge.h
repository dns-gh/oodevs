// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgentKnowledge_h_
#define __ActionParameterAgentKnowledge_h_

#include "Knowledge_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class UnitKnowledgeId;
}

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledgeConverter_ABC;
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  AgentKnowledge
    @brief  AgentKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class AgentKnowledge : public Knowledge_ABC< kernel::AgentKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             AgentKnowledge( const kernel::OrderParameter& parameter, const kernel::AgentKnowledge_ABC& entity, kernel::Controller& controller );
             AgentKnowledge( const kernel::OrderParameter& parameter, unsigned long id, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             AgentKnowledge( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             AgentKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void CommitTo( sword::MsgMissionParameter& message ) const;
    virtual void CommitTo( sword::MsgMissionParameter_Value& message ) const;
    void CommitTo( sword::UnitKnowledgeId& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ThrowInvalidKnowledge() const;
    //@}
};
    }
}

#endif // __ActionParameterAgentKnowledge_h_
