// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "Human.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "AttributesSerializer.h"
#include "ObjectListener_ABC.h"
#include "ObjectListenerComposite.h"
#include "MarkingFactory_ABC.h"
#include "FOM_Serializer_ABC.h"
#include "rpr/EntityIdentifier.h"
#include <boost/bind.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: Human constructor
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
Human::Human( Agent_ABC& agent, const std::string& name,
                              rpr::ForceIdentifier force, const rpr::EntityType& type, const MarkingFactory_ABC& markingFactory,
                              const rpr::EntityIdentifier& entityId, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, 
                              FOM_Serializer_ABC& fomSerializer, const std::string& rtiId, unsigned long simId )
    :  identifier_( rtiId )
    , listeners_ ( new ObjectListenerComposite() )
    , fomSerializer_( fomSerializer )
    , agent_     ( agent )
    , attributes_( new AttributesSerializer() )
{
    attributes_->Register( "EntityType", type );
    attributes_->Register( "EntityIdentifier", entityId );
    attributes_->Register( "ForceIdentifier", Wrapper< unsigned char >( static_cast< unsigned char >( force ) ) );
    attributes_->Register( "Marking", markingFactory.CreateMarking( name, simId ) );
    attributes_->Register( "Spatial", Spatial( true, 0., 0., 0., 0., 0. ) );
    attributes_->Register( "DamageState", Wrapper< uint32_t >( static_cast< uint32_t >( rpr::damageState_NoDamage ) ) );
    attributes_->Register( "IsConcealed", Wrapper< char >( static_cast< char >( 0 ) ) );
    agent_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Human destructor
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
Human::~Human( )
{
    agent_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Human::Serialize
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: Human::Deserialize
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::Deserialize( const ::hla::AttributeIdentifier& /*identifier*/, ::hla::Deserializer_ABC& /*deserializer*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: Human::SpatialChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    attributes_->Update( "Spatial", Spatial( false, latitude, longitude, altitude, speed, direction ) );
}

// -----------------------------------------------------------------------------
// Name: Human::FormationChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Human::EquipmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Human::EmbarkmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::EmbarkmentChanged( bool mounted )
{
    attributes_->Update( "IsConcealed", Wrapper< char >( mounted ? 1 : 0 ) );
}

// -----------------------------------------------------------------------------
// Name: Human::GetIdentifier
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
const std::string& Human::GetIdentifier( ) const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: Human::Register
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: Human::Unregister
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: Human::EmbarkmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::PlatformAdded( const std::string& /*name*/, unsigned int /*id*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Human::EmbarkmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Human::EmbarkmentChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Human::ChildrenChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::ChildrenChanged( const EventListener_ABC::T_ChildrenIds& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Human::ChildrenChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::ParentChanged( const std::string& parentId )
{
    isPartOf_.rtiId_ = Omt13String( parentId );
    attributes_->Update( "IsPartOf", isPartOf_ );
}

// -----------------------------------------------------------------------------
// Name: Human::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void Human::StateChanged( rpr::DamageState32 state )
{
    attributes_->Update( "DamageState", Wrapper< uint32_t >( static_cast< uint32_t >( state ) ) );

}

Agent_ABC* const Human::GetAgent() const
{
	return 0;
}
