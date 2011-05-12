// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectPrototype.h"
#include "preparation/StaticModel.h"
#include "preparation/ObjectsModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/ObjectAttributePrototypeFactory.h"
#include "clients_gui/LoadableLineEdit.h"
#include "ConstructionPrototype.h"
#include "CrossingSitePrototype.h"
#include "FirePrototype.h"
#include "FloodPrototype.h"
#include "LodgingPrototype.h"
#include "LogisticPrototype.h"
#include "MedicalTreatmentPrototype.h"
#include "MinePrototype.h"
#include "NBCPrototype.h"
#include "ObstaclePrototype.h"
#include "StockPrototype.h"
#include "ResourceNetworkPrototype.h"
#include "SupplyRoutePrototype.h"
#include "InputPropagationPrototype.h"
#include "ActivityTimePrototype.h"
#include "DelayPrototype.h"
#include "FirePropagationModifierPrototype.h"
#include "preparation/UrbanModel.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

namespace
{
    typedef ObjectAttributePrototypeFactory_ABC::T_AttributeContainer  T_AttributeContainer;

    void ConstructorAttribute( xml::xistream& /*xis*/, T_AttributeContainer& container, QWidget* parent, Object_ABC*& object )
    {
        // $$$$ SBO 2009-06-09: TODO...
//        const bool density = xis.attribute< std::string >( "unit-type" ) == "density";
//        xis >> xml::optional >> xml::list( "buildable", builder, &ConstructorBuilder::BuildPrototype, density );
//        xis >> xml::optional >> xml::list( "improvable", builder, &ConstructorBuilder::ImprovePrototype, density );
        container.push_back( new ConstructionPrototype( parent, object ) );
    }

    void BypassableAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    {
    }

    void LodgingAttribute( T_AttributeContainer& container, QWidget* parent, Object_ABC*& object )
    {
        container.push_back( new LodgingPrototype( parent, object ) );
    }

    void LogisticAttribute( T_AttributeContainer& container, QWidget* parent, Controllers& controllers, Object_ABC*& object )
    {
        container.push_back( new LogisticPrototype( parent, controllers, object ) );
    }

    void FloodAttribute( T_AttributeContainer& container, QWidget* parent, Controllers& controllers, const DetectionMap& detection, Object_ABC*& object )
    {
        container.push_back( new FloodPrototype( parent, object, controllers, detection ) );
    }

    void StockAttribute( xml::xistream& /*xis*/, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& resolver, const tools::GeneralConfig& /*config*/, Object_ABC*& object )
    {
        container.push_back( new StockPrototype( parent, resolver, object ) );
    }

    void ContaminationAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& resolver, Object_ABC*& object )
    {
        int toxic = xis.attribute< int >( "max-toxic" );
        container.push_back( new NBCPrototype( parent, resolver, toxic, object ) );
    }

    void MedicalTreatmentAttribute( T_AttributeContainer& container, QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType, std::string >& resolver, Object_ABC*& object )
    {
        container.push_back( new MedicalTreatmentPrototype( parent, resolver, object ) );
    }

    void InterferenceAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    {
    }

    void InteractWithEnemyAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    {
    }

    void ResourceNetworkAttribute( T_AttributeContainer& container, QWidget* parent, Controllers& controllers, const UrbanModel& urbanModel, const ObjectsModel& objectsModel, const tools::StringResolver< ResourceNetworkType >& resources, Object_ABC*& object )
    {
        container.push_back( new ResourceNetworkPrototype( parent, object, controllers, urbanModel, objectsModel, resources ) );
    }

    template< typename T >
    struct Capacity
    {
        static void Build( T_AttributeContainer& container, QWidget* parent, Object_ABC*& object )
        {
            container.push_back( new T( parent, object ) );
        }
    };

    class FinalizableBuilders
    {
    public:
        FinalizableBuilders()
        {
            Reset();
        }

        void AddBurn( T_AttributeContainer& container, QWidget* parent, const kernel::ObjectTypes& resolver, Object_ABC*& object )
        {
            pFirePrototype_ = new FirePrototype( parent, resolver, object );
            container.push_back( pFirePrototype_ );
        }

        void AddPropagation( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& /*resolver*/, const tools::GeneralConfig& config, Object_ABC*& object )
        {
            std::string model( xis.attribute< std::string >( "model" ) );
            if( model == "input" )
                container.push_back( new InputPropagationPrototype( parent, config, object ) );
            else if( model == "fire" )
                bHasFirePropagation_ = true;
        }

        void Finalize()
        {
            if( pFirePrototype_ )
                pFirePrototype_->SetHasFirePropagation( bHasFirePropagation_ );
            Reset();
        }

    private:
        void Reset()
        {
            pFirePrototype_ = 0;
            bHasFirePropagation_ = false;

        }
        FirePrototype* pFirePrototype_;
        bool bHasFirePropagation_;
    };

    /*
    * Register capacity tag
    */
    ObjectAttributePrototypeFactory_ABC& FactoryBuilder( Controllers& controllers, const ObjectTypes& resolver, const DetectionMap& detection, ObjectsModel& objectsModel, const UrbanModel& urbanModel, const tools::GeneralConfig& config, Object_ABC*& object )
    {
        ObjectAttributePrototypeFactory* factory = new ObjectAttributePrototypeFactory();
        factory->Register( "constructor"               , boost::bind( &::ConstructorAttribute, _1, _2, _3, boost::ref( object ) ) );
        factory->Register( "activable"                 , boost::bind( &Capacity< ObstaclePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "time-limited"              , boost::bind( &Capacity< ActivityTimePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "supply-route"              , boost::bind( &Capacity< SupplyRoutePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "bridging"                  , boost::bind( &Capacity< CrossingSitePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "delay"                     , boost::bind( &Capacity< DelayPrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "fire-propagation-modifier" , boost::bind( &Capacity< FirePropagationModifierPrototype >::Build, _2, _3, boost::ref( object ) ) );

        factory->Register( "flood"                     , boost::bind( &::FloodAttribute, _2, _3, boost::ref( controllers ), boost::cref( detection ), boost::ref( object ) ) );
        factory->Register( "lodging"                   , boost::bind( &::LodgingAttribute, _2, _3, boost::ref( object ) ) );
        factory->Register( "logistic"                  , boost::bind( &::LogisticAttribute, _2, _3, boost::ref( controllers ), boost::ref( object ) ) );
        factory->Register( "interact-with-enemy"       , boost::bind( &::InteractWithEnemyAttribute, _2, _3, boost::ref( object ) ) );
        factory->Register( "interference"              , boost::bind( &::InterferenceAttribute, _2, _3, boost::ref( object ) ) );

        factory->Register( "medical"                   , boost::bind( &::MedicalTreatmentAttribute, _2, _3, boost::ref( resolver ), boost::ref( object ) ) );
        factory->Register( "stock"                     , boost::bind( &::StockAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( config ), boost::ref( object ) ) );
        factory->Register( "contamination"             , boost::bind( &::ContaminationAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( object ) ) );
        factory->Register( "resources"                 , boost::bind( &::ResourceNetworkAttribute, _2, _3, boost::ref( controllers ), boost::cref( urbanModel ), boost::cref( objectsModel ), boost::cref( resolver ), boost::ref( object ) ) );

        boost::shared_ptr< FinalizableBuilders > pFinalizableBuilders( new FinalizableBuilders() );
        factory->Register( "burn"                      , boost::bind( &FinalizableBuilders::AddBurn, pFinalizableBuilders, _2, _3, boost::ref( resolver ), boost::ref( object ) ) );
        factory->Register( "propagation"               , boost::bind( &FinalizableBuilders::AddPropagation, pFinalizableBuilders, _1, _2, _3, boost::ref( resolver ), boost::ref( config ), boost::ref( object ) ) );
        factory->RegisterFinalizeCreate( boost::bind( &FinalizableBuilders::Finalize, pFinalizableBuilders ) );

        return *factory;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, ObjectsModel& objectsModel, const UrbanModel& urbanModel, ParametersLayer& layer, const tools::GeneralConfig& config )
: ObjectPrototype_ABC( parent, controllers, model.coordinateConverter_, model.objectTypes_, layer, FactoryBuilder( controllers, model.objectTypes_, model.detection_, objectsModel, urbanModel, config, creation_ ) )
    , model_   ( objectsModel )
    , creation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype destructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::~ObjectPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::DoCommit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::DoCommit()
{
    creation_ = model_.CreateObject( *teams_->GetValue(), *objectTypes_->GetValue(), GetCurrentName(), GetCurrentLocation() );
    if( creation_ )
        ObjectPrototype_ABC::DoCommit();
    creation_ = 0;
}
