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
#include "preparation/TeamsModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/ObjectAttributePrototypeFactory.h"

//#include "ConstructionPrototype.h"
//#include "BypassPrototype.h"
#include "ObstaclePrototype.h"
#include "NBCPrototype.h"
#include "FirePrototype.h"
#include "MedicalTreatmentPrototype.h"
#include "MinePrototype.h"
#include "LogisticPrototype.h"
#include "CrossingSitePrototype.h"
#include "SupplyRoutePrototype.h"
#include "InputPropagationPrototype.h"
#include "ActivityTimePrototype.h"

#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

using namespace kernel;
using namespace gui;

namespace
{
    typedef ObjectAttributePrototypeFactory_ABC::T_AttributeContainer  T_AttributeContainer;

    class ConstructorBuilder
    {
    public:
        ConstructorBuilder( T_AttributeContainer& container, QWidget* parent, Object_ABC*& object )
            : container_ ( container )
            , parent_ ( parent )
            , object_ ( object ) {}

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
        Object_ABC*& object_;
    };

    void ConstructorAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, Object_ABC*& object )
    {
        ConstructorBuilder builder( container, parent, object );
        std::string type( xml::attribute< std::string >( xis, "unit-type" ) );

        bool density = ( type == "density" );
        xis >> xml::optional() >> xml::list( "buildable", builder, &ConstructorBuilder::BuildPrototype, density );
        xis >> xml::optional() >> xml::list( "improvable", builder, &ConstructorBuilder::ImprovePrototype, density );
    }

    void BypassableAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    {
    }

    void LogisticAttribute( T_AttributeContainer& container, QWidget* parent, Controllers& controllers, Object_ABC*& object )
    {
        container.push_back( new LogisticPrototype( parent, controllers, object ) );
    }

    void PropagationAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& resolver, Object_ABC*& object )
    {
        std::string model( xml::attribute< std::string >( xis, "model" ) );
        if( model == "input" )
            container.push_back( new InputPropagationPrototype( parent, object ) );
        if( model == "fire" )
            container.push_back( new FirePrototype( parent, resolver, object ) );
    }

    void ContaminationAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& resolver, Object_ABC*& object )
    {
        int toxic = xml::attribute< int >( xis, "max-toxic" );
        container.push_back( new NBCPrototype( parent, resolver, toxic, object ) );
    }

    void MedicalTreatmentAttribute( T_AttributeContainer& container, QWidget* parent, const Resolver_ABC< MedicalTreatmentType >& resolver, Object_ABC*& object )
    {
        container.push_back( new MedicalTreatmentPrototype( parent, resolver, object ) );
    }
    
    void InterferenceAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    {
    }

    void InteractWithEnemyAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    {
    }

    template< typename T >
    struct Capacity
    {
        static void Build( T_AttributeContainer& container, QWidget* parent, Object_ABC*& object )
        {
            container.push_back( new T( parent, object ) );
        }
    };

    /*
    * Register capacity tag
    */
    ObjectAttributePrototypeFactory_ABC* FactoryMaker( Controllers& controllers, 
                                                       const ObjectTypes& resolver, Object_ABC*& object )
    {
        ObjectAttributePrototypeFactory* factory = new ObjectAttributePrototypeFactory();
        factory->Register( "constructor"        , boost::bind( &ConstructorAttribute, _1, _2, _3, boost::ref( object ) ) );
        factory->Register( "activable"          , boost::bind( &Capacity< ObstaclePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "time-limited"       , boost::bind( &Capacity< ActivityTimePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "supply-route"       , boost::bind( &Capacity< SupplyRoutePrototype >::Build, _2, _3, boost::ref( object ) ) );
        factory->Register( "bridging"           , boost::bind( &Capacity< CrossingSitePrototype >::Build, _2, _3, boost::ref( object ) ) );
        
        factory->Register( "logistic"           , boost::bind( &LogisticAttribute, _2, _3, boost::ref( controllers ), boost::ref( object ) ) );
        factory->Register( "interact-with-enemy", boost::bind( &InteractWithEnemyAttribute, _2, _3, boost::ref( object ) ) );
        factory->Register( "interference"       , boost::bind( &InterferenceAttribute, _2, _3, boost::ref( object ) ) );
                            
        factory->Register( "healable"           , boost::bind( &MedicalTreatmentAttribute, _2, _3, boost::ref( resolver ), boost::ref( object ) ) );
        factory->Register( "propagation"        , boost::bind( &PropagationAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( object ) ) );        

        factory->Register( "contamination"      , boost::bind( &ContaminationAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( object ) ) );  
        return factory;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, TeamsModel& teamsModel, ParametersLayer& layer, gui::SymbolIcons& icons )
    : ObjectPrototype_ABC( parent, controllers, model.objectTypes_, layer, 
                           * FactoryMaker( controllers, model.objectTypes_, creation_ ),
                           icons )
    , teamsModel_ ( teamsModel )
    , creation_   ( 0 )
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
void ObjectPrototype::Commit()
{
    const QString name = name_->text();
    const kernel::Team_ABC* team = teams_->GetValue();
    
    creation_ = teamsModel_.CreateObject( *team, const_cast< ObjectType& >( GetType() ), name, *location_ );
    if( creation_ )
        ObjectPrototype_ABC::Commit();
    creation_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::GetType
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
const ObjectType& ObjectPrototype::GetType() const
{
    const ObjectType* type = objectTypes_->GetValue();
    if( !type )
        throw std::runtime_error( "Object prototype has no type" );
    return *type;
}
