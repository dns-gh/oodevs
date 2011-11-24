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

#include "Localisation.h"
#include "Agent_ABC.h"
#include "DecisionalState.h"
#include "tools/Resolver.h"
#include "protocol/SimulationSenders.h"
#include <map>

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
            , public kernel::Updatable_ABC< sword::UnitPathFind >
            , public kernel::Updatable_ABC< sword::UnitEnvironmentType >
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
    virtual void DoUpdate( const sword::UnitChangeSuperior&  asnMsg );
    virtual void DoUpdate( const sword::UnitOrder&           asnMsg );
    virtual void DoUpdate( const sword::UnitPathFind&        asnMsg );
    virtual void DoUpdate( const sword::UnitEnvironmentType& message );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void SetSuperior    ( dispatcher::Automat_ABC& superior );
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
    virtual const std::vector< dispatcher::Humans >& Troops() const;
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
    //! @name Types
    //@{
    struct Satisfaction
    {
        float safety_;
        float lodging_;
        float health_;
    };
    struct HumanRepartition
    {
        float male_;
        float female_;
        float children_;

        HumanRepartition( float male, float female, float children ) : male_( male ), female_( female ), children_( children ) {}
    };
    typedef std::map< unsigned long, float > T_Affinities;
    typedef T_Affinities::const_iterator   CIT_Affinities;
    //@}

private:
    Model_ABC&                                 model_;
    const kernel::AgentType&                   type_;
    const std::string                          name_;
    dispatcher::Automat_ABC*                   automat_;
    const bool                                 bPC_;
    geometry::Point2d                          position_; // x = longitude, y = latitude
    sword::RgbColor                            color_;
    unsigned int                               nDirection_;
    unsigned int                               nHeight_;
    unsigned int                               nAltitude_;
    unsigned int                               nSpeed_;
    unsigned int                               nOperationalStateValue_;
    tools::Resolver< const kernel::Agent_ABC > reinforcements_;
    kernel::Agent_ABC*                         pReinforced_;
    bool                                       bDead_;
    bool                                       bNeutralized_;
    bool                                       bStealthModeEnabled_;
    bool                                       bUnderground_;
    bool                                       isMounted_;
    bool                                       bHumanTransportersAvailable_;
    sword::UnitAttributes::Posture             nLastPosture_;
    sword::UnitAttributes::Posture             nCurrentPosture_;
    unsigned int                               nPostureCompletion_;
    unsigned int                               nInstallationState_;
    bool                                       bNbcProtectionSuitEnabled_;
    std::vector< unsigned int >                nbcAgentTypesContaminating_;
    int                                        contaminationPercentage_;
    float                                      contaminationQuantity_;
    bool                                       communicationJammed_;
    unsigned int                               knowledgeGroupJammed_;
    bool                                       bRadioEmitterEnabled_;
    bool                                       bRadioRecieverEnabled_;
    bool                                       bRadarEnabled_;
    tools::Resolver< const kernel::Agent_ABC > transportedAgents_;
    kernel::Agent_ABC*                         pTransporter_;
    sword::ForceRatio::Value                   nForceRatioState_;
    sword::EnumMeetingEngagementStatus         nCloseCombatState_;
    sword::EnumOperationalStatus               nOperationalState_;
    sword::UnitAttributes::FireAvailability    nIndirectFireAvailability_;
    sword::RulesOfEngagement::Value            nRoe_;
    sword::UnitAttributes::CrowdRoe            nPopulationRoe_;
    sword::UnitAttributes::EnumUnitTiredness   nTiredness_;
    sword::UnitAttributes::EnumUnitMorale      nMorale_;
    sword::UnitAttributes::EnumUnitExperience  nExperience_;
    sword::UnitAttributes::EnumUnitStress      nStress_;
    const kernel::Team_ABC*                    pSideSurrenderedTo_;
    bool                                       bPrisonner_;
    bool                                       bRefugeeManaged_;
    DecisionalState                            decisionalInfos_;
    tools::Resolver< Equipment >               equipments_;
    //tools::Resolver< Humans >                  troops_;
    std::vector< Humans >                      troops_;
    tools::Resolver< Dotation >                dotations_;
    tools::Resolver< Loan >                    borrowings_;
    tools::Resolver< Loan >                    lendings_;
    std::auto_ptr< AgentLogMedical >           pLogMedical_;
    std::auto_ptr< AgentLogMaintenance >       pLogMaintenance_;
    std::auto_ptr< AgentLogSupply >            pLogSupply_;
    std::auto_ptr< AgentOrder >                order_;
    std::map< std::string, std::string >       extensions_;
    std::string                                criticalIntelligence_;
    Localisation                               currentPath_;
    T_Affinities                               affinities_;
    int                                        transportedCrowd_;
    std::auto_ptr< Satisfaction >              statisfaction_;
    std::auto_ptr< HumanRepartition >          humanRepartition_;
};

}

#endif // __Agent_h_
