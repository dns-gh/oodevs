 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_


#include "clients_kernel/Automat_ABC.h"
#include "SimpleEntity.h"
#include "DecisionalState.h"

namespace Common
{
    enum EnumAutomatMode;
    enum EnumMeetingEngagementStatus;
    enum EnumOperationalStatus;
    class MsgAutomatChangeLogisticLinks;
    class MsgAutomatChangeSuperior;
    class MsgAutomatChangeKnowledgeGroup;
    class MsgAutomatOrder;
}

namespace MsgsSimToClient
{
    enum ForceRatio_Value;
    enum RulesOfEngagement_Value;

    class MsgAutomatCreation;
    class MsgDecisionalState;
    class MsgLogSupplyQuotas;
    class MsgAutomatAttributes;
}

namespace kernel
{
    class Formation_ABC;
    class Knowledge_ABC;
    class ModelVisitor_ABC;
    class Team_ABC;
}
namespace dispatcher
{
    class Model_ABC;
    class DotationQuota;
    class AutomatOrder;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: NLD 2006-09-19
// =============================================================================
class Automat : public SimpleEntity< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( Model_ABC& model, const MsgsSimToClient::MsgAutomatCreation& msg );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::AutomatType& GetType() const;
    bool IsEngaged() const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::MsgAutomatCreation&             msg );
    void Update( const MsgsSimToClient::MsgDecisionalState&             msg );
    void Update( const MsgsSimToClient::MsgAutomatAttributes&           msg );
    void Update( const MsgsSimToClient::MsgLogSupplyQuotas&             msg );
    void Update( const Common::MsgAutomatChangeLogisticLinks&  msg );
    void Update( const Common::MsgAutomatChangeSuperior&       msg );
    void Update( const Common::MsgAutomatChangeKnowledgeGroup& msg );
    void Update( const Common::MsgAutomatOrder&                msg );

    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual kernel::Team_ABC& GetTeam() const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    virtual void Register( kernel::Automat_ABC& automat );
    virtual void Remove( kernel::Automat_ABC& automat );
    virtual const tools::Resolver< kernel::Automat_ABC >& GetAutomats() const;
    virtual void Register( kernel::Agent_ABC& automat );
    virtual void Remove( kernel::Agent_ABC& automat );
    virtual void NotifyParentDestroyed();
    virtual const tools::Resolver< kernel::Agent_ABC >& GetAgents() const;
    virtual kernel::Automat_ABC* GetParentAutomat() const;
    virtual kernel::Formation_ABC* GetFormation() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ChangeKnowledgeGroup( unsigned long id );
    template< typename Superior >
    void ChangeSuperior( const Superior& superior );
    void NotifyDestructionToChildAutomats();
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC&              model_;
    const unsigned long type_; // XML reference - no resolved by dispatcher
    const std::string   name_;
    kernel::Team_ABC&   team_;
    kernel::Formation_ABC* parentFormation_;
    kernel::Automat_ABC*   parentAutomat_;
    kernel::KnowledgeGroup_ABC* knowledgeGroup_;
    tools::Resolver< DotationQuota > quotas_;

    Common::EnumAutomatMode                     nAutomatState_;
    MsgsSimToClient::ForceRatio_Value           nForceRatioState_;
    Common::EnumMeetingEngagementStatus         nCloseCombatState_;
    Common::EnumOperationalStatus               nOperationalState_;
    MsgsSimToClient::RulesOfEngagement_Value    nRoe_;

    kernel::Automat_ABC* pTC2_;
    kernel::Automat_ABC* pLogMaintenance_;
    kernel::Automat_ABC* pLogMedical_;
    kernel::Automat_ABC* pLogSupply_;

    std::auto_ptr< AutomatOrder > order_;

    DecisionalState decisionalInfos_;

    tools::Resolver< kernel::Agent_ABC >   agents_;
    tools::Resolver< kernel::Automat_ABC > childAutomats_;
    //@}
};

}

#endif // __Automat_h_
