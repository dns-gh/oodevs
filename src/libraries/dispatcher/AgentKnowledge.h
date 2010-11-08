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
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitKnowledgeUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledge( Model& model, const MsgsSimToClient::MsgUnitKnowledgeCreation& asnMsg );
    virtual ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitKnowledgeUpdate& asnMsg );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::Agent_ABC* GetEntity() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual MsgsSimToClient::EnumUnitIdentificationLevel GetMaxPerceptionLevel() const;
    virtual geometry::Point2d GetPosition() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MsgsSimToClient::AutomatPerception > T_AutomatePerceptionVector;
    struct T_Optionals
    {
      unsigned pertinencePresent : 1;
      unsigned identification_levelPresent : 1;
      unsigned max_identification_levelPresent : 1;
      unsigned etat_opPresent : 1;
      unsigned mortPresent : 1;
      unsigned speedPresent : 1;
      unsigned nature_pcPresent : 1;
      unsigned renduPresent : 1;
      unsigned prisonnierPresent : 1;
      unsigned refugie_pris_en_comptePresent : 1;
      unsigned positionPresent : 1;
      unsigned directionPresent : 1;
      unsigned campPresent : 1;
      unsigned perception_par_compagniePresent : 1;
    };
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const kernel::KnowledgeGroup_ABC& knowledgeGroup_;
    const dispatcher::Agent_ABC& agent_;
    const Common::UnitType type_;
    unsigned int nRelevance_;
    MsgsSimToClient::EnumUnitIdentificationLevel nPerceptionLevel_;
    MsgsSimToClient::EnumUnitIdentificationLevel nMaxPerceptionLevel_;
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
    T_Optionals optionals_;
    //@}
};

}

#endif // __AgentKnowledge_h_
