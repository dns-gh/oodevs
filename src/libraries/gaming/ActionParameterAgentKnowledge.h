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

#include "AgentKnowledge_ABC.h"
#include "ActionParameterEntity.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
}

class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ActionParameterAgentKnowledge
    @brief  ActionParameterAgentKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class ActionParameterAgentKnowledge : public ActionParameterEntity< AgentKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterAgentKnowledge( const QString& name );
    explicit ActionParameterAgentKnowledge( const kernel::OrderParameter& parameter );
             ActionParameterAgentKnowledge( const kernel::OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterAgentKnowledge( const QString& name, unsigned long id, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterAgentKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterAgentKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~ActionParameterAgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_UnitKnowledge& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAgentKnowledge( const ActionParameterAgentKnowledge& );            //!< Copy constructor
    ActionParameterAgentKnowledge& operator=( const ActionParameterAgentKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterAgentKnowledge_h_
