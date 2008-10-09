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

#include "Parameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class AgentKnowledgeConverter_ABC;
    class Controller;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  AgentKnowledgeList
    @brief  AgentKnowledgeList
*/
// Created: SBO 2007-05-24
// =============================================================================
class AgentKnowledgeList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgeList( const kernel::OrderParameter& parameter );
             AgentKnowledgeList( const kernel::OrderParameter& parameter, const ASN1T_UnitKnowledgeList& asn, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             AgentKnowledgeList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~AgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledgeList( const AgentKnowledgeList& );            //!< Copy constructor
    AgentKnowledgeList& operator=( const AgentKnowledgeList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAgentKnowledge( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    //@}
};

    }
}

#endif // __ActionParameterAgentKnowledgeList_h_
