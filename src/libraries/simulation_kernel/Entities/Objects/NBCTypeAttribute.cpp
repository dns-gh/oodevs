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
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeNBCType.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( NBCTypeAttribute, "NBCTypeAttribute" )

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
    std::string agentType( xml::attribute( xis, "type", std::string() ) );
    pAgent_ = MIL_NBCType::Find( agentType );
    if( !pAgent_ )
        xis.error( "Unknown 'AgentNBC' '" + agentType + "' for NBC object" );
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
NBCTypeAttribute::NBCTypeAttribute( const ASN1T_ObjectAttributes& asn )
    : pAgent_            ( 0 )
    , concentration_     ( 0 )
    , width_             ( 0 )
    , length_            ( 0 )
    , propagationAngle_  ( 0 )
    , sourceLifeDuration_( 0 /*asn.nbc_agent.source_life_duration*/ )
{
    // pAgent_ = MIL_NBCType::Find( asn.nbc_agent.agent_id );
    if( !pAgent_ )
        throw std::runtime_error( "Unknown 'NBC agent' for nbc object attribute" );	

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
    : pAgent_          ( attr.pAgent_ )
    , concentration_   ( attr.GetConcentration() )
    , width_           ( attr.GetWidth() )
    , length_          ( attr.GetLength() )
    , propagationAngle_( attr.GetPropagationAngle() )
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

    NotifyAttributeUpdated( eOnCreation );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::load
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void NBCTypeAttribute::load( MIL_CheckPointInArchive& ar, const uint )
{
    std::string agentName;
    
    ar >> agentName
       >> concentration_;     

    pAgent_ = MIL_NBCType::Find( agentName );
    if( !pAgent_ )
        throw std::runtime_error( "Unknown 'nbc agent' '" + agentName + "' for nbc object attribute" );	
}
    
// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::save
// Created: RFT 2008-07-03
// -----------------------------------------------------------------------------
void NBCTypeAttribute::save( MIL_CheckPointOutArchive& ar, const uint ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );        
    ar << pAgent_->GetName()
       << concentration_;    
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Instanciate
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeNBCType( *this ) );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::SendFullState
// Created: RFT 2008-06-18
// -----------------------------------------------------------------------------
void NBCTypeAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
//    asn.m.nbc_agentPresent      = 1;
//    asn.nbc_agent.concentration = concentration_;
//    asn.nbc_agent.agent_id      = pAgent_->GetID();
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Send
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
void NBCTypeAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        SendFullState( asn );        
        Reset();
    }
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
        << xml::end();
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