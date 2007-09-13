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
#include "IdentifierValue.h"
#include "Selector.h"
#include "Connectors.h"
#include "Task.h"
#include "Plotter.h"
#include "Distance.h"
#include "Filter.h"
#include "Count.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: FunctionFactory constructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
FunctionFactory::FunctionFactory( dispatcher::ClientPublisher_ABC& publisher )
    : publisher_( publisher )
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
// Name: boost::shared_ptr< Task > FunctionFactory::CreateTask
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Task > FunctionFactory::CreateTask( xml::xistream& xis )
{
    boost::shared_ptr< Task > result( new Task( publisher_ ) );
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
    else if( type == "reduce" )
        Reduce( xis, result );
    else if( type == "transform" )
        Transform( xis, result );
    else if( type == "plot" )
        Plot( xis, result );
    else
        throw std::runtime_error( "Unknown function type '" + type + "'" );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Extract
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
template< typename Value >
void FunctionFactory::Extract( const std::string& name, xml::xistream& , Task& result )
{
    DispatcherFactory< IdentifierValue, Value > factory;
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
    xis >> xml::attribute( "value", value ) >> xml::attribute( "name", name );
    if( value == "operational-state" )
        Extract< attributes::OperationalState >( name, xis, result );
    else if( value == "position" )
        Extract< attributes::Position >( name, xis, result );
    else if( value == "maintenance-handling" )
        Extract< existences::MaintenanceHandling >( name, xis, result );
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
    else
        ReductionError( functionName );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Reduce
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void FunctionFactory::Reduce( xml::xistream& xis, Task& result )
{
    std::string name, type;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "type", type );
    if( type == "float" )
        Reduce< float >( name, xis, result );
    else if( type == "position" )
        Reduce< ::Position >( name, xis, result );
    else if( type == "bool" )
        Reduce< bool >( name, xis, result );
    else
        TypeError( type );
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
// Name: FunctionFactory::Transform
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void FunctionFactory::Transform( xml::xistream& xis, Task& result )
{
    std::string function, name, type;
    xis >> xml::attribute( "function", function )
        >> xml::attribute( "name", name )
        >> xml::optional() >> xml::attribute( "type", type );
    if( function == "distance" )
        Transform2< Distance< unsigned long > >( name, xis, result );
    else if( function == "filter" )
    {
        if( type == "bool" )
            Transform2< Filter< unsigned long, bool > >( name, xis, result );
        else
            TypeError( type );
    }
    else
        TransformationError( function );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Plot
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void FunctionFactory::Plot( xml::xistream& xis, Task& result )
{
    const std::string type = xml::attribute< std::string >( xis, "type" );
    if( type == "float" )
        Plot< float >( result );
    else if( type == "unsigned" || type == "unsigned int" )
        Plot< unsigned >( result );
    else
        TypeError( type );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Plot
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Plot( Task& result )
{
    boost::shared_ptr< Plotter< unsigned long, T > > plotter( new Plotter< unsigned long, T >() );
    result.SetResult( plotter );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::TypeError
// Created: AGE 2007-09-13
// -----------------------------------------------------------------------------
void FunctionFactory::TypeError( const std::string& name )
{
    throw std::runtime_error( "Unknown type '" + name + "'" );
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
