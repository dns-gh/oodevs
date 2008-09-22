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

#include "Entity.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledgeConverter_ABC;
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
class AgentKnowledge : public Entity< kernel::AgentKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledge( const kernel::OrderParameter& parameter );
             AgentKnowledge( const kernel::OrderParameter& parameter, unsigned long id, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             AgentKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             AgentKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_UnitKnowledge& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledge( const AgentKnowledge& );            //!< Copy constructor
    AgentKnowledge& operator=( const AgentKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};
    }
}

#endif // __ActionParameterAgentKnowledge_h_
