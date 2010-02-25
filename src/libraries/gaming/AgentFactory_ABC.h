// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentFactory_ABC_h_
#define __AgentFactory_ABC_h_

namespace MsgsSimToClient
{
  class MsgAutomatCreation;
  class MsgUnitCreation;
  class MsgPopulationCreation;
}

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
    class Population_ABC;
}

// =============================================================================
/** @class  AgentFactory_ABC
    @brief  AgentFactory_ABC
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
  virtual kernel::Automat_ABC*    Create( const MsgsSimToClient::MsgAutomatCreation& message ) = 0;
    virtual kernel::Agent_ABC*      Create( const MsgsSimToClient::MsgUnitCreation& message ) = 0;
    virtual kernel::Population_ABC* Create( const MsgsSimToClient::MsgPopulationCreation& message ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory_ABC( const AgentFactory_ABC& );            //!< Copy constructor
    AgentFactory_ABC& operator=( const AgentFactory_ABC& ); //!< Assignement operator
    //@}
};

#endif // __AgentFactory_ABC_h_
