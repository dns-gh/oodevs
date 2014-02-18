// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ListLengthFactory.h"
#include "Length.h"
#include "ListLength.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ListLengthFactory constructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListLengthFactory::ListLengthFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListLengthFactory destructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListLengthFactory::~ListLengthFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListLengthFactory::IsFixed
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
bool ListLengthFactory::IsFixed( const std::string& content ) const
{
    return Length( content ).IsFixed();
}

// -----------------------------------------------------------------------------
// Name: ListLengthFactory::CompileList
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void ListLengthFactory::CompileList( const std::string& content, RenderingContext_ABC& context )
{
    Length base( content );
    base.ReachTop( context );
    base.SetupLineWidth();
}

// -----------------------------------------------------------------------------
// Name: ListLengthFactory::CreateBaseProperty
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Property_ABC* ListLengthFactory::CreateBaseProperty( const std::string& content ) const
{
    return new Length( content );
}

// -----------------------------------------------------------------------------
// Name: ListLengthFactory::CreateListProperty
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Property_ABC* ListLengthFactory::CreateListProperty( unsigned list ) const
{
    return new ListLength( list );
}
