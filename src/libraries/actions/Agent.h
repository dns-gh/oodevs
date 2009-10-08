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

namespace kernel
{
    class Agent_ABC;
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
             Agent( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Agent( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::Controller& controller );
             Agent( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::Controller& controller );
             Agent( xml::xistream& xis, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, kernel::Controller& controller );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::Agent_ABC >::CommitTo;
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_Unit& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterAgent_h_
