// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ResultsFactory.h"
#include "TypeDispatcher.h"
#include "Task.h"
#include "Plotter.h"
#include "Indicator.h"

// -----------------------------------------------------------------------------
// Name: ResultsFactory constructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ResultsFactory::ResultsFactory( dispatcher::ClientPublisher_ABC& publisher, int context )
    : publisher_( publisher )
    , context_( context )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResultsFactory destructor
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
ResultsFactory::~ResultsFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResultsFactory::Result
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
template< typename T >
void ResultsFactory::Result( const std::string& , xml::xistream& xis, Task& result ) const
{
    const std::string function = xml::attribute< std::string >( xis, "function" );
    boost::shared_ptr< Result_ABC > f;
    if( function == "plot" )
        f.reset( new Plotter< unsigned long, T >( publisher_, context_ ) );
    else if( function == "indicator" )
        f.reset( new Indicator< unsigned long, T >( publisher_, xml::attribute< std::string >( xis, "name" ) ) );
    else
        Error( function );
    result.SetResult( f );
}

TYPE_DISPATCH_HELPER( Result, ResultsFactory )

// -----------------------------------------------------------------------------
// Name: ResultsFactory::CreateElement
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ResultsFactory::CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const
{
    if( type != "result" )
        return;

    ResultDispatcher functor( this );
    TypeDispatcher dispatcher( "", xis, result );
    dispatcher.Dispatch( functor );
}

// -----------------------------------------------------------------------------
// Name: ResultsFactory::Error
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void ResultsFactory::Error( const std::string& name )
{
    throw std::runtime_error( "Unknown result '" + name + "'" );
}
