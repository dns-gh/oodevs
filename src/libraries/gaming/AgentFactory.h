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

namespace kernel
{
    class Controllers;
    class AgentTypes;
    class Entity_ABC;
    class Workers;
}

class Model;
class Simulation;
class Publisher_ABC;
class StaticModel;
class RcEntityResolver_ABC;

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
             AgentFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, Publisher_ABC& publisher, const Simulation& simulation, kernel::Workers& workers, const RcEntityResolver_ABC& rcResolver );
    virtual ~AgentFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Automat_ABC*    Create( const ASN1T_MsgAutomatCreation& asnMsg );
    virtual kernel::Agent_ABC*      Create( const ASN1T_MsgUnitCreation& asnMsg );
    virtual kernel::Population_ABC* Create( const ASN1T_MsgPopulationCreation& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentFactory( const AgentFactory& );            //!< Copy constructor
    AgentFactory& operator=( const AgentFactory& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AttachExtensions( kernel::Entity_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    Publisher_ABC& publisher_;
    const Simulation& simulation_;
    kernel::Workers& workers_;
    const RcEntityResolver_ABC& rcResolver_;
    //@}
};

#endif // __AgentFactory_h_
