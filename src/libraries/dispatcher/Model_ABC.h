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

#include "clients_kernel/Team_ABC.h"
#include "game_asn/Simulation.h"
#include "MessageHandler_ABC.h"
#include "CompositeFactory.h"
#include "EntityPublisher.h"
#include "Sendable.h"
#include "tools/Resolver.h"

namespace kernel
{
    class AgentTypes;
    class AgentType;
    class MissionType;
    class FragOrderType;
    class Entity_ABC;
	class ObjectType;
	class ObjectTypes;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class LogConsignMaintenance;
    class LogConsignSupply;
    class LogConsignMedical;
    class Fire;
    class PopulationFire;
    class FireEffect;
    class Report;
    class Side;
    class KnowledgeGroup;
    class Formation;
    class Automat;
    class Agent;
    class Object;
    class Population;
    class AgentKnowledge;
    class ObjectKnowledge;
    class PopulationKnowledge;
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
    explicit Model_ABC() {}
    virtual ~Model_ABC() {}
    //@}

    //! @name data accessor
    //@{
    virtual const tools::Resolver_ABC< Sendable< kernel::Team_ABC > >& Sides() const = 0;
    virtual const tools::Resolver_ABC< KnowledgeGroup >&        KnowledgeGroups() const = 0;
    virtual const tools::Resolver_ABC< Formation >&             Formations() const = 0;
    virtual const tools::Resolver_ABC< Automat >&               Automats() const = 0;
    virtual const tools::Resolver_ABC< Agent >&                 Agents() const = 0;
    virtual const tools::Resolver_ABC< Object >&                Objects() const = 0;
    virtual const tools::Resolver_ABC< Population >&            Populations() const = 0;
    virtual const tools::Resolver_ABC< AgentKnowledge >&        AgentKnowledges() const = 0;
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
