// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AttributeFactory.h"
#include "Object.h"
#include "AltitudeModifierAttribute.h"
#include "ConstructionAttribute.h"
#include "CrossingSiteAttribute.h"
#include "DelayAttribute.h"
#include "FireAttribute.h"
#include "FloodAttribute.h"
#include "InteractionHeightAttribute.h"
#include "InputToxicCloudAttribute.h"
#include "LodgingAttribute.h"
#include "LogisticAttribute.h"
#include "MineAttribute.h"
#include "NBCAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "OccupantAttribute.h"
#include "PopulationAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "StockAttribute.h"
#include "SupplyRouteAttribute.h"
#include "TimeLimitedAttribute.h"
#include "UndergroundAttribute.h"
#include "DisasterAttribute.h"
#include "BuildableCapacity.h"
#include "ImprovableCapacity.h"
#include "TrafficabilityAttribute.h"
#include "protocol/Protocol.h"
#include "protocol/MessageParameters.h"

using namespace sword;

namespace
{
    template< typename T, typename I = T  >
    struct AddBuilder
    {
        static void Add( Object& object, xml::xistream& xis )
        {
            object.GetAttribute< T, I >() = T( xis );
        }
    };

    template< typename Capacity, typename Attribute >
    void InitializeDotation( Object& object )
    {
        if( object.RetrieveAttribute< Attribute >() )
            return;
        Capacity* capacity = object.Retrieve< Capacity >();
        if( capacity )
            object.GetAttribute< Attribute >() = Attribute( *capacity->GetDotationCategory(), capacity->GetMaxDotation() );
    }

    template<>
    struct AddBuilder< MineAttribute >
    {
        static void Add( Object& object, xml::xistream& xis )
        {
            InitializeDotation< ImprovableCapacity, MineAttribute >( object );
            object.GetAttribute< MineAttribute >().Load( xis );
        }
    };

    template<>
    struct AddBuilder< ConstructionAttribute >
    {
        static void Add( Object& object, xml::xistream& xis )
        {
            InitializeDotation< BuildableCapacity, ConstructionAttribute >( object );
            object.GetAttribute< ConstructionAttribute >().Load( xis );
        }
    };

    template<>
    struct AddBuilder< ResourceNetworkAttribute >
    {
        static void Add( Object& object, xml::xistream& xis )
        {
            object.GetAttribute< ResourceNetworkAttribute >() = ResourceNetworkAttribute( xis, object );
        }
    };

    template<>
    struct AddBuilder< FloodAttribute >
    {
        static void Add( Object& object, xml::xistream& xis )
        {
            object.GetAttribute< FloodAttribute >() = FloodAttribute( xis, object.GetLocalisation() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::AttributeFactory
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AttributeFactory::AttributeFactory()
{
    Register( "logistic-base", boost::bind( &AddBuilder< LogisticAttribute >::Add, _1, _2 ) );
    Register( "mine", boost::bind( &AddBuilder< MineAttribute >::Add, _1, _2 ) );
    Register( "construction", boost::bind( &AddBuilder< ConstructionAttribute >::Add, _1, _2 ) );
    Register( "obstacle", boost::bind( &AddBuilder< ObstacleAttribute >::Add, _1, _2 ) );
    Register( "nbc-agents", boost::bind( &AddBuilder< NBCAttribute >::Add, _1, _2 ) );
    Register( "input-toxic-cloud", boost::bind( &AddBuilder< InputToxicCloudAttribute, ToxicAttribute_ABC >::Add, _1, _2 ) );
    Register( "disaster", boost::bind( &AddBuilder< DisasterAttribute >::Add, _1, _2 ) );
    Register( "activity-time", boost::bind( &AddBuilder< TimeLimitedAttribute >::Add, _1, _2 ) );
    Register( "crossing-site", boost::bind( &AddBuilder< CrossingSiteAttribute >::Add, _1, _2 ) );
    Register( "fire", boost::bind( &AddBuilder< FireAttribute >::Add, _1, _2 ) );
    Register( "supply-route", boost::bind( &AddBuilder< SupplyRouteAttribute >::Add, _1, _2 ) );
    Register( "interaction-height", boost::bind( &AddBuilder< InteractionHeightAttribute >::Add, _1, _2 ) );
    Register( "population-filter", boost::bind( &AddBuilder< PopulationAttribute >::Add, _1, _2 ) );
    Register( "stock", boost::bind( &AddBuilder< StockAttribute >::Add, _1, _2 ) );
    Register( "max-size", boost::bind( &AddBuilder< OccupantAttribute >::Add, _1, _2 ) );
    Register( "delay", boost::bind( &AddBuilder< DelayAttribute >::Add, _1, _2 ) );
    Register( "resources", boost::bind( &AddBuilder< ResourceNetworkAttribute >::Add, _1, _2 ) );
    Register( "flood", boost::bind( &AddBuilder< FloodAttribute >::Add, _1, _2 ) );
    Register( "altitude-modifier", boost::bind( &AddBuilder< AltitudeModifierAttribute >::Add, _1, _2 ) );
    Register( "lodging", boost::bind( &AddBuilder< LodgingAttribute >::Add, _1, _2 ) );
    Register( "underground", boost::bind( &AddBuilder< UndergroundAttribute >::Add, _1, _2 ) );
    Register( "trafficability", boost::bind( &AddBuilder< TrafficabilityAttribute >::Add, _1, _2 ) );
    Register( "bypass", boost::bind( &AddBuilder< BypassAttribute >::Add, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AttributeFactory::~AttributeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Register
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void AttributeFactory::Register( const std::string& attribute, const T_CallBack& callback )
{
    if( ! callbacks_.insert( std::make_pair( attribute, callback ) ).second )
        throw MASA_EXCEPTION( "Capacity '" + attribute + "' already registered." );
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Create
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void AttributeFactory::Create( Object& object, const std::string& attribute, xml::xistream& xis ) const
{
    const CIT_Callbacks it = callbacks_.find( attribute );
    if( it != callbacks_.end() )
        it->second( object, xis );
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Initialize
// Created: LGY 2010-08-23
// -----------------------------------------------------------------------------
void AttributeFactory::Initialize( Object& object ) const
{
    // $$$$ _RC_ JSR 2011-01-10: Pourquoi rajoute t'on obligatoirement ConstructionAttribute? Ca n'a pas forcément de sens.
    // Initialize construction with 100%
    object.GetAttribute< ConstructionAttribute >();
    object.GetAttribute< OccupantAttribute >();     // $$$$ LDC: All physical objects have an occupant attribute.
}

namespace
{
    void CheckAttribteCount( const std::string& name, const sword::MissionParameter_Value& value, int count )
    {
        protocol::Check( value.list_size() == count, STR( name << " attribute must have " << count  << " parameters" ) );
    }
    void CheckQuantity( const sword::MissionParameter_Value& value, unsigned int index )
    {
        protocol::Check( value.list( index ).has_quantity(), STR( "list[" << index << "] must have a quantity" ) );
    }
    void CheckBool( const sword::MissionParameter_Value& value, unsigned int index )
    {
        protocol::Check( value.list( index ).has_booleanvalue(), STR( "list[" << index << "] must have a boolean value" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Create
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void AttributeFactory::Create( Object& object, const sword::MissionParameter& parameter ) const
{
    for( int i = 0; i < parameter.value_size(); ++i )
    {
        const auto& attributes = parameter.value( i );
        protocol::Check( attributes.list_size() > 0, "invalid empty parameter" );
        protocol::Check( attributes.list( 0 ).has_identifier(), "first element must be an identifier" );
        const unsigned int actionId = attributes.list( 0 ).identifier();
        switch( actionId )
        {
        case ObjectMagicAction::obstacle:
            object.GetAttribute< ObstacleAttribute >() = ObstacleAttribute( attributes );
            break;
        case ObjectMagicAction::mine:
            object.GetAttribute< MineAttribute >() = MineAttribute( attributes );
            break;
        case ObjectMagicAction::time_limit:
            object.GetAttribute< TimeLimitedAttribute >() = TimeLimitedAttribute( attributes );
            break;
        case ObjectMagicAction::bypass:
            object.GetAttribute< BypassAttribute >() = BypassAttribute( attributes );
            break;
        case ObjectMagicAction::effect_delay:
            object.GetAttribute< DelayAttribute >() = DelayAttribute( attributes );
            break;
        case ObjectMagicAction::lodging:
            object.GetAttribute< LodgingAttribute >() = LodgingAttribute( attributes );
            break;
        case ObjectMagicAction::logistic:
            object.GetAttribute< LogisticAttribute >() = LogisticAttribute( attributes );
            break;
        case ObjectMagicAction::nbc:
            object.GetAttribute< NBCAttribute >() = NBCAttribute( attributes );
            break;
        case ObjectMagicAction::crossing_site:
            object.GetAttribute< CrossingSiteAttribute >() = CrossingSiteAttribute( attributes );
            break;
        case ObjectMagicAction::supply_route:
            object.GetAttribute< SupplyRouteAttribute >() = SupplyRouteAttribute( attributes );
            break;
        case ObjectMagicAction::fire:
            object.GetAttribute< FireAttribute >() = FireAttribute( attributes );
            break;
        case ObjectMagicAction::interaction_height:
            object.GetAttribute< InteractionHeightAttribute >() = InteractionHeightAttribute( attributes );
            break;
        case ObjectMagicAction::stock:
            object.GetAttribute< StockAttribute >() = StockAttribute( attributes );
            break;
        case ObjectMagicAction::flood:
            object.GetAttribute< FloodAttribute >() = FloodAttribute( attributes, object.GetLocalisation() );
            break;
        case ObjectMagicAction::resource_network:
            object.GetAttribute< ResourceNetworkAttribute >() = ResourceNetworkAttribute( object );
            break;
        case ObjectMagicAction::altitude_modifier:
            object.GetAttribute< AltitudeModifierAttribute >() = AltitudeModifierAttribute( attributes, object.GetLocalisation(), object.GetID() );
            break;
        case ObjectMagicAction::underground:
            object.GetAttribute< UndergroundAttribute >() = UndergroundAttribute( attributes );
            break;
        case ObjectMagicAction::trafficability:
            object.GetAttribute< TrafficabilityAttribute >() = TrafficabilityAttribute( attributes );
            break;
        case ObjectMagicAction::disaster:
            object.GetAttribute< DisasterAttribute >() = DisasterAttribute( attributes );
            break;
        case ObjectMagicAction::density:
        {
            if( attributes.list_size() > 1 && attributes.list( 1 ).has_areal() )
            {
                double density = attributes.list( 1 ).areal();
                BuildableCapacity* capacity = object.Retrieve< BuildableCapacity >();
                if( capacity )
                    capacity->SetDensity( density );
                ConstructionAttribute* attribute = object.RetrieveAttribute< ConstructionAttribute >();
                if( attribute )
                    attribute->SetDensity( density );
            }
            break;
        }
        default:
            break;
        }
    }
}
