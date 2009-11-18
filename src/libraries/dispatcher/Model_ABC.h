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

#include "game_asn/Simulation.h"
#include "MessageHandler_ABC.h"
#include "CompositeFactory.h"
#include "EntityPublisher.h"
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
    virtual const tools::Resolver_ABC< Side >&                  sides() const = 0;
    virtual const tools::Resolver_ABC< KnowledgeGroup >&        knowledgeGroups() const = 0;
    virtual const tools::Resolver_ABC< Formation >&             formations() const = 0;
    virtual const tools::Resolver_ABC< Automat >&               automats() const = 0;
    virtual const tools::Resolver_ABC< Agent >&                 agents() const = 0;
    virtual const tools::Resolver_ABC< Object >&                objects() const = 0;
    virtual const tools::Resolver_ABC< Population >&            populations() const = 0;
    virtual const tools::Resolver_ABC< AgentKnowledge >&        agentKnowledges() const = 0;
    virtual const tools::Resolver_ABC< ObjectKnowledge >&       objectKnowledges() const = 0;
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   populationKnowledges() const = 0;
    virtual const tools::Resolver_ABC< LogConsignMaintenance >& logConsignsMaintenance() const = 0;
    virtual const tools::Resolver_ABC< LogConsignSupply >&      logConsignsSupply() const = 0;
    virtual const tools::Resolver_ABC< LogConsignMedical >&     logConsignsMedical() const = 0;
    virtual const tools::Resolver_ABC< Fire >&                  fires() const = 0;
    virtual const tools::Resolver_ABC< PopulationFire >&        populationFires() const = 0;
    virtual const tools::Resolver_ABC< FireEffect >&            fireEffects() const = 0;
    virtual const tools::Resolver_ABC< Report >&                reports() const = 0;
    virtual const tools::Resolver_ABC< UrbanObject >&           urbanBlocks() const = 0;
    //@}
};

}

#endif // __Model_h_
