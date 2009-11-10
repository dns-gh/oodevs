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

#include <boost/bind.hpp>

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
    if ( ! callbacks_.insert( std::make_pair( attribute, callback ) ).second )
		throw std::invalid_argument( "capacity '" + attribute + "' already registered." );
}

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Create
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void AttributeFactory::Create( Object& object, const std::string& attribute, xml::xistream& xis ) const
{
	const CIT_Callbacks it = callbacks_.find( attribute );
	if ( it != callbacks_.end() )
        it->second( object, xis );
}

#define ASN_CREATE_ATTRIBUTE( ASN, CLASS )      \
    if( asn.m.##ASN##Present )    \
        object.GetAttribute< CLASS >() = CLASS( asn );

// -----------------------------------------------------------------------------
// Name: AttributeFactory::Create
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void AttributeFactory::Create( Object& object, const ASN1T_ObjectAttributes& asn ) const
{
    // Initialize construction with 100%
    ConstructionAttribute& construction = object.GetAttribute< ConstructionAttribute >();
    
    object.GetAttribute< OccupantAttribute >();     // $$$$ LDC: All physical objects have an occupant attribute.
    ASN_CREATE_ATTRIBUTE( obstacle, ObstacleAttribute );
    ASN_CREATE_ATTRIBUTE( mine, MineAttribute );
    ASN_CREATE_ATTRIBUTE( activity_time, TimeLimitedAttribute );
    ASN_CREATE_ATTRIBUTE( bypass, BypassAttribute );
    ASN_CREATE_ATTRIBUTE( logistic, LogisticAttribute );
    ASN_CREATE_ATTRIBUTE( nbc, NBCAttribute );
    ASN_CREATE_ATTRIBUTE( crossing_site, CrossingSiteAttribute );
    ASN_CREATE_ATTRIBUTE( supply_route, SupplyRouteAttribute );
    //ASN_CREATE_ATTRIBUTE( toxic_cloud, ??? );
    ASN_CREATE_ATTRIBUTE( fire, FireAttribute );
    ASN_CREATE_ATTRIBUTE( medical_treatment, MedicalTreatmentAttribute );
    ASN_CREATE_ATTRIBUTE( interaction_height, InteractionHeightAttribute );
}
