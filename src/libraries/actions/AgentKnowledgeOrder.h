// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgentKnowledgeOrder_h_
#define __ActionParameterAgentKnowledgeOrder_h_

#include "AgentKnowledge.h"

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  AgentKnowledgeOrder
    @brief  Agent knowledge order
*/
// Created: LGY 2011-07-07
// =============================================================================
class AgentKnowledgeOrder : public AgentKnowledge
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgeOrder( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver,
                                  const kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~AgentKnowledgeOrder();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool CheckKnowledgeValidity() const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Helpers
    //@{
    unsigned long RetrieveId() const;
    //@}

private:
    //! @name Member Data
    //@{
    const kernel::EntityResolver_ABC& resolver_;
    const kernel::AgentKnowledgeConverter_ABC& converter_;
    const kernel::Entity_ABC& owner_;
    unsigned int agentId_;
    //@}
};
}
}

#endif // __ActionParameterAgentKnowledgeOrder_h_
