// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/MIL_KnowledgeGroup.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 9 $
// $Workfile: MIL_KnowledgeGroup.h $
//
// *****************************************************************************

#ifndef __MIL_KnowledgeGroup_h_
#define __MIL_KnowledgeGroup_h_

#include "MIL.h"

class DEC_KnowledgeBlackBoard;
class DEC_KS_AgentKnowledgeSynthetizer;
class DEC_KS_Alzheimer;
class DEC_KS_RapFor;
class DEC_KS_NetworkUpdater;
class DEC_KS_KnowledgeGroupQuerier;
class DEC_KS_Sharing;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;

class MIL_Army;
class MIL_Automate;

#include "MIL_KnowledgeGroupType.h"

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup
{
    MT_COPYNOTALLOWED( MIL_KnowledgeGroup )

public:
    //! @name Types
    //@{
    typedef std::vector< MIL_Automate* >     T_AutomateVector;
    typedef T_AutomateVector::iterator       IT_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;
    //@}

public:
     MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, uint nID, MIL_Army& army, MIL_InputArchive& archive );
     MIL_KnowledgeGroup();
    ~MIL_KnowledgeGroup();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );

    void UpdateKnowledges();
    bool IsPerceived     ( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceived     ( const DEC_Knowledge_Agent&  knowledge ) const;

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;
    //@}

    //! @name Accessors
    //@{
          uint                          GetID                                     () const;
    const MIL_KnowledgeGroupType&       GetType                                   () const;
          MIL_Army&                     GetArmy                                   () const;
    const T_AutomateVector&             GetAutomates                              () const;
    const DEC_KnowledgeBlackBoard&      GetKnowledge                              () const;
    const DEC_KS_KnowledgeGroupQuerier& GetKSQuerier                              () const;
          DEC_KS_Sharing&               GetKSSharing                              ();
          MT_Float                      GetKnowledgeMaxLifeTime                   () const;
          MT_Float                      GetKnowledgeMaxDistBtwKnowledgeAndRealUnit() const;
    //@}

    //! @name Network
    //@{
    void SendCreation ();
    void SendKnowledge();
    //@}
    
private:
    const MIL_KnowledgeGroupType* pType_;
    const uint                    nID_;
          MIL_Army*               pArmy_;

    DEC_KnowledgeBlackBoard*          pKnowledgeBlackBoard_;
    DEC_KS_AgentKnowledgeSynthetizer* pKsAgentKnowledgeSynthetizer_;
    DEC_KS_Alzheimer*                 pKsAlzheimer_;
    DEC_KS_RapFor*                    pKsRapFor_;
    DEC_KS_Sharing*                   pKsSharing_;
    DEC_KS_NetworkUpdater*            pKsNetworkUpdater_;
    DEC_KS_KnowledgeGroupQuerier*     pKsQuerier_;

    T_AutomateVector automates_;

private:
    static std::set< uint > ids_;
};

#include "MIL_KnowledgeGroup.inl"

#endif // __MIL_KnowledgeGroup_h_
