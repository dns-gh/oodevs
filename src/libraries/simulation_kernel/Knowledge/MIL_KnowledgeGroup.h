// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
#include "tools/Resolver.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/export.hpp>
#include <boost/shared_ptr.hpp>

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
class KnowledgeGroupFactory_ABC;
class MIL_Agent_ABC;
class MIL_Army_ABC;
class MIL_Automate;
class MIL_Object;
class MIL_Object_ABC;
class MIL_Population;
class PHY_PerceptionLevel;
class KnowledgesVisitor_ABC;

namespace sword
{
    class MissionParameters;
    class KnowledgeGroupCreation;
    class KnowledgeGroupUpdate;
    class KnowledgeGroupCreationRequest;
    class KnowledgeMagicAction;
}

// =============================================================================
// @class  MIL_KnowledgeGroup
// Created: JVT 2004-08-03
// =============================================================================
class MIL_KnowledgeGroup : private boost::noncopyable
    , public boost::enable_shared_from_this< MIL_KnowledgeGroup >
{
public:
    //! @name Types
    //@{
    typedef std::vector< MIL_Automate* >       T_AutomateVector;
    typedef T_AutomateVector::iterator        IT_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;

    typedef std::vector< boost::shared_ptr< MIL_KnowledgeGroup > > T_KnowledgeGroupVector;
    typedef T_KnowledgeGroupVector::iterator         IT_KnowledgeGroupVector;
    typedef T_KnowledgeGroupVector::const_iterator  CIT_KnowledgeGroupVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, unsigned int id, MIL_Army_ABC& army );
             MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army_ABC& army, boost::shared_ptr< MIL_KnowledgeGroup >& parent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory ); // LTO
             MIL_KnowledgeGroup();
             MIL_KnowledgeGroup( const MIL_KnowledgeGroup& source, const MIL_Agent_ABC& pion, boost::shared_ptr< MIL_KnowledgeGroup >& parent );
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
    void Clone( const MIL_KnowledgeGroup& source );
    void RegisterKnowledgeGroup( boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup );
    void InitializeKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    void UnregisterKnowledgeGroup( boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup );
    boost::shared_ptr< MIL_KnowledgeGroup > FindKnowledgeGroup ( unsigned int id ) const;
    void SetType( MIL_KnowledgeGroupType *type ){ type_ = type; }
    void RefreshTimeToDiffuseToKnowledgeGroup();
    void RegisterAutomate  ( MIL_Automate& automate );
    void UnregisterAutomate( MIL_Automate& automate );
    void RegisterPion( const MIL_Agent_ABC& agent );
    void UnregisterPion( const MIL_Agent_ABC& agent );

    void AppendAddedKnowledge( TER_Agent_ABC::T_AgentPtrVector& perceivableAgents, TER_Object_ABC::T_ObjectVector& perceivableObjects, TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector& perceivablePopulationDensity, TER_PopulationFlow_ABC::T_ConstPopulationFlowVector& perceivablePopulationFlow ) const;

    void UpdateKnowledges(int currentTimeStep);
    void UpdateObjectKnowledges(int currentTimeStep);
    void CleanKnowledges ();
    bool IsPerceived     ( const DEC_Knowledge_Object& knowledge ) const;
    bool IsPerceived     ( const DEC_Knowledge_Agent&  knowledge ) const;
    bool IsPerceptionDistanceHacked( MIL_Agent_ABC& agentKnown ) const;
    bool IsPerceptionDistanceHacked( MIL_Object_ABC& objectKnown ) const;
    bool IsPerceptionDistanceHacked( MIL_Population& populationKnown ) const;
    bool CanReport() const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Agent_ABC& agentKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Object_ABC& ObjectKnown ) const;
    const PHY_PerceptionLevel& GetPerceptionLevel( MIL_Population& populationKnown ) const;

    bool operator==( const MIL_KnowledgeGroup& rhs ) const;
    bool operator!=( const MIL_KnowledgeGroup& rhs ) const;

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
    const DEC_KnowledgeBlackBoard_KnowledgeGroup* GetKnowledge() const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( unsigned int ) const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObject( const MIL_Object_ABC& object ) const;
    boost::shared_ptr< DEC_Knowledge_Object > ResolveKnowledgeObjectByObjectID( unsigned int ) const;
    // LTO begin
    const T_KnowledgeGroupVector&                 GetKnowledgeGroups() const;
          double                                GetTimeToDiffuseToKnowledgeGroup() const;
          bool                                    IsEnabled() const;
          void                                    SetParent( boost::shared_ptr< MIL_KnowledgeGroup >& parent );
    // LTO end
          bool IsJammed() const;
          void Accept( KnowledgesVisitor_ABC& visitor ) const;
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

    boost::shared_ptr< DEC_Knowledge_Object > CreateKnowledgeObject( MIL_Object_ABC& perceived );
    DEC_Knowledge_Agent& CreateKnowledgeAgent ( const MIL_Agent_ABC& perceived );
    DEC_Knowledge_Population& CreateKnowledgePopulation( MIL_Population& perceived );
    void UpdateKnowledgeFromTransported( const MIL_Agent_ABC& perceived );

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
    void CreateKnowledgeFromObjectPerception( boost::shared_ptr< DEC_Knowledge_Object >& object );

    void ApplyOnKnowledgesPopulationPerception( int currentTimeStep );
    void ApplyOnKnowledgesAgentPerception( int currentTimeStep );
    DEC_Knowledge_Population& GetPopulationKnowledgeToUpdate( MIL_Population& populationKnown );
    void UpdatePopulationKnowledgeFromCollision( const DEC_Knowledge_PopulationCollision& collision, int currentTimeStep  );
    void UpdatePopulationKnowledgeFromPerception( const DEC_Knowledge_PopulationPerception& perception, int currentTimeStep  );
    DEC_Knowledge_Agent& GetAgentKnowledgeToUpdate( const MIL_Agent_ABC& agentKnown );
    void UpdateAgentKnowledgeFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep );
    void UpdateAgentKnowledgeFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Agent_ABC& agent, unsigned int perception );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Object_ABC& agent, unsigned int perception );
    void HackPerceptionLevelFromParentKnowledgeGroup( MIL_Population& population, unsigned int perception );
    boost::shared_ptr< DEC_Knowledge_Object > GetObjectKnowledgeToUpdate( MIL_Object_ABC& objectKnown );
    void ApplyAgentPerception( const MIL_Agent_ABC& pion, int currentTimeStep );
    void ApplyPopulationPerception( const MIL_Agent_ABC& pion, int currentTimeStep );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_KnowledgeGroupType* type_;
    unsigned int id_;
    std::string name_;
    MIL_Army_ABC* army_;
    MIL_KnowledgeGroup* parent_;
    DEC_KnowledgeBlackBoard_KnowledgeGroup* knowledgeBlackBoard_;
    T_AutomateVector automates_;
    std::set< unsigned int > additionalPerceptions_;
    T_KnowledgeGroupVector knowledgeGroups_; // LTO
    double timeToDiffuse_; // LTO
    bool isActivated_; // LTO
    bool hasBeenUpdated_;
    bool isJammed_;
    bool createdByJamming_;
    const MIL_Agent_ABC* jammedPion_;
    static MIL_IDManager idManager_;

    std::vector< const MIL_Agent_ABC* > pions_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_KnowledgeGroup )

#endif // __MIL_KnowledgeGroup_h_
