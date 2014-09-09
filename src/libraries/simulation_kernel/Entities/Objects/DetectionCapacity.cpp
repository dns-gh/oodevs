// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DetectionCapacity.h"
#include "AnimatorAttribute.h"
#include "DetectorAttribute.h"
#include "MIL_Time_ABC.h"
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
#include "tools/Codec.h"

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
                throw MASA_EXCEPTION( xis.context() + "detection acquisition-time: base-time < 0" );
            else
                rDetectionTime_ = static_cast< float >( MIL_Tools::ConvertSecondsToSim( rDetectionTime_ ) );
        }
    }
    else if( acquisitionType == "recognition" )
    {
        if( ReadTime( xis, "time", rRecognitionTime_ ) )
        {
            if( rRecognitionTime_ < 0 )
                throw MASA_EXCEPTION( xis.context() + "recognition acquisition-time: base-time < 0" );
            else
                rRecognitionTime_ = static_cast< float >( MIL_Tools::ConvertSecondsToSim( rRecognitionTime_ ) );
        }
    }
    else if( acquisitionType == "identification" )
    {
        if( ReadTime( xis, "time", rIdentificationTime_ ) )
        {
            if( rIdentificationTime_ < 0 )
                throw MASA_EXCEPTION( xis.context() + "identification acquisition-time: base-time < 0" );
            else
                rIdentificationTime_ = static_cast< float >( MIL_Tools::ConvertSecondsToSim( rIdentificationTime_ ) );
        }
    }
    else
        throw MASA_EXCEPTION( xis.context() + "Unknown acquisition-time: " + acquisitionType );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity()
    : rDetectionTime_( 0 )
    , rRecognitionTime_( 0 )
    , rIdentificationTime_( 0 )
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
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
    file & rDetectionTime_;
    file & rRecognitionTime_;
    file & rIdentificationTime_;
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
                MIL_Agent_ABC* pion = automates.front()->GetPionPC();
                if( pion )
                {
                    PHY_RoleInterface_Perceiver& role = const_cast< MIL_Agent_ABC& >( *pion ).GetRole< PHY_RoleInterface_Perceiver >();
                    role.NotifyExternalPerception( agent_, perceptionLevel_ );
                }
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
    auto it = agentInsideMap_.find( &agent );
    if( it == agentInsideMap_.end() )
        return;
    int currentTime = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    int detectionTime = it->second;
    auto animators = object.GetAttribute< AnimatorAttribute >().GetAnimators();
    const MIL_Agent_ABC* detector = object.GetAttribute< DetectorAttribute >().GetDetector();
    if( detector )
        animators.insert( detector );
    for( auto it = animators.begin(); it != animators.end(); ++it )
    {
        MIL_Agent_ABC* animator = const_cast< MIL_Agent_ABC* >( *it );
        if( animator && !animator->IsDead() )
        {
                PHY_RoleInterface_Perceiver& role = animator->GetRole< PHY_RoleInterface_Perceiver >();
                if( detectionTime + rIdentificationTime_ < currentTime )
                    role.NotifyExternalPerception( agent, PHY_PerceptionLevel::identified_ );
                else if( detectionTime + rRecognitionTime_ < currentTime )
                    role.NotifyExternalPerception( agent, PHY_PerceptionLevel::recognized_ );
                else if( detectionTime + rDetectionTime_ < currentTime )
                    role.NotifyExternalPerception( agent, PHY_PerceptionLevel::detected_ );
        }
    }
    int maxAnimators = object.GetAttribute< AnimatorAttribute >().GetMaxAnimators();
    if( animators.empty() && !maxAnimators && object.GetArmy() )
    {
        if( detectionTime + rIdentificationTime_ < currentTime )
        {
            KnowledgeCreation knowledgeCreation( agent, PHY_PerceptionLevel::identified_ );
            object.GetArmy()->ApplyOnKnowledgeGroup( knowledgeCreation );
        }
        else if( detectionTime + rRecognitionTime_ < currentTime )
        {
            KnowledgeCreation knowledgeCreation( agent, PHY_PerceptionLevel::recognized_ );
            object.GetArmy()->ApplyOnKnowledgeGroup( knowledgeCreation );
        }
        else if( detectionTime + rDetectionTime_ < currentTime )
        {
            KnowledgeCreation knowledgeCreation( agent, PHY_PerceptionLevel::detected_ );
            object.GetArmy()->ApplyOnKnowledgeGroup( knowledgeCreation );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentEntering
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& agent )
{
    agentInsideMap_.insert( std::make_pair( &agent, MIL_Time_ABC::GetTime().GetCurrentTimeStep() ) );
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
