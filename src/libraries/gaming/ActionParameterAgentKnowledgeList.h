// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgentKnowledgeList_h_
#define __ActionParameterAgentKnowledgeList_h_

#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
}

class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ActionParameterAgentKnowledgeList
    @brief  ActionParameterAgentKnowledgeList
*/
// Created: SBO 2007-05-24
// =============================================================================
class ActionParameterAgentKnowledgeList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterAgentKnowledgeList( const kernel::OrderParameter& parameter );
             ActionParameterAgentKnowledgeList( const kernel::OrderParameter& parameter, const ASN1T_UnitKnowledgeList& asn, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterAgentKnowledgeList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~ActionParameterAgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAgentKnowledgeList( const ActionParameterAgentKnowledgeList& );            //!< Copy constructor
    ActionParameterAgentKnowledgeList& operator=( const ActionParameterAgentKnowledgeList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAgentKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    //@}
};

#endif // __ActionParameterAgentKnowledgeList_h_
