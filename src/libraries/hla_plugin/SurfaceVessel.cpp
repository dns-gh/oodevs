// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "SurfaceVessel.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AttributesUpdater.h"
#include "MarkingFactory_ABC.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"
#include "EntityIdentifierResolver_ABC.h"
#include "FOM_Serializer_ABC.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
   void ReadEntityType( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, rpr::EntityType& type )
   {
       type.Deserialize( deserializer );
       listener.TypeChanged( identifier, type );
       listener.EquipmentUpdated( identifier, type, 1 );
   }
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVessel::SurfaceVessel( Agent_ABC& agent, unsigned long identifier,
                              const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                              unsigned short siteID, unsigned short applicationID, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
    : identifier_( name )
    , listeners_ ( new ObjectListenerComposite() )
    , agent_     ( &agent )
    , entityIdentifierResolver_ ( entityIdentifierResolver )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater(identifier_, *listeners_) )
    , simIdentifier_ ( identifier )
    , force_ ( force )
    , type_ ( type )
    , marking_( markingFactory.CreateMarking( identifier_, identifier ) )
    , entityIdentifier_( siteID, applicationID, identifier )
    , spatial_ ( true, 0., 0., 0., 0., 0. )
{
    RegisterAttributes();
    agent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
SurfaceVessel::SurfaceVessel( const std::string& identifier, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
    : identifier_( identifier )
    , listeners_ ( new ObjectListenerComposite() )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater(identifier_, *listeners_) )
    , agent_ ( 0 )
    , entityIdentifierResolver_ ( entityIdentifierResolver )
    , spatial_ ( true, 0., 0., 0., 0., 0. )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel destructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVessel::~SurfaceVessel()
{
    if( agent_ )
        agent_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Serialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributesUpdater_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Deserialize
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributesUpdater_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::SpatialChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    spatial_.Refresh( true, latitude, longitude, altitude, speed, direction );
    attributesUpdater_->Update( "Spatial", spatial_ );
    listeners_->Moved( identifier_, latitude, longitude );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::FormationChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::EquipmentChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::EmbarkmentChanged
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
void SurfaceVessel::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void SurfaceVessel::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void SurfaceVessel::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::RegisterAttributes
// Created: AHC 2012-03-07
// -----------------------------------------------------------------------------
void SurfaceVessel::RegisterAttributes( )
{
    attributesUpdater_->Register( "EntityType", boost::bind( &ReadEntityType, _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributesUpdater_->Register( "EntityIdentifier", boost::bind( &FOM_Serializer_ABC::ReadEntityIdentifier, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( entityIdentifier_ ), boost::ref( entityIdentifierResolver_ ) ), entityIdentifier_ );
    attributesUpdater_->Register( "ForceIdentifier", boost::bind( &FOM_Serializer_ABC::ReadForceIdentifier, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8 >( static_cast< int8 >( force_ ) ) );
    attributesUpdater_->Register( "Marking", boost::bind( &FOM_Serializer_ABC::ReadMarking, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( marking_ ) ), marking_ );
    attributesUpdater_->Register( "Spatial", boost::bind( &FOM_Serializer_ABC::ReadSpatial, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( spatial_ ) ), spatial_ );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::Attach
// Created: AHC 2012-03-09
// -----------------------------------------------------------------------------
void SurfaceVessel::Attach( Agent_ABC* agent, unsigned long /*simId*/ )
{
    if( agent_ )
        agent_->Unregister( *this );
    agent_ = agent;
    agent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::SetIdentifier
// Created: AHC 2012-03-15
// -----------------------------------------------------------------------------
void SurfaceVessel::SetIdentifier( const std::string& id )
{
    identifier_ = id;
    attributesUpdater_.reset( new AttributesUpdater(identifier_, *listeners_) );
    entityIdentifierResolver_.Register( entityIdentifier_,  identifier_ );
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& SurfaceVessel::GetIdentifier( ) const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::GetIdentifier
// Created: AHC 2012-04-20
// -----------------------------------------------------------------------------
void SurfaceVessel::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::EmbarkmentChanged
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void SurfaceVessel::PlatformAdded( const std::string& /*name*/, unsigned int /*id*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void SurfaceVessel::ChildrenChanged( const EventListener_ABC::T_ChildrenIds& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void SurfaceVessel::ParentChanged( const std::string& /*parentId*/ )
{
    // NOTHING
}