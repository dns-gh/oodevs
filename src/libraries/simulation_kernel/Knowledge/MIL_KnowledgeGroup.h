// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_KnowledgeGroup_h_
#define __MIL_KnowledgeGroup_h_

#include "MIL.h"
#include "MIL_KnowledgeGroupType.h"
#include "Tools/MIL_IDManager.h"
#include "tools/Resolver.h"
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
class MIL_Object;
class MIL_Object_ABC;
class MIL_Population;
class PHY_PerceptionLevel;

// LTO begin
class KnowledgeGroupFactory_ABC;
class MIL_KnowledgeGroup;

namespace sword
{
    class MissionParameters;
    class KnowledgeGroupCreation;
    class KnowledgeGroupUpdate;
    class KnowledgeGroupCreationRequest;
    class KnowledgeMagicAction;
}
// LTO end

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< MIL_Automate* >       T_AutomateVector;
    typedef T_AutomateVector::iterator        IT_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;

    typedef std::vector< MIL_KnowledgeGroup* >        T_KnowledgeGroupVector;
    typedef T_KnowledgeGroupVector::iterator         IT_KnowledgeGroupVector;
    typedef T_KnowledgeGroupVector::const_iterator  CIT_KnowledgeGroupVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, unsigned int id, MIL_Army_ABC& army );
             MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory ); // LTO
             MIL_KnowledgeGroup();
             MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source, const MIL_Agent_ABC& pion, MIL_KnowledgeGroup* parent );
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
    MIL_KnowledgeGroup* FindKnowledgeGroup ( unsigned int id ) const;
    void SetType( MIL_KnowledgeGroupType *type ){ type_ = type; }
    void RefreshTimeToDiffuseToKnowledgeGroup();
    // LTO end
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );

    void UpdateKnowledges(int currentTimeStep);
    void CleanKnowledges ();
    bool IsPerceived     ( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceived     ( const DEC_Knowledge_Agent&  knowledge ) const;
    bool IsPerceptionDistanceHacked( MIL_Agent_ABC& agentKnown ) const;
    bool IsPerceptionDistanceHacked( MIL_Object_ABC& objectKnown ) const;
    bool IsPerceptionDistanceHacked( MIL_Population& populationKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Agent_ABC& agentKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Object_ABC& ObjectKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Population& populationKnown ) const;

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;
    //@}

    //! @name Operations
    //@{
    void OnReceiveKnowledgeGroupCreation   ( const sword::KnowledgeGroupCreationRequest& message );
    void OnReceiveKnowledgeGroupUpdate     ( const sword::KnowledgeMagicAction& message, const tools::Resolver< MIL_Army_ABC >& armies );
    void Destroy();
    //@}

    //! @name Accessors
    //@{
          unsigned int                            GetId       () const;
    const MIL_KnowledgeGroupType&                 GetType     () const;
          MIL_Army_ABC&                           GetArmy     () const;
    const T_AutomateVector&                       GetAutomates() const;
    const DEC_KnowledgeBlackBoard_KnowledgeGroup& GetKnowledge() const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int ) const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    // LTO begin
    const T_KnowledgeGroupVector&                 GetKnowledgeGroups() const;
          MIL_KnowledgeGroup*                     GetParent   () const;
          double                                GetTimeToDiffuseToKnowledgeGroup() const;
          bool                                    IsEnabled() const;
          void                                    SetParent( MIL_KnowledgeGroup* parent );
    // LTO end
          bool IsJammed() const;
    //@}

    //! @name Network
    //@{
    void SendCreation ( unsigned int context = 0 ) const;
    void SendFullState() const;
    void SendKnowledge( unsigned int context ) const;
    // LTO begin
    void UpdateKnowledgeGroup();
    void MoveKnowledgeGroup( MIL_KnowledgeGroup *newParent );
    void ApplyOnKnowledgesPerception( int currentTimeStep );
    // LTO end

    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject ( const MIL_Army_ABC& teamKnowing, MIL_Object_ABC& objectKnown );
    DEC_Knowledge_Agent& CreateKnowledgeAgent ( MIL_Agent_ABC& perceived );
    DEC_Knowledge_Population& CreateKnowledgePopulation( MIL_Population& perceived );

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgent( UnaryFunction& fct ) const
    {
        GetKnowledge().ApplyOnKnowledgesAgent( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulation( UnaryFunction& fct ) const
    {
        GetKnowledge().ApplyOnKnowledgesPopulation( fct );
    }
    //@}

private:
    //! @name Helpers
    //@{
    bool OnReceiveKnowledgeGroupEnable        ( const sword::MissionParameters& message );
    bool OnReceiveKnowledgeGroupChangeSuperior( const sword::MissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies, bool hasParent );
    bool OnReceiveKnowledgeGroupSetType       ( const sword::MissionParameters& message );
    bool OnReceiveKnowledgeGroupAddKnowledge  ( const sword::MissionParameters& message );

    void CreateKnowledgeFromAgentPerception( const DEC_Knowledge_Agent& agent );
    void CreateKnowledgeFromPopulationPerception( const DEC_Knowledge_Population& population );

    void ApplyOnKnowledgesPopulationPerception( int currentTimeStep );
    void ApplyOnKnowledgesAgentPerception( int currentTimeStep );
    DEC_Knowledge_Population& GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown );
    void UpdatePopulationKnowledgeFromCollision( const DEC_Knowledge_PopulationCollision& collision, int currentTimeStep  );
    void UpdatePopulationKnowledgeFromPerception( const DEC_Knowledge_PopulationPerception& perception, int currentTimeStep  );
    DEC_Knowledge_Agent& GetAgentKnowledgeToUpdate( MIL_Agent_ABC& agentKnown );
    void UpdateAgentKnowledgeFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep );
    void UpdateAgentKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Agent_ABC& agent, unsigned int perception );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Object_ABC& agent, unsigned int perception );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Population& population, unsigned int perception );
    boost::shared_ptr< DEC_Knowledge_Object > GetObjectKnowledgeToUpdate( MIL_Object_ABC& objectKnown );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_KnowledgeGroupType* type_;
    unsigned int                  id_;
    std::string                   name_;  
    MIL_Army_ABC*           army_;
    MIL_KnowledgeGroup*     parent_; // LTO
    DEC_KnowledgeBlackBoard_KnowledgeGroup* knowledgeBlackBoard_;
    T_AutomateVector        automates_;
    T_KnowledgeGroupVector  knowledgeGroups_; // LTO
    double                  timeToDiffuse_; // LTO
    bool                    isActivated_; // LTO
    bool                    hasBeenUpdated_;
    bool                    isJammed_;
    bool                    createdByJamming_;
    const MIL_Agent_ABC*    jammedPion_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_KnowledgeGroup )

#endif // __MIL_KnowledgeGroup_h_
