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
       listener.EquipmentUpdated( identifier, type, 1, 0, 0, 0 );
   }
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel constructor
// Created: SLI 2011-10-04
// -----------------------------------------------------------------------------
SurfaceVessel::SurfaceVessel( Agent_ABC& agent, const std::string& name,
                              rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                              const rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer,
                              const std::string& rtiId, unsigned long simId )
    : identifier_( rtiId )
    , listeners_ ( new ObjectListenerComposite() )
    , agent_     ( &agent )
    , entityIdentifierResolver_ ( entityIdentifierResolver )
    , fomSerializer_( fomSerializer )
    , attributesUpdater_( new AttributesUpdater(identifier_, *listeners_) )
    , force_ ( force )
    , type_ ( type )
    , marking_( markingFactory.CreateMarking( name, simId ) )
    , entityIdentifier_( entityId )
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
    spatial_.Refresh( false, latitude, longitude, altitude, speed, direction );
    attributesUpdater_->Update( "Spatial", spatial_ );
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
void SurfaceVessel::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
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
    attributesUpdater_->Register( "ForceIdentifier", boost::bind( &FOM_Serializer_ABC::ReadForceIdentifier, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributesUpdater_->Register( "Marking", boost::bind( &FOM_Serializer_ABC::ReadMarking, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( marking_ ) ), marking_ );
    attributesUpdater_->Register( "Spatial", boost::bind( &FOM_Serializer_ABC::ReadSpatial, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( spatial_ ) ), spatial_ );
    attributesUpdater_->Register( "IsPartOf", boost::bind( &FOM_Serializer_ABC::ReadIsPartOf, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( isPartOf_ ) ), isPartOf_ );
    attributesUpdater_->Register( "IsConcealed", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ),  Wrapper< uint8_t >( 0 ) );
    attributesUpdater_->Register( "DamageState", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ),  Wrapper< uint32_t >( rpr::damageState_NoDamage ) );
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
void SurfaceVessel::ParentChanged( const std::string& parentId )
{
    isPartOf_.rtiId_ = Omt13String( parentId );
    attributesUpdater_->Update( "IsPartOf", isPartOf_ );
}

// -----------------------------------------------------------------------------
// Name: SurfaceVessel::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void SurfaceVessel::StateChanged( rpr::DamageState32 state )
{
    attributesUpdater_->Update( "DamageState", Wrapper< uint32_t >( static_cast< uint32_t >( state ) ) );
}

Agent_ABC* const SurfaceVessel::GetAgent() const
{
	return agent_;
}
