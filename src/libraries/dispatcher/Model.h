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

#include "game_asn/Asn.h"
#include "ModelsContainer.h"
#include "MessageHandler_ABC.h"
#include "CompositeFactory.h"

namespace kernel
{
    class AgentTypes;
}

namespace dispatcher
{

class Config;
class ModelVisitor_ABC;
class Side;
class KnowledgeGroup;
class Formation;
class Automat;
class Agent;    
class Object;
class AgentKnowledge;
class ObjectKnowledge;
class LogConsignMaintenance;
class LogConsignSupply;
class LogConsignMedical;
class Population;
class PopulationKnowledge;
class SimulationModel;
class ClientPublisher_ABC;
class Limit;
class Lima;
class Entity_ABC;
class Fire;
class PopulationFire;
class FireEffect;
class FolkModel;
class Intelligence;
class Report;
class CompositeFactory;
class Factory_ABC;

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
    explicit Model( const Config& config );
    virtual ~Model();
    //@}

    //! @name Update
    //@{
    void Reset();

    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    void Update( const ASN1T_MsgsSimToClient& asnMsg );
    void Send  ( ClientPublisher_ABC& publisher ) const;

    void SendReplayInfo( ClientPublisher_ABC& publisher, unsigned totalTicks, ASN1T_EnumSimulationState status, unsigned factor ) const;
    void SendFirstTick( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    void Accept( ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
          ModelsContainer< Side           >& GetSides          ();
    const ModelsContainer< Side           >& GetSides          () const;
          ModelsContainer< KnowledgeGroup >& GetKnowledgeGroups();
    const ModelsContainer< KnowledgeGroup >& GetKnowledgeGroups() const;
          ModelsContainer< AgentKnowledge >& GetAgentKnowledges();
    const ModelsContainer< AgentKnowledge >& GetAgentKnowledges() const;
          ModelsContainer< Formation      >& GetFormations     ();
    const ModelsContainer< Formation      >& GetFormations     () const;
          ModelsContainer< Automat        >& GetAutomats       ();
    const ModelsContainer< Automat        >& GetAutomats       () const;
          ModelsContainer< Agent          >& GetAgents         ();
    const ModelsContainer< Agent          >& GetAgents         () const;
          ModelsContainer< Object         >& GetObjects        ();
    const ModelsContainer< Object         >& GetObjects        () const;
          ModelsContainer< Population     >& GetPopulations    ();
    const ModelsContainer< Limit          >& GetLimits         () const;
    const ModelsContainer< Lima           >& GetLimas          () const;

    const kernel::AgentTypes& GetAgentTypes() const;

    void RegisterFactory( Factory_ABC& factory )
    {
        compositeFactory_->Register( factory );
    }
    void UnregisterFactory( Factory_ABC& factory )
    {
        compositeFactory_->Unregister( factory );
    }

    template< typename T >
    void AddExtensions( T& entity )
    {
        AddExtensions( &entity );
        compositeFactory_->Apply( ExtensionFactory_ABC< T >::Create, entity );
    }
    void AddExtensions( Entity_ABC* entity )
    {
        compositeFactory_->Apply( ExtensionFactory_ABC< Entity_ABC >::Create, *entity );
    }
    void AddExtensions( void* ) {}
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
    void CreateUpdate( ModelsContainer< T >& container, unsigned id, const P& parameter );
    template< typename T >
    void UpdateAnyAgent( unsigned id, const T& message );
    //@}

private:
    SimulationModel*    pSimulationModel_;
    std::auto_ptr< FolkModel > folkModel_;

    std::auto_ptr< CompositeFactory >         compositeFactory_;

    ModelsContainer< Side                   > sides_;
    ModelsContainer< KnowledgeGroup         > knowledgeGroups_;
    ModelsContainer< Formation              > formations_;
    ModelsContainer< Automat                > automats_;
    ModelsContainer< Agent                  > agents_;
    ModelsContainer< Object                 > objects_;
    ModelsContainer< AgentKnowledge         > agentKnowledges_;
    ModelsContainer< ObjectKnowledge        > objectKnowledges_;
    ModelsContainer< LogConsignMaintenance  > logConsignsMaintenance_;
    ModelsContainer< LogConsignSupply       > logConsignsSupply_;
    ModelsContainer< LogConsignMedical      > logConsignsMedical_;
    ModelsContainer< Population             > populations_;
    ModelsContainer< PopulationKnowledge    > populationKnowledges_;
    ModelsContainer< Limit                  > limits_;
    ModelsContainer< Lima                   > limas_;
    ModelsContainer< Fire                   > fires_;
    ModelsContainer< PopulationFire         > populationFires_;
    ModelsContainer< FireEffect             > fireEffects_;
    ModelsContainer< Intelligence           > intelligences_;
    ModelsContainer< Report                 > reports_;

    std::auto_ptr< kernel::AgentTypes >     agentTypes_;
};

}

#include "Model.inl"

#endif // __Model_h_
