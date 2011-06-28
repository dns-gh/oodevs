// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 17 $
// $Workfile: MIL_Automate.h $
//
// *****************************************************************************

#ifndef __MIL_Automate_h_
#define __MIL_Automate_h_

#include "MIL.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/MIL_VisitableEntity_ABC.h"
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
    class AutomatOrder;
    class MissionParameters;
    class FragOrder;
    class SetAutomatMode;
    class UnitCreationRequest;
    class UnitMagicAction;
}

namespace xml
{
    class xostream;
    class xistream;
}

class DEC_AutomateDecision;
class DEC_Knowledge_Agent;
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
class MIL_AgentTypePion;
class MIL_AutomateOrderManager;
class PHY_SupplyDotationState;
class PHY_SupplyStockState;
class PHY_DotationCategory;
class MIL_DotationSupplyManager;
class MIL_StockSupplyManager;
class MIL_DotationSupplyManager;
class MIL_StockSupplyManager;
class MIL_Color;
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
    typedef std::vector< MIL_AgentPion* >  T_PionVector;
    typedef T_PionVector::iterator         IT_PionVector;
    typedef T_PionVector::const_iterator   CIT_PionVector;
    typedef T_PionVector::reverse_iterator RIT_PionVector;

    typedef std::vector< MIL_Automate* >     T_AutomateVector;
    typedef T_AutomateVector::iterator       IT_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;

    typedef std::map< const MIL_AutomateLOG*, PHY_SupplyDotationState* > T_SupplyDotationStateMap;
    typedef T_SupplyDotationStateMap::iterator                           IT_SupplyDotationStateMap;
    typedef T_SupplyDotationStateMap::const_iterator                     CIT_SupplyDotationStateMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MIL_Entity_ABC& parent, xml::xistream& xis, unsigned int gcPause, unsigned int gcMult );
             MIL_Automate( const MIL_AutomateType& type, unsigned int nID, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context );
    virtual ~MIL_Automate();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
            void WriteODB             ( xml::xostream& xos ) const;
    virtual void WriteLogisticLinksODB( xml::xostream& xos ) const;
    //@}

    //! @name Initialize
    //@{
    void ReadOverloading ( xml::xistream& xis );
    void ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
          unsigned int                      GetID            () const;
    const MIL_AutomateType&                 GetType          () const;
          MIL_Army_ABC&                     GetArmy          () const;
          MIL_KnowledgeGroup&               GetKnowledgeGroup() const;
    const MIL_AutomateOrderManager&         GetOrderManager  () const;
          MIL_AutomateOrderManager&         GetOrderManager  ();
          MIL_AgentPion&                    GetPionPC        () const;
    const T_PionVector&                     GetPions         () const; // Including pion PC
    const T_AutomateVector&                 GetAutomates     () const;
          MIL_Automate*                     GetParentAutomate() const;
    const DEC_AutomateDecision&             GetDecision      () const;
          DEC_AutomateDecision&             GetDecision      () ;
          DEC_KnowledgeBlackBoard_Automate& GetKnowledge     () const;
          bool                              IsEngaged        () const;

          logistic::LogisticHierarchy_ABC&  GetLogisticHierarchy() const;

    // logistics
    MIL_AutomateLOG*                        GetBrainLogistic () const;
    MIL_AutomateLOG*                        FindLogisticManager() const; // Returns logistic chief
    const MIL_Color& GetColor() const;
    //@}

    //! @name Visitor
    //@{
    virtual void Apply( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor ) const;
    //@}

    //! @name Operations
    //@{
    bool CheckComposition() const;
    void RegisterPion  ( MIL_AgentPion& pion, bool registerPC = true );
    void UnregisterPion( MIL_AgentPion& pion );
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );
            void UpdateDecision  ( float duration );
            void UpdateKnowledges( int currentTimeStep );
            void CleanKnowledges ();
    virtual void UpdateNetwork   () const;
    virtual void UpdateState     ();
    virtual void Clean           ();

    template< typename T > void ApplyOnHierarchy( T& functor )
    {
        functor(*this);
        for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
            (**it).ApplyOnHierarchy( functor );
        for( CIT_PionVector it = pions_.begin(); it != pions_.end(); ++it )
            functor( **it );
    }

    bool IsPerceived( const DEC_Knowledge_Agent&  knowledge ) const;
    bool IsPerceived( const DEC_Knowledge_Object& knowledge ) const;
    //@}

    //! @name Prisoners
    //@{
          bool      IsSurrendered           () const;
    const MIL_Army_ABC* GetArmySurrenderedTo() const;
          bool      NotifyCaptured          ( const MIL_AgentPion& pionTakingPrisoner );
          bool      NotifyReleased          ();
          bool      NotifyImprisoned        ( const MIL_Object_ABC& camp );
    //@}

    //! @name Refugees $$$$ A revoir
    //@{
    void NotifyRefugeeOriented( const MIL_AgentPion& pionManaging );
    void NotifyRefugeeReleased();
    void NotifyRefugeeReleased( const MIL_Object_ABC& camp );
    //@}

    //! @name Network
    //@{
            void SendCreation                      ( unsigned int context = 0 ) const;
    virtual void SendFullState                     () const;
            void SendKnowledge                     () const;

            void OnReceiveOrder                ( const sword::AutomatOrder&                 msg );
            void OnReceiveFragOrder            ( const sword::FragOrder&           msg );
            void OnReceiveSetAutomateMode      ( const sword::SetAutomatMode&      msg );
            void OnReceiveUnitCreationRequest  ( const sword::UnitCreationRequest& msg );
            void OnReceiveUnitCreationRequest  ( const sword::UnitMagicAction&     msg );
            void OnReceiveUnitMagicAction      ( const sword::UnitMagicAction&     msg, const tools::Resolver< MIL_Army_ABC >& armies );
            void OnReceiveMagicActionMoveTo    ( const sword::UnitMagicAction&     msg );
            void OnReceiveChangeKnowledgeGroup ( const sword::UnitMagicAction&     msg, const tools::Resolver< MIL_Army_ABC >& armies );
            void OnReceiveChangeSuperior       ( const sword::UnitMagicAction&     msg, const tools::Resolver< MIL_Formation >& formations );
    virtual void OnReceiveLogSupplyPushFlow    ( const sword::MissionParameters&            msg );
    virtual void OnReceiveLogSupplyPullFlow    ( const sword::MissionParameters&            msg );

    virtual void Serialize( sword::ParentEntity& message ) const;
    //@}

    //! @name Misc
    //@{
    bool   GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const;
    double GetAlivePionsMaxSpeed  () const;
    //@}

    //! @name Dynamic pions
    //@{
    MIL_AgentPion& CreatePion ( const MIL_AgentTypePion& type, const MT_Vector2D& vPosition );
    void           DestroyPion( MIL_AgentPion& pion );
    //@}

    //! @name Logistic : supply
    //@{
    virtual void NotifyQuotaThresholdReached( const PHY_DotationCategory& dotationCategory ) const;

    void NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void NotifyDotationSupplied    ( const PHY_SupplyDotationState& supplyState );
    void RequestDotationSupply     ();

    void     NotifyStockSupplyNeeded   ( const PHY_DotationCategory& dotationCategory );
    void     NotifyStockSupplied       ( const PHY_SupplyStockState& supplyState );
    void     NotifyStockSupplyCanceled ( const PHY_SupplyStockState& supplyState );

    double   GetQuota                  ( const MIL_AutomateLOG& supplier, const PHY_DotationCategory& dotationCategory ) const;
    void     ConsumeQuota              ( const MIL_AutomateLOG& supplier, const PHY_DotationCategory& dotationCategory, double rQuotaConsumed );
    //@}

    //! @name Tools
    //@{
    void Engage();
    void Disengage();
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Automate( const MIL_AutomateType& type, unsigned int nID = 0 );
    //@}

    //! @name Tools
    //@{
    void Surrender      ( const MIL_Army_ABC& amrySurrenderedTo );
    void CancelSurrender();
    //@}

private:
    //! @name Tools
    //@{
    void Initialize( xml::xistream& xis, unsigned int gcPause, unsigned int gcMult );
    //@}

    //! @name Helpers
    //@{
    void ReadAutomatSubordinate( xml::xistream& xis );
    void ReadUnitSubordinate( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AutomateType*                                    pType_;
    const unsigned int                                         nID_;
    MIL_Formation*                                             pParentFormation_;
    MIL_Automate*                                              pParentAutomate_;
    bool                                                       bEngaged_;
    MIL_KnowledgeGroup*                                        pKnowledgeGroup_;
    MIL_AutomateOrderManager*                                  pOrderManager_;
    MIL_AgentPion*                                             pPionPC_;
    T_PionVector                                               pions_; // Including pion PC
    T_PionVector                                               recycledPions_; // Dynamic pions
    T_AutomateVector                                           automates_;
    bool                                                       bAutomateModeChanged_;
    unsigned int                                               nTickRcDotationSupplyQuerySent_;
    // Knowledge
    DEC_KnowledgeBlackBoard_Automate*                          pKnowledgeBlackBoard_;
    // Surrendered / prisoner
    const MIL_Army_ABC*                                        pArmySurrenderedTo_;
    // Logistic
    std::auto_ptr< logistic::LogisticHierarchy >               pLogisticHierarchy_;
    std::auto_ptr< MIL_AutomateLOG >                           pBrainLogistic_;
    boost::shared_ptr< PHY_ActionLogistic< MIL_AutomateLOG > > pLogisticAction_;
    std::auto_ptr< MIL_DotationSupplyManager >                 pDotationSupplyManager_;
    std::auto_ptr< MIL_StockSupplyManager >                    pStockSupplyManager_;
    std::auto_ptr< MIL_DictionaryExtensions >                  pExtensions_;
    std::auto_ptr< MIL_Color >                                 pColor_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Automate* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Automate* role, const unsigned int /*version*/ );

};

BOOST_CLASS_EXPORT_KEY( MIL_Automate )

#endif // __MIL_Automate_h_
