// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Node.h"
#include "PropertyGroup.h"
#include "Transformations.h"
#include "References_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: Node constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Node::Node( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : properties_     ( new PropertyGroup( input, factory ) )
    , transformations_( new Transformations( input ) )
{
    input >> xml::optional() >> xml::attribute( "id", id_ );
    references.Register( id_, *this );
}

// -----------------------------------------------------------------------------
// Name: Node constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Node::Node( const Node& rhs )
    : properties_     ( new PropertyGroup( *rhs.properties_ ) )
    , transformations_( new Transformations( *rhs.transformations_ ) )
    , id_             ( rhs.id_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Node destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Node::~Node()
{
    delete properties_;
    delete transformations_;
}

// -----------------------------------------------------------------------------
// Name: Node::GetId
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
const std::string& Node::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Node::Draw
// Created: AGE 2006-08-14
// -----------------------------------------------------------------------------
void Node::Draw( RenderingContext_ABC& context, References_ABC& references ) const
{
    transformations_->Apply();
    properties_->Push( context, references );
    DrawInternal( context, references );
    properties_->Pop( context, references );
    transformations_->Undo();
}

// -----------------------------------------------------------------------------
// Name: Node::Compile
// Created: AGE 2006-08-17
// -----------------------------------------------------------------------------
Node& Node::Compile( RenderingContext_ABC& context, References_ABC& references ) const
{
    properties_->Push( context, references );
    Node& result = CompileInternal( context, references );
    properties_->Pop( context, references );
    if( ! result.GetId().empty() )
        references.Register( result.GetId(), result );
    return result;
}

