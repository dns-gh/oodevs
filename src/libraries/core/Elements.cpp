// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Elements.h"
#include "Model.h"

using namespace core;

// -----------------------------------------------------------------------------
// Name: Elements constructor
// Created: MCO 2012-10-19
// -----------------------------------------------------------------------------
Elements::Elements()
    : size_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Elements::operator[]
// Created: MCO 2012-10-19
// -----------------------------------------------------------------------------
Model& Elements::operator[]( std::size_t index )
{
    return *elements_.at( index );
}

// -----------------------------------------------------------------------------
// Name: Elements::operator[]
// Created: MCO 2012-10-19
// -----------------------------------------------------------------------------
const Model& Elements::operator[]( std::size_t index ) const
{
    return *elements_.at( index );
}

// -----------------------------------------------------------------------------
// Name: Elements::Size
// Created: MCO 2012-10-19
// -----------------------------------------------------------------------------
std::size_t Elements::Size() const
{
    return size_;
}

// -----------------------------------------------------------------------------
// Name: Model::Clear
// Created: BAX 2012-10-17
// -----------------------------------------------------------------------------
void Elements::Clear()
{
    for( T_Elements::iterator it = elements_.begin(); it != elements_.end(); ++it )
        delete *it;
    elements_.clear();
    size_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Elements::Add
// Created: BAX 2012-10-17
// -----------------------------------------------------------------------------
Model& Elements::Add( Model* child )
{
    elements_.insert( elements_.begin() + size_, child );
    ++size_;
    return *child;
}

// -----------------------------------------------------------------------------
// Name: Elements::Copy
// Created: BAX 2012-10-17
// -----------------------------------------------------------------------------
void Elements::Copy( Model& model ) const
{
    for( std::size_t i = 0; i < size_; ++i )
        model.AddElement() = *elements_[ i ];
}

// -----------------------------------------------------------------------------
// Name: Elements::Hide
// Created: BAX 2012-10-17
// -----------------------------------------------------------------------------
const Model& Elements::Hide( std::size_t index )
{
    if( index >= size_ )
        throw std::out_of_range( "index out of range" );
    const Model& hidden = *elements_.at( index );
    std::iter_swap( elements_.begin() + index, elements_.begin() + size_ - 1 );
    --size_;
    return hidden;
}

// -----------------------------------------------------------------------------
// Name: Elements::Remove
// Created: BAX 2012-10-17
// -----------------------------------------------------------------------------
void Elements::Remove( const Model& value )
{
    T_Elements::iterator it = std::find( elements_.begin(), elements_.end(), &value );
    if( it == elements_.end() )
        return;
    delete *it;
    if( std::distance( elements_.begin(), it ) < static_cast< int >( size_ ) )
        --size_;
    elements_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: Elements::Index
// Created: BAX 2012-10-17
// -----------------------------------------------------------------------------
std::size_t Elements::Index( const Model& value ) const
{
    for( std::size_t i = 0; i < size_; ++i )
        if( elements_.at( i ) == &value )
            return i;
    return SIZE_MAX;
}
