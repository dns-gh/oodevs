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
#include "MIL_Object_ABC.h"
#include "ActivableCapacity.h"
#include "AltitudeModifierCapacity.h"
#include "AttitudeModifierCapacity.h"
#include "AttritionCapacity.h"
#include "AvoidanceCapacity.h"
#include "BorderCapacity.h"
#include "BridgingCapacity.h"
#include "BurnCapacity.h"
#include "BurnSurfaceCapacity.h"
#include "BypassableCapacity.h"
#include "CloudPropagationCapacity.h"
#include "ConstructionCapacity.h"
#include "ContaminationCapacity.h"
#include "DecontaminationCapacity.h"
#include "DelayCapacity.h"
#include "DetectionCapacity.h"
#include "FireForbiddenCapacity.h"
#include "FirePropagationModifierCapacity.h"
#include "FloodCapacity.h"
#include "InfrastructureCapacity.h"
#include "InputPropagationCapacity.h"
#include "InteractIfEquippedCapacity.h"
#include "InteractIfHeightCapacity.h"
#include "InteractWithSideCapacity.h"
#include "InterferenceCapacity.h"
#include "IntoxicationCapacity.h"
#include "LodgingCapacity.h"
#include "LogisticCapacity.h"
#include "MedicalCapacity.h"
#include "MobilityCapacity.h"
#include "PerceptionCapacity.h"
#include "PopulationFilterCapacity.h"
#include "ProtectionCapacity.h"
#include "ResourceNetworkCapacity.h"
#include "ScatteringCapacity.h"
#include "SpawnCapacity.h"
#include "StructuralCapacity.h"
#include "SupplyCapacity.h"
#include "TerrainHeuristicCapacity.h"
#include "TimeLimitedCapacity.h"
#include "UndergroundCapacity.h"
#include "UniversalCapacity.h"
#include "UrbanDestructionCapacity.h"
#include "WorkableCapacity.h"
#include "TrafficabilityCapacity.h"
#include "DisasterCapacity.h"

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
            if( T* capacity = object.Retrieve< T >() )
                capacity->Update( xis, object );
            else
                object.AddCapacity( new T( xis ) );
        }
    };

    void AddConstructor( ObjectPrototype& prototype, xml::xistream& xis )
    {
        ConstructionCapacity constructor( prototype, xis );
        xis >> xml::list( constructor, &ConstructionCapacity::AddCapacity );
    }

    class FinalizableBuilders
    {
    public:
        FinalizableBuilders()
        {
            Reset();
        }

        void AddBurn()
        {
            bHasBurn_ = true;
        }

        void AddPropagation( ObjectPrototype& prototype, xml::xistream& xis )
        {
            const std::string model( xis.attribute< std::string >( "model", std::string() ) );
            if( model == "shapefile-input" )
                prototype.AddCapacity< PropagationCapacity_ABC >( new InputPropagationCapacity( xis ) );
            else if( model == "fire" )
            {
                bHasFirePropagation_ = true;
                prototype.AddCapacity< BurnSurfaceCapacity >( new BurnSurfaceCapacity( xis ) );
            }
            else if( model == "cloud" )
                prototype.AddCapacity( new CloudPropagationCapacity() );
        }

        void Finalize( ObjectPrototype& prototype )
        {
            if( bHasBurn_ && !bHasFirePropagation_ )
                prototype.AddCapacity( new BurnCapacity() );
            Reset();
        }

    private:
        void Reset()
        {
            bHasBurn_ = false;
            bHasFirePropagation_ = false;

        }
        bool bHasBurn_;
        bool bHasFirePropagation_;
    };
}

using namespace legacy;

// -----------------------------------------------------------------------------
// Name: CapacityFactory constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CapacityFactory::CapacityFactory()
{
    // $$$$ BCI 2011-01-05: comment faire plus simple?
    boost::shared_ptr< FinalizableBuilders > pFinalizableBuilders( new FinalizableBuilders() );

    DoRegister( "activable", boost::bind( &AddBuilder< ActivableCapacity >::Add, _1, _2 ) );
    DoRegister( "altitude-modifier", boost::bind( &AddBuilder< AltitudeModifierCapacity >::Add, _1, _2 ) );
    DoRegister( "attitude-modifier", boost::bind( &AddBuilder< AttitudeModifierCapacity >::Add, _1, _2 ) );
    DoRegister( "attrition", boost::bind( &AddBuilder< AttritionCapacity >::Add, _1, _2 ) );
    DoRegister( "avoidable", boost::bind( &AddBuilder< AvoidanceCapacity >::Add, _1, _2 ) );
    DoRegister( "border", boost::bind( &AddBuilder< BorderCapacity >::Add, _1, _2 ) );
    DoRegister( "bridging", boost::bind( &AddBuilder< BridgingCapacity >::Add, _1, _2 ) );
    DoRegister( "burn", boost::bind( &FinalizableBuilders::AddBurn, pFinalizableBuilders ) );
    DoRegister( "bypassable", boost::bind( &AddBuilder< BypassableCapacity >::Add, _1, _2 ) );
    DoRegister( "constructor", boost::bind( &AddConstructor, _1, _2 ) );
    DoRegister( "contamination", boost::bind( &AddBuilder< ContaminationCapacity >::Add, _1, _2 ) );
    DoRegister( "decontamination", boost::bind( &AddBuilder< DecontaminationCapacity >::Add, _1, _2 ) );
    DoRegister( "delay", boost::bind( &AddBuilder< DelayCapacity >::Add, _1, _2 ) );
    DoRegister( "detection", boost::bind( &AddBuilder< DetectionCapacity >::Add, _1, _2 ) );
    DoRegister( "disaster", boost::bind( &AddBuilder< DisasterCapacity >::Add, _1, _2 ) );
    DoRegister( "fire-forbidden", boost::bind( &AddBuilder< FireForbiddenCapacity >::Add, _1, _2 ) );
    DoRegister( "fire-propagation-modifier", boost::bind( &AddBuilder< FirePropagationModifierCapacity >::Add, _1, _2 ) );
    DoRegister( "flood", boost::bind( &AddBuilder< FloodCapacity >::Add, _1, _2 ) );
    DoRegister( "heuristic", boost::bind( &AddBuilder< TerrainHeuristicCapacity >::Add, _1, _2 ) );
    DoRegister( "infrastructure", boost::bind( &AddBuilder< InfrastructureCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< InfrastructureCapacity >::Update, _1, _2 ) );
    DoRegister( "interact-with-enemy", boost::bind( &AddBuilder< InteractWithSideCapacity >::Add, _1, _2 ) );
    DoRegister( "interaction-height", boost::bind( &AddBuilder< InteractIfHeightCapacity >::Add, _1, _2 ) );
    DoRegister( "interference", boost::bind( &AddBuilder< InterferenceCapacity >::Add, _1, _2 ) );
    DoRegister( "intoxication", boost::bind( &AddBuilder< IntoxicationCapacity >::Add, _1, _2 ) );
    DoRegister( "lodging", boost::bind( &AddBuilder< LodgingCapacity >::Add, _1, _2 ) );
    DoRegister( "logistic", boost::bind( &AddBuilder< LogisticCapacity >::Add, _1, _2 ) );
    DoRegister( "mobility", boost::bind( &AddBuilder< MobilityCapacity >::Add, _1, _2 ) );
    DoRegister( "perception", boost::bind( &AddBuilder< PerceptionCapacity >::Add, _1, _2 ) );
    DoRegister( "population-filter", boost::bind( &AddBuilder< PopulationFilterCapacity >::Add, _1, _2 ) );
    DoRegister( "propagation", boost::bind( &FinalizableBuilders::AddPropagation, pFinalizableBuilders, _1, _2 ) );
    DoRegister( "protection", boost::bind( &AddBuilder< ProtectionCapacity >::Add, _1, _2 ) );
    DoRegister( "resources", boost::bind( &AddBuilder< ResourceNetworkCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< ResourceNetworkCapacity >::Update, _1, _2 ) );
    DoRegister( "scattering", boost::bind( &AddBuilder< ScatteringCapacity >::Add, _1, _2 ) );
    DoRegister( "spawn", boost::bind( &AddBuilder< SpawnCapacity >::Add, _1, _2 ) );
    DoRegister( "structural", boost::bind( &AddBuilder< StructuralCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< StructuralCapacity >::Update, _1, _2 ) );
    DoRegister( "supply", boost::bind( &AddBuilder< SupplyCapacity >::Add, _1, _2 ) );
    DoRegister( "supply-route", boost::bind( &AddBuilder< InteractIfEquippedCapacity >::Add, _1, _2 ) );
    DoRegister( "time-limited", boost::bind( &AddBuilder< TimeLimitedCapacity >::Add, _1, _2 ) );
    DoRegister( "trafficability", boost::bind( &AddBuilder< TrafficabilityCapacity >::Add, _1, _2 ) );
    DoRegister( "underground-network", boost::bind( &AddBuilder< UndergroundCapacity >::Add, _1, _2 ) );
    DoRegister( "universal", boost::bind( &AddBuilder< UniversalCapacity >::Add, _1, _2 ) );
    DoRegister( "urban-destruction", boost::bind( &AddBuilder< UrbanDestructionCapacity >::Add, _1, _2 ) );
    DoRegister( "workable", boost::bind( &AddBuilder< WorkableCapacity >::Add, _1, _2 ) );
    
    RegisterFinalizeCreate( boost::bind( &FinalizableBuilders::Finalize, pFinalizableBuilders, _1 ) );
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
        throw MASA_EXCEPTION( "Prototype capacity '" + capacity + "' already registered." );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::Register
// Created: JSR 2010-08-12
// -----------------------------------------------------------------------------
void CapacityFactory::Register( const std::string& capacity, const Object_CallBack& callback )
{
    if( ! objectCallbacks_.insert( std::make_pair( capacity, callback ) ).second )
        throw MASA_EXCEPTION( "Object capacity '" + capacity + "' already registered." );
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
void CapacityFactory::Create( ObjectPrototype& prototype, const std::string& capacity, xml::xistream& xis ) const
{
    const CIPrototype_Callbacks it = prototypeCallbacks_.find( capacity );
    if( it != prototypeCallbacks_.end() )
        it->second( prototype, xis );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::FinalizeCreate
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
void CapacityFactory::FinalizeCreate( ObjectPrototype& prototype ) const
{
    for( FinalizePrototype_CallBacks::const_iterator it = finalizePrototypeCallbacks_.begin(); it != finalizePrototypeCallbacks_.end(); ++it )
        ( *it )( prototype );
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory::RegisterFinalizeCreate
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
void CapacityFactory::RegisterFinalizeCreate( FinalizePrototype_CallBack finalizePrototypeCallback )
{
    finalizePrototypeCallbacks_.push_back( finalizePrototypeCallback );
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
