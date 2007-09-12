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
#include "IdentifierValue.h"
#include "Selector.h"
#include "Connectors.h"
#include "Task.h"
#include "Plotter.h"
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
// Name: std::auto_ptr< Task > FunctionFactory::CreateTask
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
std::auto_ptr< Task > FunctionFactory::CreateTask( xml::xistream& xis )
{
    std::auto_ptr< Task > result( new Task( publisher_ ) );
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
    std::auto_ptr< Connector > connector( new Connector() );
    std::auto_ptr< ModelFunction_ABC > function( factory(
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
    else
        throw std::runtime_error( "Unknown value to extract '" + value + "'" );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Reduce
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Reduce( const std::string& name, xml::xistream& xis, Task& result )
{
    std::auto_ptr< HandlerConnector< T > > connector( new HandlerConnector< T >() );
    std::auto_ptr< Function_ABC > function;
    const std::string functionName = xml::attribute< std::string >( xis, "function" );
    if( functionName == "select" )
    {
        const unsigned long id = xml::attribute< unsigned long >( xis, "id" );
        function.reset( new Selector< unsigned long, T >( id, *connector ) );
    }
    result.AddFunction( name, function );
    result.AddConnector( name, connector );
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
    else
        throw std::runtime_error( "Unknown type '" + type + "'" );
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
    else
        throw std::runtime_error( "Unknown type '" + type + "'" );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Plot
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Plot( Task& result )
{
    std::auto_ptr< Plotter< T > > plotter( new Plotter< T >() );
    result.SetResult( plotter );
}
