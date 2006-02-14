// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "ASN_Types.h"
#include "Agent_ABC.h"
#include "Resolver_ABC.h"

class AgentType;
class AutomatType;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent : public Agent_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const ASN1T_MsgAutomateCreation& message, const Resolver_ABC< AutomatType >& resolver );
    explicit Agent( const ASN1T_MsgPionCreation& message, const Resolver_ABC< AgentType >& resolver );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const;
    virtual unsigned long GetId() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    AutomatType*  automatType_;
    AgentType*    type_;
    std::string   name_;
    unsigned long id_;
    //@}
};

#endif // __Agent_h_
