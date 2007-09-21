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
    const std::string functionName = xml::attribute( xis, "function", type );
    const std::string id           = xml::attribute( xis, "id", type );
    AfterActionFactory& function = factories.Get( functionName.c_str() );
    Register( id, *function.Create().release() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::LoadItem
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::LoadItem( const std::string& type, xml::xistream& xis )
{
    const std::string id = xml::attribute( xis, "id", type );
    Get( id ).Connect( xis, *this );
}

namespace 
{
    struct Serializer
    {
        Serializer( xml::xostream& xos ) : xos_( &xos ) {}
        void operator()( const AfterActionItem_ABC& item ) const
        {
            item.Commit( *xos_ );
        }
        xml::xostream* xos_;
    };
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::Commit
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
std::string AfterActionFunction::Commit() const
{
    xml::xostringstream xos;
    xos << xml::start( "indicator" );
    Apply( Serializer( xos ) );
    xos << xml::end();
    return xos.str();
}
