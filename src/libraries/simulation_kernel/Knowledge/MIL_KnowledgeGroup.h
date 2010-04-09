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
#include "MIL_KnowledgeGroupType.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/Resolver.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xostream;
}

class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_AgentPerception;
class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationCollision;
class DEC_Knowledge_PopulationPerception;
class KnowledgeGroupFactory_ABC;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_Object_ABC;
class MIL_Population;

// LTO begin
class KnowledgeGroupFactory_ABC;
class MIL_KnowledgeGroup;
namespace MsgsSimToClient
{
    class MsgKnowledgeGroupCreation;
    class MsgKnowledgeGroupUpdate;
}

namespace MsgsClientToSim
{ 
    class MsgKnowledgeGroupCreationRequest;
    class MsgKnowledgeGroupUpdateRequest;
}
// LTO end

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup
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
    MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, unsigned int nID, MIL_Army_ABC& army );
    MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* pParent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory ); // LTO
    MIL_KnowledgeGroup();
    MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source );

    virtual ~MIL_KnowledgeGroup();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    
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
    void OnReceiveMsgKnowledgeGroupCreation   ( const MsgsClientToSim::MsgKnowledgeGroupCreationRequest& message );
    void OnReceiveMsgKnowledgeGroupUpdate     ( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message, const tools::Resolver< MIL_Army_ABC >& armies );
    void Destroy();
    //@}


    //! @name Accessors
    //@{
          unsigned int                            GetID       () const;
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
          bool IsJammedKnowledgeGroup() const;
          void Jam( const MIL_Agent_ABC& pion );
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    void SendKnowledge() const;
    // LTO begin
    void UpdateKnowledgeGroup();
    void MoveKnowledgeGroup( MIL_KnowledgeGroup *pNewParent );
    void ApplyOnKnowledgesPopulationPerception();
    void ApplyOnKnowledgesAgentPerception( int currentTimeStep );
    // LTO end

    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject ( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown );
    
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
    bool                    hasBeenUpdated_;  
    bool                    isJammedKnowledgeGroup_;
    const MIL_Agent_ABC*    jamedPion_;

    bool OnReceiveMsgKnowledgeGroupEnable        ( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message );
    bool OnReceiveMsgKnowledgeGroupChangeSuperior( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message, const tools::Resolver< MIL_Army_ABC >& armies );
    bool OnReceiveMsgKnowledgeGroupSetType       ( const MsgsClientToSim::MsgKnowledgeGroupUpdateRequest& message );

    void CreateKnowledgesFromAgentPerception( const DEC_Knowledge_Agent& agent );
    void CreateKnowledgesFromPopulationPerception( const DEC_Knowledge_Population& population );

    DEC_Knowledge_Population& GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown ) const;
    void UpdatePopulationKnowledgeFromCollision( const DEC_Knowledge_PopulationCollision& collision );
    void UpdatePopulationKnowledgeFromPerception( const DEC_Knowledge_PopulationPerception& perception );
    DEC_Knowledge_Agent& GetAgentKnowledgeToUpdate( MIL_Agent_ABC& agentKnown ) const;
    void UpdateAgentKnowledgeFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep );
    void UpdateAgentKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep );
        //@}
    
private:
    static std::set< unsigned int > ids_;
    static MIL_IDManager idManager_;
};

BOOST_CLASS_EXPORT_KEY( MIL_KnowledgeGroup )

#endif // __MIL_KnowledgeGroup_h_
