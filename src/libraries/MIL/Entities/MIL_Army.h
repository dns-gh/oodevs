// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_Army.h $
// $Author: Jvt $
// $Modtime: 6/04/05 17:14 $
// $Revision: 9 $
// $Workfile: MIL_Army.h $
//
// *****************************************************************************

#ifndef __MIL_Army_h_
#define __MIL_Army_h_

#include "MIL.h"

#include "MT_Tools/MT_Converter.h"

class DEC_KnowledgeBlackBoard;
class DEC_KS_ObjectKnowledgeSynthetizer;
class DEC_KS_Alzheimer;
class DEC_KS_NetworkUpdater;
class DEC_KS_ArmyQuerier;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class MIL_KnowledgeGroup;
class MIL_EntityManager;
struct ASN1T_MsgChangeDiplomatie;

// =============================================================================
// @class  MIL_Army
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Army
{
    MT_COPYNOTALLOWED( MIL_Army )

public:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_KnowledgeGroup* > T_KnowledgeGroupMap;
    typedef T_KnowledgeGroupMap::const_iterator   CIT_KnowledgeGroupMap;
    //@}

public:
     MIL_Army( const std::string& strName, uint nID, MIL_InputArchive& archive );
     MIL_Army();
    ~MIL_Army();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();
    //@}

    //! @name Init
    //@{
    void InitializeDiplomacy( const MIL_EntityManager& entityManager, MIL_InputArchive& archive );
    //@}

    //! @name Operations
    //@{
    bool       IsPerceived( const DEC_Knowledge_Object& knowledge ) const;
    E_Tristate IsAFriend  ( const DEC_Knowledge_Agent & knowledge ) const;
    E_Tristate IsAFriend  ( const MIL_Army& army )                  const;
    E_Tristate IsAnEnemy  ( const DEC_Knowledge_Agent & knowledge ) const;
    E_Tristate IsAnEnemy  ( const MIL_Army& army                  ) const;
    E_Tristate IsNeutral  ( const MIL_Army& army                  ) const;
    bool       operator== ( const MIL_Army& rhs )                   const;
    bool       operator!= ( const MIL_Army& rhs )                   const;
    //@}

    //! @name Knowledge
    //@{
    void                UpdateKnowledges  ();
    void                CleanKnowledges   ();
    MIL_KnowledgeGroup* FindKnowledgeGroup( uint nID ) const;
    //@}

    //! @name Accessors
    //@{
          uint                               GetID                          () const;
    const std::string&                       GetName                        () const;
    const T_KnowledgeGroupMap&               GetKnowledgeGroups             () const;
    const DEC_KS_ArmyQuerier&                GetKSQuerier                   () const;
          DEC_KS_ObjectKnowledgeSynthetizer& GetKSObjectKnowledgeSynthetizer();
    //@}

    //! @name Network
    //@{
    void SendCreation               ();
    void SendFullState              ();
    void SendKnowledge              ();
    void OnReceiveMsgChangeDiplomacy( ASN1T_MsgChangeDiplomatie& msg, MIL_MOSContextID nCtx );
    //@}

private:
    //! @name Types
    //@{
    enum E_Diplomacy
    {
        eUnknown,
        eFriend,
        eEnemy,
        eNeutral
    };

public:
    typedef std::map< const MIL_Army*, E_Diplomacy > T_ArmyRelationMap;
    typedef T_ArmyRelationMap::const_iterator        CIT_ArmyRelationMap;
    //@}

private:
    //! @name Tools
    //@{
    E_Diplomacy GetRelationWith( const MIL_Army& army ) const;
    //@}

private:
    const std::string   strName_;
    const uint          nID_;
    T_KnowledgeGroupMap knowledgeGroups_;
    T_ArmyRelationMap   relations_;

    DEC_KnowledgeBlackBoard*           pKnowledgeBlackBoard_;
    DEC_KS_ObjectKnowledgeSynthetizer* pKsObjectKnowledgeSynthetizer_;
    DEC_KS_Alzheimer*                  pKsAlzheimer_;
    DEC_KS_NetworkUpdater*             pKsNetworkUpdater_;
    DEC_KS_ArmyQuerier*                pKsQuerier_;

private:
    static MT_Converter< std::string, E_Diplomacy > relationConverter_;
};

#include "MIL_Army.inl"

#endif // __MIL_Army_h_
