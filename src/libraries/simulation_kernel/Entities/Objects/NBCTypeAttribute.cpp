// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NBCTypeAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( NBCTypeAttribute )

BOOST_CLASS_EXPORT( DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< NBCTypeAttribute > )

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute constructor
// Created: RFT 2008-06-08
// -----------------------------------------------------------------------------
NBCTypeAttribute::NBCTypeAttribute( xml::xistream& xis )
    : pAgent_            ( 0 )
    , concentration_     ( 0 )
    , width_             ( 0 )
    , length_            ( 0 )
    , propagationAngle_  ( 0 )
    , sourceLifeDuration_( 0 )
{
    std::string agentType( xis.attribute< std::string >( "type", std::string() ) );
    pAgent_ = MIL_NBCType::Find( agentType );
    if( !pAgent_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown 'AgentNBC' '" + agentType + "' for NBC object" );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute constructor
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
NBCTypeAttribute::NBCTypeAttribute()
    : pAgent_            ( 0 )
    , concentration_     ( 0 )
    , width_             ( 0 )
    , length_            ( 0 )
    , propagationAngle_  ( 0 )
    , sourceLifeDuration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute constructor
// Created: RFT 2008-07-21
// -----------------------------------------------------------------------------
NBCTypeAttribute::NBCTypeAttribute( const sword::ObjectAttributes& /*asn*/ )
    : pAgent_            ( 0 )
    , concentration_     ( 0 )
    , width_             ( 0 )
    , length_            ( 0 )
    , propagationAngle_  ( 0 )
    , sourceLifeDuration_( 0 /*asn.nbc_agent.source_life_duration*/ )
{
    // pAgent_ = MIL_NBCType::Find( asn.nbc_agent.agent_id );
    if( !pAgent_ )
        throw MASA_EXCEPTION( "Unknown 'NBC agent' for nbc object attribute" );

    // width_            = MIL_NBCType::GetWidth();
    // length_           = MIL_NBCType::GetLength();
    // propagationAngle_ = MIL_NBCType::GetPropagationAngle();
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute destructor
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
NBCTypeAttribute::~NBCTypeAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute constructor
// Created: RFT 2008-07-21
// -----------------------------------------------------------------------------
NBCTypeAttribute::NBCTypeAttribute( const NBCTypeAttribute& attr )
    : pAgent_( attr.pAgent_ )
    , concentration_( attr.GetConcentration() )
    , width_( attr.GetWidth() )
    , length_( attr.GetLength() )
    , propagationAngle_( attr.GetPropagationAngle() )
    , sourceLifeDuration_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::operator=
// Created: RFT 2008-05-30
// -----------------------------------------------------------------------------
NBCTypeAttribute& NBCTypeAttribute::operator=( const NBCTypeAttribute& rhs )
{
    concentration_    = rhs.concentration_;
    pAgent_           = rhs.pAgent_;
    width_            = rhs.width_;
    length_           = rhs.length_;
    propagationAngle_ = rhs.propagationAngle_;

    NotifyAttributeUpdated( eOnUpdate );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::load
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void NBCTypeAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string agentName;

    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> agentName;
    ar >> concentration_;

    pAgent_ = MIL_NBCType::Find( agentName );
    if( !pAgent_ )
        throw MASA_EXCEPTION( "Unknown 'nbc agent' '" + agentName + "' for nbc object attribute" );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::save
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void NBCTypeAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << pAgent_->GetName();
    ar << concentration_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Instanciate
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< NBCTypeAttribute > >(
        *new DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< NBCTypeAttribute >() );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void NBCTypeAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< NBCTypeAttribute, NBCTypeAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::SendFullState
// Created: RFT 2008-06-18
// -----------------------------------------------------------------------------
void NBCTypeAttribute::SendFullState( sword::ObjectAttributes& /*message*/ ) const
{
//    asn.m.nbc_agentPresent      = 1;
//    asn.nbc_agent.concentration = concentration_;
//    asn.nbc_agent.agent_id      = pAgent_->GetID();
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Send
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
bool NBCTypeAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) )
    {
        SendFullState( asn );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::WriteODB
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "nbc" )
            << xml::attribute( "concentration"  , concentration_ )
            << xml::attribute( "agent"          , pAgent_->GetName() )
        << xml::end;
}

bool NBCTypeAttribute::IsSource() const
{
    return ( sourceLifeDuration_ > 0 );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::ComputeHeatNaturalEvolution
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::UpdateSourceLifeDuration( float time , float timeOfLastUpdate )
{
    sourceLifeDuration_ -= ( int )( time - timeOfLastUpdate );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::ComputeHeatNaturalEvolution
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::ComputeAgentConcentrationEvolution( bool bHasASource )
{
    //Compute the new temperature and send a notification if it has changed
    int concentration = pAgent_->ComputeAgentConcentrationEvolution( bHasASource , concentration_ );
    UpdateConcentration( concentration );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::UpdateHeat
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::UpdateConcentration( int concentration )
{
    if( concentration != concentration_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        concentration_ = concentration;
    }
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::GetConcentration
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
int NBCTypeAttribute::GetConcentration() const
{
    return concentration_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::GetAgentType
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
const MIL_NBCType& NBCTypeAttribute::GetAgentType() const
{
    return *pAgent_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::GetSourceLifeDuration
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
int NBCTypeAttribute::GetSourceLifeDuration() const
{
    return sourceLifeDuration_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::GetWidth
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
unsigned int NBCTypeAttribute::GetWidth() const
{
    return width_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::GetLength
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
unsigned int NBCTypeAttribute::GetLength() const
{
    return length_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::GetPropagationAngle
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
unsigned int NBCTypeAttribute::GetPropagationAngle() const
{
    return propagationAngle_;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Update
// Created: ABR 2011-08-05
// -----------------------------------------------------------------------------
bool NBCTypeAttribute::Update( const NBCTypeAttribute& rhs )
{
    if( concentration_ != rhs.concentration_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        concentration_ = rhs.concentration_;
    }
    if( pAgent_ != rhs.pAgent_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        pAgent_ = rhs.pAgent_;
    }
    if( width_ != rhs.width_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        width_ = rhs.width_;
    }
    if( length_ != rhs.length_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        length_ = rhs.length_;
    }
    if( propagationAngle_ != rhs.propagationAngle_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        propagationAngle_ = rhs.propagationAngle_;
    }
    return NeedUpdate( eOnUpdate );
}
