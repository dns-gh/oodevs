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

#include "Automat_ABC.h"
#include "DecisionalState.h"
#include "LogisticEntity.h"

namespace Common
{
    enum EnumAutomatMode;
    enum EnumMeetingEngagementStatus;
    enum EnumOperationalStatus;
    class MsgChangeLogisticLinks;
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
    class MsgAutomatAttributes;
}

namespace dispatcher
{
    class AutomatOrder;
    class DotationQuota;
    class Formation_ABC;
    class KnowledgeGroup_ABC;
    class Model_ABC;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: NLD 2006-09-19
// =============================================================================
class Automat : public Automat_ABC
              , public kernel::Extension_ABC
              , public kernel::Updatable_ABC< MsgsSimToClient::MsgAutomatCreation >
              , public kernel::Updatable_ABC< MsgsSimToClient::MsgDecisionalState >
              , public kernel::Updatable_ABC< MsgsSimToClient::MsgAutomatAttributes >
              , public kernel::Updatable_ABC< Common::MsgChangeLogisticLinks >
              , public kernel::Updatable_ABC< Common::MsgAutomatChangeSuperior >
              , public kernel::Updatable_ABC< Common::MsgAutomatChangeKnowledgeGroup >
              , public kernel::Updatable_ABC< Common::MsgAutomatOrder >
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
    virtual bool IsEngaged() const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgAutomatCreation&    msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgDecisionalState&    msg );
    virtual void DoUpdate( const MsgsSimToClient::MsgAutomatAttributes&  msg );
    virtual void DoUpdate( const Common::MsgChangeLogisticLinks&         msg );
    virtual void DoUpdate( const Common::MsgAutomatChangeSuperior&       msg );
    virtual void DoUpdate( const Common::MsgAutomatChangeKnowledgeGroup& msg );
    virtual void DoUpdate( const Common::MsgAutomatOrder&                msg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual dispatcher::Team_ABC& GetTeam() const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    virtual void Register( dispatcher::Automat_ABC& automat );
    virtual void Remove( dispatcher::Automat_ABC& automat );
    virtual const tools::Resolver< dispatcher::Automat_ABC >& GetAutomats() const;
    virtual void Register( dispatcher::Agent_ABC& automat );
    virtual void Remove( dispatcher::Agent_ABC& automat );
    virtual const tools::Resolver< dispatcher::Agent_ABC >& GetAgents() const;
    virtual dispatcher::Automat_ABC* GetParentAutomat() const;
    virtual dispatcher::Formation_ABC* GetFormation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ChangeKnowledgeGroup( unsigned long id );
    template< typename Message >
    void ChangeSuperior( const Message& superior );
    virtual void SetSuperior( dispatcher::Formation_ABC& superior );
    virtual void SetSuperior( dispatcher::Automat_ABC& superior );
    void ResetSuperior();
    template< typename Superior >
    void MoveChildren( Superior& superior );
    void MoveAgents( dispatcher::Automat_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    const unsigned long type_; // XML reference - no resolved by dispatcher
    dispatcher::Team_ABC& team_;
    dispatcher::Formation_ABC* parentFormation_;
    dispatcher::Automat_ABC* parentAutomat_;
    dispatcher::KnowledgeGroup_ABC* knowledgeGroup_;
    Common::EnumAutomatMode nAutomatState_;
    MsgsSimToClient::ForceRatio_Value nForceRatioState_;
    Common::EnumMeetingEngagementStatus nCloseCombatState_;
    Common::EnumOperationalStatus nOperationalState_;
    MsgsSimToClient::RulesOfEngagement_Value nRoe_;
    kernel::Automat_ABC* pTC2_;
    LogisticEntity       logisticEntity_;
    std::auto_ptr< AutomatOrder > order_;
    DecisionalState decisionalInfos_;
    tools::Resolver< dispatcher::Agent_ABC >   agents_;
    tools::Resolver< dispatcher::Automat_ABC > automats_;
    std::map< std::string, std::string > extensions_;
    std::string symbol_;
    //@}
};

}

#endif // __Automat_h_
