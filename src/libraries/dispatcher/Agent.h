 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "Agent_ABC.h"
#include "DecisionalState.h"
#include "tools/Resolver.h"
#include "protocol/SimulationSenders.h"

namespace sword
{
    class UnitCreation;
    class UnitAttributes;
    class DecisionalState;
    class LogMedicalState;
    class LogMaintenanceState;
    class LogSupplyState;
}

namespace sword
{
    class UnitChangeSuperior;
    class UnitOrder;
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
            , public kernel::Updatable_ABC< sword::UnitCreation >
            , public kernel::Updatable_ABC< sword::UnitAttributes >
            , public kernel::Updatable_ABC< sword::DecisionalState >
            , public kernel::Updatable_ABC< sword::LogMedicalState >
            , public kernel::Updatable_ABC< sword::LogMaintenanceState >
            , public kernel::Updatable_ABC< sword::LogSupplyState >
            , public kernel::Updatable_ABC< sword::UnitChangeSuperior >
            , public kernel::Updatable_ABC< sword::UnitOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             Agent( Model_ABC& model, const sword::UnitCreation& msg, const tools::Resolver_ABC< kernel::AgentType >& types );
    virtual ~Agent();
    //@}

    //! @name Main
    //@{
    virtual void DoUpdate( const sword::UnitCreation&        asnMsg );
    virtual void DoUpdate( const sword::UnitAttributes&      asnMsg );
    virtual void DoUpdate( const sword::DecisionalState&     asnMsg );
    virtual void DoUpdate( const sword::LogMedicalState&     asnMsg );
    virtual void DoUpdate( const sword::LogMaintenanceState& asnMsg );
    virtual void DoUpdate( const sword::LogSupplyState&      asnMsg );
    virtual void DoUpdate( const sword::UnitChangeSuperior&           asnMsg );
    virtual void DoUpdate( const sword::UnitOrder&                    asnMsg );

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
    virtual sword::EnumOperationalStatus GetOperationalState() const;
    virtual unsigned short GetOperationalStateValue() const;
    virtual const tools::Resolver< dispatcher::Equipment >& Equipments() const;
    virtual const tools::Resolver< dispatcher::Humans >& Troops() const;
    virtual unsigned short GetAltitude() const;
    virtual unsigned short GetSpeed() const;
    virtual unsigned short GetDirection() const;
    virtual sword::ForceRatio_Value GetForceRatio() const;
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
    sword::UnitAttributes_Posture              nLastPosture_;
    sword::UnitAttributes_Posture              nCurrentPosture_;
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
    sword::ForceRatio_Value                       nForceRatioState_;
    sword::EnumMeetingEngagementStatus                     nCloseCombatState_;
    sword::EnumOperationalStatus                           nOperationalState_;
    sword::UnitAttributes_FireAvailability     nIndirectFireAvailability_;
    sword::RulesOfEngagement_Value                nRoe_;
    sword::UnitAttributes_CrowdRoe             nPopulationRoe_;
    sword::EnumUnitTiredness                               nTiredness_;
    sword::EnumUnitMorale                                  nMorale_;
    sword::EnumUnitExperience                              nExperience_;
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
    std::map< std::string, std::string >                    extensions_;
};

}

#endif // __Agent_h_
