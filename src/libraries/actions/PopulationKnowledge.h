// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPopulationKnowledge_h_
#define __ActionParameterPopulationKnowledge_h_

#include "Knowledge_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class CrowdKnowledgeId;
}

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class EntityResolver_ABC;
    class Population_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  PopulationKnowledge
    @brief  PopulationKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class PopulationKnowledge : public Knowledge_ABC< kernel::PopulationKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             PopulationKnowledge( const kernel::OrderParameter& parameter, unsigned long id, kernel::AgentKnowledgeConverter_ABC& converter,
                                  const kernel::Entity_ABC& owner, kernel::Controller& controller, const kernel::EntityResolver_ABC& entities );
             PopulationKnowledge( const kernel::OrderParameter& parameter, const kernel::PopulationKnowledge_ABC* knowledge, kernel::Controller& controller );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& asn ) const;
    void CommitTo( sword::CrowdKnowledgeId& asn ) const;
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

    }
}

#endif // __ActionParameterPopulationKnowledge_h_
