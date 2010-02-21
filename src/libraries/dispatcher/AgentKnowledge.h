 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "clients_kernel/AgentKnowledge_ABC.h"
#include "SimpleEntity.h"
#include "protocol/SimulationSenders.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace Common
{
    class MsgCoordLatLong;
    class MsgUnitType;
}

namespace MsgsSimToClient
{
    class MsgUnitKnowledgeCreation;
    class MsgUnitKnowledgeUpdate;
    enum EnumUnitIdentificationLevel;
    class AutomatPerception;
}

namespace dispatcher
{
    class Model;
    class KnowledgeGroup;
    class Agent;
    class Side;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  AgentKnowledge
    @brief  AgentKnowledge
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentKnowledge : public SimpleEntity< kernel::AgentKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    AgentKnowledge( Model& model, const MsgsSimToClient::MsgUnitKnowledgeCreation& asnMsg );
    virtual ~AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& asnMsg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetRecognizedEntity() const;
    virtual const kernel::Agent_ABC* GetEntity() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MsgsSimToClient::AutomatPerception > T_AutomatePerceptionVector;

    //$$$ bullshit
    struct T_Optionals
    {
      unsigned pertinencePresent : 1;
      unsigned identification_levelPresent : 1;
      unsigned max_identification_levelPresent : 1;
      unsigned etat_opPresent : 1;
      unsigned mortPresent : 1;
      unsigned positionPresent : 1;
      unsigned directionPresent : 1;
      unsigned speedPresent : 1;
      unsigned campPresent : 1;
      unsigned nature_pcPresent : 1;
      unsigned perception_par_compagniePresent : 1;
      unsigned renduPresent : 1;
      unsigned prisonnierPresent : 1;
      unsigned refugie_pris_en_comptePresent : 1;
    };
    //@}

public:
    //! @name Member data
    //@{
    const Model&                        model_;
    const KnowledgeGroup&               knowledgeGroup_;
    const Agent&                        agent_;
    const Common::MsgUnitType&          type_;

    unsigned int                        nRelevance_;
    MsgsSimToClient::EnumUnitIdentificationLevel   nPerceptionLevel_;
    MsgsSimToClient::EnumUnitIdentificationLevel   nMaxPerceptionLevel_;
    unsigned int                        nOperationalState_;
    bool                                bDead_;
    geometry::Point2d                   position_;
    unsigned int                        nDirection_;
    unsigned int                        nSpeed_;
    const kernel::Team_ABC*             team_;
    bool                                bPC_; //$$$ A VIRER
    T_AutomatePerceptionVector          automatePerceptions_;
    bool                                bSurrendered_;
    bool                                bPrisoner_;
    bool                                bRefugeeManaged_;

    T_Optionals                         optionals_;
    //@}
};

}

#endif // __AgentKnowledge_h_
