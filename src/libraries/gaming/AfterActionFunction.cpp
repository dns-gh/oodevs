// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionFunction.h"
#include "AfterActionFactory.h"
#include "AfterActionItem_ABC.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionFunction constructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunction::AfterActionFunction( Resolver_ABC< AfterActionFactory, QString >& factories, xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ) )
{
    xis >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &AfterActionFunction::ReadParameter )
        >> xml::end()
        >> xml::start( "indicator" )
            >> xml::list( *this, &AfterActionFunction::CreateItem, factories )
        >> xml::end()
        >> xml::start( "indicator" )
            >> xml::list( *this, &AfterActionFunction::LoadItem )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction destructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunction::~AfterActionFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::ReadParameter
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::ReadParameter( xml::xistream& xis )
{

}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::CreateItem
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::CreateItem( const std::string& type, xml::xistream& xis, Resolver_ABC< AfterActionFactory, QString >& factories )
{
    if( type != "plot" ) // $$$$ AGE 2007-09-20: 
    {
        const std::string functionName = xml::attribute< std::string >( xis, "function" );
        const std::string id           = xml::attribute< std::string >( xis, "id" );
        AfterActionFactory& function = factories.Get( functionName.c_str() );
        Register( id, *function.Create().release() );
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::LoadItem
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::LoadItem( const std::string& type, xml::xistream& xis )
{
    if( type != "plot" ) // $$$$ AGE 2007-09-20: 
    {
        const std::string id = xml::attribute< std::string >( xis, "id" );
        Get( id ).Connect( xis, *this );
    }
}
