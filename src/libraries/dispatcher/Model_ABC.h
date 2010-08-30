// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Model_ABC_h_
#define __Model_ABC_h_

#include "tools/Resolver.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace Common
{
    enum EnumSimulationState;
}

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
    class UrbanObject_ABC;
    class ObjectKnowledge_ABC;
    class UrbanKnowledge_ABC;
    class Population_ABC;
    class PopulationFire;
    class PopulationKnowledge;
    class Report;
    class Team_ABC;
    class UrbanObject;
    class Factory_ABC;
    class ClientPublisher_ABC;

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

    //! @name Operations
    //@{
    virtual void RegisterFactory( Factory_ABC& factory ) = 0;
    virtual void UnregisterFactory( Factory_ABC& factory ) = 0;

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const = 0;

    virtual void Send( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendFirstTick( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, Common::EnumSimulationState status, unsigned int factor ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const tools::Resolver_ABC< Team_ABC >&              Sides() const = 0;
    virtual const tools::Resolver_ABC< KnowledgeGroup_ABC >&    KnowledgeGroups() const = 0;
    virtual const tools::Resolver_ABC< Formation_ABC >&         Formations() const = 0;
    virtual const tools::Resolver_ABC< Automat_ABC >&           Automats() const = 0;
    virtual const tools::Resolver_ABC< Agent_ABC >&             Agents() const = 0;
    virtual const tools::Resolver_ABC< Object_ABC >&            Objects() const = 0;
    virtual const tools::Resolver_ABC< UrbanObject_ABC >&       UrbanBlocks() const = 0;
    virtual const tools::Resolver_ABC< Population_ABC >&        Populations() const = 0;
    virtual const tools::Resolver_ABC< AgentKnowledge_ABC >&    AgentKnowledges() const = 0;
    virtual const tools::Resolver_ABC< ObjectKnowledge_ABC >&   ObjectKnowledges() const = 0;
    virtual const tools::Resolver_ABC< UrbanKnowledge_ABC >&    UrbanKnowledges() const = 0;
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   PopulationKnowledges() const = 0;
    virtual const tools::Resolver_ABC< LogConsignMaintenance >& LogConsignsMaintenance() const = 0;
    virtual const tools::Resolver_ABC< LogConsignSupply >&      LogConsignsSupply() const = 0;
    virtual const tools::Resolver_ABC< LogConsignMedical >&     LogConsignsMedical() const = 0;
    virtual const tools::Resolver_ABC< Fire >&                  Fires() const = 0;
    virtual const tools::Resolver_ABC< PopulationFire >&        PopulationFires() const = 0;
    virtual const tools::Resolver_ABC< FireEffect >&            FireEffects() const = 0;
    virtual const tools::Resolver_ABC< Report >&                Reports() const = 0;
    //@}
};

}

#endif // __Model_h_
