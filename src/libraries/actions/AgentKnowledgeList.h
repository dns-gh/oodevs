// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgentKnowledgeList_h_
#define __ActionParameterAgentKnowledgeList_h_

#include "Parameter.h"
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgUnitKnowledgeList;
}

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class AgentKnowledgeConverter_ABC;
    class Controller;
    class EntityResolver_ABC;
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
             AgentKnowledgeList( const kernel::OrderParameter& parameter, const Common::MsgUnitKnowledgeList& message, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             AgentKnowledgeList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~AgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;
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
    void ReadAgentKnowledge( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    //@}
};

    }
}

#endif // __ActionParameterAgentKnowledgeList_h_
