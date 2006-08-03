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

class Controllers;
class Model;
class AgentTypes;
class Entity_ABC;
class Simulation;
class Workers;
class Publisher_ABC;
class StaticModel;

// =============================================================================
/** @class  AgentFactory
    @brief  Agent_ABC factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class AgentFactory : public AgentFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, Publisher_ABC& publisher, const Simulation& simulation, Workers& workers );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual Agent_ABC* Create( const ASN1T_MsgAutomateCreation& asnMsg );
    virtual Agent_ABC* Create( const ASN1T_MsgPionCreation& asnMsg );
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
    void AttachExtensions( Entity_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    Publisher_ABC& publisher_;
    const Simulation& simulation_;
    Workers& workers_;
    //@}
};

#endif // __AgentFactory_h_
