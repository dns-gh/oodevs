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

#include "MessageHandler_ABC.h"
#include "CompositeFactory.h"
#include "Model_ABC.h"

namespace kernel
{
    class StaticModel;
    class ExtensionTypes;
    class Entity_ABC;
    class EntityBase_ABC;
}

namespace dispatcher
{
    class Action;
    class Config;
    class DetectionRangeEffect;
    class Fire;
    class FireEffect;
    class FolkModel;
    class LogConsignFuneral;
    class LogConsignMaintenance;
    class LogConsignMedical;
    class LogConsignSupply;
    class LogRequestSupply;
    class MeteoModel;
    class Pathfind;
    class PopulationFire;
    class Report;
    class SimulationModel;
    class UrbanKnowledge_ABC;
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

    virtual void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned int totalTicks, const std::string& endDateTime, sword::EnumSimulationState status, unsigned int factor, unsigned int firstTick = 1 ) const;
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
    virtual const tools::Resolver_ABC< PopulationKnowledge >&   PopulationKnowledges() const { return populationKnowledges_; }

    virtual void SetToTasker( sword::Tasker& tasker, unsigned int id ) const;
    const kernel::ExtensionTypes& GetExtensionTypes() const;
    //@}

private:
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
    void Destroy( tools::Resolver< T >& resolver, unsigned id, const M* message );
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
    void ClearLogisticRequests( unsigned int unitId );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::StaticModel&               staticModel_;
    MemoryLogger_ABC&                        logger_;
    std::unique_ptr< SimulationModel >       simulation_;
    std::unique_ptr< CompositeFactory >      compositeFactory_;
    std::unique_ptr< FolkModel >             folk_;
    std::unique_ptr< MeteoModel >            meteoModel_;
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
    tools::Resolver< LogRequestSupply >      logRequestsSupply_;
    tools::Resolver< LogConsignMedical >     logConsignsMedical_;
    tools::Resolver< LogConsignFuneral >     logConsignsFuneral_;
    tools::Resolver< Fire >                  fires_;
    tools::Resolver< PopulationFire >        populationFires_;
    tools::Resolver< FireEffect >            fireEffects_;
    tools::Resolver< DetectionRangeEffect >  detectionRangeEffects_;
    tools::Resolver< Report >                reports_;
    tools::Resolver< Action >                actions_;
    tools::Resolver< Pathfind >              pathfinds_;
    //@}
};

}

#endif // __Model_h_
