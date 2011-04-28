// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectPrototype.h"
#include "CrossingSitePrototype.h"
#include "SupplyRoutePrototype.h"
#include "LogisticPrototype.h"
#include "NBCPrototype.h"
#include "MinePrototype.h"
#include "ObstaclePrototype.h"
#include "MedicalTreatmentPrototype.h"
#include "FirePrototype.h"
#include "FirePropagationModifierPrototype.h"
#include "ActivityTimePrototype.h"
#include "DelayPrototype.h"
#include "FloodPrototype.h"
#include "ResourceNetworkPrototype.h"
#include "StockPrototype.h"
#include "actions/ActionTiming.h"
#include "actions/Army.h"
#include "actions/Location.h"
#include "actions/ObjectMagicAction.h"
#include "actions/ParameterList.h"
#include "actions/String.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/ObjectAttributePrototypeFactory.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace actions;
using namespace gui;
using namespace parameters;

namespace
{
    typedef ObjectAttributePrototypeFactory_ABC::T_AttributeContainer  T_AttributeContainer;

    class ConstructorBuilder
    {
    public:
        ConstructorBuilder( T_AttributeContainer& container, QWidget* parent, ParameterList*& attributesList )
            : container_ ( container )
            , parent_ ( parent )
            , attributesList_ ( attributesList ) {}

        void BuildPrototype( xml::xistream& /*xis*/, bool /*density*/ )
        {
        }

        void ImprovePrototype( xml::xistream& /*xis*/, bool /*density*/ )
        {
        }
    private:
        ConstructorBuilder& operator=( const ConstructorBuilder& );
        ConstructorBuilder( const ConstructorBuilder& );

    private:
        T_AttributeContainer& container_;
        QWidget* parent_;
        ParameterList*& attributesList_;
    };

    void ConstructorAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, ParameterList*& attributesList )
    {
        ConstructorBuilder builder( container, parent, attributesList );
        std::string type( xis.attribute< std::string >( "unit-type" ) );

        bool density = ( type == "density" );
        xis >> xml::optional >> xml::list( "buildable", builder, &ConstructorBuilder::BuildPrototype, density );
        xis >> xml::optional >> xml::list( "improvable", builder, &ConstructorBuilder::ImprovePrototype, density );
    }

    //void BypassableAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    //{
    //}

    void LogisticAttribute( T_AttributeContainer& container, QWidget* parent, kernel::Controllers& controllers, ParameterList*& attributesList )
    {
        container.push_back( new LogisticPrototype( parent, controllers, attributesList ) );
    }

    void StockAttribute( xml::xistream& /*xis*/, T_AttributeContainer& container, QWidget* parent, const kernel::ObjectTypes& resolver, ParameterList*& attributesList )
    {
        container.push_back( new StockPrototype( parent, resolver, attributesList ) );
    }

    void ContaminationAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const kernel::ObjectTypes& resolver, ParameterList*& attributesList )
    {
        int toxic = xis.attribute< int >( "max-toxic" );
        container.push_back( new NBCPrototype( parent, resolver, toxic, attributesList ) );
    }

    void MedicalTreatmentAttribute( T_AttributeContainer& container, QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver, ParameterList*& attributesList )
    {
        container.push_back( new MedicalTreatmentPrototype( parent, resolver, attributesList ) );
    }

    void ResourceNetworkAttribute( T_AttributeContainer& container, QWidget* parent, ParameterList*& attributesList )
    {
        container.push_back( new ResourceNetworkPrototype( parent, attributesList ) );
    }

    template<typename T>
    struct Capacity
    {
        static void Build( T_AttributeContainer& container, QWidget* parent, ParameterList*& attributesList )
        {
            container.push_back( new T( parent, attributesList ) );
        }
    };

    class FinalizableBuilders
    {
    public:
        FinalizableBuilders()
        {
            Reset();
        }

        void AddBurn( T_AttributeContainer& container, QWidget* parent, const kernel::ObjectTypes& resolver, ParameterList*& attributesList )
        {
            pFirePrototype_ = new FirePrototype( parent, resolver, attributesList );
            container.push_back( pFirePrototype_ );
        }

        void AddPropagation( xml::xistream& xis )
        {
            const std::string model( xis.attribute< std::string >( "model", std::string() ) );
            if( model == "fire" )
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

    ObjectAttributePrototypeFactory_ABC* FactoryMaker( kernel::Controllers& controllers, const kernel::ObjectTypes& resolver, ParameterList*& attributesList )
    {
        ObjectAttributePrototypeFactory* factory = new ObjectAttributePrototypeFactory();
        factory->Register( "constructor"               , boost::bind( &ConstructorAttribute, _1, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "activable"                 , boost::bind( &Capacity< ObstaclePrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "time-limited"              , boost::bind( &Capacity< ActivityTimePrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "delay"                     , boost::bind( &Capacity< DelayPrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "supply-route"              , boost::bind( &Capacity< SupplyRoutePrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "bridging"                  , boost::bind( &Capacity< CrossingSitePrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "flood"                     , boost::bind( &Capacity< FloodPrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "logistic"                  , boost::bind( &LogisticAttribute, _2, _3, boost::ref( controllers ), boost::ref( attributesList ) ) );
        factory->Register( "medical"                   , boost::bind( &MedicalTreatmentAttribute, _2, _3, boost::ref( resolver ), boost::ref( attributesList ) ) );
        factory->Register( "contamination"             , boost::bind( &ContaminationAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( attributesList ) ) );
        factory->Register( "fire-propagation-modifier" , boost::bind( &Capacity< FirePropagationModifierPrototype >::Build, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "resources"                 , boost::bind( &ResourceNetworkAttribute, _2, _3, boost::ref( attributesList ) ) );
        factory->Register( "stock"                     , boost::bind( &StockAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( attributesList ) ) );

        boost::shared_ptr< FinalizableBuilders > pFinalizableBuilders( new FinalizableBuilders() );
        factory->Register( "burn"                      , boost::bind( &FinalizableBuilders::AddBurn, pFinalizableBuilders, _2, _3, boost::ref( resolver ), boost::ref( attributesList ) ) );
        factory->Register( "propagation"               , boost::bind( &FinalizableBuilders::AddPropagation, pFinalizableBuilders, _1 ) );
        factory->RegisterFinalizeCreate( boost::bind( &FinalizableBuilders::Finalize, pFinalizableBuilders ) );

        return factory;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, gui::ParametersLayer& layer )
    : ObjectPrototype_ABC( parent, controllers, model.objectTypes_, layer, *FactoryMaker( controllers, model.objectTypes_, attributesList_ ) )
    , static_( model )
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
// Name: ObjectPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void ObjectPrototype::Commit( actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation )
{
    if( CheckValidity() )
    {
        kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "create_object" );
        ObjectMagicAction* action = new ObjectMagicAction( 0, actionType, controllers_.controller_, true );
        action->Rename( tools::translate( "gaming_app::Action", "Object Creation" ) );
        tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

        action->AddParameter( *new String( it.NextElement(), objectTypes_->GetValue()->GetType() ) );
        kernel::Point point;
        action->AddParameter( *new Location( it.NextElement(), static_.coordinateConverter_, location_? *location_ : point ) );
        action->AddParameter( *new String( it.NextElement(), name_->text().isEmpty() ? "" : name_->text().ascii() ) );
        action->AddParameter( *new Army( it.NextElement(), *teams_->GetValue(), controllers_.controller_ ) );

        attributesList_ = new ParameterList( it.NextElement() );
        action->AddParameter( *attributesList_ );

        ObjectPrototype_ABC::Commit();

        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation ) );
        action->RegisterAndPublish( actionsModel );

        ObjectPrototype_ABC::Clean();
    }
}
