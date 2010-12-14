 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "Automat_ABC.h"
#include "DecisionalState.h"
#include "LogisticEntity.h"

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
              , public kernel::Updatable_ABC< sword::AutomatCreation >
              , public kernel::Updatable_ABC< sword::DecisionalState >
              , public kernel::Updatable_ABC< sword::AutomatAttributes >
              , public kernel::Updatable_ABC< sword::ChangeLogisticLinks >
              , public kernel::Updatable_ABC< sword::AutomatChangeSuperior >
              , public kernel::Updatable_ABC< sword::AutomatChangeKnowledgeGroup >
              , public kernel::Updatable_ABC< sword::AutomatOrder >
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( Model_ABC& model, const sword::AutomatCreation& msg );
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
    virtual void DoUpdate( const sword::AutomatCreation&    msg );
    virtual void DoUpdate( const sword::DecisionalState&    msg );
    virtual void DoUpdate( const sword::AutomatAttributes&  msg );
    virtual void DoUpdate( const sword::ChangeLogisticLinks&         msg );
    virtual void DoUpdate( const sword::AutomatChangeSuperior&       msg );
    virtual void DoUpdate( const sword::AutomatChangeKnowledgeGroup& msg );
    virtual void DoUpdate( const sword::AutomatOrder&                msg );

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
    sword::EnumAutomatMode nAutomatState_;
    sword::ForceRatio_Value nForceRatioState_;
    sword::EnumMeetingEngagementStatus nCloseCombatState_;
    sword::EnumOperationalStatus nOperationalState_;
    sword::RulesOfEngagement_Value nRoe_;
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
