// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionFunctions.h"
#include "AfterActionFactory.h"
#include "AfterActionFunction.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionFunctions constructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunctions::AfterActionFunctions( Resolver_ABC< AfterActionFactory, QString >& factories )
    : factories_( factories )
{
    Load( "functions.xml" ); // $$$$ AGE 2007-09-20: 
}
 
// -----------------------------------------------------------------------------
// Name: AfterActionFunctions destructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunctions::~AfterActionFunctions()
{
    for( CIT_Functions it = functions_.begin(); it != functions_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctions::Load
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunctions::Load( const std::string& functions )
{
    xml::xifstream xis( functions );
    xis >> xml::start( "functions" )
            >> xml::list( "function", *this, &AfterActionFunctions::ReadFunction )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctions::ReadFunction
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunctions::ReadFunction( xml::xistream& xis )
{
    functions_.push_back( new AfterActionFunction( factories_, xis ) );
}
