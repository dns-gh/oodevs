// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RawDataHazardContourGroup.h"
#include "AttributesUpdater.h"
#include "ObjectListenerComposite.h"
#include "SerializationTools.h"
#include "TacticalObject_ABC.h"
#include "FOM_Serializer_ABC.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"

#pragma warning( push, 0 )
#include "protocol/proto/common.pb.h"
#pragma warning( pop )

#include <geocoord/Geodetic.h>
#include <geocoord/PlanarCartesian.h>
#include <hla/Deserializer_ABC.h>
#include <hla/Serializer_ABC.h>
#include <hla/AttributeIdentifier.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <numeric>

using namespace plugins::hla;

namespace
{

void ReadForceIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::ForceIdentifier& force )
{
    int8_t tmpForce;
    deserializer >> tmpForce;
    listener.SideChanged( identifier, static_cast< rpr::ForceIdentifier >( tmpForce ) );
    force = static_cast< rpr::ForceIdentifier >( tmpForce );
}
void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityType& type )
{
    type.Deserialize( deserializer );
    listener.TypeChanged( identifier, type );
}
void ReadEntityIdentifier( ::hla::Deserializer_ABC& deserializer, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/, rpr::EntityIdentifier& entityId )
{
    entityId.Deserialize( deserializer );
}
void ReadNothing( ::hla::Deserializer_ABC& /*deserializer*/, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/ )
{
    // NOTHING
}
void ReadSpatial( ::hla::Deserializer_ABC& , const std::string& , ObjectListener_ABC& , Spatial&  )
{
}
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup constructor
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
RawDataHazardContourGroup::RawDataHazardContourGroup( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/,
        const rpr::EntityIdentifier& /*entityId*/, const std::string& rtiId )
    : object_( &object )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( rtiId )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
    object_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup constructor
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
RawDataHazardContourGroup::RawDataHazardContourGroup( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup destructor
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
RawDataHazardContourGroup::~RawDataHazardContourGroup()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Serialize
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Deserialize
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::GetIdentifier
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
const std::string& RawDataHazardContourGroup::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Register
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Unregister
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::RegisterAttributes
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::RegisterAttributes()
{
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::SpatialChanged
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::SpatialChanged( const TacticalObjectEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;

}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::ResetAttributes
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Attach
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::ResourcesChanged
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::ResourcesChanged( const TacticalObjectEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}
