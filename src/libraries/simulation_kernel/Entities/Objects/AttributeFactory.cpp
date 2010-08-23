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
#include "ConstructionAttribute.h"
#include "CrossingSiteAttribute.h"
#include "DelayAttribute.h"
#include "FireAttribute.h"
#include "InteractionHeightAttribute.h"
#include "InputToxicCloudAttribute.h"
#include "LogisticAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "MineAttribute.h"
#include "NBCAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "OccupantAttribute.h"
#include "PopulationAttribute.h"
#include "SupplyRouteAttribute.h"
#include "TimeLimitedAttribute.h"
#include "StockAttribute.h"
#include "ImprovableCapacity.h"
#include "BuildableCapacity.h"
#include "protocol/protocol.h"
#include <boost/bind.hpp>

using namespace MsgsClientToSim;

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
            InitializeDotation< BuildableCapacity, MineAttribute >( object );
            object.GetAttribute< MineAttribute >().Load( xis );
        }
    };

    template<>
    struct AddBuilder< ConstructionAttribute >
    {
        static void Add( Object& object, xml::xistream& xis )
        {
            InitializeDotation< ImprovableCapacity, ConstructionAttribute >( object );
            InitializeDotation< BuildableCapacity, ConstructionAttribute >( object );
            object.GetAttribute< ConstructionAttribute >().Load( xis );
        }
    };

}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::AttributeFactory
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
AttributeFactory::AttributeFactory()
{
    Register( "tc2", boost::bind( &AddBuilder< LogisticAttribute >::Add, _1, _2 ) );
    Register( "mine", boost::bind( &AddBuilder< MineAttribute >::Add, _1, _2 ) );
    Register( "construction", boost::bind( &AddBuilder< ConstructionAttribute >::Add, _1, _2 ) );
    Register( "obstacle", boost::bind( &AddBuilder< ObstacleAttribute >::Add, _1, _2 ) );
    Register( "nbc-agents", boost::bind( &AddBuilder< NBCAttribute >::Add, _1, _2 ) );
    Register( "input-toxic-cloud", boost::bind( &AddBuilder< InputToxicCloudAttribute, ToxicAttribute_ABC >::Add, _1, _2 ) );
    Register( "activity-time", boost::bind( &AddBuilder< TimeLimitedAttribute >::Add, _1, _2 ) );
    Register( "crossing-site", boost::bind( &AddBuilder< CrossingSiteAttribute >::Add, _1, _2 ) );
    Register( "fire", boost::bind( &AddBuilder< FireAttribute >::Add, _1, _2 ) );
    Register( "supply-route", boost::bind( &AddBuilder< SupplyRouteAttribute >::Add, _1, _2 ) );
    Register( "medical-treatment", boost::bind( &AddBuilder< MedicalTreatmentAttribute >::Add, _1, _2 ) );
    Register( "interaction-height", boost::bind( &AddBuilder< InteractionHeightAttribute >::Add, _1, _2 ) );
    Register( "population-filter", boost::bind( &AddBuilder< PopulationAttribute >::Add, _1, _2 ) );
    Register( "stock", boost::bind( &AddBuilder< StockAttribute >::Add, _1, _2 ) );
    Register( "max-size", boost::bind( &AddBuilder< OccupantAttribute >::Add, _1, _2 ) );
    Register( "delay", boost::bind( &AddBuilder< DelayAttribute >::Add, _1, _2 ) );
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
        throw std::invalid_argument( "capacity '" + attribute + "' already registered." );
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
    // Initialize construction with 100%
    object.GetAttribute< ConstructionAttribute >();
    object.GetAttribute< OccupantAttribute >();     // $$$$ LDC: All physical objects have an occupant attribute.
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Create
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void AttributeFactory::Create( Object& object, const Common::MsgMissionParameter_Value& attributes ) const
{
    Initialize( object );
    for( int i = 0; i < attributes.list_size(); ++i )
    {
        const Common::MsgMissionParameter_Value& attribute = attributes.list( i );
        if( attribute.list_size() == 0 ) // it should be a list of lists
            return;

        unsigned int actionId = attribute.list( 0 ).identifier(); // first element is the type
        switch( actionId )
        {
        case MsgObjectMagicAction_Attribute_obstacle:
            object.GetAttribute< ObstacleAttribute >() = ObstacleAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_mine:
            object.GetAttribute< MineAttribute >() = MineAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_activity_time:
            object.GetAttribute< TimeLimitedAttribute >() = TimeLimitedAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_bypass:
            object.GetAttribute< BypassAttribute >() = BypassAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_effect_delay:
            object.GetAttribute< DelayAttribute >() = DelayAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_logistic:
            object.GetAttribute< LogisticAttribute >() = LogisticAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_nbc:
            object.GetAttribute< NBCAttribute >() = NBCAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_crossing_site:
            object.GetAttribute< CrossingSiteAttribute >() = CrossingSiteAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_supply_route:
            object.GetAttribute< SupplyRouteAttribute >() = SupplyRouteAttribute( attribute );
            break;
        /*case MsgObjectMagicAction_Attribute_toxic_cloud:
            object.GetAttribute< ??? >() = ???( attribute );
            break;*/
        case MsgObjectMagicAction_Attribute_fire:
            object.GetAttribute< FireAttribute >() = FireAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_medical_treatment:
            object.GetAttribute< MedicalTreatmentAttribute >() = MedicalTreatmentAttribute( attribute );
            break;
        case MsgObjectMagicAction_Attribute_interaction_height:
            object.GetAttribute< InteractionHeightAttribute >() = InteractionHeightAttribute( attribute );
            break;
        default:
            break;
        }
    }
}
