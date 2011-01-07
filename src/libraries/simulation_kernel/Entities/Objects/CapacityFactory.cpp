// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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
#include "FloodCapacity.h"
#include "MedicalCapacity.h"
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
#include "UniversalCapacity.h"
#include "WorkableCapacity.h"
#include "SealOffCapacity.h"
#include "SupplyCapacity.h"
#include "InteractIfHeightCapacity.h"
#include "PopulationFilterCapacity.h"
#include "BurnCapacity.h"
#include "BurnSurfaceCapacity.h"
#include "MIL_Object_ABC.h"
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
            object.Get< T >().Update( xis, object ); // should exist
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
            : bHasBurn_( false )
            , bHasFirePropagation_( false )
        {
        }

        void AddBurn()
        {
            bHasBurn_ = true;
        }

        void AddPropagation( ObjectPrototype& prototype, xml::xistream& xis )
        {
            const std::string model( xis.attribute< std::string >( "model", std::string() ) );
            if( model == "input" )
                prototype.AddCapacity< PropagationCapacity_ABC >( new InputPropagationCapacity( xis ) );
            else if( model == "fire" )
            {
                bHasFirePropagation_ = true;
                prototype.AddCapacity< BurnSurfaceCapacity >( new BurnSurfaceCapacity( xis ) );
            }
        }

        void Finalize( ObjectPrototype& prototype )
        {
            if( bHasBurn_ && !bHasFirePropagation_ )
                prototype.AddCapacity( new BurnCapacity() );
        }

    private:
        bool bHasBurn_;
        bool bHasFirePropagation_;
    };
}

// -----------------------------------------------------------------------------
// Name: CapacityFactory constructor
// Created: JCR 2008-05-22
// -----------------------------------------------------------------------------
CapacityFactory::CapacityFactory()
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
    DoRegister( "medical", boost::bind( &AddBuilder< MedicalCapacity >::Add, _1, _2 ) );
    DoRegister( "heuristic", boost::bind( &AddBuilder< TerrainHeuristicCapacity >::Add, _1, _2 ) );
    DoRegister( "interact-with-enemy", boost::bind( &AddBuilder< InteractWithEnemyCapacity >::Add, _1, _2 ) );
    DoRegister( "interference", boost::bind( &AddBuilder< InterferenceCapacity >::Add, _1, _2 ) );
    DoRegister( "intoxication", boost::bind( &AddBuilder< IntoxicationCapacity >::Add, _1, _2 ) );
    // logistic is not needed except during prepa
    DoRegister( "supply-route", boost::bind( &AddBuilder< InteractIfEquippedCapacity >::Add, _1, _2 ) );
    DoRegister( "mobility", boost::bind( &AddBuilder< MobilityCapacity >::Add, _1, _2 ) );
    DoRegister( "protection", boost::bind( &AddBuilder< ProtectionCapacity >::Add, _1, _2 ) );
    DoRegister( "time-limited", boost::bind( &AddBuilder< TimeLimitedCapacity >::Add, _1, _2 ) );
    DoRegister( "workable", boost::bind( &AddBuilder< WorkableCapacity >::Add, _1, _2 ) );
    DoRegister( "supply", boost::bind( &AddBuilder< SupplyCapacity >::Add, _1, _2 ) );
    DoRegister( "detection", boost::bind( &AddBuilder< DetectionCapacity >::Add, _1, _2 ) );
    DoRegister( "spawn", boost::bind( &AddBuilder< SpawnCapacity >::Add, _1, _2 ) );
    DoRegister( "perception", boost::bind( &AddBuilder< PerceptionCapacity >::Add, _1, _2 ) );
    DoRegister( "attitude-modifier", boost::bind( &AddBuilder< AttitudeModifierCapacity >::Add, _1, _2 ) );
    DoRegister( "scattering", boost::bind( &AddBuilder< ScatteringCapacity >::Add, _1, _2 ) );
    DoRegister( "sealoff", boost::bind( &AddBuilder< SealOffCapacity >::Add, _1, _2 ) );
    DoRegister( "structural-state", boost::bind( &AddBuilder< StructuralCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< StructuralCapacity >::Update, _1, _2 ) );
    // $$$$ JSR 2010-09-08: on garde l'attribut "structural" en double emploi de "structural-state" : il est utilis� par les objets et on ne peut pas changer les xsd pour l'instant.
    // Ca sera � supprimer quand les xml des objets seront � jour (voire avec RPD).
    DoRegister( "structural", boost::bind( &AddBuilder< StructuralCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< StructuralCapacity >::Update, _1, _2 ) );
    DoRegister( "resources", boost::bind( &AddBuilder< ResourceNetworkCapacity >::Add, _1, _2 ), boost::bind( &UpdateBuilder< ResourceNetworkCapacity >::Update, _1, _2 ) );
    DoRegister( "interaction-height", boost::bind( &AddBuilder< InteractIfHeightCapacity >::Add, _1, _2 ) );
    DoRegister( "population-filter", boost::bind( &AddBuilder< PopulationFilterCapacity >::Add, _1, _2 ) );

    // $$$$ BCI 2011-01-05: comment faire plus simple?
    boost::shared_ptr< FinalizableBuilders > pFinalizableBuilders( new FinalizableBuilders() );
    DoRegister( "burn", boost::bind( &FinalizableBuilders::AddBurn, pFinalizableBuilders ) );
    DoRegister( "propagation", boost::bind( &FinalizableBuilders::AddPropagation, pFinalizableBuilders, _1, _2 ) );
    RegisterFinalizeCreate( boost::bind( &FinalizableBuilders::Finalize, pFinalizableBuilders, _1 ) );
    DoRegister( "flood", boost::bind( &AddBuilder< FloodCapacity >::Add, _1, _2 ) );
    DoRegister( "universal", boost::bind( &AddBuilder< UniversalCapacity >::Add, _1, _2 ) );
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
// Name: CapacityFactory::FinalizeCreate
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
void CapacityFactory::FinalizeCreate( ObjectPrototype& prototype )
{
    for( FinalizePrototype_CallBacks::iterator it = finalizePrototypeCallbacks_.begin(); it != finalizePrototypeCallbacks_.end(); ++it )
        (*it)( prototype );
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
