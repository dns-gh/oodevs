// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_ABC_h_
#define __AgentFactory_ABC_h_

#include "ASN_Types.h"
class Agent;
class Population;

// =============================================================================
/** @class  AgentFactory_ABC
    @brief  AgentFactory_ABC
    // $$$$ AGE 2006-02-13: Make an only EntityFactory ?
*/
// Created: AGE 2006-02-13
// =============================================================================
class AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory_ABC() {};
    virtual ~AgentFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual Agent* Create( const ASN1T_MsgAutomateCreation& asnMsg ) = 0;
    virtual Agent* Create( const ASN1T_MsgPionCreation& asnMsg ) = 0;
    virtual Population* Create( const ASN1T_MsgPopulationCreation& asnMsg ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory_ABC( const AgentFactory_ABC& );            //!< Copy constructor
    AgentFactory_ABC& operator=( const AgentFactory_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AgentFactory_ABC_h_
