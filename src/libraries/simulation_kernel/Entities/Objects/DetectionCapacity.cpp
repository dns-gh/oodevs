// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentServer.h"
#include "AnimatorAttribute.h"
#include "DetectionCapacity.h"
#include "MIL_Singletons.h"
#include "DetectorAttribute.h"
#include "MIL_Object_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/MIL_Army_ABC.h"
#include "Knowledge/KnowledgeVisitor_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "simulation_terrain/TER_Localisation.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DetectionCapacity )

namespace
{
    template< typename T >
    bool ReadTime( xml::xistream& xis, const std::string& name, T& time )
    {
        std::string timeString;
        xis >> xml::optional >> xml::attribute( name, timeString );
        return tools::DecodeTime( timeString, time );
    }
};

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity( xml::xistream& xis )
    : rDetectionTime_( 0 ) // LTO
    , rRecognitionTime_( 0 ) // LTO
    , rIdentificationTime_( 0 ) // LTO
{
    // LTO begin
    xis >> xml::optional
        >> xml::start( "acquisition-times" )
            >> xml::list( "acquisition-time", *this, &DetectionCapacity::ReadAcquisitionTime )
        >> xml::end;
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ReadAcquisitionTime
// Created: SLG 2010-02-11
// LTO
// -----------------------------------------------------------------------------
void DetectionCapacity::ReadAcquisitionTime( xml::xistream& xis )
{
    std::string acquisitionType;
    xis >> xml::attribute( "level", acquisitionType );

    if( acquisitionType == "detection" )
    {
        if( ReadTime( xis, "time", rDetectionTime_ ) )
        {
            if( rDetectionTime_ < 0 )
                xis.error( "detection acquisition-time: base-time < 0" );
            else
                rDetectionTime_ = static_cast< float >( MIL_Tools::ConvertSecondsToSim( rDetectionTime_ ) );
        }
    }
    else if( acquisitionType == "recognition" )
    {
        if( ReadTime( xis, "time", rRecognitionTime_ ) )
        {
            if( rRecognitionTime_ < 0 )
                xis.error( "recognition acquisition-time: base-time < 0" );
            else
                rRecognitionTime_ = static_cast< float >( MIL_Tools::ConvertSecondsToSim( rRecognitionTime_ ) );
        }
    }
    else if( acquisitionType == "identification" )
    {
        if( ReadTime( xis, "time", rIdentificationTime_ ) )
        {
            if( rIdentificationTime_ < 0 )
                xis.error( "identification acquisition-time: base-time < 0" );
            else
                rIdentificationTime_ = static_cast< float >( MIL_Tools::ConvertSecondsToSim( rIdentificationTime_ ) );
        }
    }
    else
        xis.error( "Unknown acquisition-time: " + acquisitionType );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity( const DetectionCapacity& from )
    : rDetectionTime_( from.rDetectionTime_ ) // LTO
    , rRecognitionTime_( from.rRecognitionTime_ ) // LTO
    , rIdentificationTime_( from.rIdentificationTime_ ) // LTO
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity Destructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::~DetectionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DetectionCapacity::serialize
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
template< typename Archive >
void DetectionCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this )
         & rDetectionTime_ // LTO
         & rRecognitionTime_ // LTO
         & rIdentificationTime_; // LTO
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    DetectionCapacity* capacity = new DetectionCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

namespace
{
    class KnowledgeCreation : public KnowledgeVisitor_ABC
    {
    public:
        KnowledgeCreation( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& perceptionLevel )
            : agent_( agent ), perceptionLevel_( perceptionLevel )
        {}
        virtual ~KnowledgeCreation()
        {}
        virtual void visit( const MIL_KnowledgeGroup& knowledgeGroup )
        {
            std::vector<MIL_Automate* > automates = knowledgeGroup.GetAutomates();
            if( !automates.empty() )
            {
                MIL_Agent_ABC& pion = automates.front()->GetPionPC();
                PHY_RoleInterface_Perceiver& role = const_cast< MIL_Agent_ABC& >( pion ).GetRole< PHY_RoleInterface_Perceiver >();
                role.NotifyExternalPerception( agent_, perceptionLevel_ );
            }
        }
    private:
        MIL_Agent_ABC& agent_;
        const PHY_PerceptionLevel& perceptionLevel_;
    };
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentInside
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent )
{
    CIT_AgentMap it = agentInsideMap_.find( &agent );
    if( it != agentInsideMap_.end() )
    {
        int currentTime = MIL_Singletons::GetTime().GetCurrentTick();
        const AnimatorAttribute::T_AgentSet& animators = object.GetAttribute< AnimatorAttribute >().GetAnimators();
        for( AnimatorAttribute::CIT_AgentSet itAnimator = animators.begin(); itAnimator != animators.end(); ++itAnimator )
        {
            PHY_RoleInterface_Perceiver& role = const_cast< MIL_Agent_ABC& >(**itAnimator).GetRole< PHY_RoleInterface_Perceiver >();
            role.NotifyExternalPerception( agent, PHY_PerceptionLevel::identified_ );
        }

        const MIL_Agent_ABC* detector = object.GetAttribute< DetectorAttribute >().GetDetector();
        if( detector && !detector->IsDead() )
        {
            PHY_RoleInterface_Perceiver& role = const_cast< MIL_Agent_ABC& >( *detector ).GetRole< PHY_RoleInterface_Perceiver >();

        // LTO begin
            if( it->second + rIdentificationTime_ < currentTime )
                role.NotifyExternalPerception( agent, PHY_PerceptionLevel::identified_ );
            else if( it->second + rRecognitionTime_ < currentTime )
                role.NotifyExternalPerception( agent, PHY_PerceptionLevel::recognized_ );
            else if( it->second + rDetectionTime_ < currentTime )
                role.NotifyExternalPerception( agent, PHY_PerceptionLevel::detected_ );
        // LTO end
        }
        // LTO begin
        else if( !detector && object.GetArmy() )
        {
            if( it->second + rIdentificationTime_ < currentTime )
            {
                KnowledgeCreation knowledgeCreation( agent, PHY_PerceptionLevel::identified_ );
                object.GetArmy()->ApplyOnKnowledgeGroup( knowledgeCreation );
            }
            else if( it->second + rRecognitionTime_ < currentTime )
            {
                KnowledgeCreation knowledgeCreation( agent, PHY_PerceptionLevel::recognized_ );
                object.GetArmy()->ApplyOnKnowledgeGroup( knowledgeCreation );
            }
            else if( it->second + rDetectionTime_ < currentTime )
            {
                KnowledgeCreation knowledgeCreation( agent, PHY_PerceptionLevel::detected_ );
                object.GetArmy()->ApplyOnKnowledgeGroup( knowledgeCreation );
            }
        }
    // LTO end

    }
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentEntering
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    agentInsideMap_.insert( std::make_pair( &agent, MIL_Singletons::GetTime().GetCurrentTick() ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentExiting
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    agentInsideMap_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: WorkableCapacity::AddDetector
// Created: SLG 2010-02-16
// -----------------------------------------------------------------------------
void DetectionCapacity::AddDetector( MIL_Object_ABC& object, const MIL_Agent_ABC& agent )
{
    object.GetAttribute< DetectorAttribute >().AddDetector( agent );
}
