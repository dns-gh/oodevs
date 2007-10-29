// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "FunctionFactory.h"
#include "DispatchedFunctionHelper.h"
#include "Attributes.h"
#include "Existences.h"
#include "Events.h"
#include "IdentifierValue.h"
#include "Selector.h"
#include "Connectors.h"
#include "Task.h"
#include "Plotter.h"
#include "Distance.h"
#include "Filter.h"
#include "Count.h"
#include "IsOneOf.h"
#include "Domain.h"
#include "Derivate.h"
#include "Integrate.h"
#include "Minimum.h"
#include "Maximum.h"
#include "Adder.h"
#include "Meaner.h"
#include "Composer.h"
#include "Constant.h"
#include "Contains.h"
#include "Compare.h"
#include "TypeDispatcher.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: FunctionFactory constructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
FunctionFactory::FunctionFactory()
    : currentContext_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory destructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
FunctionFactory::~FunctionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Describe
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void FunctionFactory::Describe( xml::xostream& xos )
{
    xos << xml::start( "functions" )
            << "$$$$ Not implemented $$$$"
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Task > FunctionFactory::CreateTask
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Task > FunctionFactory::CreateTask( int context, xml::xistream& xis )
{
    currentContext_ = context;
    boost::shared_ptr< Task > result( new Task() );
    xis >> xml::start( "indicator" )
            >> xml::list( *this, &FunctionFactory::CreateFunction, *result )
        >> xml::end();
    result->Connect( xis );
    return result;
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::CreateFunction
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void FunctionFactory::CreateFunction( const std::string& type, xml::xistream& xis, Task& result )
{
    if( type == "extract" )
        Extract( xis, result );
    else if( type == "transform" )
        Transform( xis, result );
    else if( type == "reduce" )
        Reduce( xis, result );
    else if( type == "constant" )
        MakeConstant( xis, result );
    else if( type == "plot" )
        Plot( xis, result );
    else
        throw std::runtime_error( "Unknown function type '" + type + "'" );
}

#define TYPE_DISPATCH_HELPER( name )                                            \
struct FunctionFactory::name##Dispatcher                                        \
{                                                                               \
    name##Dispatcher( FunctionFactory* that ): that_( that ) {}                 \
    template< typename T >                                                      \
    void operator()( const std::string& n, xml::xistream& xis, Task& result )   \
    {                                                                           \
        that_->name< T >( n, xis, result );                                     \
    }                                                                           \
    FunctionFactory* that_;                                                     \
};

TYPE_DISPATCH_HELPER( MakeConstant )

// -----------------------------------------------------------------------------
// Name: FunctionFactory::MakeConstant
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::MakeConstant( const std::string& name, xml::xistream& xis, Task& result )
{
    typedef FunctionConnector< IdentifierValue::Type, T > Connector;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< ModelFunction_ABC > function( new ::Constant< T >( xis, connector->handlers_.Parameter() ) );

    result.AddExtractor( function );
    result.AddConnector( name, connector );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::MakeConstant
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void FunctionFactory::MakeConstant( xml::xistream& xis, Task& result )
{
    MakeConstantDispatcher functor( this );
    TypeDispatcher dispatcher( xis, result );
    dispatcher.Dispatch( functor );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Extract
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
template< typename Value >
void FunctionFactory::Extract( const std::string& name, xml::xistream& xis, Task& result )
{
    DispatcherFactory< IdentifierValue, Value > factory( xis );
    typedef FunctionConnector< IdentifierValue::Type, typename Value::Type > Connector;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< ModelFunction_ABC > function( factory(
        connector->handlers_.KeyParameter(),
        connector->handlers_.Parameter() ) );

    result.AddExtractor( function );
    result.AddConnector( name, connector );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Extract
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void FunctionFactory::Extract( xml::xistream& xis, Task& result )
{
    std::string value, name;
    xis >> xml::attribute( "function", value ) >> xml::attribute( "id", name );
    if( value == "operational-state" )
        Extract< attributes::OperationalState >( name, xis, result );
    else if( value == "position" )
        Extract< attributes::Position >( name, xis, result );
    else if( value == "resources" )
        Extract< attributes::Resources >( name, xis, result );
    else if( value == "maintenance-handling-unit" )
        Extract< existences::MaintenanceHandlingUnitId >( name, xis, result );
    else if( value == "direct-fire-unit" )
        Extract< existences::DirectFireUnitId >( name, xis, result );
    else if( value == "fire-component-damage" )
        Extract< events::FireComponentDamages >( name, xis, result );
    else
        ValueError( value );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::ReduceFunction
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
template< typename F >
void FunctionFactory::ReduceFunction( const std::string& name, xml::xistream& xis, Task& result )
{
    typedef typename F::Key_Type K;
    typedef typename F::Argument_Type T;
    typedef typename F::Result_Type R;
    boost::shared_ptr< FunctionConnector< K, R > > connector( new FunctionConnector< K, R >() );
    boost::shared_ptr< Function_ABC > function( new F( xis, *connector ) );
    result.AddFunction( name, function );
    result.AddConnector( name, connector );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Reduce
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Reduce( const std::string& name, xml::xistream& xis, Task& result )
{
    typedef unsigned long K;
    const std::string functionName = xml::attribute< std::string >( xis, "function" );

    if( functionName == "select" )
        ReduceFunction< Selector< K, T > >( name, xis, result );
    else if( functionName == "count" )
        ReduceFunction< Count< K, T > >( name, xis, result );
    else if( functionName == "minimum" )
        ReduceFunction< Minimum< K, T > >( name, xis, result );
    else if( functionName == "maximum" )
        ReduceFunction< Maximum< K, T > >( name, xis, result );
    else if( functionName == "sum" )
        ReduceFunction< Adder< K, T > >( name, xis, result );
    else if( functionName == "mean" )
        ReduceFunction< Meaner< K, T > >( name, xis, result );
    else
        ReductionError( functionName );
}


TYPE_DISPATCH_HELPER( Reduce )

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Reduce
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void FunctionFactory::Reduce( xml::xistream& xis, Task& result )
{
    ReduceDispatcher functor( this );
    TypeDispatcher dispatcher( xis, result );
    dispatcher.Dispatch( functor );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Transform2
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Transform2( const std::string& name, xml::xistream& xis, Task& result )
{
    typedef FunctionConnector< typename T::Key_Type, typename T::Result_Type > Connector;
    typedef KeyMarshaller< typename T::Key_Type,
                           typename T::First_Argument_Type,
                           typename T::Second_Argument_Type > Marshaller;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< T > function( new T( xis, *connector ) );
    boost::shared_ptr< Marshaller > marshaller( new Marshaller( *function ) );

    result.AddConnector( name, connector );
    result.AddFunction ( function );
    result.AddFunction ( name, marshaller );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Transform1
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Transform1( const std::string& name, xml::xistream& xis, Task& result )
{
    typedef FunctionConnector< typename T::Key_Type, typename T::Result_Type > Connector;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< T > function( new T( xis, *connector ) );

    result.AddConnector( name, connector );
    result.AddFunction ( name, function );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Compose
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Compose( const std::string& name, xml::xistream& , Task& result )
{
    typedef Composer< unsigned long, unsigned long, T > C;
    typedef FunctionConnector< typename C::Key_Type, typename C::Result_Type > Connector;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< C > function( new C( *connector ) );
    result.AddConnector( name, connector );
    result.AddFunction ( name, function );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Transform
// Created: AGE 2007-09-14
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Transform( const std::string& name, xml::xistream& xis, Task& result )
{
    const std::string function = xml::attribute< std::string >( xis, "function" );
    if( function == "distance" )
        Transform2< Distance< unsigned long > >( name, xis, result );
    else if( function == "contains" )
        Transform2< ::Contains< unsigned long > >( name, xis, result );
    else if( function == "filter" )
        Transform2< Filter< unsigned long, T > >( name, xis, result );
    else if( function == "is-one-of" )
        Transform1< IsOneOf< unsigned long, T > >( name, xis, result );
    else if( function == "derivate" )
        Transform1< Derivate< unsigned long, T > >( name, xis, result );
    else if( function == "integrate" )
        Transform1< Integrate< unsigned long, T > >( name, xis, result );
    else if( function == "domain" )
        Transform1< Domain< unsigned long, T > >( name, xis, result );
    else if( function == "compare" )
        Transform2< Compare< unsigned long, T > >( name, xis, result );
    else if( function == "compose" )
        Compose< T >( name, xis, result );
    else
        TransformationError( function );
}

TYPE_DISPATCH_HELPER( Transform )

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Transform
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void FunctionFactory::Transform( xml::xistream& xis, Task& result )
{
    TransformDispatcher functor( this );
    TypeDispatcher dispatcher( xis, result, true );
    dispatcher.Dispatch( functor );
}

TYPE_DISPATCH_HELPER( Plot )

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Plot
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void FunctionFactory::Plot( xml::xistream& xis, Task& result )
{
    PlotDispatcher functor( this );
    TypeDispatcher dispatcher( "", xis, result, true );
    dispatcher.Dispatch( functor );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Plot
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Plot( const std::string& , xml::xistream& , Task& result )
{
    boost::shared_ptr< Plotter< unsigned long, T > > plotter( new Plotter< unsigned long, T >( currentContext_ ) );
    result.SetResult( plotter );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::ValueError
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void FunctionFactory::ValueError( const std::string& name )
{
    throw std::runtime_error( "Unknown value to extract '" + name + "'" );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::ReductionError
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void FunctionFactory::ReductionError( const std::string& name )
{
    throw std::runtime_error( "Unknown reduction '" + name + "'" );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::TransformationError
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void FunctionFactory::TransformationError( const std::string& name )
{
    throw std::runtime_error( "Unknown transformation '" + name + "'" );
}
