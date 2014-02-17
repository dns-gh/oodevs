// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ListDashArrayFactory.h"
#include "ListDashArray.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ListDashArrayFactory constructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListDashArrayFactory::ListDashArrayFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListDashArrayFactory destructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListDashArrayFactory::~ListDashArrayFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListDashArrayFactory::IsFixed
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
bool ListDashArrayFactory::IsFixed( const std::string& content ) const
{
    return content == "none"  || content.find( "px" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: ListDashArrayFactory::CompileList
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void ListDashArrayFactory::CompileList( const std::string& content, RenderingContext_ABC& context )
{
    DashArray dash( content );
    dash.ReachTop( context );
}

// -----------------------------------------------------------------------------
// Name: ListDashArrayFactory::CreateBaseProperty
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Property_ABC* ListDashArrayFactory::CreateBaseProperty( const std::string& content ) const
{
    return new DashArray( content );
}

// -----------------------------------------------------------------------------
// Name: ListDashArrayFactory::CreateListProperty
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Property_ABC* ListDashArrayFactory::CreateListProperty( unsigned list ) const
{
    return new ListDashArray( list );
}
