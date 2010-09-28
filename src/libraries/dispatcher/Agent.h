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

#include "Agent_ABC.h"
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
    class Team_ABC;
}

namespace dispatcher
{
    class Automat_ABC;
    class Model_ABC;
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
class Agent : public dispatcher::Agent_ABC
            , public kernel::Extension_ABC
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitCreation >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitAttributes >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgDecisionalState >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogMedicalState >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogMaintenanceState >
            , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogSupplyState >
            , public kernel::Updatable_ABC< Common::MsgUnitChangeSuperior >
            , public kernel::Updatable_ABC< Common::MsgUnitOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( Model_ABC& model, const MsgsSimToClient::MsgUnitCreation& msg, const tools::Resolver_ABC< kernel::AgentType >& types );
    virtual ~Agent();
    //@}

    //! @name Main
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitCreation&        asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitAttributes&      asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgDecisionalState&     asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogMedicalState&     asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogMaintenanceState& asnMsg );
    virtual void DoUpdate( const MsgsSimToClient::MsgLogSupplyState&      asnMsg );
    virtual void DoUpdate( const Common::MsgUnitChangeSuperior&           asnMsg );
    virtual void DoUpdate( const Common::MsgUnitOrder&                    asnMsg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void SetSuperior( dispatcher::Automat_ABC& superior );
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::AgentType& GetType() const;
    virtual bool IsCommandPost() const;
    virtual const geometry::Point2d& GetPosition() const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual const dispatcher::Automat_ABC& GetSuperior() const;
    virtual Common::EnumOperationalStatus GetOperationalState() const;
    virtual unsigned short GetOperationalStateValue() const;
    virtual const tools::Resolver< dispatcher::Equipment >& Equipments() const;
    virtual const tools::Resolver< dispatcher::Humans >& Troops() const;
    virtual unsigned short GetAltitude() const;
    virtual unsigned short GetSpeed() const;
    virtual unsigned short GetDirection() const;
    virtual MsgsSimToClient::ForceRatio_Value GetForceRatio() const;
    virtual const Order_ABC* GetOrder() const;
    virtual bool IsMounted() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

private:
    Model_ABC&                                              model_;
    const kernel::AgentType&                                type_;
    const std::string                                       name_;
    dispatcher::Automat_ABC*                                automat_;
    const bool                                              bPC_;

    geometry::Point2d                                       position_; // x = longitude, y = latitude
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
    bool                                                    isMounted_;
    bool                                                    bHumanTransportersAvailable_;
    MsgsSimToClient::MsgUnitAttributes_Posture              nLastPosture_;
    MsgsSimToClient::MsgUnitAttributes_Posture              nCurrentPosture_;
    unsigned int                                            nPostureCompletion_;
    unsigned int                                            nInstallationState_;
    bool                                                    bNbcProtectionSuitEnabled_;
    std::vector< unsigned int >                             nbcAgentTypesContaminating_;
    int                                                     contaminationPercentage_;
    float                                                   contaminationQuantity_;

    bool                                                    communicationJammed_;
    unsigned int                                            knowledgeGroupJammed_;

    bool                                                    bRadioEmitterEnabled_;
    bool                                                    bRadioRecieverEnabled_;
    bool                                                    bRadarEnabled_;
    tools::Resolver< const kernel::Agent_ABC >              transportedAgents_;
    kernel::Agent_ABC*                                      pTransporter_;
    MsgsSimToClient::ForceRatio_Value                       nForceRatioState_;
    Common::EnumMeetingEngagementStatus                     nCloseCombatState_;
    Common::EnumOperationalStatus                           nOperationalState_;
    MsgsSimToClient::MsgUnitAttributes_FireAvailability     nIndirectFireAvailability_;
    MsgsSimToClient::RulesOfEngagement_Value                nRoe_;
    MsgsSimToClient::MsgUnitAttributes_CrowdRoe             nPopulationRoe_;
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
