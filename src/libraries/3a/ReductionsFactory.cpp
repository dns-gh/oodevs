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
    else if( functionName == "threshold" )
        ReduceFunction< Threshold< K, T > >( name, xis, result );
    else
        Error( functionName );
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

// -----------------------------------------------------------------------------
// Name: ReductionsFactory::Error
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ReductionsFactory::Error( const std::string& name )
{
    throw std::runtime_error( "Unknown reduction '" + name + "'" );
}
