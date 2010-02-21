// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ObjectPrototype.h"
#include "gaming/StaticModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/ObjectAttributePrototypeFactory.h"

#include "CrossingSitePrototype.h"
#include "SupplyRoutePrototype.h"
#include "LogisticPrototype.h"
#include "NBCPrototype.h"
#include "MinePrototype.h"
#include "ObstaclePrototype.h"
#include "MedicalTreatmentPrototype.h"
#include "FirePrototype.h"
#include "ActivityTimePrototype.h"

#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>
#include <protocol/clientsenders.h>
#include <protocol/publisher_ABC.h>


using namespace kernel;
using namespace gui;

namespace
{
    typedef ObjectAttributePrototypeFactory_ABC::T_AttributeContainer  T_AttributeContainer;

    class ConstructorBuilder
    {
    public:
        ConstructorBuilder( T_AttributeContainer& container, QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& message )
            : container_ ( container )
            , parent_ ( parent )
            , message_ ( message ) {}

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
        MsgsClientToSim::MsgMagicActionCreateObject& message_;
    };

    void ConstructorAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& message )
    {
        ConstructorBuilder builder( container, parent, message );
        std::string type( xml::attribute< std::string >( xis, "unit-type" ) );

        bool density = ( type == "density" );
        xis >> xml::optional() >> xml::list( "buildable", builder, &ConstructorBuilder::BuildPrototype, density );
        xis >> xml::optional() >> xml::list( "improvable", builder, &ConstructorBuilder::ImprovePrototype, density );
    }

    //void BypassableAttribute( T_AttributeContainer& /*container*/, QWidget* /*parent*/, Object_ABC*& /*object*/ )
    //{
    //}

    void LogisticAttribute( T_AttributeContainer& container, QWidget* parent, Controllers& controllers, MsgsClientToSim::MsgMagicActionCreateObject& message )
    {
        container.push_back( new LogisticPrototype( parent, controllers, message ) );
    }
    
    void PropagationAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& resolver, MsgsClientToSim::MsgMagicActionCreateObject& message )
    {
        std::string model( xml::attribute< std::string >( xis, "model" ) );
        if( model == "input" )
        {
            // NOT ALLOWED DURING GAMING SESSION
            // container.push_back( new InputPropagationPrototype( parent, resolver, message ) );
        }
        if( model == "fire" )
            container.push_back( new FirePrototype( parent, resolver, message ) );
    }

    void ContaminationAttribute( xml::xistream& xis, T_AttributeContainer& container, QWidget* parent, const ObjectTypes& resolver, MsgsClientToSim::MsgMagicActionCreateObject& message )
    {
        int toxic = xml::attribute< int >( xis, "max-toxic" );
        container.push_back( new NBCPrototype( parent, resolver, toxic, message ) );
    }

    void MedicalTreatmentAttribute( T_AttributeContainer& container, QWidget* parent, const tools::Resolver_ABC< MedicalTreatmentType >& resolver, MsgsClientToSim::MsgMagicActionCreateObject& message )
    {
        container.push_back( new MedicalTreatmentPrototype( parent, resolver, message ) );
    }

    template<typename T>
    struct Capacity
    {
        static void Build( T_AttributeContainer& container, QWidget* parent, MsgsClientToSim::MsgMagicActionCreateObject& message )
        {
            container.push_back( new T( parent, message ) );
        }
    };
    
    ObjectAttributePrototypeFactory_ABC* FactoryMaker( Controllers& controllers, 
                                                       const ObjectTypes& resolver, MsgsClientToSim::MsgMagicActionCreateObject& message )
    {
        ObjectAttributePrototypeFactory* factory = new ObjectAttributePrototypeFactory();
        factory->Register( "constructor"    , boost::bind( &ConstructorAttribute, _1, _2, _3, boost::ref( message ) ) );
        factory->Register( "activable"      , boost::bind( &Capacity< ObstaclePrototype >::Build, _2, _3, boost::ref( message ) ) );
        factory->Register( "time-limited"   , boost::bind( &Capacity< ActivityTimePrototype >::Build, _2, _3, boost::ref( message ) ) );
        factory->Register( "supply-route"   , boost::bind( &Capacity< SupplyRoutePrototype >::Build, _2, _3, boost::ref( message ) ) );
        factory->Register( "bridging"       , boost::bind( &Capacity< CrossingSitePrototype >::Build, _2, _3, boost::ref( message ) ) );
        
        factory->Register( "logistic"       , boost::bind( &LogisticAttribute, _2, _3, boost::ref( controllers ), boost::ref( message ) ) );

        factory->Register( "healable"       , boost::bind( &MedicalTreatmentAttribute, _2, _3, boost::ref( resolver ), boost::ref( message ) ) );
        factory->Register( "propagation"    , boost::bind( &PropagationAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( message ) ) );
                       
        factory->Register( "contamination"  , boost::bind( &ContaminationAttribute, _1, _2, _3, boost::ref( resolver ), boost::ref( message ) ) );
        return factory;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: SBO 2006-04-18
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, gui::ParametersLayer& layer )
    : ObjectPrototype_ABC( parent, controllers, model.objectTypes_, layer, *FactoryMaker( controllers, model.objectTypes_, creation_ ) )
    , serializer_( model.coordinateConverter_, *creation_.mutable_location() )
{
    //msg_().action.t               = T_MsgObjectMagicAction_action_create_object;
    *msg_().mutable_action()->mutable_create_object() = creation_;
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
void ObjectPrototype::Commit( Publisher_ABC& publisher )
{    
    if( CheckValidity() )
    {    
        std::string name = name_->text().isEmpty() ? "" : name_->text().ascii();
        creation_.set_name( name.c_str() );
        creation_.set_team( teams_->GetValue()->GetId() );
        creation_.set_type( objectTypes_->GetValue()->GetType().c_str() );
        if( location_ )
            serializer_.Serialize( *location_ );    
        ObjectPrototype_ABC::Commit();
        msg_.Send( publisher );
        Clean();
    }
}
