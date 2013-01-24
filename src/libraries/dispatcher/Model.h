// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "MessageHandler_ABC.h"
#include "CompositeFactory.h"
#include "Model_ABC.h"

namespace kernel
{
    class AgentType;
    class AgentTypes;
    class FragOrderType;
    class MissionType;
    class ObjectType;
    class ObjectTypes;
    class StaticModel;
    class ExtensionTypes;
    class Entity_ABC;
    class EntityBase_ABC;
}

namespace dispatcher
{
    class Config;
    class FolkModel;
    class MeteoModel;
    class SimulationModel;
    class UrbanKnowledge;
    class MemoryLogger_ABC;

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
             Model( const Config& config, const kernel::StaticModel& staticModel, MemoryLogger_ABC& logger );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void Reset();

    virtual void Receive( const sword::SimToClient& asnMsg );
    void Update( const sword::SimToClient& asnMsg );
    virtual void Send( ClientPublisher_ABC& publisher ) const;

    virtual void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, sword::EnumSimulationState status, unsigned int factor, unsigned int firstTick = 1 ) const;
    virtual void SendFirstTick( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    virtual void RegisterFactory( Factory_ABC& factory );
    virtual void UnregisterFactory( Factory_ABC& factory );

    template< typename T >
    void AddExtensions( T& entity )
    {
        AddExtensions( entity, &entity );
    }
    //@}

    //! @name Accessors
    //@{
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const tools::Resolver_ABC< Team_ABC >&              Sides() const { return sides_; }
    virtual const tools::Resolver_ABC< KnowledgeGroup_ABC >&    KnowledgeGroups() const { return knowledgeGroups_; }
    virtual const tools::Resolver_ABC< Formation_ABC >&         Formations() const { return formations_; }
    virtual const tools::Resolver_ABC< Automat_ABC >&           Automats() const { return automats_; }
    virtual const tools::Resolver_ABC< Agent_ABC >&             Agents() const { return agents_; }
    virtual const tools::Resolver_ABC< Object_ABC >&            Objects() const { return objects_; }
    virtual const tools::Resolver_ABC< UrbanObject_ABC >&       UrbanBlocks() const { return urbanBlocks_; }
    virtual const tools::Resolver_ABC< Population_ABC >&        Populations() const { return populations_; }
    virtual const tools::Resolver_ABC< Inhabitant_ABC >&        Inhabitants() const { return inhabitants_; }
    virtual const tools::Resolver_ABC< AgentKnowledge_ABC >&    AgentKnowledges() const { return agentKnowledges_; }
    virtual const tools::Resolver_ABC< ObjectKnowledge_ABC >&   ObjectKnowledges() const { return objectKnowledges_; }
    virtual const tools::Resolver_ABC< UrbanKnowledge_ABC >&    UrbanKnowledges() const { return urbanKnowledges_; }
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   PopulationKnowledges() const { return populationKnowledges_; }
    virtual const tools::Resolver_ABC< LogConsignMaintenance >& LogConsignsMaintenance() const { return logConsignsMaintenance_; }
    virtual const tools::Resolver_ABC< LogConsignSupply >&      LogConsignsSupply() const { return logConsignsSupply_; }
    virtual const tools::Resolver_ABC< LogConsignMedical >&     LogConsignsMedical() const { return logConsignsMedical_; }
    virtual const tools::Resolver_ABC< LogConsignFuneral >&     LogConsignsFuneral() const { return logConsignsFuneral_; }
    virtual const tools::Resolver_ABC< Fire >&                  Fires() const { return fires_; }
    virtual const tools::Resolver_ABC< PopulationFire >&        PopulationFires() const { return populationFires_; }
    virtual const tools::Resolver_ABC< FireEffect >&            FireEffects() const { return fireEffects_; }
    virtual const tools::Resolver_ABC< Report >&                Reports() const { return reports_; }

    virtual void SetToTasker( sword::Tasker& tasker, unsigned int id ) const;
    unsigned int TaskerToId( const sword::Tasker& tasker ) const;
    const kernel::ExtensionTypes& GetExtensionTypes() const;
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
    void CreateUpdate2( tools::Resolver< Base >& resolver, const Message& message );
    template< typename Concrete, typename Base, typename Message >
    void CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message );
    template< typename Concrete, typename Base, typename Message, typename Parameter >
    void CreateUpdate( tools::Resolver< Base >& resolver, unsigned id, const Message& message, const Parameter& parameter );
    template< typename T >
    void UpdateAnyAgent( unsigned id, const T& message );
    template< typename T, typename M >
    void Destroy( tools::Resolver< T >& resolver, unsigned id, const M& message );

    template< typename T >
    void AddExtensions( T& entity, kernel::Entity_ABC* )
    {
        // $$$$ AGE 2008-06-20:
        entity.Attach< EntityPublisher_ABC >( *new EntityPublisher< T >( entity ) );
        compositeFactory_->Apply( &ExtensionFactory_ABC< T >::Create, entity );
        compositeFactory_->Apply( &ExtensionFactory_ABC< kernel::EntityBase_ABC >::Create, entity );
    }
    template< typename T >
    void AddExtensions( T& entity, kernel::EntityBase_ABC* )
    {
        // $$$$ AGE 2008-06-20:
        entity.Attach< EntityPublisher_ABC >( *new EntityPublisher< T >( entity ) );
        compositeFactory_->Apply( &ExtensionFactory_ABC< T >::Create, entity );
        compositeFactory_->Apply( &ExtensionFactory_ABC< kernel::EntityBase_ABC >::Create, entity );
    }
    template< typename T >
    void AddExtensions( T& , void* ) {}

    void DeleteCheckpoint( const std::string& name );
    void ClearOldReports( unsigned int tick );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::StaticModel&               staticModel_;
    MemoryLogger_ABC&                        logger_;
    std::auto_ptr< SimulationModel >         simulation_;
    std::auto_ptr< CompositeFactory >        compositeFactory_;
    std::auto_ptr< FolkModel >               folk_;
    std::auto_ptr< MeteoModel >              meteoModel_;
    const Config&                            config_;
    tools::Resolver< Team_ABC >              sides_;
    tools::Resolver< KnowledgeGroup_ABC >    knowledgeGroups_;
    tools::Resolver< Formation_ABC >         formations_;
    tools::Resolver< Automat_ABC >           automats_;
    tools::Resolver< Agent_ABC >             agents_;
    tools::Resolver< Object_ABC >            objects_;
    tools::Resolver< UrbanObject_ABC >       urbanBlocks_;
    tools::Resolver< Population_ABC >        populations_;
    tools::Resolver< Inhabitant_ABC >        inhabitants_;
    tools::Resolver< AgentKnowledge_ABC >    agentKnowledges_;
    tools::Resolver< ObjectKnowledge_ABC >   objectKnowledges_;
    tools::Resolver< UrbanKnowledge_ABC >    urbanKnowledges_;
    tools::Resolver< PopulationKnowledge >   populationKnowledges_;
    tools::Resolver< LogConsignMaintenance > logConsignsMaintenance_;
    tools::Resolver< LogConsignSupply >      logConsignsSupply_;
    tools::Resolver< LogConsignMedical >     logConsignsMedical_;
    tools::Resolver< LogConsignFuneral >     logConsignsFuneral_;
    tools::Resolver< Fire >                  fires_;
    tools::Resolver< PopulationFire >        populationFires_;
    tools::Resolver< FireEffect >            fireEffects_;
    tools::Resolver< Report >                reports_;
    //@}
};

}

#endif // __Model_h_
