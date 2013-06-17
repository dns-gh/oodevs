// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgent_h_
#define __ActionParameterAgent_h_

#include "Entity.h"
#include "clients_kernel/Agent_ABC.h"

namespace kernel
{
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: SBO 2007-05-22
// =============================================================================
class Agent : public Entity< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( const kernel::OrderParameter& parameter, kernel::Controller& controller, bool isKnowledge = false );
             Agent( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller, bool isKnowledge = false );
             Agent( const kernel::OrderParameter& parameter, unsigned int id, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller, bool isKnowledge = false );
             Agent( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller, bool isKnowledge );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::Agent_ABC >::CommitTo;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual std::string SerializeType() const;
    //@}

private:
    //! @name Member data
    //@{
    const bool isKnowledge_;
    //@}
};

}
}

#endif // __ActionParameterAgent_h_
