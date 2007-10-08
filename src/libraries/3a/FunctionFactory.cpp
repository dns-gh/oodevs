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
#include "IsOneOf.h"
#include "Domain.h"
#include "Derivate.h"
#include "Integrate.h"
#include "Minimum.h"
#include "Maximum.h"
#include "Adder.h"
#include "Meaner.h"
#include "Events.h"
#include "Composer.h"
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

namespace
{
    // $$$$ AGE 2007-09-17: crap
    const char* extractors[][3] =
    {
        // name,                           type,       key type
        { "operational-state",            "float",    "unit" },
        { "position",                     "position", "unit" },
        { "maintenance-handling-unit",    "unit",     "maintenance-handling" },
        { "direct-fire-unit",             "unit",     "fire" },
        { "fire-component-damage",        "float",    "fire" },
        { 0, 0, 0 }
    };

    const char* transformations[][6] =
    {
        // name,        input type 1   input type 2  output type   parameter type  parameter name
        { "distance",  "position",    "position",   "float",      "",             "" },
        { "filter",    "bool",        "any",        "input 2",    "",             "" },
        { "is-one-of", "any",         "",           "bool",       "list input 1", "select" },
        { "derivate",  "any",         "",           "input 1",    "",             "" },
        { "integrate", "any",         "",           "input 1",    "",             "" },
        { "domain",    "any",         "",           "input 1",    "list key",     "select" },
        { "compose",   "any",         "any",        "input 2",    "",             "" },
        { 0, 0, 0, 0, 0, 0 }
    };

    const char* reductions[][4] =
    {
        // name,     output,     parameter type  parameter name
        { "select",  "input",    "key",          "key" },
        { "count",   "unsigned", "",             ""    },
        { "minimum", "input",    "",             ""    },
        { "maximum", "input",    "",             ""    },
        { "sum",     "input",    "",             ""    },
        { "mean",    "input",    "",             ""    },
        { 0, 0, 0, 0 }
    };

    void DescribeExtractors( xml::xostream& xos )
    {
        unsigned i = 0;
        while( extractors[i][0] )
        {
            xos << xml::start( "extract" )
                    << xml::attribute( "name",     extractors[i][0] )
                    << xml::attribute( "output",   extractors[i][1] )
                    << xml::attribute( "key-type", extractors[i][2] )
                << xml::end();
            ++i;
        }
    }
    void DescribeTransformations( xml::xostream& xos )
    {
        unsigned i = 0;
        while( transformations[i][0] )
        {
            xos << xml::start( "transform" )
                    << xml::attribute( "name",   transformations[i][0] )
                    << xml::attribute( "input1", transformations[i][1] );
            if( transformations[i][2] )
                xos << xml::attribute( "input2", transformations[i][2] );
            xos << xml::attribute( "output", transformations[i][3] );
            if( transformations[i][4] )
                xos << xml::attribute( "parameter-type", transformations[i][4] );
            if( transformations[i][5] )
                xos << xml::attribute( "parameter-name", transformations[i][5] );
            xos << xml::end();
            ++i;
        }
    }
    void DescribeReductions( xml::xostream& xos )
    {
        unsigned i = 0;
        while( reductions[i][0] )
        {
            xos << xml::start( "reduce" )
                    << xml::attribute( "name",   reductions[i][0] )
                    << xml::attribute( "output", reductions[i][1] )
                    << xml::attribute( "input1", "any" );
            if( transformations[i][2] )
                xos << xml::attribute( "parameter-type", reductions[i][2] );
            if( transformations[i][3] )
                xos << xml::attribute( "parameter-name", reductions[i][3] );
            xos << xml::end();
            ++i;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Describe
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void FunctionFactory::Describe( xml::xostream& xos )
{
    xos << xml::start( "functions" );
    DescribeExtractors( xos );
    DescribeTransformations( xos );
    DescribeReductions( xos );
    xos << xml::start( "plot" )
            << xml::attribute( "name", "plot" )
            << xml::attribute( "input1", "any" )
        << xml::end();
    xos << xml::end();
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
    xis >> xml::attribute( "function", value ) >> xml::attribute( "id", name );
    if( value == "operational-state" )
        Extract< attributes::OperationalState >( name, xis, result );
    else if( value == "position" )
        Extract< attributes::Position >( name, xis, result );
    else if( value == "maintenance-handling-unit" )
        Extract< existences::MaintenanceHandlingUnitId >( name, xis, result );
    else if( value == "direct-fire-unit" )
        Extract< existences::DirectFireUnitId >( name, xis, result );
    else if( value == "fire-component-damage" )
        Extract< events::FireComponentDamage >( name, xis, result );
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

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Reduce
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
void FunctionFactory::Reduce( xml::xistream& xis, Task& result )
{
    std::string name, type;
    xis >> xml::attribute( "id", name )
        >> xml::attribute( "type", type );
    if( type == "float" )
        Reduce< float >( name, xis, result );
    else if( type == "position" )
        Reduce< ::Position >( name, xis, result );
    else if( type == "bool" )
        Reduce< bool >( name, xis, result );
    else if( type == "unsigned long" || type == "unit" )
        Reduce< unsigned long >( name, xis, result );
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
// Name: FunctionFactory::Compose
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
template< typename T >
void FunctionFactory::Compose( const std::string& name, xml::xistream& xis, Task& result )
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
    else if( function == "compose" )
        Compose< T >( name, xis, result );
    else
        TransformationError( function );
}

namespace
{
    struct NullType
    {
        NullType() {}
        template< typename T > explicit NullType( const T& ) {}
        template< typename T > NullType& operator+=( const T& )       { return *this; }
        template< typename T > NullType  operator+ ( const T& ) const { return *this; }
        template< typename T > NullType& operator-=( const T& )       { return *this; }
        template< typename T > NullType  operator- ( const T& ) const { return *this; }
        template< typename T > NullType& operator/=( const T& )       { return *this; }
        template< typename T > NullType  operator/ ( const T& ) const { return *this; }
        template< typename T > bool operator==( const T& ) const { return false; }
        template< typename T > bool operator!=( const T& ) const { return false; }
        template< typename T > bool operator<( const T& ) const { return false; }
        template< typename T > bool operator>( const T& ) const { return false; }
    };
    std::istream& operator>>( std::istream& stream, NullType& ) { return stream; }
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::Transform
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void FunctionFactory::Transform( xml::xistream& xis, Task& result )
{
    std::string name, type;
    xis >> xml::attribute( "id", name )
        >> xml::optional() >> xml::attribute( "type", type );
    if( type == "bool" )
        Transform< bool >( name, xis, result );
    else if( type == "unsigned long" || type == "unit" )
        Transform< unsigned long >( name, xis, result );
    else if( type == "float" )
        Transform< float >( name, xis, result );
    else if( type == "position" )
        Transform< ::Position >( name, xis, result );
    else if( type.empty() )
        Transform< NullType >( name, xis, result );
    else
        TypeError( type );
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
    else if( type == "unit" )
        Plot< unsigned long >( result );
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
    boost::shared_ptr< Plotter< unsigned long, T > > plotter( new Plotter< unsigned long, T >( currentContext_ ) );
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
