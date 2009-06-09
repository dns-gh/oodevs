// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "CapacityFactory.h"
#include "ObjectPrototype.h"

#include "ActivableCapacity.h"
#include "AttritionCapacity.h"
#include "AvoidanceCapacity.h"
#include "BridgingCapacity.h"
#include "BypassableCapacity.h"
#include "ConstructionCapacity.h"
#include "ContaminationCapacity.h"
#include "DecontaminationCapacity.h"
#include "DetectionCapacity.h"
#include "ExtinguishableCapacity.h"
#include "FirePropagationCapacity.h"
#include "HealableCapacity.h"
#include "InteractIfEquippedCapacity.h"
#include "InteractWithEnemyCapacity.h"
#include "InterferenceCapacity.h"
#include "IntoxicationCapacity.h"
#include "InputPropagationCapacity.h"
#include "MobilityCapacity.h"
#include "ProtectionCapacity.h"
#include "TerrainHeuristicCapacity.h"
#include "TimeLimitedCapacity.h"
#include "WorkableCapacity.h"
#include "SupplyCapacity.h"
#include "MIL_PropagationManager.h"
#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

namespace 
{
    template< typename T >
    struct AddBuilder
    {    
        static void Add( ObjectPrototype& prototype, xml::xistream& xis )
        {
            prototype.AddCapacity( new T( xis ) );
        }
    };

    void AddConstructor( ObjectPrototype& prototype, xml::xistream& xis )
    {
        ConstructionCapacity constructor( prototype, xis );
        xis >> xml::list( constructor, &ConstructionCapacity::AddCapacity );
    }

    void AddPropagation( ObjectPrototype& prototype, xml::xistream& xis, MIL_PropagationManager& propagation )
    {
        std::string model( xml::attribute( xis, "model", std::string() ) );
        if ( model == "input" )
            prototype.AddCapacity< PropagationCapacity_ABC >( new InputPropagationCapacity( xis ) );
        if ( model == "fire" )
            prototype.AddCapacity< PropagationCapacity_ABC >( new FirePropagationCapacity( xis, propagation ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CapacityFactory::CapacityFactory()
    : propagation_ ( new MIL_PropagationManager() ) 
{
    Register( "activable", boost::bind( &AddBuilder< ActivableCapacity >::Add, _1, _2 ) );
    Register( "attrition", boost::bind( &AddBuilder< AttritionCapacity >::Add, _1, _2 ) );
    Register( "avoidable", boost::bind( &AddBuilder< AvoidanceCapacity >::Add, _1, _2 ) );
    Register( "bridging", boost::bind( &AddBuilder< BridgingCapacity >::Add, _1, _2 ) );
    Register( "bypassable", boost::bind( &AddBuilder< BypassableCapacity >::Add, _1, _2 ) );
    Register( "constructor", boost::bind( &AddConstructor, _1, _2 ) );
    Register( "contamination", boost::bind( &AddBuilder< ContaminationCapacity >::Add, _1, _2 ) );
    Register( "decontamination", boost::bind( &AddBuilder< DecontaminationCapacity >::Add, _1, _2 ) );
    Register( "extinguishable", boost::bind( &AddBuilder< ExtinguishableCapacity >::Add, _1, _2 ) );
    Register( "healable", boost::bind( &AddBuilder< HealableCapacity >::Add, _1, _2 ) );
    Register( "heuristic", boost::bind( &AddBuilder< TerrainHeuristicCapacity >::Add, _1, _2 ) );
    Register( "interact-with-enemy", boost::bind( &AddBuilder< InteractWithEnemyCapacity >::Add, _1, _2 ) );
    Register( "interference", boost::bind( &AddBuilder< InterferenceCapacity >::Add, _1, _2 ) );
    Register( "intoxication", boost::bind( &AddBuilder< IntoxicationCapacity >::Add, _1, _2 ) );
    // logistic is not needed except during prepa
    Register( "supply-route", boost::bind( &AddBuilder< InteractIfEquippedCapacity >::Add, _1, _2 ) );
    Register( "mobility", boost::bind( &AddBuilder< MobilityCapacity >::Add, _1, _2 ) );
	Register( "propagation", boost::bind( &AddPropagation, _1, _2, boost::ref( *propagation_ ) ) );	
    Register( "protection", boost::bind( &AddBuilder< ProtectionCapacity >::Add, _1, _2 ) );
    Register( "time-limited", boost::bind( &AddBuilder< TimeLimitedCapacity >::Add, _1, _2 ) );
    Register( "workable", boost::bind( &AddBuilder< WorkableCapacity >::Add, _1, _2 ) );
    Register( "supply", boost::bind( &AddBuilder< SupplyCapacity >::Add, _1, _2 ) );
    Register( "detection", boost::bind( &AddBuilder< DetectionCapacity >::Add, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory destructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CapacityFactory::~CapacityFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::Register
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void CapacityFactory::Register( const std::string& capacity, const T_CallBack& callback )
{
    if ( ! callbacks_.insert( std::make_pair( capacity, callback ) ).second )
		throw std::invalid_argument( "capacity '" + capacity + "' already registered." );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::Create
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void CapacityFactory::Create( ObjectPrototype& prototype, const std::string& capacity, xml::xistream& xis )
{
	const CIT_Callbacks it = callbacks_.find( capacity );
	if ( it != callbacks_.end() )
        it->second( prototype, xis );
}
