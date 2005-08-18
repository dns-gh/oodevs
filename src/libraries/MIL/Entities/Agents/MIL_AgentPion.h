// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentPion.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:38 $
// $Revision: 18 $
// $Workfile: MIL_AgentPion.h $
//
// *****************************************************************************

#ifndef __MIL_AgentPion_h_
#define __MIL_AgentPion_h_

#include "MIL.h"

#include "MIL_Agent_ABC.h"
#include "MIL_AgentTypePion.h"
#include "Entities/Orders/Pion/MIL_PionOrderManager.h"

class MIL_AgentPion;
class MIL_Automate;
class MIL_Fuseau;
class PHY_Action_ABC;
class DEC_KnowledgeBlackBoard;
class DEC_KS_NetworkUpdater;
class DEC_KS_ObjectInteraction;
class DEC_KS_Fire;
class DEC_KS_AgentQuerier;
class DEC_RolePion_Decision;
class HLA_UpdateFunctor;

// =============================================================================
// @class  MIL_AgentPion
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPion : public MIL_Agent_ABC
{
    MT_COPYNOTALLOWED( MIL_AgentPion )

public:
    MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_InputArchive& archive );                            // Pion dans ODB
    MIL_AgentPion( MIL_Automate& automate, MIL_InputArchive& archive );                                             // Pion PC pour automate dans ODB
    MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ); // Creation dynamique (convois, ...)
    MIL_AgentPion();
    virtual ~MIL_AgentPion();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Init
    //@{
    void ReadOverloading( MIL_InputArchive& archive );
    //@}

    //! @name Accessors
    //@{
    const std::string&              GetName               () const;
    const DEC_KnowledgeBlackBoard&  GetKnowledge          () const;        
    const DEC_KS_AgentQuerier&      GetKSQuerier          () const;
          DEC_KS_ObjectInteraction& GetKSObjectInteraction();
    const MIL_PionOrderManager&     GetOrderManager       () const;
          MIL_PionOrderManager&     GetOrderManager       ();
          DEC_RolePion_Decision&    GetDecision           (); //$$$ Dérolifier DEC_RolePion_Decision
    const DEC_RolePion_Decision&    GetDecision           () const; //$$$ Dérolifier DEC_RolePion_Decision
    
    virtual MIL_Army&                 GetArmy          () const;
            MIL_KnowledgeGroup&       GetKnowledgeGroup() const;
    virtual const MIL_Automate&       GetAutomate      () const;
    virtual       MIL_Automate&       GetAutomate      ();
    virtual const MIL_AgentTypePion&  GetType          () const;
    virtual MT_Float                  GetEtatOps       () const;
    virtual bool                      IsDead           () const;
    virtual bool                      IsNeutralized    () const;
    virtual bool                      IsPC             () const;

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const;

    const MT_Vector2D&        GetDirDanger() const;
    const MIL_Fuseau&         GetFuseau   () const;
    const T_LimaFlagedPtrMap& GetLimas    () const;

    bool CanFly      () const;
    bool IsAutonomous() const; // Drones
    //@}

    //! @name Operations
    //@{
    void PreprocessRandomBreakdowns( uint nEndDayTimeStep ) const;
    void UpdateKnowledge            ();
    void UpdateDecision             ();
    void UpdateActions              ();
    void UpdateState                ();
    void UpdateNetwork              ();
    void Clean                      ();
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Agent& CreateKnowledge ( const MIL_KnowledgeGroup& knowledgeGroup );
    virtual bool                 IsAgentPerceived( const MIL_Agent_ABC& agent ) const; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    //@}

    //! @name Network
    //@{
    void SendCreation ();
    void SendFullState();
    void SendKnowledge();
    void OnReceiveMsgUnitMagicAction( DIN::DIN_Input&           msg );
    void OnReceiveMsgUnitMagicAction( ASN1T_MsgUnitMagicAction& msg, MIL_MOSContextID nCtx );
    void OnReceiveMsgPionOrder      ( ASN1T_MsgPionOrder&       msg, MIL_MOSContextID nCtx ); 
    void OnReceiveMsgOrderConduite  ( ASN1T_MsgOrderConduite&   msg, MIL_MOSContextID nCtx );
    void OnReceiveMsgChangeAutomate ( ASN1T_MsgChangeAutomate&  msg, MIL_MOSContextID nCtx );
    void OnReceiveMagicSurrender    ();
    //@}

    //! @name HLA
    //@{
    void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Actions
    //@{
    void RegisterAction  ( PHY_Action_ABC& action );
    void UnregisterAction( PHY_Action_ABC& action );
    void CancelAllActions();
    bool HasAction       ( PHY_Action_ABC& action ) const;
    //@}

    //! @name Misc operations
    //@{
            void MagicMove       ( const MT_Vector2D& vNewPos );    
            void NotifyAttacking ( MIL_Agent_ABC& target ) const;
    virtual void NotifyAttackedBy( MIL_AgentPion& pion );
            void ChangeAutomate  ( MIL_Automate& newAutomate );
            void Surrender       ();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< PHY_Action_ABC* >  T_ActionSet;
    typedef T_ActionSet::const_iterator  CIT_ActionSet;
    //@}

private:
    //! @name Operations
    //@{
    void Initialize         ( MIL_InputArchive& archive );
    void Initialize         ( const MT_Vector2D& vPosition );
    void UpdatePhysicalState();
    //@}

    //! @name Magic actions
    //@{
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgChangeHumanFactors( ASN1T_MagicActionChangeFacteursHumains& asn );
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgMagicMove         ( ASN1T_MagicActionMoveTo&                asn );
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgResupplyHumans    ();
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgResupplyResources ();
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgResupplyEquipement();
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgResupplyAll       ();
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgResupply          ( ASN1T_MagicActionRecompletementPartiel& asn );
    ASN1T_EnumUnitAttrErrorCode OnReceiveMsgDestroyAll        ();
    //@}

private:
    const MIL_AgentTypePion*  pType_;
    const bool                bIsPC_;
          std::string         strName_;
          MIL_Automate*       pAutomate_;
          T_ActionSet         actions_;

    // Knowledge
    DEC_KnowledgeBlackBoard*  pKnowledgeBlackBoard_;
    DEC_KS_ObjectInteraction* pKsObjectInteraction_;
    DEC_KS_Fire*              pKsFire_;
    DEC_KS_AgentQuerier*      pKsQuerier_;
    DEC_KS_NetworkUpdater*    pKsNetworkUpdater_;

    MIL_PionOrderManager&     orderManager_;
};

#include "MIL_AgentPion.inl"

#endif // __MIL_AgentPion_h_
