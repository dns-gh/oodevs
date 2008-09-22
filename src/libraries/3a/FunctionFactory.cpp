// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "FunctionFactory.h"
#include "ElementFactory_ABC.h"
#include "Task.h"

#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: FunctionFactory constructor
// Created: AGE 2007-09-11
// -----------------------------------------------------------------------------
FunctionFactory::FunctionFactory()
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
// Name: FunctionFactory::Add
// Created: AGE 2008-08-04
// -----------------------------------------------------------------------------
void FunctionFactory::Add( std::auto_ptr< ElementFactory_ABC > factory )
{
    factories_.push_back( factory );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Task > FunctionFactory::CreateTask
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Task > FunctionFactory::CreateTask( xml::xistream& xis )
{
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
    std::for_each( factories_.begin(), factories_.end(),
        boost::bind( &ElementFactory_ABC::CreateElement, _1, type, boost::ref( xis ), boost::ref( result ) ) );
}
