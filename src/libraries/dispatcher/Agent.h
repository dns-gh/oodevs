 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "clients_kernel/Agent_ABC.h"
#include "SimpleEntity.h"
#include "DecisionalState.h"
#include "tools/Resolver.h"
#include "protocol/SimulationSenders.h"

namespace MsgsSimToClient
{
    class MsgUnitCreation;
    class MsgUnitAttributes;
    class MsgDecisionalState;
    class MsgLogMedicalState;
    class MsgLogMaintenanceState;
    class MsgLogSupplyState;
}

namespace Common
{
    class MsgUnitChangeSuperior;
    class MsgUnitOrder;
    class MsgCoordLatLong;
}

namespace kernel
{
    class AgentType;
    class CoordinateConverter_ABC;
    class ModelVisitor_ABC;
    class StaticModel;
    class Team_ABC;
}

namespace dispatcher
{
    class Automat;
    class Model;
    class Equipment;
    class Dotation;
    class Humans;
    class Loan;
    class AgentLogMedical;
    class AgentLogMaintenance;
    class AgentLogSupply;
    class AgentOrder;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: NLD 2006-09-19
// =============================================================================
class Agent : public SimpleEntity< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( Model& model, const MsgsSimToClient::MsgUnitCreation& msg, const kernel::StaticModel& staticModel );
    virtual ~Agent();
    //@}

    //! @name Main
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const MsgsSimToClient::MsgUnitCreation&        asnMsg );
    void Update( const MsgsSimToClient::MsgUnitAttributes&      asnMsg );
    void Update( const MsgsSimToClient::MsgDecisionalState&     asnMsg );
    void Update( const MsgsSimToClient::MsgLogMedicalState&     asnMsg );
    void Update( const MsgsSimToClient::MsgLogMaintenanceState& asnMsg );
    void Update( const MsgsSimToClient::MsgLogSupplyState&      asnMsg );
    void Update( const Common::MsgUnitChangeSuperior&  asnMsg );
    void Update( const Common::MsgUnitOrder&           asnMsg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::AgentType& GetType() const;
    virtual bool IsCommandPost() const;
    virtual const geometry::Point2d& GetPosition() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ChangeAutomat( unsigned long id );
    //@}

public:
    Model&                                                  model_;
    const kernel::AgentType&                                type_;
    const std::string                                       name_;
    Automat*                                                automat_;
    const bool                                              bPC_;

    geometry::Point2d                                       position_;
    unsigned int                                            nDirection_;
    unsigned int                                            nHeight_;
    unsigned int                                            nAltitude_;
    unsigned int                                            nSpeed_;
    unsigned int                                            nOperationalStateValue_;
    tools::Resolver< const kernel::Agent_ABC >              reinforcements_;
    kernel::Agent_ABC*                                      pReinforced_;
    bool                                                    bDead_;
    bool                                                    bNeutralized_;
    bool                                                    bStealthModeEnabled_;
    bool                                                    bLoaded_;
    bool                                                    bHumanTransportersAvailable_;
    MsgsSimToClient::MsgUnitAttributes_Posture              nLastPosture_;
    MsgsSimToClient::MsgUnitAttributes_Posture              nCurrentPosture_;
    unsigned int                                            nPostureCompletion_;
    unsigned int                                            nInstallationState_;
    bool                                                    bNbcProtectionSuitEnabled_;
    std::vector< unsigned int >                             nbcAgentTypesContaminating_;
    MsgsSimToClient::ContaminationState                     contamination_;

    bool                                                    communicationJammed_;
    unsigned int                                            knowledgeGroupJammed_;

    bool                                                    bBlackoutEnabled_;
    bool                                                    bRadarEnabled_;
    tools::Resolver< const kernel::Agent_ABC >              transportedAgents_;
    kernel::Agent_ABC*                                      pTransporter_;
    MsgsSimToClient::ForceRatio_Value                       nForceRatioState_;
    Common::EnumMeetingEngagementStatus                     nCloseCombatState_;
    Common::EnumOperationalStatus                           nOperationalState_;
    MsgsSimToClient::MsgUnitAttributes_FireAvailability     nIndirectFireAvailability_;
    MsgsSimToClient::RulesOfEngagement_Value                nRoe_;
    MsgsSimToClient::MsgUnitAttributes_PopulationRoe        nPopulationRoe_;
    Common::EnumUnitTiredness                               nTiredness_;
    Common::EnumUnitMorale                                  nMorale_;
    Common::EnumUnitExperience                              nExperience_;
    const kernel::Team_ABC*                                 pSideSurrenderedTo_;
    bool                                                    bPrisonner_;
    bool                                                    bRefugeeManaged_;
    DecisionalState                                         decisionalInfos_;

    tools::Resolver< Equipment >                            equipments_;
    tools::Resolver< Humans >                               troops_;
    tools::Resolver< Dotation >                             dotations_;
    tools::Resolver< Loan >                                 borrowings_;
    tools::Resolver< Loan >                                 lendings_;

    AgentLogMedical*                                        pLogMedical_;
    AgentLogMaintenance*                                    pLogMaintenance_;
    AgentLogSupply*                                         pLogSupply_;

    std::auto_ptr< AgentOrder >                             order_;
};

}

#endif // __Agent_h_
