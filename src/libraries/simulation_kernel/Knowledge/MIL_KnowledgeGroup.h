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
#include <boost/serialization/export.hpp>

namespace xml
{
    class xostream;
}

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;

class MIL_Army_ABC;
class MIL_Automate;

class KnowledgeGroupFactory_ABC; // LTO
class MIL_KnowledgeGroup; // LTO

#include "MIL_KnowledgeGroupType.h"

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::vector< MIL_Automate* >     T_AutomateVector;
    typedef T_AutomateVector::iterator       IT_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;

    typedef std::vector< MIL_KnowledgeGroup* >      T_KnowledgeGroupVector;
    typedef T_KnowledgeGroupVector::iterator        IT_KnowledgeGroupVector;
    typedef T_KnowledgeGroupVector::const_iterator  CIT_KnowledgeGroupVector;
    //@}

public:
    MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, uint nID, MIL_Army_ABC& army ); // LTO
    MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* pParent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    MIL_KnowledgeGroup();
    virtual ~MIL_KnowledgeGroup();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    // LTO begin
    void InitializeKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    void RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup );
    void UnregisterKnowledgeGroup( const MIL_KnowledgeGroup& knowledgeGroup );
    MIL_KnowledgeGroup* FindKnowledgeGroup ( uint nID ) const;
    void SetType( MIL_KnowledgeGroupType *pType ){ pType_ = pType; }
    void RefreshTimeToDiffuseToKnowledgeGroup();
    // LTO end
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );

    void UpdateKnowledges(int currentTimeStep);
    void CleanKnowledges ();
    bool IsPerceived     ( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceived     ( const DEC_Knowledge_Agent&  knowledge ) const;

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;
    //@}

    //! @name Operations
    //@{
    // LTO begin
    void OnReceiveMsgKnowledgeGroupEnable( const ASN1T_MsgKnowledgeGroupEnable& asnMsg );
    void OnReceiveMsgKnowledgeGroupCreation( const ASN1T_MsgKnowledgeGroupCreation& msg );
    void OnReceiveMsgKnowledgeGroupChangeSuperior( const ASN1T_MsgKnowledgeGroupChangeSuperior& msg, const tools::Resolver< MIL_Army_ABC >& armies );
    void OnReceiveMsgKnowledgeGroupDelete( const ASN1T_MsgKnowledgeGroupDelete& msg );
    void OnReceiveMsgKnowledgeGroupSetType( const ASN1T_MsgKnowledgeGroupSetType& msg );
    // LTO end
    //@}


    //! @name Accessors
    //@{
          uint                                    GetID       () const;
    const MIL_KnowledgeGroupType&                 GetType     () const;
          MIL_Army_ABC&                           GetArmy     () const;
    const T_AutomateVector&                       GetAutomates() const;
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& GetKnowledge() const;
    // LTO begin
    const T_KnowledgeGroupVector&                 GetKnowledgeGroups() const;
          MIL_KnowledgeGroup*                     GetParent   () const;
          MT_Float                                GetTimeToDiffuseToKnowledgeGroup() const;
          bool                                    IsEnabled() const;
          void                                    SetParent( MIL_KnowledgeGroup* pParent );
    // LTO end
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    void SendKnowledge() const;
    // LTO begin
    void UpdateKnowledgeGroup() const;
    
    void DeleteKnowledgeGroup();
    void MoveKnowledgeGroup( MIL_KnowledgeGroup *pNewParent );
    // LTO end

    //@}
    
private:
    const MIL_KnowledgeGroupType* pType_;
          uint                    nID_;
          MIL_Army_ABC*           pArmy_;
          MIL_KnowledgeGroup*     pParent_; // LTO

    DEC_KnowledgeBlackBoard_KnowledgeGroup* pKnowledgeBlackBoard_;

    T_AutomateVector        automates_;
    T_KnowledgeGroupVector  knowledgeGroups_; // LTO
    MT_Float                timeToDiffuse_; // LTO
    bool                    isActivated_; // LTO

private:
    static std::set< uint > ids_;
};

BOOST_CLASS_EXPORT_KEY( MIL_KnowledgeGroup )

#endif // __MIL_KnowledgeGroup_h_
