// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReductionsFactory.h"
#include "TypeDispatcher.h"
#include "Connectors.h"
#include "Task.h"
#include "Selector.h"
#include "Count.h"
#include "Minimum.h"
#include "Maximum.h"
#include "Adder.h"
#include "Meaner.h"
#include "Arithmetic.h"
#include "Threshold.h"

// -----------------------------------------------------------------------------
// Name: ReductionsFactory constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ReductionsFactory::ReductionsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReductionsFactory destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ReductionsFactory::~ReductionsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReductionsFactory::Reduce
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void ReductionsFactory::Reduce( const std::string& name, xml::xistream& xis, Task& result ) const
{
    typedef NumericValue K;
    const std::string functionName = xis.attribute< std::string >( "function" );
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
    else if( functionName == "threshold" )
        ReduceFunction< Threshold< K, T > >( name, xis, result );
    else if( functionName == "product" )
        ReduceFunction2< Arithmetic< K, NumericValue >, NumericValue >( name, xis, result, boost::bind( &NumericValue::operator*, _1, _2 ) );
    else if( functionName == "add" )
        ReduceFunction2< Arithmetic< K, NumericValue >, NumericValue >( name, xis, result, boost::bind( &NumericValue::operator+, _1, _2 ) );
    else if( functionName == "substract" )
        ReduceFunction2< Arithmetic< K, NumericValue >, NumericValue >( name, xis, result, boost::bind( &NumericValue::operator-, _1, _2 ) );
    else
        throw MASA_EXCEPTION( "Unknown reduction '" + functionName + "'" );
}

// -----------------------------------------------------------------------------
// Name: ReductionsFactory::ReduceFunction
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename F >
void ReductionsFactory::ReduceFunction( const std::string& name, xml::xistream& xis, Task& result ) const
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
// Name: ReductionsFactory::ReduceFunction2
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename F, typename T >
void ReductionsFactory::ReduceFunction2( const std::string& name, xml::xistream& xis, Task& result, const boost::function< T( const T&, const T& ) >& f ) const
{
    typedef FunctionConnector< typename F::Key_Type, typename F::Result_Type > Connector;
    typedef KeyMarshaller< typename F::Key_Type,
                           typename F::First_Argument_Type,
                           typename F::Second_Argument_Type > Marshaller;
    boost::shared_ptr< Connector > connector( new Connector() );
    boost::shared_ptr< F > function( new F( xis, *connector, f ) );
    boost::shared_ptr< Marshaller > marshaller( new Marshaller( *function ) );

    result.AddConnector( name, connector );
    result.AddFunction ( function );
    result.AddFunction ( name, marshaller );
}

TYPE_DISPATCH_HELPER( Reduce, ReductionsFactory )

// -----------------------------------------------------------------------------
// Name: ReductionsFactory::CreateElement
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ReductionsFactory::CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const
{
    if( type != "reduce" )
        return;

    ReduceDispatcher functor( this );
    TypeDispatcher dispatcher( xis, result );
    dispatcher.Dispatch( functor );
}
