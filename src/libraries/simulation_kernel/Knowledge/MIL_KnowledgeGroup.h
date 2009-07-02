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

namespace xml
{
    class xostream;
}

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;

class MIL_Army;
class MIL_Automate;

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
    //@}

public:
     MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, uint nID, MIL_Army& army );
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
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );

    void UpdateKnowledges();
    void CleanKnowledges ();
    bool IsPerceived     ( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceived     ( const DEC_Knowledge_Agent&  knowledge ) const;

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;
    //@}

    //! @name Accessors
    //@{
          uint                                    GetID       () const;
    const MIL_KnowledgeGroupType&                 GetType     () const;
          MIL_Army&                               GetArmy     () const;
    const T_AutomateVector&                       GetAutomates() const;
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& GetKnowledge() const;
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    void SendKnowledge() const;
    //@}
    
private:
    const MIL_KnowledgeGroupType* pType_;
    const uint                    nID_;
          MIL_Army*               pArmy_;

    DEC_KnowledgeBlackBoard_KnowledgeGroup* pKnowledgeBlackBoard_;

    T_AutomateVector automates_;

private:
    static std::set< uint > ids_;
};

#endif // __MIL_KnowledgeGroup_h_
