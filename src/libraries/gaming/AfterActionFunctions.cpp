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
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionFunctions constructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunctions::AfterActionFunctions( kernel::Controller& controller, Resolver_ABC< AfterActionFactory, QString >& factories )
    : controller_( controller )
    , factories_ ( factories )
{
    Load( "functions.xml" ); // $$$$ AGE 2007-09-20: 
}
 
// -----------------------------------------------------------------------------
// Name: AfterActionFunctions destructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunctions::~AfterActionFunctions()
{
    // NOTHING
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
    std::auto_ptr< AfterActionFunction > function( new AfterActionFunction( factories_, xis ) );
    const QString name = function->GetName();
    Register( name, *function.release() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctions::Create
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionFunction* AfterActionFunctions::Create()
{
    const QString base( "New function" );
    QString name( base );
    if( Find( name ) )
    {
        unsigned i = 1;
        while( Find( name ) )
            name = base + " " + QString::number( i++ );
    }
    AfterActionFunction* function = new AfterActionFunction( name.ascii() );
    Register( function->GetName(), *function );
    controller_.Update( *this );
    return function;
}


