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
#include "tools/Resolver.h"
#include "Network/NET_ASN_Messages.h"

namespace xml
{
    class xostream;
}

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;

class MIL_Army;
class MIL_Automate;

class KnowledgeGroupFactory_ABC;
class MIL_KnowledgeGroup;

#include "MIL_KnowledgeGroupType.h"

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup : private boost::noncopyable
                         , public tools::Resolver< MIL_KnowledgeGroup >
{

public:
    //! @name Types
    //@{
    typedef std::vector< MIL_Automate* >     T_AutomateVector;
    typedef T_AutomateVector::iterator       IT_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;
    //@}

public:
     MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, uint nID, MIL_Army& army );
     MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army& army, MIL_KnowledgeGroup* pParent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
     MIL_KnowledgeGroup();
    ~MIL_KnowledgeGroup();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void InitializeKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    void RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup );
    void UnregisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup );
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );
    MIL_KnowledgeGroup* FindKnowledgeGroup ( uint nID ) const;

    void OnReceiveMsgKnowledgeGroupChangeSuperior( const ASN1T_MsgKnowledgeGroupChangeSuperior& msg, const tools::Resolver< MIL_Army >& armies );
    void OnReceiveMsgKnowledgeGroupDelete( const ASN1T_MsgKnowledgeGroupDelete& msg );
    void OnReceiveMsgKnowledgeGroupSetType( const ASN1T_MsgKnowledgeGroupSetType& msg );

    void UpdateKnowledges();
    void CleanKnowledges ();
    bool IsPerceived     ( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceived     ( const DEC_Knowledge_Agent&  knowledge ) const;
    void RefreshTimeToDiffuseToKnowledgeGroup();

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;
    //@}

    //! @name Operations
    //@{
    void OnReceiveMsgKnowledgeGroupEnable( const ASN1T_MsgKnowledgeGroupEnable& asnMsg );
    //@}


    //! @name Accessors
    //@{
          uint                                    GetID       () const;
    const MIL_KnowledgeGroupType&                 GetType     () const;
          MIL_Army&                               GetArmy     () const;
    const T_AutomateVector&                       GetAutomates() const;
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& GetKnowledge() const;
          MIL_KnowledgeGroup*                     GetParent   () const;
          MT_Float                                GetTimeToDiffuseToKnowledgeGroup() const;
          bool                                    IsEnabled() const;
          void                                    SetParent( MIL_KnowledgeGroup* pParent );
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    void SendKnowledge() const;
    void UpdateKnowledgeGroup() const;
    //@}
    
private:
    const MIL_KnowledgeGroupType* pType_;
          uint                    nID_;
          MIL_Army*               pArmy_;
          MIL_KnowledgeGroup*     pParent_;

    DEC_KnowledgeBlackBoard_KnowledgeGroup* pKnowledgeBlackBoard_;

    T_AutomateVector    automates_;
    MT_Float            timeToDiffuse_;
    bool                isActivated_;

private:
    static std::set< uint > ids_;
};

#endif // __MIL_KnowledgeGroup_h_
