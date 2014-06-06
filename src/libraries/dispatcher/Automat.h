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
#include "LogisticHierarchy.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    enum EnumAutomatMode;
    enum EnumMeetingEngagementStatus;
    enum EnumOperationalStatus;
    class ChangeLogisticLinks;
    class AutomatChangeSuperior;
    class AutomatChangeKnowledgeGroup;
    class AutomatOrder;
    enum ForceRatio_Value;
    enum RulesOfEngagement_Value;
    class AutomatCreation;
    class DecisionalState;
    class AutomatAttributes;
    class RgbColor;
}

namespace dispatcher
{
    class AutomatOrder;
    class DotationQuota;
    class Formation_ABC;
    class KnowledgeGroup_ABC;
    class Model_ABC;
    class LogisticEntity;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: NLD 2006-09-19
// =============================================================================
class Automat : public Automat_ABC
              , public kernel::Extension_ABC
              , public kernel::Updatable_ABC< sword::AutomatCreation >
              , public kernel::Updatable_ABC< sword::DecisionalState >
              , public kernel::Updatable_ABC< sword::AutomatAttributes >
              , public kernel::Updatable_ABC< sword::AutomatChangeSuperior >
              , public kernel::Updatable_ABC< sword::AutomatChangeKnowledgeGroup >
              , public kernel::Updatable_ABC< sword::AutomatOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( Model_ABC& model, const sword::AutomatCreation& msg, const tools::Resolver_ABC< kernel::AutomatType >& types );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::AutomatType& GetType() const;
    virtual bool IsEngaged() const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    virtual LogisticEntity_ABC* GetLogisticEntity() const;
    virtual bool GetExtension( const std::string& key, std::string& result ) const;
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::AutomatCreation&    msg );
    virtual void DoUpdate( const sword::DecisionalState&    msg );
    virtual void DoUpdate( const sword::AutomatAttributes&  msg );
    virtual void DoUpdate( const sword::AutomatChangeSuperior&       msg );
    virtual void DoUpdate( const sword::AutomatChangeKnowledgeGroup& msg );
    virtual void DoUpdate( const sword::AutomatOrder&                msg );

    virtual void Send( sword::ParentEntity& msg ) const;

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual dispatcher::Team_ABC& GetTeam() const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    virtual void Register( dispatcher::Automat_ABC& automat );
    virtual void Remove( dispatcher::Automat_ABC& automat );
    virtual void Register( dispatcher::Agent_ABC& automat );
    virtual void Remove( dispatcher::Agent_ABC& automat );
    virtual const tools::Resolver< dispatcher::Agent_ABC >& GetAgents() const;
    virtual dispatcher::Automat_ABC* GetParentAutomat() const;
    virtual dispatcher::Formation_ABC* GetFormation() const;
    virtual const std::string& GetApp6Symbol() const;
    //@}

private:
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
    const kernel::AutomatType& type_;
    dispatcher::Team_ABC& team_;
    dispatcher::Formation_ABC* parentFormation_;
    dispatcher::Automat_ABC* parentAutomat_;
    dispatcher::KnowledgeGroup_ABC* knowledgeGroup_;
    std::unique_ptr< sword::RgbColor > color_;
    sword::EnumAutomatMode nAutomatState_;
    sword::ForceRatio_Value nForceRatioState_;
    sword::EnumMeetingEngagementStatus nCloseCombatState_;
    sword::EnumOperationalStatus nOperationalState_;
    sword::RulesOfEngagement_Value nRoe_;
    std::unique_ptr< LogisticEntity > logisticEntity_;
    LogisticHierarchy logisticHierarchy_;
    std::unique_ptr< AutomatOrder > order_;
    DecisionalState decisionalInfos_;
    tools::Resolver< dispatcher::Agent_ABC >   agents_;
    tools::Resolver< dispatcher::Automat_ABC > automats_;
    std::map< std::string, std::string > extensions_;
    std::string app6symbol_;
    std::string decisionalModel_;
    bool brainDebug_;
    bool logMaintenanceManual_;
    bool logSupplyManual_;
    //@}
};

}

#endif // __Automat_h_
