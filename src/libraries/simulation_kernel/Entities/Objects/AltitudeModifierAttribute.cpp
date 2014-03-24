// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AltitudeModifierAttribute.h"
#include "ObjectAttributes.h"
#include "MIL_AgentServer.h"
#include "MIL_Object_ABC.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "protocol/Protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( AltitudeModifierAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< AltitudeModifierAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< AltitudeModifierAttribute > )

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute()
    : height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( const xml::xistream& xis )
    : height_( xis.attribute< int >( "height" ) )
{
    if( height_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "height_ is not greater than or equal to 0" );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute constructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::AltitudeModifierAttribute( const sword::MissionParameter_Value& attributes, const TER_Localisation& localisation, unsigned int objectId )
{
    CheckCount( "altitude_modifier", attributes, 2 );
    height_ = GetQuantity( "altitude_modifier", attributes, 1 );
    ModifyAltitude( localisation, objectId );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute destructor
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute::~AltitudeModifierAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::serialize
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
template< typename Archive >
void AltitudeModifierAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this )
         & height_;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Register
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< AltitudeModifierAttribute, AltitudeModifierAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Instanciate
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< AltitudeModifierAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::WriteODB
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "altitude-modifier" )
            << xml::attribute( "height", height_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SendFullState
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_altitude_modifier()->set_height( height_ );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SendUpdate
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
bool AltitudeModifierAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
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
// Name: AltitudeModifierAttribute::operator=
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
AltitudeModifierAttribute& AltitudeModifierAttribute::operator=( const AltitudeModifierAttribute& from )
{
    height_ = from.height_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::Update
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
bool AltitudeModifierAttribute::Update( const AltitudeModifierAttribute& rhs )
{
    if( height_ != rhs.height_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        height_ = rhs.height_;
    }
    return NeedUpdate( eOnUpdate );
}

namespace
{
    PHY_RawVisionData& GetRawVisionData()
    {
        return const_cast< PHY_RawVisionData& >( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() );
    }
}
// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::ModifyAltitude
// Created: JSR 2011-05-19
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::ModifyAltitude( const TER_Localisation& localisation, unsigned int objectId ) const
{
    GetRawVisionData().ModifyAltitude( localisation, static_cast< short >( height_ ), objectId );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::ResetAltitude
// Created: JSR 2011-10-07
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::ResetAltitude( const TER_Localisation& localisation, unsigned int objectId ) const
{
    GetRawVisionData().ModifyAltitude( localisation, 0, objectId );
}

// -----------------------------------------------------------------------------
// Name: AltitudeModifierAttribute::SetHeight
// Created: MMC 2012-04-27
// -----------------------------------------------------------------------------
void AltitudeModifierAttribute::SetHeight( unsigned int height )
{
    if( height_ != height )
    {
        NotifyAttributeUpdated( eOnUpdate );
        height_ = height;
    }
}