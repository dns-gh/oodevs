// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MobilityCapacity.h"
#include "Object.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( MobilityCapacity, "MobilityCapacity" )

// -----------------------------------------------------------------------------
// Name: MobilityCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
MobilityCapacity::MobilityCapacity( xml::xistream& xis )
{
    InitializeSpeed( xis );
    InitializeSpeedPolicy( xis );
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
MobilityCapacity::MobilityCapacity( )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::InitializeSpeed
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MobilityCapacity::InitializeSpeed( xml::xistream& xis )
{
    xis >> xml::attribute( "default-speed", rDefaultSpeed_ );
    if( rDefaultSpeed_ >= 0. )
        rDefaultSpeed_ = MIL_Tools::ConvertSpeedMosToSim( rDefaultSpeed_ );
    else
        rDefaultSpeed_ = std::numeric_limits< MT_Float >::max();
}
    
// -----------------------------------------------------------------------------
// Name: MobilityCapacity::InitializeSpeedPolicy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
void MobilityCapacity::InitializeSpeedPolicy( xml::xistream& xis )
{
    std::string strSpeedPolicy( xml::attribute( xis, "unit-speed-impact-mode", std::string() ) );
    if( sCaseInsensitiveEqual()( strSpeedPolicy, "AuPlusLent" ) )
        nSpeedPolicy_ = eSpeedPolicy_Slowest;
    else if( sCaseInsensitiveEqual()( strSpeedPolicy, "VitesseMaxObjet" ) )
        nSpeedPolicy_ = eSpeedPolicy_ObjectMaxSpeed;
    else if( sCaseInsensitiveEqual()( strSpeedPolicy, "VitesseMaxAgent" ) )
    {
        nSpeedPolicy_ = eSpeedPolicy_AgentMaxSpeed;
        xis >> xml::attribute( "max-unit-percentage-speed", rSpeedPolicyMaxSpeedAgentFactor_ );
        if( rSpeedPolicyMaxSpeedAgentFactor_ < 0 || rSpeedPolicyMaxSpeedAgentFactor_ > 100 )
            xis.error( "max-unit-percentage-speed not in [0..100]" );
        rSpeedPolicyMaxSpeedAgentFactor_ /= 100.;
    }
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
MobilityCapacity::MobilityCapacity( const MobilityCapacity& from )
    : nSpeedPolicy_ ( from.nSpeedPolicy_ )
    , rDefaultSpeed_ ( from.rDefaultSpeed_ )
    , rSpeedPolicyMaxSpeedAgentFactor_ ( rSpeedPolicyMaxSpeedAgentFactor_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
MobilityCapacity::~MobilityCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::serialize
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void MobilityCapacity::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
    file & nSpeedPolicy_
         & rDefaultSpeed_
         & rSpeedPolicyMaxSpeedAgentFactor_;
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::Register
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void MobilityCapacity::Register( Object& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::Instanciate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void MobilityCapacity::Instanciate( Object& object ) const
{
    object.AddCapacity( new MobilityCapacity( *this ) );
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::GetMaxSpeed
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
MT_Float MobilityCapacity::GetDefaultSpeed() const
{
    return rDefaultSpeed_;
}

// -----------------------------------------------------------------------------
// Name: MobilityCapacity::ApplySpeedPolicy
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
MT_Float MobilityCapacity::ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const
{
    assert( rAgentSpeedWithinObject      <= rAgentMaxSpeed );
    assert( rAgentSpeedWithinEnvironment <= rAgentMaxSpeed );

    switch( nSpeedPolicy_ )
    {
        case eSpeedPolicy_Slowest        : return std::min( rAgentSpeedWithinObject, rAgentSpeedWithinEnvironment );
        case eSpeedPolicy_AgentMaxSpeed  : return rAgentMaxSpeed * rSpeedPolicyMaxSpeedAgentFactor_;
        case eSpeedPolicy_ObjectMaxSpeed : return rAgentSpeedWithinObject; // rDefaultSpeed_ ?
    }
    return 0.;
}
