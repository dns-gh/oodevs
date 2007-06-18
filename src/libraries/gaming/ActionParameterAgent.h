// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAgent_h_
#define __ActionParameterAgent_h_

#include "ActionParameterEntity.h"

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  ActionParameterAgent
    @brief  ActionParameterAgent
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterAgent : public ActionParameterEntity< kernel::Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterAgent( const kernel::OrderParameter& parameter );
             ActionParameterAgent( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
             ActionParameterAgent( const kernel::OrderParameter& parameter, unsigned int id, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );
             ActionParameterAgent( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Agent_ABC >& resolver );;
    virtual ~ActionParameterAgent();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_Unit& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAgent( const ActionParameterAgent& );            //!< Copy constructor
    ActionParameterAgent& operator=( const ActionParameterAgent& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterAgent_h_
