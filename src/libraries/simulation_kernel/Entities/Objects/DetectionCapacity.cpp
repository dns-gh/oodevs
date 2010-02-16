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
#include "Object.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( DetectionCapacity, "DetectionCapacity" )


namespace
{
    template< typename T >
    bool ReadTime( xml::xistream& xis, const std::string& name, T& time )
    {
        int seconds = 0;
        std::string timeString;
        xis >> xml::optional() >> xml::attribute( name, timeString );
        if( tools::DecodeTime( timeString, seconds ) )
        {
            time = seconds;
            return true;
        }
        return false;
    }
};

// -----------------------------------------------------------------------------
// Name: DetectionCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity( xml::xistream& xis )
    : rActionRange_( 0 )
    , rDetectionTime_( 0 )
    , rRecognitionTime_( 0 )
    , rIdentificationTime_( 0 )
{
    xis >> xml::optional() >> xml::attribute( "action-range", rActionRange_ );
    xis >> xml::optional()
        >> xml::start( "acquisition-times" )
            >> xml::list( "acquisition-time", *this, &DetectionCapacity::ReadAcquisitionTime )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ReadTime
// Created: SLG 2010-02-11
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
                rDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rDetectionTime_ );
        }
    }
    else if( acquisitionType == "recognition" )
    {
        if( ReadTime( xis, "time", rRecognitionTime_ ) )
        {
            if( rRecognitionTime_ < 0 )
                xis.error( "recognition acquisition-time: base-time < 0" );
            else
                rRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rRecognitionTime_ );
        }
    }
    else if( acquisitionType == "identification" )
    {
        if( ReadTime( xis, "time", rIdentificationTime_ ) )
        {
            if( rIdentificationTime_ < 0 )
                xis.error( "identification acquisition-time: base-time < 0" );
            else
                rIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rIdentificationTime_ );
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
// Name: SupplyCapacity constructor
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
DetectionCapacity::DetectionCapacity( const DetectionCapacity& from )
{
    UNREFERENCED_PARAMETER( from );
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
void DetectionCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this )
         & boost::serialization::base_object< MIL_InteractiveContainer_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::Register
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::Register( Object& object )
{
    object.AddCapacity( this );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( this ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::Instanciate
// Created: MGD 2009-03-05
// -----------------------------------------------------------------------------
void DetectionCapacity::Instanciate( Object& object ) const
{    
    DetectionCapacity* capacity = new DetectionCapacity( *this );
    object.AddCapacity( capacity );
    object.Register( static_cast< MIL_InteractiveContainer_ABC *>( capacity ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentInside
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentInside( Object& object, MIL_Agent_ABC& agent )
{
    CIT_AgentMap it = agentInsideMap_.find( &agent );
    if ( it != agentInsideMap_.end() )
    {
        int currentTime = MIL_Singletons::GetTime().GetCurrentTick();
        const AnimatorAttribute::T_AgentSet& animators = object.GetAttribute< AnimatorAttribute >().GetAnimators();

        for( AnimatorAttribute::CIT_AgentSet itAnimator = animators.begin(); itAnimator != animators.end(); ++itAnimator )
        {
            PHY_RoleInterface_Perceiver& role = const_cast< MIL_Agent_ABC& >(**itAnimator).GetRole< PHY_RoleInterface_Perceiver >();
            if( it->second + rIdentificationTime_ < currentTime )
                role.NotifyExternalPerception( agent, PHY_PerceptionLevel::identified_ );
            else if( it->second + rRecognitionTime_ < currentTime )
                role.NotifyExternalPerception( agent, PHY_PerceptionLevel::recognized_ );
            else if( it->second + rDetectionTime_ < currentTime )
                role.NotifyExternalPerception( agent, PHY_PerceptionLevel::detected_ );
        }
    }
}


// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentEntering
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentEntering( Object& /*object*/, MIL_Agent_ABC& agent )
{
    agentInsideMap_.insert( std::make_pair( &agent, MIL_Singletons::GetTime().GetCurrentTick() ) );
}

// -----------------------------------------------------------------------------
// Name: DetectionCapacity::ProcessAgentExiting
// Created: SLG 2010-02-11
// -----------------------------------------------------------------------------
void DetectionCapacity::ProcessAgentExiting( Object& /*object*/, MIL_Agent_ABC& agent ) 
{
    agentInsideMap_.erase( &agent );
}
  