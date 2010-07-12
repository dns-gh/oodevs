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

#include "CompositeFactory.h"
#include "EntityPublisher.h"
#include "MessageHandler_ABC.h"
#include "Model_ABC.h"
#include "tools/Resolver.h"

namespace Common
{
    enum EnumSimulationState;
}

namespace kernel
{
    class AgentType;
    class AgentTypes;
    class FormationLevels;
    class FragOrderType;
    class MissionType;
    class ModelVisitor_ABC;
    class ObjectType;
    class ObjectTypes;
    class StaticModel;
}

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
    class CompositeFactory;
    class Factory_ABC;
    class FolkModel;
    class MeteoModel;
    class SimulationModel;
    class UrbanKnowledge;

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
             Model( const tools::ExerciseConfig& config, const kernel::StaticModel& staticModel );
    virtual ~Model();
    //@}

    //! @name Update
    //@{
    void Reset();

    virtual void Receive( const MsgsSimToClient::MsgSimToClient& asnMsg );
    void Update( const MsgsSimToClient::MsgSimToClient& asnMsg );
    void Send( ClientPublisher_ABC& publisher ) const;

    void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, Common::EnumSimulationState status, unsigned factor ) const;
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
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename Concrete, typename Base, typename Message >
    void CreateUpdate( tools::Resolver< Base >& resolver, const Message& message );
    template< typename Concrete, typename Base, typename Message >
    void CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message );
    template< typename Concrete, typename Base, typename Message, typename Parameter >
    void CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message, const Parameter& parameter );
    template< typename T >
    void UpdateAnyAgent( unsigned id, const T& message );
    template< typename T >
    void Destroy( tools::Resolver< T >& resolver, unsigned id );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::StaticModel&          staticModel_;
    std::auto_ptr< SimulationModel >    simulation_;
    std::auto_ptr< CompositeFactory >   compositeFactory_;
    std::auto_ptr< FolkModel >          folk_;
    std::auto_ptr< MeteoModel >         meteoModel_;
    //@}

public:
    //! @name Dynamic model
    //@{
    virtual const tools::Resolver_ABC< dispatcher::Team_ABC >&           Sides() const { return sides_; }
    virtual const tools::Resolver_ABC< dispatcher::KnowledgeGroup_ABC >& KnowledgeGroups() const { return knowledgeGroups_; }
    virtual const tools::Resolver_ABC< dispatcher::Formation_ABC >&      Formations() const { return formations_; }
    virtual const tools::Resolver_ABC< dispatcher::Automat_ABC >&        Automats() const { return automats_; }
    virtual const tools::Resolver_ABC< dispatcher::Agent_ABC >&          Agents() const { return agents_; }
    virtual const tools::Resolver_ABC< dispatcher::Object_ABC >&         Objects() const { return objects_; }
    virtual const tools::Resolver_ABC< dispatcher::Population_ABC >&     Populations() const { return populations_; }
    virtual const tools::Resolver_ABC< dispatcher::AgentKnowledge_ABC >& AgentKnowledges() const { return agentKnowledges_; }
    virtual const tools::Resolver_ABC< ObjectKnowledge >&       ObjectKnowledges() const { return objectKnowledges_; }
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   PopulationKnowledges() const { return populationKnowledges_; }
    virtual const tools::Resolver_ABC< LogConsignMaintenance >& LogConsignsMaintenance() const { return logConsignsMaintenance_; }
    virtual const tools::Resolver_ABC< LogConsignSupply >&      LogConsignsSupply() const { return logConsignsSupply_; }
    virtual const tools::Resolver_ABC< LogConsignMedical >&     LogConsignsMedical() const { return logConsignsMedical_; }
    virtual const tools::Resolver_ABC< Fire >&                  Fires() const { return fires_; }
    virtual const tools::Resolver_ABC< PopulationFire >&        PopulationFires() const { return populationFires_; }
    virtual const tools::Resolver_ABC< FireEffect >&            FireEffects() const { return fireEffects_; }
    virtual const tools::Resolver_ABC< Report >&                Reports() const { return reports_; }
    virtual const tools::Resolver_ABC< UrbanObject >&           UrbanBlocks() const { return urbanBlocks_; }
    //@}

    //! @name Dynamic model
    //@{
private:
    tools::Resolver< dispatcher::Team_ABC >           sides_;
    tools::Resolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroups_;
    tools::Resolver< dispatcher::Formation_ABC >      formations_;
    tools::Resolver< dispatcher::Automat_ABC >        automats_;
    tools::Resolver< dispatcher::Agent_ABC >          agents_;
    tools::Resolver< dispatcher::Object_ABC >         objects_;
    tools::Resolver< dispatcher::Population_ABC >     populations_;
    tools::Resolver< dispatcher::AgentKnowledge_ABC > agentKnowledges_;

public:
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
    tools::Resolver< UrbanKnowledge >         urbanKnowledges_;
    std::auto_ptr< kernel::FormationLevels >  levels_;
    //@}
};

}

#endif // __Model_h_
