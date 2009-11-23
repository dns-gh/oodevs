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
#include "Entities/Orders/MIL_PionOrderManager.h"

#include "tools/Resolver.h"

namespace xml
{
    class xostream;
    class xistream;
}

class MIL_Army;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Fuseau;
class MIL_LimaFunction;
class DEC_Decision_ABC;
class DEC_KnowledgeBlackBoard_AgentPion;
class DEC_KS_AgentQuerier;
class HLA_UpdateFunctor;
class MIL_EntityManager;

class AlgorithmsFactories;

// =============================================================================
// @class  MIL_AgentPion
// Created: JVT 2004-08-03
// =============================================================================
class MIL_AgentPion : public MIL_Agent_ABC
{

public:
             MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis );    // Pion dans ODB
             MIL_AgentPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories ); // Creation dynamique (convois, ...)
    virtual ~MIL_AgentPion();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    void ReadOverloading( xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
    virtual DEC_KnowledgeBlackBoard_AgentPion& GetKnowledge   () const;        
    const   MIL_PionOrderManager&              GetOrderManager() const;
            MIL_PionOrderManager&              GetOrderManager();
    virtual DEC_Decision_ABC&                  GetDecision    (); //$$$ Dérolifier DEC_Decision_ABC
    virtual const   DEC_Decision_ABC&                  GetDecision    () const; //$$$ Dérolifier DEC_Decision_ABC
    virtual const AlgorithmsFactories& GetAlgorithms() const;

    virtual       MIL_Army_ABC&       GetArmy            () const;
                  MIL_KnowledgeGroup& GetKnowledgeGroup  () const;
                  int                 GetNumberOfFireHoses( int bestExtinguisherAgent );
    virtual const MIL_Automate&       GetAutomate        () const;
    virtual       MIL_Automate&       GetAutomate        ();
    virtual const MIL_AgentTypePion&  GetType            () const;
    virtual bool                      IsDead             () const;
    virtual bool                      IsNeutralized      () const;
    virtual bool                      IsPC               () const;
    virtual MT_Float                  GetMajorComponentWeight   () const;

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const;

    bool CanFly      () const;
    bool IsAutonomous() const; // Drones
    //@}

    //! @name Operations
    //@{
    void PreprocessRandomBreakdowns( uint nEndDayTimeStep ) const;
    void UpdateKnowledges          ();
    void CleanKnowledges           ();
    void UpdateDecision            ( float duration );
    void UpdateState               ();
    void UpdateNetwork             ();
    void Clean                     ();
    //@}

    //! @name Knowledge
    //@{
    virtual boost::shared_ptr< DEC_Knowledge_Agent > CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup );
    virtual bool                                     IsPerceived    ( const MIL_Agent_ABC& agent ) const; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    void SendKnowledge() const;

    void OnReceiveMsgUnitMagicAction  ( const ASN1T_MsgUnitMagicAction&    msg, const tools::Resolver< MIL_Army>& armies );
    void OnReceiveMsgOrder            ( const ASN1T_MsgUnitOrder&          msg ); 
    void OnReceiveMsgFragOrder        ( const ASN1T_MsgFragOrder&          msg );
    void OnReceiveMsgChangeSuperior   ( const MIL_EntityManager& manager, const ASN1T_MsgUnitChangeSuperior& msg );
    void OnReceiveMagicSurrender      ();
    void OnReceiveMagicCancelSurrender();
    void OnReceiveMsgMagicMove        ( const MT_Vector2D& vPosition ); // Magic move automate
    //@}

    //! @name HLA
    //@{
    void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Misc operations
    //@{
            void MagicMove       ( const MT_Vector2D& vNewPos );    
            void NotifyAttacking ( MIL_Agent_ABC& target ) const;
            void NotifyAttacking ( MIL_Population& target ) const;
    virtual void NotifyAttackedBy( MIL_AgentPion& pion );
    virtual void NotifyAttackedBy( MIL_Population& population );
            void ChangeSuperior  ( MIL_Automate& newAutomate );
    //@}

protected:
    MIL_AgentPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories );

private:
    //! @name Operations
    //@{
    void UpdatePhysicalState();
    //@}

    //! @name Magic actions
    //@{
    void OnReceiveMsgChangeHumanFactors       ( const ASN1T_MagicActionChangeHumanFactors& asn );
    void OnReceiveMsgMagicMove                ( const ASN1T_MagicActionMoveTo&                asn );
    void OnReceiveMsgResupplyHumans           ();
    void OnReceiveMsgResupplyResources        ();
    void OnReceiveMsgResupplyEquipement       ();
    void OnReceiveMsgResupplyAll              ();
    void OnReceiveMsgResupply                 ( const ASN1T_MagicActionPartialRecovery& asn );
    void OnReceiveMsgDestroyAll               ();
    void OnReceiveMsgDestroyComponent         ();
    void OnReceiveMsgRecoverHumansTransporters();
    //@}

private:
    const MIL_AgentTypePion*  pType_;
          bool                bIsPC_;
          MIL_Automate*       pAutomate_;
    
    const AlgorithmsFactories& algorithmFactories_;

    // Knowledge
    DEC_KnowledgeBlackBoard_AgentPion* pKnowledgeBlackBoard_;

    MIL_PionOrderManager&    orderManager_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_AgentPion* pion, const unsigned int /*version*/ );
  	template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_AgentPion* pion, const unsigned int /*version*/ );
};

#endif // __MIL_AgentPion_h_
