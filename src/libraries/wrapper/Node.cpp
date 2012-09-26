// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Node.h"
#include "View.h"
#include <module_api/Model.h>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <stdexcept>

using namespace sword::wrapper;

// -----------------------------------------------------------------------------
// Name: Node constructor
// Created: SLI 2011-12-07
// -----------------------------------------------------------------------------
Node::Node( SWORD_Model* node )
    : node_( node )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Node::GET/SET
// Created: BAX 2012-01-25
// -----------------------------------------------------------------------------
#define DECLARE_SET( TYPE_NAME, TYPE ) \
Node& Node::operator=( TYPE value ) \
{ \
    if( ! ::SWORD_Set##TYPE_NAME( node_, value ) ) \
        throw std::runtime_error( "unable to set " #TYPE ); \
    return *this; \
}
DECLARE_SET( Int8,   int8_t )
DECLARE_SET( Int16,  int16_t )
DECLARE_SET( Int32,  int32_t )
DECLARE_SET( Int64,  int64_t )
DECLARE_SET( Uint8,  uint8_t )
DECLARE_SET( Uint16, uint16_t )
DECLARE_SET( Uint32, uint32_t )
DECLARE_SET( Uint64, uint64_t )
DECLARE_SET( Float,  float )
DECLARE_SET( Double, double )
#undef DECLARE_SET

// -----------------------------------------------------------------------------
// Name: Node::operator=
// Created: VPR 2012-02-13
// -----------------------------------------------------------------------------
Node& Node::operator=( const std::string& value )
{
    if( ! ::SWORD_SetText( node_, value.c_str(), value.size() ) )
        throw std::runtime_error( "unable to set text" );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Node::operator=
// Created: MCO 2012-04-26
// -----------------------------------------------------------------------------
Node& Node::operator=( const View& view )
{
    if( ! ::SWORD_CopyModel( view, node_ ) )
        throw std::runtime_error( "unable copy node model" );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Node::operator[]
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
Node Node::operator[]( const std::string& key )
{
    if( key.empty() )
        return Node( node_ );
    SWORD_Model* child = ::SWORD_SetChild( node_, key.c_str() );
    if( !child )
        throw std::runtime_error( "could not retrieve child parameter '" + key + "'" );
    return Node( child );
}

// -----------------------------------------------------------------------------
// Name: Node::operator[]
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
Node Node::operator[]( const char* key )
{
    if( ! key )
        return Node( node_ );
    SWORD_Model* child = ::SWORD_SetChild( node_, key );
    if( !child )
        throw std::runtime_error( "could not retrieve child parameter '" + std::string( key ) + "'" );
    return Node( child );
}

// -----------------------------------------------------------------------------
// Name: Node::operator[]
// Created: SLI 2012-01-19
// -----------------------------------------------------------------------------
Node Node::operator[]( size_t key )
{
    SWORD_Model* child = ::SWORD_SetChildInt( node_, boost::numeric_cast< unsigned int >( key ) );
    if( !child )
        throw std::runtime_error( "could not retrieve child parameter '" + boost::lexical_cast< std::string >( key ) + "'" );
    return Node( child );
}

// -----------------------------------------------------------------------------
// Name: Node::AddElement
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
Node Node::AddElement()
{
    SWORD_Model* element = ::SWORD_AddElement( node_ );
    if( ! element )
        throw std::runtime_error( "could not add element to node" );
    return Node( element );
}

// -----------------------------------------------------------------------------
// Name: Node::MarkForRemove
// Created: SLI 2012-07-20
// -----------------------------------------------------------------------------
Node& Node::MarkForRemove()
{
    if( ! ::SWORD_MarkForRemove( node_ ) )
        throw std::runtime_error( "could not mark node for remove" );
    return *this;
}
