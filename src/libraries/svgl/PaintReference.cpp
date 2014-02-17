// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PaintReference.h"
#include "RenderingContext_ABC.h"
#include "References_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: PaintReference constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
PaintReference::PaintReference( const std::string& reference )
    : target_( reference )
{
    if( target_.size() > 5 && target_.substr( 0, 4 ) == "url(" )
        target_ = target_.substr( 4, target_.size() - 5 );
    if( ! target_.empty() && target_[0] == '#' )
        target_ = target_.substr( 1 );
}

// -----------------------------------------------------------------------------
// Name: PaintReference destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
PaintReference::~PaintReference()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PaintReference::Setup
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
bool PaintReference::Setup( References_ABC& references, float opacity ) const
{
    return references.Setup( target_, opacity );
}

// -----------------------------------------------------------------------------
// Name: PaintReference::Clone
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
PaintReference& PaintReference::Clone() const
{
    return *new PaintReference( *this );
}
