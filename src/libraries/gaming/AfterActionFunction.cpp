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
    function.Create( *this, id );
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

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::SetParameter
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterActionFunction::SetParameter( const QString& name, const std::string& value )
{
    Resolver< AfterActionParameter, std::string >::Get( name.ascii() ).Set( value );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::Remove
// Created: AGE 2007-10-01
// -----------------------------------------------------------------------------
void AfterActionFunction::Remove( AfterActionItem_ABC& item )
{
    for( ItemResolver::IT_Elements it = ItemResolver::elements_.begin(); it != ItemResolver::elements_.end(); ++it )
        if( it->second == &item )
        {
            delete it->second;
            ItemResolver::elements_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::Remove
// Created: AGE 2007-10-01
// -----------------------------------------------------------------------------
void AfterActionFunction::Remove( AfterActionParameter& item )
{
    for( ParameterResolver::IT_Elements it = ParameterResolver::elements_.begin(); it != ParameterResolver::elements_.end(); ++it )
        if( it->second == &item )
        {
            delete it->second;
            ParameterResolver::elements_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::Add
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterActionFunction::Add( const std::string& id, AfterActionItem_ABC& item )
{
    ItemResolver::Register( id, item );
}
