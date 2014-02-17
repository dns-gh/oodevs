// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "TextAnchor.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: TextAnchor constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextAnchor::TextAnchor( const std::string& content )
    : factor_( 0 )
{
    if( content ==  "middle" )
        factor_ = 0.5f;
    else if( content == "end" )
        factor_ = 1.f;
}

// -----------------------------------------------------------------------------
// Name: TextAnchor constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextAnchor::TextAnchor( const TextAnchor& rhs )
    : factor_( rhs.factor_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextAnchor destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextAnchor::~TextAnchor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TextAnchor::GetFactor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
float TextAnchor::GetFactor() const
{
    return factor_;
}

// -----------------------------------------------------------------------------
// Name: TextAnchor::Clone
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TextAnchor& TextAnchor::Clone() const
{
    return *new TextAnchor( *this );
}
