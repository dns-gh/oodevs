// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Container.h"
#include "Factory_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Container constructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
Container::Container( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory )
    : Node( input, references, propertyFactory )
{
    input >> xml::list( *this, &Container::ReadChild, factory, references );
}

// -----------------------------------------------------------------------------
// Name: Container constructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
Container::Container( const Container& rhs )
    : Node( rhs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Container destructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
Container::~Container()
{
    for( CIT_Children it = children_.begin(); it != children_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Container::ReadChild
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
void Container::ReadChild( const std::string& name, xml::xistream& input, const Factory_ABC& factory, References_ABC& references )
{
    Node_ABC* child = factory.Create( name, input, references );
    if( child )
        children_.push_back( child );
}

// -----------------------------------------------------------------------------
// Name: Container::DrawChildren
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
void Container::DrawChildren( RenderingContext_ABC& context, References_ABC& references ) const
{
    for( CIT_Children it = children_.begin(); it != children_.end(); ++it )
        (*it)->Draw( context, references );
}

// -----------------------------------------------------------------------------
// Name: Container::CompileChildren
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
Container& Container::CompileChildren( Container& newNode, RenderingContext_ABC& context, References_ABC& references ) const
{
    for( CIT_Children it = children_.begin(); it != children_.end(); ++it )
    {
        Node_ABC& child = (*it)->Compile( context, references );
        newNode.children_.push_back( &child );
    }
    return newNode;
}
