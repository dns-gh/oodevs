// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ExtensionType.h"
#include "AttributeType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ExtensionType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
ExtensionType::ExtensionType( xml::xistream& xis )
{
    xis >> xml::list( "attribute", *this, &ExtensionType::ReadAttribute );
}

// -----------------------------------------------------------------------------
// Name: ExtensionType destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
ExtensionType::~ExtensionType()
{
    tools::StringResolver< AttributeType >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: ExtensionType::ReadAttribute
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void ExtensionType::ReadAttribute( xml::xistream& xis )
{
    Register( xis.attribute< std::string >( "name" ), *new AttributeType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: ExtensionType::GetAttributeTypes
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void ExtensionType::GetAttributeTypes( const std::string& type, T_AttributesTypes& attributeTypes )
{
    auto it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AttributeType& attribute = it.NextElement();
        if( attribute.Extends( type ) )
            attributeTypes.push_back( &attribute );
    }
}
