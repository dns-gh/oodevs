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

#include "game_asn/Simulation.h"
#include "clients_kernel/Automat_ABC.h"
#include "SimpleEntity.h"
#include "DecisionalState.h"

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
             Automat( Model_ABC& model, const ASN1T_MsgAutomatCreation& msg );
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
    void Update( const ASN1T_MsgAutomatCreation&             msg );
    void Update( const ASN1T_MsgDecisionalState&             msg );
    void Update( const ASN1T_MsgAutomatAttributes&           msg );
    void Update( const ASN1T_MsgLogSupplyQuotas&             msg );
    void Update( const ASN1T_MsgAutomatChangeLogisticLinks&  msg );
    void Update( const ASN1T_MsgAutomatChangeSuperior&       msg );
    void Update( const ASN1T_MsgAutomatChangeKnowledgeGroup& msg );
    void Update( const ASN1T_MsgAutomatOrder&                msg );

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

    ASN1T_EnumAutomatMode             nAutomatState_;
    ASN1T_EnumForceRatioStatus        nForceRatioState_;
    ASN1T_EnumMeetingEngagementStatus nCloseCombatState_;
    ASN1T_EnumOperationalStatus       nOperationalState_;
    ASN1T_EnumRoe                     nRoe_;

    kernel::Automat_ABC* pTC2_;
    kernel::Automat_ABC* pLogMaintenance_;
    kernel::Automat_ABC* pLogMedical_;
    kernel::Automat_ABC* pLogSupply_;

    std::auto_ptr< AutomatOrder > order_;

    DecisionalState decisionalInfos_;

    tools::Resolver< kernel::Agent_ABC >   agents_;
    tools::Resolver< kernel::Automat_ABC > automats_;
    //@}
};

}

#endif // __Automat_h_
