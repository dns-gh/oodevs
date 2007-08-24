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

#include "game_asn/Asn.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"
#include "Entity_ABC.h"
#include "DecisionalState.h"

namespace dispatcher
{
class Dispatcher;
class Side;
class KnowledgeGroup;
class Model;
class Formation;
class Agent;
class DotationQuota;
class AutomatOrder;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: NLD 2006-09-19
// =============================================================================
class Automat : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( Model& model, const ASN1T_MsgAutomatCreation& msg );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    unsigned long                GetID    () const;
    ModelRefsContainer< Agent >& GetAgents();
    bool IsEngaged() const;
    //@}

    //! @name Main
    //@{
    void Update        ( const ASN1T_MsgAutomatCreation&                msg );
    void Update        ( const ASN1T_MsgDecisionalState&                msg );
    void Update        ( const ASN1T_MsgAutomatAttributes&              msg );
    void Update        ( const ASN1T_MsgLogSupplyQuotas&                msg );
    void Update        ( const ASN1T_MsgAutomatChangeLogisticLinksAck&  msg );
    void Update        ( const ASN1T_MsgAutomatChangeLogisticLinks&     msg );
    void Update        ( const ASN1T_MsgAutomatChangeKnowledgeGroupAck& msg );
    void Update        ( const ASN1T_MsgAutomatOrder&                   msg );
    virtual void SendCreation  ( ClientPublisher_ABC& publisher ) const;
    virtual void SendSpecialUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;

    void Accept( ModelVisitor_ABC& visitor );
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

private:
          Model&                              model_;
    const unsigned long                       nID_;
    const unsigned long                       nType_; // XML reference - no resolved by dispatcher
    const std::string                         strName_;
          Side&                               side_;
          Formation&                          formation_;
          KnowledgeGroup*                     pKnowledgeGroup_;
          ModelRefsContainer< Agent >         agents_;
          ModelsContainer   < DotationQuota > quotas_;

    ASN1T_EnumAutomatMode             nAutomatState_;
    ASN1T_EnumForceRatioStatus        nForceRatioState_;
    ASN1T_EnumMeetingEngagementStatus nCloseCombatState_;
    ASN1T_EnumOperationalStatus        nOperationalState_;
    ASN1T_EnumRoe                     nRoe_;

    Automat* pTC2_;
    Automat* pLogMaintenance_;
    Automat* pLogMedical_;
    Automat* pLogSupply_;

    AutomatOrder* pOrder_;

    DecisionalState decisionalInfos_;
};

}

#include "Automat.inl"

#endif // __Automat_h_
