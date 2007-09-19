// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionItem.h"
#include "AfterActionItemBuilder_ABC.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: AfterActionItem constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItem::AfterActionItem( const std::string& type, xml::xistream& xis )
    : type_     ( type )
    , name_     ( xml::attribute< std::string >( xis, "name" ) )
    , output_   ( xml::attribute< std::string >( xis, "output" ) )
    , input1_   ( xml::attribute( xis, "input1", std::string() ) )
    , input2_   ( xml::attribute( xis, "input2", std::string() ) )
    , paramName_( xml::attribute( xis, "parameter-name", std::string() ) )
    , paramType_( xml::attribute( xis, "parameter-type", std::string() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionItem::~AfterActionItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::GetType
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
const std::string& AfterActionItem::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::GetName
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
const std::string& AfterActionItem::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionItem::Build
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionItem::Build( AfterActionItemBuilder_ABC& builder ) const
{
    builder.Start( name_ );
    if( ! input1_.empty() )
        builder.AddInput ( input1_ );
    if( ! input2_.empty() )
        builder.AddInput ( input2_ );
    if( ! output_.empty() )
        builder.AddOutput( output_ );
    if( ! paramName_.empty() )
        builder.AddParameter( paramType_, paramName_ );
}
