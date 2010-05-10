// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPopulationKnowledge_h_
#define __ActionParameterPopulationKnowledge_h_

#include "Entity.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgPopulationKnowledge;
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
class PopulationKnowledge : public Entity< kernel::PopulationKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledge( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             PopulationKnowledge( const kernel::OrderParameter& parameter, unsigned long id, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
             PopulationKnowledge( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::AgentKnowledgeConverter_ABC& converter, const kernel::Entity_ABC& owner, kernel::Controller& controller );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void CommitTo( Common::MsgPopulationKnowledge& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledge( const PopulationKnowledge& );            //!< Copy constructor
    PopulationKnowledge& operator=( const PopulationKnowledge& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

    }
}

#endif // __ActionParameterAgentKnowledge_h_
