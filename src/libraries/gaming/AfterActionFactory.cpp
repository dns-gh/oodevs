// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionFactory.h"
#include "AfterActionItem.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: AfterActionFactory constructor
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
AfterActionFactory::AfterActionFactory( const std::string& type, xml::xistream& xis )
    : AfterActionItem( type, xis )
    , type_( type.c_str() )
    , name_( xml::attribute< std::string >( xis, "name" ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionFactory destructor
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
AfterActionFactory::~AfterActionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionFactory::GetType
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
const QString& AfterActionFactory::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFactory::GetName
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
const QString& AfterActionFactory::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< AfterActionItem_ABC > AfterActionFactory::Create
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
std::auto_ptr< AfterActionItem_ABC > AfterActionFactory::Create() const
{
    return std::auto_ptr< AfterActionItem_ABC >( new AfterActionItem( *this ) );
}
