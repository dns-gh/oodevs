// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AggregateEntity.h"
#include "Agent_ABC.h"
#include "SerializationTools.h"
#include "Spatial.h"
#include "AggregateMarking.h"
#include "SilentEntity.h"
#include "Formation.h"
#include "Dimension.h"
#include "AttributesUpdater.h"
#include "ObjectListener_ABC.h"
#include "MarkingFactory_ABC.h"
#include "FOM_Serializer_ABC.h"
#include "ObjectListenerComposite.h"
#include "EntityIdentifierResolver_ABC.h"
#include "rpr/EntityAppearance.h"
#include <hla/AttributeIdentifier.h>
#include <hla/Deserializer_ABC.h>
#include <boost/bind.hpp>

using namespace plugins::hla;

namespace
{
    void ReadSubIdentifiers( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, 
        void (ObjectListener_ABC::*callback)( const std::string&, const std::set< std::string >& ), Omt13StringArray& var )
    {
        var.Deserialize( deserializer );
        std::set< std::string > s;
        const std::vector< Omt13String >& values = var.GetValues();
        std::for_each( values.begin(), values.end(), [&](const Omt13String& v)
        {
            s.insert( v.str() );
        });
        (listener.*callback)( identifier, s );
    }
}


// -----------------------------------------------------------------------------
// Name: AggregateEntity constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AggregateEntity::AggregateEntity( Agent_ABC& agent, const std::string& name,
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
    , marking_( markingFactory.CreateAggregateMarking( name, simId ) )
    , entityIdentifier_( entityId )
    , aggregateState_( 1 ) // fully aggregated
    , spatial_ ( true, 0., 0., 0., 0., 0. )
{
    RegisterAttributes( );
    agent_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
AggregateEntity::AggregateEntity( const std::string& identifier , EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
    : identifier_( identifier )
    , listeners_ ( new ObjectListenerComposite() )
    , agent_     ( 0 )
    , entityIdentifierResolver_ ( entityIdentifierResolver )
    , fomSerializer_( fomSerializer )
    , numberOfSilentEntities_( 0 )
    , attributesUpdater_ ( new AttributesUpdater( identifier, *listeners_ ) )
    , marking_( )
    , spatial_ ( true, 0., 0., 0., 0., 0. )
{
    RegisterAttributes( );
}


// -----------------------------------------------------------------------------
// Name: AggregateEntity destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
AggregateEntity::~AggregateEntity()
{
    if( agent_ )
        agent_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Serialize
// Created: SBO 2008-02-20
// -----------------------------------------------------------------------------
void AggregateEntity::Serialize( ::hla::UpdateFunctor_ABC& functor, bool updateAll ) const
{
    attributesUpdater_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Deserialize
// Created: SLI 2011-02-08
// -----------------------------------------------------------------------------
void AggregateEntity::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributesUpdater_->Deserialize( identifier.ToString(), deserializer );
}

namespace
{
    template< typename T >
    bool Find( unsigned int type, const T& value )
    {
        return value.type_ == type;
    }
}

namespace
{
    void FillAppearance( std::size_t count, uint8_t status, std::vector< uint32_t >& values )
    {
        rpr::EntityAppearance_Land app;
        app.SetDamage(status);
        for( ; count > 0; --count )
            values.push_back( app.value_ );
    }
}
// -----------------------------------------------------------------------------
// Name: AggregateEntity::EquipmentChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available,
        unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages )
{
    auto result = std::find_if( equipments_.begin(), equipments_.end(), boost::bind( &::Find< T_Equipment >, type, _1 ) );
    if( result == equipments_.end() )
        equipments_.push_back( T_Equipment( type, available, dead, lightDamages, heavyDamages, entityType ) );
    else
    {
        result->available_ = available;
        result->dead_ = dead;
        result->lightDamages_ = lightDamages;
        result->heavyDamages_ = heavyDamages;
    }
    numberOfSilentEntities_ = static_cast< unsigned short >( equipments_.size() );
    attributesUpdater_->Update( "NumberOfSilentEntities", Wrapper< unsigned short >( numberOfSilentEntities_ ) );
    std::vector< SilentEntity > entities;
    std::for_each( equipments_.begin(), equipments_.end(), [&]( const T_Equipment& equipment )
    {
        const uint16_t count = static_cast< uint16_t >( equipment.available_ + equipment.dead_ + equipment.lightDamages_ + equipment.heavyDamages_ );
        SilentEntity silent(equipment.entityType_, count );
        if( equipment.available_ != count )
        {
            FillAppearance( equipment.dead_, 3, silent.entityAppearance_ );
            FillAppearance( equipment.lightDamages_, 1, silent.entityAppearance_ );
            FillAppearance( equipment.heavyDamages_, 2, silent.entityAppearance_ );
        }
        entities.push_back( silent );
    });
    attributesUpdater_->Update( "SilentEntities", Wrapper< std::vector< SilentEntity > >( entities ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::SpatialChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    spatial_.Refresh( false, latitude, longitude, altitude, speed, direction );
    attributesUpdater_->Update( "Spatial", spatial_ );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::FormationChanged
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AggregateEntity::FormationChanged( bool isOnRoad )
{
    attributesUpdater_->Update( "Formation", Formation( isOnRoad ) );
    attributesUpdater_->Update( "Dimensions", Dimension( isOnRoad ) );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::EmbarkmentChanged
// Created: SLI 2011-06-16
// -----------------------------------------------------------------------------
void AggregateEntity::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void AggregateEntity::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void AggregateEntity::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener ) ;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::RegisterAttributes
// Created: AHC 2012-03-07
// -----------------------------------------------------------------------------
void AggregateEntity::RegisterAttributes( )
{
    attributesUpdater_->Register( "EntityType", boost::bind( &FOM_Serializer_ABC::ReadEntityType, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( type_ ) ), type_ );
    attributesUpdater_->Register( "EntityIdentifier", boost::bind( &FOM_Serializer_ABC::ReadEntityIdentifier, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( entityIdentifier_ ), boost::ref( entityIdentifierResolver_ ) ), entityIdentifier_ );
    attributesUpdater_->Register( "ForceIdentifier", boost::bind( &FOM_Serializer_ABC::ReadForceIdentifier, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( force_ ) ), Wrapper< int8_t >( static_cast< int8_t >( force_ ) ) );
    attributesUpdater_->Register( "AggregateMarking", boost::bind( &FOM_Serializer_ABC::ReadAggregateMarking, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( marking_ ) ), marking_ );
    attributesUpdater_->Register( "AggregateState", boost::bind( &FOM_Serializer_ABC::ReadUnsignedChar, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( aggregateState_ ) ), Wrapper< unsigned char >( aggregateState_ ) );
    attributesUpdater_->Register( "Dimensions", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ), Dimension( false ) );
    attributesUpdater_->Register( "Spatial", boost::bind( &FOM_Serializer_ABC::ReadSpatial, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( spatial_ ) ), spatial_ );
    attributesUpdater_->Register( "Formation", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ), Formation( false ) );
    attributesUpdater_->Register( "NumberOfSilentEntities", boost::bind( &FOM_Serializer_ABC::ReadNumberOfSilentEntities, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( numberOfSilentEntities_ ) ), Wrapper< unsigned short >( 0 ) );
    attributesUpdater_->Register( "SilentEntities", boost::bind( &FOM_Serializer_ABC::ReadSilentEntities, boost::ref( fomSerializer_ ), _1, _2, _3, boost::ref( numberOfSilentEntities_ ) ), Wrapper< std::vector< SilentEntity > >( std::vector< SilentEntity >() ) );
    attributesUpdater_->Register( "SubAggregateIdentifiers", boost::bind( &ReadSubIdentifiers, _1, _2, _3, &ObjectListener_ABC::SubAgregatesChanged, boost::ref( subAggregates_ ) ), subAggregates_ ); // no sub aggregates identifiers
    attributesUpdater_->Register( "EntityIdentifiers", boost::bind( &ReadSubIdentifiers, _1, _2, _3, &ObjectListener_ABC::SubEntitiesChanged, boost::ref( entities_ ) ), entities_ ); // no entity identifiers
    attributesUpdater_->Register( "NumberOfVariableDatums", boost::bind( &FOM_Serializer_ABC::ReadNothing, boost::ref( fomSerializer_ ), _1, _2, _3 ), Wrapper< uint32_t >( 0 ) ); // no variable datums
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::Attach
// Created: AHC 2012-03-09
// -----------------------------------------------------------------------------
void AggregateEntity::Attach( Agent_ABC* agent, unsigned long /*simId*/ )
{
    if( agent_ )
        agent_->Unregister( *this );
    agent_ = agent;
    agent_->Register( *this );
}
// -----------------------------------------------------------------------------
// Name: AggregateEntity::GetIdentifier
// Created: AHC 2012-04-18
// -----------------------------------------------------------------------------
const std::string& AggregateEntity::GetIdentifier( ) const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::GetIdentifier
// Created: AHC 2012-04-20
// -----------------------------------------------------------------------------
void AggregateEntity::ResetAttributes()
{
    aggregateState_ = 1;
    attributesUpdater_->Update( "AggregateState", Wrapper< unsigned char >( aggregateState_ ) );
    attributesUpdater_->Update( "SilentAggregates", Wrapper< uint32_t >( 0 ) );
    attributesUpdater_->Update( "NumberOfVariableDatums", Wrapper< uint32_t >( 0 ) );
    // TODO silent entities
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::PlatformAdded
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void AggregateEntity::PlatformAdded( const std::string& name, unsigned int /*id*/ )
{
    entities_.Add( name );
    attributesUpdater_->Update( "EntityIdentifiers", entities_ );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AggregateEntity::ChildrenChanged( const EventListener_ABC::T_ChildrenIds& children )
{
    subAggregates_.Clear();
    std::for_each( children.begin(), children.end(), [&](const EventListener_ABC::T_ChildrenIds::value_type& v)
        {
            subAggregates_.Add( v );
        });
    attributesUpdater_->Update( "SubAggregateIdentifiers", subAggregates_ );
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void AggregateEntity::ParentChanged( const std::string& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateEntity::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void AggregateEntity::StateChanged( rpr::DamageState32 /*state*/ )
{
    // NOTHING
}

Agent_ABC* const AggregateEntity::GetAgent() const
{
    return agent_;
}
