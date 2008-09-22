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
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Automat_ABC.h"
#include "SimpleEntity.h"
#include "DecisionalState.h"

namespace dispatcher
{
    class Model;
    class DotationQuota;
    class AutomatOrder;
    class ModelVisitor_ABC;
    class Formation;
    class Agent;
    class Automat;
    class KnowledgeGroup;
    class Side;

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
             Automat( Model& model, const ASN1T_MsgAutomatCreation& msg );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::AutomatType& GetType() const;
    bool IsEngaged() const;
    void Accept( ModelVisitor_ABC& visitor ) const;
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

public:
    //! @name Member data
    //@{
    Model&              model_;
    const unsigned long type_; // XML reference - no resolved by dispatcher
    const std::string   name_;
    Side&               team_;
    Formation*          parentFormation_;
    Automat*            parentAutomat_;
    KnowledgeGroup*     knowledgeGroup_;
    kernel::Resolver< DotationQuota > quotas_;

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

    kernel::Resolver< Agent >   agents_;
    kernel::Resolver< Automat > automats_;
    //@}
};

}

#endif // __Automat_h_
