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
#include "Entities/Orders/MIL_AutomateOrderManager.h"

namespace xml
{
    class xostream;
    class xistream;
}

class MIL_Formation;
class MIL_AutomateType;
class MIL_AgentPion;
class MIL_Army_ABC;
class MIL_Army;
class MIL_KnowledgeGroup;
class MIL_Fuseau;
class MIL_AutomateLOG;
class MIL_Object_ABC;
class MIL_AgentTypePion;
class MIL_LimaFunction;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_AutomateDecision;
class PHY_SupplyDotationState;
class PHY_DotationCategory;
class NET_ASN_MsgAutomatCreation;
class DEC_KnowledgeBlackBoard_Automate;

// =============================================================================
// @class  MIL_Automate
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Automate : public MIL_Entity_ABC
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
             MIL_Automate( const MIL_AutomateType& type, uint nID, MIL_Formation& formation, xml::xistream& xis );
             MIL_Automate( const MIL_AutomateType& type, uint nID, MIL_Automate&  parent   , xml::xistream& xis);
             MIL_Automate();
    virtual ~MIL_Automate();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

            void WriteODB             ( xml::xostream& xos ) const;
    virtual void WriteLogisticLinksODB( xml::xostream& xos ) const;
    //@}

    //! @name Initialize
    //@{
            void ReadOverloading ( xml::xistream& xis );
    virtual void ReadLogisticLink( MIL_AutomateLOG& superior, xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
          uint                              GetID            () const;
    const MIL_AutomateType&                 GetType          () const;
          MIL_Army_ABC&                     GetArmy          () const;
          MIL_KnowledgeGroup&               GetKnowledgeGroup() const;
          MIL_AutomateLOG*                  GetTC2           () const;
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
    //@}
        
    //! @name Operations
    //@{
    bool CheckComposition() const;

    void RegisterPion  ( MIL_AgentPion& pion );
    void UnregisterPion( MIL_AgentPion& pion );

    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );

            void UpdateDecision  ( float duration );
            void UpdateKnowledges();
            void CleanKnowledges ();
    virtual void UpdateNetwork   () const;
    virtual void UpdateState     ();
    virtual void Clean           ();
    
    template< typename T > void ApplyOnHierarchy( T& functor );

    bool IsPerceived ( const DEC_Knowledge_Agent&  knowledge ) const;
    bool IsPerceived ( const DEC_Knowledge_Object& knowledge ) const;
    //@}

    //! @name Prisoners
    //@{
          bool      IsSurrendered       () const;
    const MIL_Army* GetArmySurrenderedTo() const;
          bool      NotifyCaptured      ( const MIL_AgentPion& pionTakingPrisoner );
          bool      NotifyReleased      ();
          bool      NotifyImprisoned    ( const MIL_Object_ABC& camp );
    //@}

    //! @name Refugees $$$$ A revoir
    //@{
    void NotifyRefugeeOriented( const MIL_AgentPion& pionManaging );
    void NotifyRefugeeReleased();
    void NotifyRefugeeReleased( const MIL_Object_ABC& camp );
    //@}

    //! @name Network
    //@{
            void SendCreation                     () const;
    virtual void SendFullState                    () const;
            void SendKnowledge                    () const;

            void OnReceiveMsgOrder                ( const ASN1T_MsgAutomatOrder&                msg );
            void OnReceiveMsgFragOrder            ( const ASN1T_MsgFragOrder&                   msg );
            void OnReceiveMsgSetAutomateMode      ( const ASN1T_MsgSetAutomatMode&              msg );
            void OnReceiveMsgUnitCreationRequest  ( const ASN1T_MsgUnitCreationRequest&         msg );
            void OnReceiveMsgUnitMagicAction      ( const ASN1T_MsgUnitMagicAction&             msg );
            void OnReceiveMsgChangeKnowledgeGroup ( const ASN1T_MsgAutomatChangeKnowledgeGroup& msg );
            void OnReceiveMsgChangeSuperior       ( const ASN1T_MsgAutomatChangeSuperior&       msg );
    virtual void OnReceiveMsgChangeLogisticLinks  ( const ASN1T_MsgAutomatChangeLogisticLinks&  msg );
    virtual void OnReceiveMsgLogSupplyChangeQuotas( const ASN1T_MsgLogSupplyChangeQuotas&       msg );
    virtual void OnReceiveMsgLogSupplyPushFlow    ( const ASN1T_MsgLogSupplyPushFlow&           msg );
    //@}

    //! @name Misc
    //@{
    bool     GetAlivePionsBarycenter( MT_Vector2D& barycenter ) const;
    MT_Float GetAlivePionsMaxSpeed  () const;
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
    void Engage   ();
    void Disengage();
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Automate( const MIL_AutomateType& type );
    //@}

    //! @name Tools
    //@{
    virtual void             SendLogisticLinks() const;
            void             Surrender        ( const MIL_Army& amrySurrenderedTo );
            void             CancelSurrender  ();
            MIL_AutomateLOG* GetNominalTC2    () const;
    //@}

private:
    //! @name Tools
    //@{
    void Initialize( xml::xistream& xis ); 
    //@}
    
    //! @name Helpers
    //@{
    void ReadAutomatSubordinate( xml::xistream& xis ); 
    void ReadUnitSubordinate   ( xml::xistream& xis );         
    //@}

protected:
    // Logistic
    MIL_AutomateLOG* pTC2_;
    MIL_AutomateLOG* pNominalTC2_;
    
private:
    const MIL_AutomateType* pType_;
    const uint              nID_;
          MIL_Formation*    pParentFormation_;
          MIL_Automate*     pParentAutomate_;
          bool              bEngaged_;

    MIL_KnowledgeGroup*      pKnowledgeGroup_;
    MIL_AutomateOrderManager orderManager_;
    MIL_AgentPion*           pPionPC_;
    T_PionVector             pions_; // Including pion PC
    T_PionVector             recycledPions_; // Dynamic pions
    T_AutomateVector         automates_;

    bool                     bAutomateModeChanged_;

    // Logistic : supply
    bool                     bDotationSupplyNeeded_;
    bool                     bDotationSupplyExplicitlyRequested_;
    T_SupplyDotationStateMap dotationSupplyStates_;

    uint                     nTickRcDotationSupplyQuerySent_;

    // Knowledge
    DEC_KnowledgeBlackBoard_Automate* pKnowledgeBlackBoard_;

    // Surrendered / prisoner
    const MIL_Army*             pArmySurrenderedTo_;
};

#include "MIL_Automate.inl"

#endif // __MIL_Automate_h_


