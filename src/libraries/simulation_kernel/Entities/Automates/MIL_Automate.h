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

#include "Entities/Actions/PHY_Actor.h"
#include "Entities/Orders/Automate/MIL_AutomateOrderManager.h"

class MIL_Formation;
class MIL_AutomateType;
class MIL_AgentPion;
class MIL_Army;
class MIL_KnowledgeGroup;
class MIL_Fuseau;
class MIL_AutomateLOG;
class MIL_CampPrisonniers;
class MIL_CampRefugies;
class MIL_AgentTypePion;
class MIL_LimaFunction;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_AutomateDecision;
class PHY_SupplyDotationState;
class PHY_DotationCategory;
class NET_ASN_MsgAutomateCreation;
class DEC_KnowledgeBlackBoard_Automate;


// =============================================================================
// @class  MIL_Automate
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Automate : public PHY_Actor
{
    MT_COPYNOTALLOWED( MIL_Automate )

public:
    //! @name Types
    //@{
    typedef std::vector< MIL_AgentPion* >  T_PionVector;
    typedef T_PionVector::iterator         IT_PionVector;
    typedef T_PionVector::const_iterator   CIT_PionVector;
    typedef T_PionVector::reverse_iterator RIT_PionVector;

    typedef std::map< const MIL_AutomateLOG*, PHY_SupplyDotationState* > T_SupplyDotationStateMap;
    typedef T_SupplyDotationStateMap::iterator                           IT_SupplyDotationStateMap;
    typedef T_SupplyDotationStateMap::const_iterator                     CIT_SupplyDotationStateMap;
    //@}

public:
             MIL_Automate( const MIL_AutomateType& type, uint nID, MIL_Formation& formation, MIL_InputArchive& archive );
             MIL_Automate();
    virtual ~MIL_Automate();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

            void WriteODB             ( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteLogisticLinksODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Initialize
    //@{
            void ReadOverloading ( MIL_InputArchive& archive );
    virtual void ReadLogisticLink( MIL_AutomateLOG& superior, MIL_InputArchive& archive );
    //@}

    //! @name Accessors
    //@{
    const std::string&                      GetName          () const;
          uint                              GetID            () const;
    const MIL_AutomateType&                 GetType          () const;
          MIL_Army&                         GetArmy          () const;
          MIL_KnowledgeGroup&               GetKnowledgeGroup() const;
          MIL_AutomateLOG*                  GetTC2           () const;
    const MIL_AutomateOrderManager&         GetOrderManager  () const;
          MIL_AutomateOrderManager&         GetOrderManager  ();
          MIL_AgentPion&                    GetPionPC        () const;
    const T_PionVector&                     GetPions         () const; // Including pion PC
          DEC_AutomateDecision&             GetDecision      () const;
          DEC_KnowledgeBlackBoard_Automate& GetKnowledge     () const;
          bool                              IsEmbraye        () const;

    const MIL_Fuseau&                       GetFuseau        () const;
    const MT_Vector2D&                      GetDirDanger     () const;
          MIL_LimaOrder*                    FindLima         ( const MIL_LimaFunction& function ) const;
          MIL_LimaOrder*                    FindLima         ( uint nID ) const;
    //@}
        
    //! @name Operations
    //@{
    bool CheckComposition() const;

    void RegisterPion  ( MIL_AgentPion& pion );
    void UnregisterPion( MIL_AgentPion& pion );

            void UpdateDecision  ();
            void UpdateKnowledges();
            void CleanKnowledges ();
    virtual void UpdateNetwork   () const;
    virtual void UpdateState     ();
    virtual void Clean           ();

    bool IsPerceived ( const DEC_Knowledge_Agent&  knowledge ) const;
    bool IsPerceived ( const DEC_Knowledge_Object& knowledge ) const;
    //@}

    //! @name Prisoners
    //@{
          bool                 IsSurrendered            () const;
          bool                 IsPrisoner               () const;
    const MIL_CampPrisonniers* GetPrisonerCamp          () const;
          bool                 TakePrisoner             ( const MIL_AgentPion& pionTakingPrisoner, const MIL_CampPrisonniers& camp );
          void                 NotifyInsidePrisonerCamp ( const MIL_CampPrisonniers& camp );
          void                 NotifyOutsidePrisonerCamp( const MIL_CampPrisonniers& camp );
    //@}

    //! @name Refugees
    //@{
    const MIL_CampRefugies* GetRefugeeCamp                  () const;
          bool              OrientateRefugee                ( const MIL_CampRefugies& camp );
          void              NotifyRefugeeManagedStateChanged( bool bManaged );
          void              NotifyInsideRefugeeCamp         ( const MIL_CampRefugies& camp );
          void              NotifyOutsideRefugeeCamp        ( const MIL_CampRefugies& camp );
    //@}

    //! @name Network
    //@{
            void SendCreation                     () const;
    virtual void SendFullState                    () const;
            void SendKnowledge                    () const;

            void OnReceiveMsgAutomateOrder        ( ASN1T_MsgAutomateOrder&                 msg, MIL_MOSContextID nCtx );
            void OnReceiveMsgSetAutomateMode      ( ASN1T_MsgSetAutomateMode&               msg, MIL_MOSContextID nCtx );
            void OnReceiveMsgOrderConduite        ( ASN1T_MsgOrderConduite&                 msg, MIL_MOSContextID nCtx );
            void OnReceiveMsgUnitMagicAction      ( ASN1T_MsgUnitMagicAction&               msg, MIL_MOSContextID nCtx );
            void OnReceiveMsgChangeKnowledgeGroup ( ASN1T_MsgChangeGroupeConnaissance&      msg, MIL_MOSContextID nCtx );
    virtual void OnReceiveMsgChangeLogisticLinks  ( ASN1T_MsgChangeLiensLogistiques&        msg, MIL_MOSContextID nCtx );
    virtual void OnReceiveMsgLogSupplyChangeQuotas( ASN1T_MsgLogRavitaillementChangeQuotas& msg, MIL_MOSContextID nCtx );
    virtual void OnReceiveMsgLogSupplyPushFlow    ( ASN1T_MsgLogRavitaillementPousserFlux&  msg, MIL_MOSContextID nCtx );
    //@}

    //! @name Misc
    //@{
    bool GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const;
    //@}

    //! @name Dynamic pions
    //@{
    MIL_AgentPion& CreatePion ( const MIL_AgentTypePion& type, const MT_Vector2D& vPosition );
    void           DestroyPion( MIL_AgentPion& pion );
    //@}

    //! @name Logistic : supply
    //@{
    void NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory );
    void NotifyDotationSupplied    ( const PHY_SupplyDotationState& supplyState );
    void RequestDotationSupply     ();
    //@}

    //! @name Tools
    //@{
    void Embraye();
    void Debraye();
    //@}

protected:
    //! @name Tools
    //@{
    virtual void SendLogisticLinks() const;
            void Surrender        ();
    //@}

private:
    //! @name Tools
    //@{
    void InitializeSubordinates( MIL_InputArchive& archive );
    //@}

protected:
    // Logistic
    MIL_AutomateLOG* pTC2_;
    MIL_AutomateLOG* pNominalTC2_;
    
private:
    const MIL_AutomateType* pType_;
    const uint              nID_;
          MIL_Formation*    pFormation_;
          std::string       strName_;
          bool              bEmbraye_;

    MIL_KnowledgeGroup*      pKnowledgeGroup_;
    DEC_AutomateDecision*    pDecision_; 
    MIL_AutomateOrderManager orderManager_;
    MIL_AgentPion*           pPionPC_;
    T_PionVector             pions_; // Including pion PC
    T_PionVector             recycledPions_; // Dynamic pions

    bool                     bAutomateModeChanged_;

    // Logistic : supply
    bool                     bDotationSupplyNeeded_;
    bool                     bDotationSupplyExplicitlyRequested_;
    T_SupplyDotationStateMap dotationSupplyStates_;

    uint                     nTickRcDotationSupplyQuerySent_;

    // Knowledge
    DEC_KnowledgeBlackBoard_Automate* pKnowledgeBlackBoard_;

    // Surrendered / prisoner
          bool                  bSurrendered_;
          bool                  bPrisoner_;
    const MIL_CampPrisonniers*  pPrisonerCamp_;

    // Refugees
    const MIL_CampRefugies*     pRefugeeCamp_;
};

#include "MIL_Automate.inl"

#endif // __MIL_Automate_h_

