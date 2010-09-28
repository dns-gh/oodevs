// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FireAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeFire.h"
#include "MIL_AgentServer.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( FireAttribute )

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute()
    : heat_        ( 0 )
    , pClass_    ( 0 )
    , timeOfLastUpdate_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , width_            ( 0 )
    , length_           ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::FireAttribute
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( xml::xistream& xis )
    : heat_        ( 0 )
    , pClass_    ( 0 )
    , timeOfLastUpdate_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
    , width_            ( MIL_FireClass::GetWidth() )
    , length_           ( MIL_FireClass::GetLength() )
{
    std::string className( xml::attribute( xis, "class", std::string() ) );
    pClass_ = MIL_FireClass::Find( className );
    if( !pClass_ )
        xis.error( "Unknown 'Fire class' '" + className + "' for fire object attribute" );
    heat_ = pClass_->GetDefaultHeat();
    xis >> xml::optional
        >> xml::attribute( "heat", heat_ );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const Common::MsgMissionParameter_Value& attributes )
    : heat_        ( 0 )
    , pClass_    ( 0 )
    , width_    ( MIL_FireClass::GetWidth() )
    , length_   ( MIL_FireClass::GetLength() )
    , timeOfLastUpdate_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
    pClass_ = MIL_FireClass::Find( attributes.list( 1 ).identifier() );
    if( !pClass_ )
        throw std::runtime_error( "Unknown 'Fire class' for fire object attribute" );
    heat_ = pClass_->GetDefaultHeat();
//    asn.fire.heat
}

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const FireAttribute& attr )
    : pClass_    ( attr.pClass_ )
    , heat_        ( attr.pClass_->GetDefaultHeat() )
    , width_    ( attr.width_ )
    , length_   ( attr.length_ )
    , timeOfLastUpdate_ ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
FireAttribute& FireAttribute::operator=( const FireAttribute& rhs )
{
    timeOfLastUpdate_ = rhs.timeOfLastUpdate_;
    heat_ = rhs.heat_;
    pClass_ = rhs.pClass_;
    width_ = rhs.width_;
    length_ = rhs.length_;
    NotifyAttributeUpdated( eOnCreation );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void FireAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::string className;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> className
       >> heat_
       >> timeOfLastUpdate_
       >> width_
       >> length_;
    pClass_ = MIL_FireClass::Find( className );
    if( !pClass_ )
        throw std::runtime_error( "Unknown 'Fire class' '" + className + "' for fire object attribute" );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void FireAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << pClass_->GetName()
       << heat_
       << timeOfLastUpdate_
       << width_
       << length_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Instanciate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void FireAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeFire( *this ) );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Register
// Created: JSR 2010-03-12
// -----------------------------------------------------------------------------
void FireAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< FireAttribute, FireAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void FireAttribute::SendFullState( Common::ObjectAttributes& asn ) const
{
//    asn.set_firePresent( 1 );
    asn.mutable_fire()->set_heat( heat_ );
    asn.mutable_fire()->set_class_id( pClass_->GetID() );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void FireAttribute::SendUpdate( Common::ObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        SendFullState( asn );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::WriteODB
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void FireAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "fire" )
            << xml::attribute( "heat", heat_ )
            << xml::attribute( "class", pClass_->GetName() )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::ComputeHeatNaturalEvolution
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void FireAttribute::ComputeHeatEvolution( unsigned int initial, unsigned int time )
{
    //Compute the new temperature and send a notification if it has changed
    int heat = pClass_->ComputeHeatEvolution( heat_, initial, timeOfLastUpdate_ );
    UpdateHeat( heat, time );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::UpdateHeat
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void FireAttribute::ComputeHeatWhenExtinguishing( MIL_FireClass::E_FireExtinguisherAgent extinguisherAgent, int numberOfFireHoses )
{
    //Compute the new temperature and send a notification if it has changed
    int heat = pClass_->ComputeHeatWhenExtinguishing( heat_, extinguisherAgent, numberOfFireHoses );
    UpdateHeat( heat, MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() );
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::UpdateHeat
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void FireAttribute::UpdateHeat( int heat, unsigned int time )
{
    if( heat != heat_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        heat_ = heat;
    }
    timeOfLastUpdate_ = time;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::UpdateHeat
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
int FireAttribute::GetHeat() const
{
    return heat_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::UpdateHeat
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
const MIL_FireClass& FireAttribute::GetClass() const
{
    return *pClass_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetWidth
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
unsigned int FireAttribute::GetWidth() const
{
    return width_;
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::GetLength
// Created: RFT 2008-06-09
// -----------------------------------------------------------------------------
unsigned int FireAttribute::GetLength() const
{
    return length_;
}