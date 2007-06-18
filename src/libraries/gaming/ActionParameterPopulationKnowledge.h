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

#include "PopulationKnowledge_ABC.h"
#include "ActionParameterEntity.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Population_ABC;
}

class AgentKnowledgeConverter_ABC;

// =============================================================================
/** @class  ActionParameterPopulationKnowledge
    @brief  ActionParameterPopulationKnowledge
    // $$$$ SBO 2007-05-24: watch for deletion !!!
*/
// Created: SBO 2007-05-24
// =============================================================================
class ActionParameterPopulationKnowledge : public ActionParameterEntity< PopulationKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterPopulationKnowledge( const kernel::OrderParameter& parameter );
             ActionParameterPopulationKnowledge( const kernel::OrderParameter& parameter, unsigned long id, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
             ActionParameterPopulationKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Population_ABC >& resolver, AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner );
    virtual ~ActionParameterPopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_PopulationKnowledge& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPopulationKnowledge( const ActionParameterPopulationKnowledge& );            //!< Copy constructor
    ActionParameterPopulationKnowledge& operator=( const ActionParameterPopulationKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterAgentKnowledge_h_
