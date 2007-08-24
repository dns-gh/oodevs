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

namespace dispatcher
{

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
             Model();
    virtual ~Model();
    //@}

    //! @name Update
    //@{
    void Reset();

    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    void Update( const ASN1T_MsgsSimToClient& asnMsg );

    void Send  ( ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    void Accept( ModelVisitor_ABC& visitor ) const;
    void StartSynchronisation();
    void EndSynchronisation( ClientPublisher_ABC& publisher );
    //@}

    //! @name Synchro
    //@{
    void FlagForCreation     ( Entity_ABC& synch );
    void FlagForUpdate       ( Entity_ABC& synch );
    void FlagForSpecialUpdate( Entity_ABC& synch );
    void FlagForDestruction  ( Entity_ABC& synch );
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

    bool synching_;
};

}

#include "Model.inl"

#endif // __Model_h_
