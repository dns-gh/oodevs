// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "References.h"
#include "Node_ABC.h"
#include "Paint_ABC.h"
#include "Style_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: References constructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
References::References()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: References destructor
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
References::~References()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: References::Clear
// Created: AGE 2006-09-06
// -----------------------------------------------------------------------------
void References::Clear()
{
    nodes_.clear();
}

// -----------------------------------------------------------------------------
// Name: References::Register
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void References::Register( const std::string& id, const Node_ABC& element )
{
    nodes_[ id ] = &element;
}

// -----------------------------------------------------------------------------
// Name: References::Draw
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void References::Draw( const std::string& id, RenderingContext_ABC& context )
{
    const Node_ABC* node = nodes_[ id ];
    if( node )
        node->Draw( context, *this );
}

// -----------------------------------------------------------------------------
// Name: References::Register
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
void References::Register( const std::string& id, const Paint_ABC& paint )
{
    paints_[ id ] = &paint;
}
    
// -----------------------------------------------------------------------------
// Name: References::Setup
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
bool References::Setup( const std::string& id, float opacity )
{
    const Paint_ABC* paint = paints_[ id ];
    return paint && paint->Setup( *this, opacity );
}

// -----------------------------------------------------------------------------
// Name: References::Register
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void References::Register( const std::string& id, const Style_ABC& style )
{
    styles_[ id ] = &style;
}

// -----------------------------------------------------------------------------
// Name: References::Push
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void References::Push( const std::string& id, RenderingContext_ABC& context )
{
    const Style_ABC* style = styles_[ id ];
    if( style ) 
        style->Push( context, *this );
}

// -----------------------------------------------------------------------------
// Name: References::Pop
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void References::Pop( const std::string& id, RenderingContext_ABC& context )
{
    const Style_ABC* style = styles_[ id ];
    if( style ) 
        style->Pop( context, *this );
}
