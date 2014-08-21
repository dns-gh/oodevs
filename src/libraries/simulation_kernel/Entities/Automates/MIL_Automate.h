// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Automate_h_
#define __MIL_Automate_h_

#include "MIL.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/MIL_VisitableEntity_ABC.h"
#include "Entities/Specialisations/LOG/LogisticHierarchyOwner_ABC.h"
#include <tools/Resolver.h>
#include <map>

namespace logistic
{
    class LogisticHierarchy;
    class LogisticHierarchy_ABC;
}

namespace sword
{
    class DEC_Logger;
    class AutomatOrder;
    class MissionParameters;
    class FragOrder;
    class SetAutomatMode;
    class UnitCreationRequest;
    class UnitMagicAction;
    class PullFlowParameters;
}

namespace xml
{
    class xostream;
    class xistream;
}

class DEC_Decision_ABC;
class DEC_KnowledgeBlackBoard_Automate;
class DEC_Knowledge_Object;
class MIL_Formation;
class MIL_AutomateType;
class MIL_AgentPion;
class MIL_Army_ABC;
class MIL_DictionaryExtensions;
class MIL_KnowledgeGroup;
class MIL_AutomateLOG;
class MIL_Object_ABC;
class MIL_AutomateOrderManager;
class PHY_DotationCategory;
class MIL_DotationSupplyManager;
class MIL_StockSupplyManager;
class MIL_Color;
class PHY_Dotation;
class PHY_DotationStock;
class MissionController_ABC;
template < typename T > class PHY_ActionLogistic;

// =============================================================================
// @class  MIL_Automate
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Automate : public MIL_Entity_ABC
                   , public MIL_VisitableEntity_ABC< MIL_AgentPion >
                   , public logistic::LogisticHierarchyOwner_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< MIL_AgentPion* > T_PionVector;
    typedef std::vector< MIL_Automate* >  T_AutomateVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MissionController_ABC& controller, MIL_Entity_ABC& parent, xml::xistream& xis, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger );
             MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MissionController_ABC& controller, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger, unsigned int context, const MIL_DictionaryExtensions& extensions );
    virtual ~MIL_Automate();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    virtual void WriteLogisticLinksODB( xml::xostream& xos ) const;
    //@}

    //! @name Initialize
    //@{
    void ReadOverloading ( xml::xistream& xis );
    void ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis );
    void Finalize();
    //@}

    //! @name Accessors
    //@{
    const MIL_AutomateType& GetType() const;
    MIL_Army_ABC& GetArmy() const;
    boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;
    const MIL_AutomateOrderManager& GetOrderManager() const;
    MIL_AutomateOrderManager& GetOrderManager();
    MIL_AgentPion* GetPionPC() const;
    const T_PionVector& GetPions() const; // Including pion PC
    const T_AutomateVector& GetAutomates() const;
    MIL_Automate* GetParentAutomate() const;
    const DEC_Decision_ABC& GetDecision() const;
    DEC_Decision_ABC& GetDecision() ;
    DEC_KnowledgeBlackBoard_Automate& GetKnowledge() const;
    bool IsEngaged() const;
    const MT_Vector2D& GetPosition () const;
    logistic::LogisticHierarchy_ABC& GetLogisticHierarchy() const;
    virtual bool CanEmitReports() const;
    bool IsMasaLife() const;

    MIL_StockSupplyManager& GetStockSupplyManager() const;
    MIL_DotationSupplyManager& GetDotationSupplyManager() const;

    // logistics
    virtual unsigned int GetLogisticId() const;
    MIL_AutomateLOG* GetBrainLogistic () const;
    MIL_AutomateLOG* FindLogisticManager() const; // Returns logistic chief
    const MIL_Color& GetColor() const;
    //@}

    //! @name Visitor
    //@{
    virtual void Apply( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor ) const;
    virtual void Apply( MIL_EntitiesVisitor_ABC& visitor ) const;
    virtual void Apply( const std::function< void( const MIL_AgentPion& ) >& visitor ) const;
    void Apply2( std::function< void( const MIL_AgentPion&, PHY_Dotation& ) > visitor ) const;
    void Apply2( std::function< void( const MIL_AgentPion&, PHY_DotationStock& ) > visitor ) const;
    //@}

    //! @name Operations
    //@{
    bool CheckComposition() const;
    void RegisterPion( MIL_AgentPion& pion );
    void SetCommandPost( MIL_AgentPion* pion );
    void UnregisterPion( MIL_AgentPion& pion );
    void RegisterAutomate( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );
    void DestroyPion( MIL_AgentPion& pion );
    void UpdateDecision( float duration );
    void UpdateKnowledges( int currentTimeStep );
    void CleanKnowledges();
    void CleanDeletedAgentKnowledges();
    void DeleteRequestsForRequester( MIL_AgentPion& pion );
    virtual void UpdateNetwork() const;
    virtual void UpdateState();
    virtual void Clean();

    template< typename T > void ApplyOnHierarchy( T& functor )
    {
        functor( *this );
        for( auto it = automates_.begin(); it != automates_.end(); ++it )
            ( **it ).ApplyOnHierarchy( functor );
        for( auto it = pions_.begin(); it != pions_.end(); ++it )
            functor( **it );
    }

    bool IsPerceived( const DEC_Knowledge_Object& knowledge ) const;
    //@}

    //! @name Prisoners
    //@{
    bool IsSurrendered() const;
    const MIL_Army_ABC* GetArmySurrenderedTo() const;
    bool NotifyCaptured( const MIL_AgentPion& pionTakingPrisoner );
    bool NotifyReleased();
    bool NotifyImprisoned( const MIL_Object_ABC& camp );
    //@}

    //! @name Refugees $$$$ A revoir
    //@{
    void NotifyRefugeeOriented( const MIL_AgentPion& pionManaging );
    void NotifyRefugeeReleased();
    void NotifyRefugeeReleased( const MIL_Object_ABC& camp );
    //@}

    //! @name Network
    //@{
    void SendCreation( unsigned int context = 0 ) const;
    virtual void SendFullState( unsigned int context = 0 ) const;
    void SendKnowledge( unsigned int context = 0 ) const;

    uint32_t OnReceiveOrder( const sword::AutomatOrder& msg );
    void OnReceiveFragOrder( const sword::FragOrder& msg, const std::function< void( uint32_t ) >& sendAck );
    void OnReceiveSetAutomateMode( const sword::SetAutomatMode& msg );
    void OnReceiveUnitCreationRequest( const sword::UnitCreationRequest& msg, unsigned int nCtx );
    unsigned int OnReceiveUnitCreationRequest( const sword::UnitMagicAction& msg, unsigned int nCtx );
    void OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Army_ABC >& armies );
    void OnReceiveMagicActionMoveTo( const sword::UnitMagicAction& msg );
    void OnReceiveChangeKnowledgeGroup( const sword::UnitMagicAction& msg );
    void OnReceiveChangeSuperior( const sword::UnitMagicAction& msg, const tools::Resolver< MIL_Formation >& formations );
    bool OnReceiveLogSupplyPullFlow( const sword::PullFlowParameters& msg, MIL_AutomateLOG& supplier );
    void OnReloadBrain( const sword::MissionParameters& msg );
    void OnChangeBrainDebug( const sword::MissionParameters& msg );

    virtual void Serialize( sword::ParentEntity& message ) const;
    //@}

    //! @name Misc
    //@{
    bool GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const;
    double GetAlivePionsMaxSpeed  () const;
    //@}

    //! @name Logistic : supply
    //@{
    virtual void NotifyLinkAdded( const logistic::LogisticLink_ABC& link ) const;
    virtual void NotifyLinkRemoved( const logistic::LogisticLink_ABC& link ) const;

    virtual void NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const;
    virtual void NotifyQuotaExceeded( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters ) const;

    void NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void RequestDotationSupply     ();

    void NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    //@}

    //! @name Tools
    //@{
    void SurrenderWithUnits( const MIL_Army_ABC& armySurrenderedTo );
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MissionController_ABC& controller );
    //@}

    //! @name Tools
    //@{
    void Surrender( const MIL_Army_ABC& armySurrenderedTo );
    void CancelSurrender();
    //@}

private:
    //! @name Tools
    //@{
    void Initialize( xml::xistream& xis, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger );

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Automate* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Automate* role, const unsigned int /*version*/ );
    void Engage();
    void Disengage();
    //@}

    //! @name Helpers
    //@{
    void ReadAutomatSubordinate( xml::xistream& xis );
    void ReadUnitSubordinate( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AutomateType* pType_;
    MIL_Formation* pParentFormation_;
    MIL_Automate* pParentAutomate_;
    boost::shared_ptr< MIL_KnowledgeGroup > pKnowledgeGroup_;
    MIL_AutomateOrderManager* pOrderManager_;
    MIL_AgentPion* pPionPC_;
    T_PionVector pions_; // Including pion PC
    T_PionVector pionsToDelete_;
    T_AutomateVector automates_;
    bool bEngaged_;
    bool bAutomateModeChanged_;
    // Knowledge
    DEC_KnowledgeBlackBoard_Automate* pKnowledgeBlackBoard_;
    // Surrendered / prisoner
    const MIL_Army_ABC* pArmySurrenderedTo_;
    // Logistic
    std::unique_ptr< logistic::LogisticHierarchy > pLogisticHierarchy_;
    std::unique_ptr< MIL_AutomateLOG > pBrainLogistic_;
    boost::shared_ptr< PHY_ActionLogistic< MIL_AutomateLOG > > pLogisticAction_;
    std::unique_ptr< MIL_DotationSupplyManager > pDotationSupplyManager_;
    std::unique_ptr< MIL_StockSupplyManager > pStockSupplyManager_;
    std::unique_ptr< MIL_DictionaryExtensions > pExtensions_;
    std::unique_ptr< MIL_Color > pColor_;
    std::string symbol_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Automate )

#endif // __MIL_Automate_h_
