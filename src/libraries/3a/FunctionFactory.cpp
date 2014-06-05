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
#include <xeumeuleu/xml.hpp>
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
void FunctionFactory::Add( std::unique_ptr< ElementFactory_ABC > factory )
{
    factories_.push_back( factory.release() );
}

// -----------------------------------------------------------------------------
// Name: FunctionFactory::CreateTask
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
boost::shared_ptr< Task > FunctionFactory::CreateTask( xml::xistream& xis, unsigned int firstTick, unsigned int lastTick )
{
    boost::shared_ptr< Task > result( new Task( firstTick, lastTick ) );
    xis >> xml::list( *this, &FunctionFactory::CreateFunction, *result );
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
