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
#include "ENT/ENT_Enums.h"
#include <tools/Resolver.h>
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
            , public kernel::Updatable_ABC< sword::CrowdConcentrationDetection >
            , public kernel::Updatable_ABC< sword::CrowdFlowDetection >
            , public kernel::Updatable_ABC< sword::DecisionalState >
            , public kernel::Updatable_ABC< sword::LogMaintenanceState >
            , public kernel::Updatable_ABC< sword::LogMedicalState >
            , public kernel::Updatable_ABC< sword::LogSupplyState >
            , public kernel::Updatable_ABC< sword::ObjectDetection >
            , public kernel::Updatable_ABC< sword::UnitAttributes >
            , public kernel::Updatable_ABC< sword::UnitChangeSuperior >
            , public kernel::Updatable_ABC< sword::UnitCreation >
            , public kernel::Updatable_ABC< sword::UnitDestruction >
            , public kernel::Updatable_ABC< sword::UnitDetection >
            , public kernel::Updatable_ABC< sword::UnitEnvironmentType >
            , public kernel::Updatable_ABC< sword::UnitOrder >
            , public kernel::Updatable_ABC< sword::UnitPathFind >
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
    virtual void DoUpdate( const sword::UnitDestruction&     asnMsg );
    virtual void DoUpdate( const sword::UnitAttributes&      asnMsg );
    virtual void DoUpdate( const sword::DecisionalState&     asnMsg );
    virtual void DoUpdate( const sword::LogMedicalState&     asnMsg );
    virtual void DoUpdate( const sword::LogMaintenanceState& asnMsg );
    virtual void DoUpdate( const sword::LogSupplyState&      asnMsg );
    virtual void DoUpdate( const sword::UnitChangeSuperior&  asnMsg );
    virtual void DoUpdate( const sword::UnitOrder&           asnMsg );
    virtual void DoUpdate( const sword::UnitPathFind&        asnMsg );
    virtual void DoUpdate( const sword::UnitEnvironmentType& message );
    virtual void DoUpdate( const sword::UnitDetection&       message );
    virtual void DoUpdate( const sword::ObjectDetection&     message );
    virtual void DoUpdate( const sword::CrowdConcentrationDetection& message );
    virtual void DoUpdate( const sword::CrowdFlowDetection& message );

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
    virtual bool GetExtension( const std::string& key, std::string& result ) const;
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

    struct UnitDetectionData
    {
        sword::UnitVisibility_Level currentLevel_;
        sword::UnitVisibility_Level maxLevel_;
    };
    typedef std::map< unsigned long, UnitDetectionData > T_UnitDetection;
    typedef std::map< unsigned long, sword::UnitVisibility_Level > T_ObjectDetection;

    struct CrowdConcentrationDetectionData
    {
        unsigned long crowdId_;
        sword::UnitVisibility_Level level_;
    };
    typedef std::map< unsigned long, CrowdConcentrationDetectionData > T_CrowdConcentrationDetection;

    struct CrowdFlowDetectionData
    {
        unsigned long crowdId_;
        Localisation path_;
    };
    typedef std::map< unsigned long, CrowdFlowDetectionData > T_CrowdFlowDetection;
    //@}

private:
    Model_ABC&                                 model_;
    const kernel::AgentType&                   type_;
    dispatcher::Automat_ABC*                   automat_;
    geometry::Point2d                          position_; // x = longitude, y = latitude
    std::unique_ptr< sword::RgbColor >         color_;
    unsigned int                               nDirection_;
    unsigned int                               nSensorsDirection_;
    unsigned int                               nHeight_;
    unsigned int                               nAltitude_;
    unsigned int                               nSpeed_;
    unsigned int                               nOperationalStateValue_;
    tools::Resolver< const kernel::Agent_ABC > reinforcements_;
    kernel::Agent_ABC*                         pReinforced_;
    bool                                       bPC_;
    bool                                       bDead_;
    bool                                       bNeutralized_;
    bool                                       bStealthModeEnabled_;
    bool                                       bUnderground_;
    bool                                       isMounted_;
    bool                                       bHumanTransportersAvailable_;
    bool                                       bNbcProtectionSuitEnabled_;
    bool                                       communicationJammed_;
    bool                                       bRadioEmitterEnabled_;
    bool                                       bRadioRecieverEnabled_;
    bool                                       bRadarEnabled_;
    bool                                       bPrisonner_;
    bool                                       bRefugeeManaged_;
    bool                                       contaminated_;
    sword::UnitAttributes::Posture             nLastPosture_;
    sword::UnitAttributes::Posture             nCurrentPosture_;
    unsigned int                               nPostureCompletion_;
    unsigned int                               nInstallationState_;
    std::vector< unsigned int >                nbcAgentTypesContaminating_;
    int                                        contaminationPercentage_;  // deprecated
    int                                        decontaminationPercentage_;
    float                                      contaminationQuantity_;
    float                                      dose_;
    unsigned int                               knowledgeGroupJammed_;
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
    DecisionalState                            decisionalInfos_;
    tools::Resolver< Equipment >               equipments_;
    //tools::Resolver< Humans >                  troops_;
    std::vector< Humans >                      troops_;
    tools::Resolver< Dotation >                dotations_;
    tools::Resolver< Loan >                    borrowings_;
    tools::Resolver< Loan >                    lendings_;
    std::unique_ptr< AgentLogMedical >         pLogMedical_;
    std::unique_ptr< AgentLogMaintenance >     pLogMaintenance_;
    std::unique_ptr< AgentLogSupply >          pLogSupply_;
    std::unique_ptr< AgentOrder >              order_;
    std::map< std::string, std::string >       extensions_;
    std::string                                criticalIntelligence_;
    Localisation                               currentPath_;
    T_Affinities                               affinities_;
    int                                        transportedCrowd_;
    std::unique_ptr< Satisfaction >            statisfaction_;
    std::unique_ptr< HumanRepartition >        humanRepartition_;
    std::string                                decisionalModel_;
    bool                                       brainDebug_;
    T_UnitDetection                            unitDetections_;
    T_ObjectDetection                          objectDetections_;
    T_CrowdConcentrationDetection              crowdConcentrationDetections_;
    T_CrowdFlowDetection                       crowdFlowDetections_;
    std::string                                app6Symbol_;
    E_NatureLevel                              level_;
};

}

#endif // __Agent_h_
