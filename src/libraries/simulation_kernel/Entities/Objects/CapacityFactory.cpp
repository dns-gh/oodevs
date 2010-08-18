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
#include "AttitudeModifierCapacity.h"
#include "AttritionCapacity.h"
#include "AvoidanceCapacity.h"
#include "BridgingCapacity.h"
#include "BypassableCapacity.h"
#include "ConstructionCapacity.h"
#include "ContaminationCapacity.h"
#include "DecontaminationCapacity.h"
#include "DelayCapacity.h"
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
#include "PerceptionCapacity.h"
#include "ProtectionCapacity.h"
#include "ResourceNetworkCapacity.h"
#include "ScatteringCapacity.h"
#include "SpawnCapacity.h"
#include "StructuralCapacity.h"
#include "TerrainHeuristicCapacity.h"
#include "TimeLimitedCapacity.h"
#include "WorkableCapacity.h"
#include "SupplyCapacity.h"
#include "MIL_Object_ABC.h"
#include "MIL_PropagationManager.h"
#include <xeumeuleu/xml.hpp>
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

    template< typename T >
    struct UpdateBuilder
    {
        static void Update( MIL_Object_ABC& object, xml::xistream& xis )
        {
            T* capacity = object.Retrieve< T >();
            if( capacity )
                capacity->Update( xis );
            else
                T( xis ).Instanciate( object );
        }
    };

    void AddConstructor( ObjectPrototype& prototype, xml::xistream& xis )
    {
        ConstructionCapacity constructor( prototype, xis );
        xis >> xml::list( constructor, &ConstructionCapacity::AddCapacity );
    }

    void AddPropagation( ObjectPrototype& prototype, xml::xistream& xis, MIL_PropagationManager& propagation )
    {
        const std::string model( xis.attribute< std::string >( "model", std::string() ) );
        if( model == "input" )
            prototype.AddCapacity< PropagationCapacity_ABC >( new InputPropagationCapacity( xis ) );
        if( model == "fire" )
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
    DoRegister( "activable", boost::bind( &AddBuilder< ActivableCapacity >::Add, _1, _2 ) );
    DoRegister( "attrition", boost::bind( &AddBuilder< AttritionCapacity >::Add, _1, _2 ) );
    DoRegister( "avoidable", boost::bind( &AddBuilder< AvoidanceCapacity >::Add, _1, _2 ) );
    DoRegister( "bridging", boost::bind( &AddBuilder< BridgingCapacity >::Add, _1, _2 ) );
    DoRegister( "bypassable", boost::bind( &AddBuilder< BypassableCapacity >::Add, _1, _2 ) );
    DoRegister( "constructor", boost::bind( &AddConstructor, _1, _2 ) );
    DoRegister( "contamination", boost::bind( &AddBuilder< ContaminationCapacity >::Add, _1, _2 ) );
    DoRegister( "decontamination", boost::bind( &AddBuilder< DecontaminationCapacity >::Add, _1, _2 ) );
    DoRegister( "delay", boost::bind( &AddBuilder< DelayCapacity >::Add, _1, _2 ) );
    DoRegister( "extinguishable", boost::bind( &AddBuilder< ExtinguishableCapacity >::Add, _1, _2 ) );
    DoRegister( "healable", boost::bind( &AddBuilder< HealableCapacity >::Add, _1, _2 ) );
    DoRegister( "heuristic", boost::bind( &AddBuilder< TerrainHeuristicCapacity >::Add, _1, _2 ) );
    DoRegister( "interact-with-enemy", boost::bind( &AddBuilder< InteractWithEnemyCapacity >::Add, _1, _2 ) );
    DoRegister( "interference", boost::bind( &AddBuilder< InterferenceCapacity >::Add, _1, _2 ) );
    DoRegister( "intoxication", boost::bind( &AddBuilder< IntoxicationCapacity >::Add, _1, _2 ) );
    // logistic is not needed except during prepa
    DoRegister( "supply-route", boost::bind( &AddBuilder< InteractIfEquippedCapacity >::Add, _1, _2 ) );
    DoRegister( "mobility", boost::bind( &AddBuilder< MobilityCapacity >::Add, _1, _2 ) );
    DoRegister( "propagation", boost::bind( &AddPropagation, _1, _2, boost::ref( *propagation_ ) ) );
    DoRegister( "protection", boost::bind( &AddBuilder< ProtectionCapacity >::Add, _1, _2 ) );
    DoRegister( "time-limited", boost::bind( &AddBuilder< TimeLimitedCapacity >::Add, _1, _2 ) );
    DoRegister( "workable", boost::bind( &AddBuilder< WorkableCapacity >::Add, _1, _2 ) );
    DoRegister( "supply", boost::bind( &AddBuilder< SupplyCapacity >::Add, _1, _2 ) );
    DoRegister( "detection", boost::bind( &AddBuilder< DetectionCapacity >::Add, _1, _2 ) );
    DoRegister( "spawn", boost::bind( &AddBuilder< SpawnCapacity >::Add, _1, _2 ) );
    DoRegister( "perception", boost::bind( &AddBuilder< PerceptionCapacity >::Add, _1, _2 ) );
    DoRegister( "attitude-modifier", boost::bind( &AddBuilder< AttitudeModifierCapacity >::Add, _1, _2 ) );
    DoRegister( "scattering", boost::bind( &AddBuilder< ScatteringCapacity >::Add, _1, _2 ) );
    DoRegister( "structural", boost::bind( &AddBuilder< StructuralCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< StructuralCapacity >::Update, _1, _2 ) );
    DoRegister( "resources", boost::bind( &AddBuilder< ResourceNetworkCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< ResourceNetworkCapacity >::Update, _1, _2 ) );
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
void CapacityFactory::Register( const std::string& capacity, const Prototype_CallBack& callback )
{
    if( ! prototypeCallbacks_.insert( std::make_pair( capacity, callback ) ).second )
        throw std::invalid_argument( "prototype capacity '" + capacity + "' already registered." );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::Register
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void CapacityFactory::Register( const std::string& capacity, const Object_CallBack& callback )
{
    if( ! objectCallbacks_.insert( std::make_pair( capacity, callback ) ).second )
        throw std::invalid_argument( "object capacity '" + capacity + "' already registered." );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::DoRegister
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void CapacityFactory::DoRegister( const std::string& capacity, const Prototype_CallBack& prototypeCallback )
{
    Register( capacity, prototypeCallback );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::DoRegister
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void CapacityFactory::DoRegister( const std::string& capacity, const Prototype_CallBack& prototypeCallback, const Object_CallBack& objectCallback )
{
    Register( capacity, prototypeCallback );
    Register( capacity, objectCallback );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::Create
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
void CapacityFactory::Create( ObjectPrototype& prototype, const std::string& capacity, xml::xistream& xis )
{
    const CIPrototype_Callbacks it = prototypeCallbacks_.find( capacity );
    if( it != prototypeCallbacks_.end() )
        it->second( prototype, xis );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::Update
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void CapacityFactory::Update( MIL_Object_ABC& object, const std::string& capacity, xml::xistream& xis ) const
{
    const CIObject_Callbacks it = objectCallbacks_.find( capacity );
    if( it != objectCallbacks_.end() )
        it->second( object, xis );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::GetPropagationManager
// Created: JSR 2010-03-12
// -----------------------------------------------------------------------------
MIL_PropagationManager* CapacityFactory::GetPropagationManager() const
{
    return propagation_.get();
}
