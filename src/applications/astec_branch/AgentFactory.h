// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_h_
#define __AgentFactory_h_

#include "AgentFactory_ABC.h"

// =============================================================================
/** @class  AgentFactory
    @brief  Agent factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory();
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual Agent* Create( const ASN1T_MsgAutomateCreation& asnMsg );
    virtual Agent* Create( const ASN1T_MsgPionCreation& asnMsg );
    virtual Population* Create( const ASN1T_MsgPopulationCreation& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AgentFactory_h_
