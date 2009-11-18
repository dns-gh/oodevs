// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "game_asn/Simulation.h"
#include "MessageHandler_ABC.h"
#include "Model_ABC.h"
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
    class ModelVisitor_ABC;
    class LogConsignMaintenance;
    class LogConsignSupply;
    class LogConsignMedical;
    class SimulationModel;
    class ClientPublisher_ABC;
    class Fire;
    class PopulationFire;
    class FireEffect;
    class FolkModel;
    class Report;
    class CompositeFactory;
    class Factory_ABC;

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
class Model : public MessageHandler_ABC, public Model_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Model( const tools::ExerciseConfig& config );
    virtual ~Model();
    //@}

    //! @name Update
    //@{
    void Reset();

    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    void Update( const ASN1T_MsgsSimToClient& asnMsg );
    void Send( ClientPublisher_ABC& publisher ) const;

    void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, ASN1T_EnumSimulationState status, unsigned factor ) const;
    void SendFirstTick( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    void RegisterFactory( Factory_ABC& factory );
    void UnregisterFactory( Factory_ABC& factory );

    template< typename T >
    void AddExtensions( T& entity )
    {
        AddExtensions( entity, &entity );
        
    }
    template< typename T >
    void AddExtensions( T& entity, kernel::Entity_ABC* )
    {
        // $$$$ AGE 2008-06-20: 
        entity.Attach< EntityPublisher_ABC >( *new EntityPublisher< T >( entity ) );
        compositeFactory_->Apply( &ExtensionFactory_ABC< T >::Create, entity );
        compositeFactory_->Apply( &ExtensionFactory_ABC< kernel::Entity_ABC >::Create, entity );
    }
    template< typename T >
    void AddExtensions( T& , void* ) {}
    //@}

    //! @name Accessors
    //@{
    void Accept( ModelVisitor_ABC& visitor ) const;
    const tools::Resolver_ABC< kernel::AgentType >& GetAgentTypes() const;
    const tools::Resolver_ABC< kernel::MissionType >& GetMissionTypes() const;
    const tools::Resolver_ABC< kernel::FragOrderType >& GetFragOrderTypes() const;
	const tools::Resolver_ABC< kernel::ObjectType, std::string >& GetObjectTypes() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename T, typename P >
    void CreateUpdate( tools::Resolver< T >& resolver, const P& parameter );
    template< typename T, typename P >
    void CreateUpdate( tools::Resolver< T >& resolver, unsigned id, const P& parameter );
    template< typename T >
    void UpdateAnyAgent( unsigned id, const T& message );
    template< typename T >
    void Destroy( tools::Resolver< T >& resolver, unsigned id );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< SimulationModel >    simulation_;
    std::auto_ptr< CompositeFactory >   compositeFactory_;

    std::auto_ptr< FolkModel >          folk_;
    std::auto_ptr< kernel::AgentTypes > agentTypes_;
	std::auto_ptr< kernel::ObjectTypes > objectTypes_;
    //@}

public:
    //! @name Dynamic model
    //@{
    virtual const tools::Resolver_ABC< Side >&                  sides() const { return sides_; }
    virtual const tools::Resolver_ABC< KnowledgeGroup >&        knowledgeGroups() const { return knowledgeGroups_; }
    virtual const tools::Resolver_ABC< Formation >&             formations() const { return formations_; }
    virtual const tools::Resolver_ABC< Automat >&               automats() const { return automats_; }
    virtual const tools::Resolver_ABC< Agent >&                 agents() const { return agents_; }
    virtual const tools::Resolver_ABC< Object >&                objects() const { return objects_; }
    virtual const tools::Resolver_ABC< Population >&            populations() const { return populations_; }
    virtual const tools::Resolver_ABC< AgentKnowledge >&        agentKnowledges() const { return agentKnowledges_; }
    virtual const tools::Resolver_ABC< ObjectKnowledge >&       objectKnowledges() const { return objectKnowledges_; }
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   populationKnowledges() const { return populationKnowledges_; }
    virtual const tools::Resolver_ABC< LogConsignMaintenance >& logConsignsMaintenance() const { return logConsignsMaintenance_; }
    virtual const tools::Resolver_ABC< LogConsignSupply >&      logConsignsSupply() const { return logConsignsSupply_; }
    virtual const tools::Resolver_ABC< LogConsignMedical >&     logConsignsMedical() const { return logConsignsMedical_; }
    virtual const tools::Resolver_ABC< Fire >&                  fires() const { return fires_; }
    virtual const tools::Resolver_ABC< PopulationFire >&        populationFires() const { return populationFires_; }
    virtual const tools::Resolver_ABC< FireEffect >&            fireEffects() const { return fireEffects_; }
    virtual const tools::Resolver_ABC< Report >&                reports() const { return reports_; }
    virtual const tools::Resolver_ABC< UrbanObject >&           urbanBlocks() const { return urbanBlocks_; }
    //@}
        

    //! @name Dynamic model
    //@{
    tools::Resolver< Side >                   sides_;
    tools::Resolver< KnowledgeGroup >         knowledgeGroups_;
    tools::Resolver< Formation >              formations_;
    tools::Resolver< Automat >                automats_;
    tools::Resolver< Agent >                  agents_;
    tools::Resolver< Object >                 objects_;
    tools::Resolver< Population >             populations_;
    tools::Resolver< AgentKnowledge >         agentKnowledges_;
    tools::Resolver< ObjectKnowledge >        objectKnowledges_;
    tools::Resolver< PopulationKnowledge >    populationKnowledges_;
    tools::Resolver< LogConsignMaintenance >  logConsignsMaintenance_;
    tools::Resolver< LogConsignSupply >       logConsignsSupply_;
    tools::Resolver< LogConsignMedical >      logConsignsMedical_;
    tools::Resolver< Fire >                   fires_;
    tools::Resolver< PopulationFire >         populationFires_;
    tools::Resolver< FireEffect >             fireEffects_;
    tools::Resolver< Report >                 reports_;
    tools::Resolver< UrbanObject >            urbanBlocks_;
    //@}
};

}

#endif // __Model_h_
