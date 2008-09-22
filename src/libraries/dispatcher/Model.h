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

#include "game_asn/Simulation.h"
#include "MessageHandler_ABC.h"
#include "CompositeFactory.h"
#include "EntityPublisher.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class AgentTypes;
    class AgentType;
    class MissionType;
    class FragOrderType;
    class Entity_ABC;
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

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: NLD 2006-09-19
// =============================================================================
class Model : public MessageHandler_ABC
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
    const kernel::Resolver_ABC< kernel::AgentType >& GetAgentTypes() const;
    const kernel::Resolver_ABC< kernel::MissionType >& GetMissionTypes() const;
    const kernel::Resolver_ABC< kernel::FragOrderType >& GetFragOrderTypes() const;
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
    void CreateUpdate( kernel::Resolver< T >& resolver, const P& parameter );
    template< typename T, typename P >
    void CreateUpdate( kernel::Resolver< T >& resolver, unsigned id, const P& parameter );
    template< typename T >
    void UpdateAnyAgent( unsigned id, const T& message );
    template< typename T >
    void Destroy( kernel::Resolver< T >& resolver, unsigned id );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< SimulationModel >    simulation_;
    std::auto_ptr< CompositeFactory >   compositeFactory_;

    std::auto_ptr< FolkModel >          folk_;
    std::auto_ptr< kernel::AgentTypes > agentTypes_;
    //@}

public:
    //! @name Dynamic model
    //@{
    kernel::Resolver< Side >                   sides_;
    kernel::Resolver< KnowledgeGroup >         knowledgeGroups_;
    kernel::Resolver< Formation >              formations_;
    kernel::Resolver< Automat >                automats_;
    kernel::Resolver< Agent >                  agents_;
    kernel::Resolver< Object >                 objects_;
    kernel::Resolver< Population >             populations_;
    kernel::Resolver< AgentKnowledge >         agentKnowledges_;
    kernel::Resolver< ObjectKnowledge >        objectKnowledges_;
    kernel::Resolver< PopulationKnowledge >    populationKnowledges_;
    kernel::Resolver< LogConsignMaintenance >  logConsignsMaintenance_;
    kernel::Resolver< LogConsignSupply >       logConsignsSupply_;
    kernel::Resolver< LogConsignMedical >      logConsignsMedical_;
    kernel::Resolver< Fire >                   fires_;
    kernel::Resolver< PopulationFire >         populationFires_;
    kernel::Resolver< FireEffect >             fireEffects_;
    kernel::Resolver< Report >                 reports_;
    //@}
};

}

#endif // __Model_h_
