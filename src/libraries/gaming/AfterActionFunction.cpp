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
#include "AfterActionParameter.h"
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
// Name: AfterActionFunction constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionFunction::AfterActionFunction( const std::string& name )
    : name_( name )
{
    // NOTHING
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
    const std::string name = xml::attribute< std::string >( xis, "name" );
    ParameterResolver::Register( name, *new AfterActionParameter( xis ) );
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
    ItemResolver::Register( id, *function.Create( *this ).release() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::LoadItem
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::LoadItem( const std::string& type, xml::xistream& xis )
{
    const std::string id = xml::attribute( xis, "id", type );
    ItemResolver::Get( id ).Connect( xis, *this );
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
// Name: AfterActionFunction::GetName
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
QString AfterActionFunction::GetName() const
{
    return name_.c_str();
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::Commit
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
std::string AfterActionFunction::Commit() const
{
    xml::xostringstream xos;
    xos << xml::start( "indicator" );
    ItemResolver::Apply( Serializer( xos ) );
    xos << xml::end();
    return xos.str();
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::CreateParameterIterator
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
Iterator< const AfterActionParameter& > AfterActionFunction::CreateParameterIterator() const
{
    return ParameterResolver::CreateIterator();
}

// -----------------------------------------------------------------------------
// AfterActionFunction::CreateItemIterator
// Created: AGE 2007-09-24
// -----------------------------------------------------------------------------
kernel::Iterator< const AfterActionItem_ABC& > AfterActionFunction::CreateItemIterator() const
{
    return ItemResolver::CreateIterator();
}
