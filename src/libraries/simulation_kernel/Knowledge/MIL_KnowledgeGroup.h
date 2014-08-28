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
#include "simulation_terrain/TER_Agent_ABC.h"
#include "simulation_terrain/TER_PopulationConcentration_ABC.h"
#include "simulation_terrain/TER_PopulationFlow_ABC.h"
#include "simulation_terrain/TER_Object_ABC.h"
#include "Tools/MIL_IDManager.h"
#include <tools/Resolver.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/export.hpp>
#include <boost/scoped_ptr.hpp>

namespace xml
{
    class xostream;
}

class DEC_BlackBoard_CanContainKnowledgeObject;
class DEC_KnowledgeBlackBoard_KnowledgeGroup;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_AgentPerception;
class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationCollision;
class DEC_Knowledge_PopulationPerception;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_Object;
class MIL_Object_ABC;
class MIL_Population;
class PHY_PerceptionLevel;
class KnowledgesVisitor_ABC;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_ObjectPerception;

namespace sword
{
    class MissionParameters;
    class KnowledgeMagicAction;
    class KnowledgeGroupMagicActionAck;
}

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup : public boost::enable_shared_from_this< MIL_KnowledgeGroup >
                         , private boost::noncopyable
{
private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Automate* > T_AutomateVector;
    typedef std::vector< MIL_Population* > T_PopulationVector;
    typedef std::vector< boost::shared_ptr< MIL_KnowledgeGroup > > T_KnowledgeGroupVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_KnowledgeGroup();
             MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, MIL_KnowledgeGroup& parent );
             MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, MIL_Army_ABC& parent, bool crowd );
             MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent );
             MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source, const MIL_Agent_ABC& pion, MIL_KnowledgeGroup* parent );
    virtual ~MIL_KnowledgeGroup();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    void WriteODB( xml::xostream& xos ) const;
    void WriteKnowledges( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Clone( const MIL_KnowledgeGroup& source );
    void RegisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup );
    void UnregisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup );
    boost::shared_ptr< MIL_KnowledgeGroup > FindKnowledgeGroup ( unsigned int id ) const;
    void RefreshTimeToDiffuseToKnowledgeGroup();
    void RegisterAutomate( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );
    void RegisterSharingPerceptions( const MIL_Agent_ABC& agent );
    void UnregisterSharingPerceptions( const MIL_Agent_ABC& agent );
    void RegisterSharingKnowledges( const MIL_Agent_ABC& agent );
    void UnregisterSharingKnowledges( const MIL_Agent_ABC& agent );
    void RegisterPopulation( MIL_Population& population );
    void UnregisterPopulation( MIL_Population& population );

    void AppendAddedKnowledge( TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, TER_Object_ABC::T_ObjectVector& perceivableObjects, TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector& perceivablePopulationDensity, TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivablePopulationFlow ) const;

    void UpdateKnowledges(int currentTimeStep);
    void UpdateObjectKnowledges(int currentTimeStep);
    void CleanKnowledges();
    void CleanDeletedAgentKnowledges();
    bool IsPerceived( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceptionDistanceHacked( MIL_Agent_ABC& agentKnown ) const;
    bool IsPerceptionDistanceHacked( const MIL_Object_ABC& objectKnown ) const;
    bool IsPerceptionDistanceHacked( MIL_Population& populationKnown ) const;
    bool CanReport() const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Agent_ABC& agentKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( const MIL_Object_ABC& ObjectKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Population& populationKnown ) const;

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;

    void OnReceiveKnowledgeGroupUpdate( const sword::KnowledgeMagicAction& message, sword::KnowledgeGroupMagicActionAck& ack, const tools::Resolver< MIL_Army_ABC >& armies );
    void Destroy();
    void Merge( const MIL_KnowledgeGroup& subGroup );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetId () const;
    const MIL_KnowledgeGroupType& GetType() const;
    MIL_Army_ABC& GetArmy() const;
    const T_AutomateVector& GetAutomates() const;
    const DEC_KnowledgeBlackBoard_KnowledgeGroup* GetKnowledge() const;
    DEC_BlackBoard_CanContainKnowledgeObject* GetKnowledgeObjectContainer() const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int ) const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int ) const;
    // LTO begin
    const T_KnowledgeGroupVector& GetKnowledgeGroups() const;
    bool IsEnabled() const;
    // LTO end
    bool IsJammed() const;
    bool IsCrowd() const;
    void Accept( KnowledgesVisitor_ABC& visitor ) const;
    //@}

    //! @name Network
    //@{
    void SendCreation ( unsigned int context = 0 ) const;
    void SendFullState() const;
    void SendKnowledge( unsigned int context ) const;
    void UpdateKnowledgeGroup();
    void ApplyOnKnowledgesPerception( int currentTimeStep );

    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject ( MIL_Object_ABC& objectKnown );
    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( boost::shared_ptr< DEC_Knowledge_Object >& object );
    void AddEphemeralObjectKnowledge( MIL_Object_ABC& objectKnown );
    DEC_Knowledge_Agent& CreateKnowledgeAgent ( const MIL_Agent_ABC& perceived );
    DEC_Knowledge_Population& CreateKnowledgePopulation( MIL_Population& perceived );
    DEC_Knowledge_Population& GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown );
    void UpdateKnowledgeFromTransported( const MIL_Agent_ABC& perceived );
    const MIL_Agent_ABC* GetJammedPion() const;

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgent( UnaryFunction& fct ) const
    {
        if( knowledgeBlackBoard_ )
            knowledgeBlackBoard_->ApplyOnKnowledgesAgent( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulation( UnaryFunction& fct ) const
    {
        if( knowledgeBlackBoard_ )
            knowledgeBlackBoard_->ApplyOnKnowledgesPopulation( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesObject( UnaryFunction& fct ) const
    {
        if( knowledgeBlackBoard_ )
            knowledgeBlackBoard_->ApplyOnKnowledgesObject( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesAgentPerception( UnaryFunction& fct ) const
    {
        if( jammedPion_ )
            jammedPion_->GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( fct );
    }
    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulationPerception( UnaryFunction& fct ) const
    {
        if( jammedPion_ )
            jammedPion_->GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesPopulationCollision( UnaryFunction& fct ) const
    {
        if( jammedPion_ )
            jammedPion_->GetKnowledge().GetKnowledgePopulationCollisionContainer().ApplyOnKnowledgesPopulationCollision( fct );
    }
    template < class UnaryFunction >
    void ApplyOnKnowledgesObjectPerception( UnaryFunction& fct ) const
    {
        if( jammedPion_ )
            jammedPion_->GetKnowledge().GetKnowledgeObjectPerceptionContainer().ApplyOnKnowledgesObjectPerception( fct );
    }

    template < class UnaryFunction >
    void ApplyOnKnowledgesObjectCollision( UnaryFunction& fct ) const
    {
        if( jammedPion_ )
            jammedPion_->GetKnowledge().GetKnowledgeObjectCollisionContainer().ApplyOnKnowledgesObjectCollision( fct );
    }
    //@}

private:
    //! @name Helpers
    //@{
    bool OnReceiveKnowledgeGroupEnable( const sword::MissionParameters& message );
    bool OnReceiveKnowledgeGroupChangeSuperior( const sword::MissionParameters& message, const tools::Resolver< MIL_Army_ABC >& armies, bool hasParent );
    bool OnReceiveKnowledgeGroupSetType( const sword::MissionParameters& message );
    unsigned long OnReceiveKnowledgeGroupAddKnowledge( const sword::MissionParameters& message );
    void OnReceiveRename( const sword::MissionParameters& parameters );

    void CreateKnowledgeFromAgentPerception( const DEC_Knowledge_Agent& agent );
    void CreateKnowledgeFromPopulationPerception( const DEC_Knowledge_Population& population );
    void CreateKnowledgeFromObjectPerception( boost::shared_ptr< DEC_Knowledge_Object >& object );

    void ApplyOnKnowledgesPopulationPerception( int currentTimeStep );
    void ApplyOnKnowledgesAgentPerception( int currentTimeStep );
    void ApplyOnKnowledgesObjectPerception( int currentTimeStep );
    void UpdatePopulationKnowledgeFromCollision( const DEC_Knowledge_PopulationCollision& collision, int currentTimeStep );
    void UpdatePopulationKnowledgeFromPerception( const DEC_Knowledge_PopulationPerception& perception, int currentTimeStep );
    void UpdateObjectKnowledgeFromCollision( const DEC_Knowledge_ObjectCollision& collision, int currentTimeStep );
    void UpdateObjectKnowledgeFromPerception( const DEC_Knowledge_ObjectPerception& perception, int currentTimeStep );
    DEC_Knowledge_Agent& GetAgentKnowledgeToUpdate( const MIL_Agent_ABC& agentKnown );
    void UpdateAgentKnowledgeFromCrowdPerception( MIL_Agent_ABC& agent, int currentTimeStep );
    void UpdateConcentrationKnowledgeFromCrowdPerception( TER_PopulationConcentration_ABC& concentration, int currentTimeStep );
    void UpdateFlowKnowledgeFromCrowdPerception( TER_PopulationFlow_ABC& flow, int currentTimeStep );
    void UpdateObjectKnowledgeFromCrowdPerception( MIL_Object_ABC& object );
    void UpdateAgentKnowledgeFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep );
    void UpdateAgentKnowledgeFromAgent( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep );
    void UpdateObjectKnowledgeFromAgent( boost::shared_ptr< DEC_Knowledge_Object >& objectKnowledge, int currentTimeStep );
    void UpdateAgentKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep );
    void UpdateObjectKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Object& objectKnowledge, int currentTimeStep );
    void UpdatePopulationKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Population& pnowledge, int currentTimeStep );
    void UpdatePopulationKnowledgeFromAgent( const DEC_Knowledge_Population& knowledge, int currentTimeStep );
    void UpdateCrowdKnowledgeFromCrowdPerception( const MIL_Population& perveicer, const MIL_Population& perceived );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Agent_ABC& agent, unsigned int perception );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Object_ABC& agent, unsigned int perception );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Population& population, unsigned int perception );
    boost::shared_ptr< DEC_Knowledge_Object > GetObjectKnowledgeToUpdate( MIL_Object_ABC& objectKnown );
    void ApplyAgentPerception( const MIL_Agent_ABC& pion, int currentTimeStep );
    void ApplyPopulationPerception( const MIL_Agent_ABC& pion, int currentTimeStep );
    void UpdateObjectPerception( const MIL_KnowledgeGroup& group, int currentTimeStep );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_KnowledgeGroupType* type_;
    unsigned int id_;
    std::string name_;
    MIL_Army_ABC* army_;
    MIL_KnowledgeGroup* parent_;
    boost::scoped_ptr< DEC_KnowledgeBlackBoard_KnowledgeGroup > knowledgeBlackBoard_;
    T_AutomateVector automates_;
    std::set< unsigned int > additionalPerceptions_;
    T_KnowledgeGroupVector knowledgeGroups_; // LTO
    T_PopulationVector populations_;
    double timeToDiffuse_; // LTO
    bool isActivated_; // LTO
    bool hasBeenUpdated_;
    bool isJammed_;
    bool createdByJamming_;
    bool crowd_;
    bool bDiffuseToKnowledgeGroup_;
    bool hasSavedCurrentKnowledge_;
    const MIL_Agent_ABC* jammedPion_;
    static MIL_IDManager idManager_;
    std::vector< const MIL_Agent_ABC* > sharingPercetionsGroup_;
    std::vector< const MIL_Agent_ABC* > sharingKnowledgesGroup_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_KnowledgeGroup )

#endif // __MIL_KnowledgeGroup_h_
