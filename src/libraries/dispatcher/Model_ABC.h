// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_ABC_h_
#define __Model_ABC_h_

#include "Sendable.h"
#include "tools/Resolver.h"

namespace dispatcher
{
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class Automat_ABC;
    class Fire;
    class FireEffect;
    class Formation_ABC;
    class KnowledgeGroup_ABC;
    class LogConsignMaintenance;
    class LogConsignMedical;
    class LogConsignSupply;
    class Object_ABC;
    class ObjectKnowledge;
    class Population_ABC;
    class PopulationFire;
    class PopulationKnowledge;
    class Report;
    class Team_ABC;
    class UrbanObject;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: NLD 2006-09-19
// =============================================================================

class Model_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Model_ABC() {}
    virtual ~Model_ABC() {}
    //@}

    //! @name data accessor
    //@{
    virtual const tools::Resolver_ABC< dispatcher::Team_ABC >&           Sides() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::KnowledgeGroup_ABC >& KnowledgeGroups() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::Formation_ABC >&      Formations() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::Automat_ABC >&        Automats() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::Agent_ABC >&          Agents() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::Object_ABC >&         Objects() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::Population_ABC >&     Populations() const = 0;
    virtual const tools::Resolver_ABC< dispatcher::AgentKnowledge_ABC >& AgentKnowledges() const = 0;
    virtual const tools::Resolver_ABC< ObjectKnowledge >&       ObjectKnowledges() const = 0;
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   PopulationKnowledges() const = 0;
    virtual const tools::Resolver_ABC< LogConsignMaintenance >& LogConsignsMaintenance() const = 0;
    virtual const tools::Resolver_ABC< LogConsignSupply >&      LogConsignsSupply() const = 0;
    virtual const tools::Resolver_ABC< LogConsignMedical >&     LogConsignsMedical() const = 0;
    virtual const tools::Resolver_ABC< Fire >&                  Fires() const = 0;
    virtual const tools::Resolver_ABC< PopulationFire >&        PopulationFires() const = 0;
    virtual const tools::Resolver_ABC< FireEffect >&            FireEffects() const = 0;
    virtual const tools::Resolver_ABC< Report >&                Reports() const = 0;
    virtual const tools::Resolver_ABC< UrbanObject >&           UrbanBlocks() const = 0;
    //@}
};

}

#endif // __Model_h_
