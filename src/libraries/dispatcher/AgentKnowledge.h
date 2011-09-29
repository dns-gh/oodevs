 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "AgentKnowledge_ABC.h"
#include "protocol/SimulationSenders.h"

namespace kernel
{
    class KnowledgeGroup_ABC;
}

namespace dispatcher
{
    class Model;
    class Agent_ABC;

// =============================================================================
/** @class  AgentKnowledge
    @brief  AgentKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentKnowledge : public dispatcher::AgentKnowledge_ABC
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledge( Model& model, const sword::UnitKnowledgeCreation& asnMsg );
    virtual ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UnitKnowledgeUpdate& asnMsg );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::Agent_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual sword::UnitIdentification::Level GetMaxPerceptionLevel() const;
    virtual geometry::Point2d GetPosition() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< sword::AutomatPerception > T_AutomatePerceptionVector;
    struct T_Optionals
    {
      unsigned pertinencePresent : 1;
      unsigned identification_levelPresent : 1;
      unsigned max_identification_levelPresent : 1;
      unsigned operational_statePresent : 1;
      unsigned deadPresent : 1;
      unsigned speedPresent : 1;
      unsigned command_postPresent : 1;
      unsigned renduPresent : 1;
      unsigned prisonerPresent : 1;
      unsigned refugees_managedPresent : 1;
      unsigned positionPresent : 1;
      unsigned directionPresent : 1;
      unsigned campPresent : 1;
      unsigned perceptionsPresent : 1;
      unsigned critical_intelligencePresent : 1;
    };
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const kernel::KnowledgeGroup_ABC& knowledgeGroup_;
    const dispatcher::Agent_ABC& agent_;
    const sword::UnitType type_;
    unsigned int nRelevance_;
    sword::UnitIdentification::Level nPerceptionLevel_;
    sword::UnitIdentification::Level nMaxPerceptionLevel_;
    unsigned int nOperationalState_;
    bool bDead_;
    geometry::Point2d position_; // $$$$ _RC_ SBO 2010-05-27: x = latitude, y = longitude !
    unsigned int nDirection_;
    unsigned int nSpeed_;
    const kernel::Team_ABC* team_;
    bool bPC_; //$$$ A VIRER
    T_AutomatePerceptionVector automatePerceptions_;
    unsigned long surrendered_;
    bool bPrisoner_;
    bool bRefugeeManaged_;
    std::string criticalIntelligence_;
    T_Optionals optionals_;
    //@}
};

}

#endif // __AgentKnowledge_h_
