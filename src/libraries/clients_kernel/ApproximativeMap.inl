// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: ApproximativeMap constructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
template< typename T >
ApproximativeMap< T >::ApproximativeMap( int defaultThreshold )
    : defaultThreshold_( defaultThreshold )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ApproximativeMap destructor
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
template< typename T >
ApproximativeMap< T >::~ApproximativeMap()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ApproximativeMap::Add
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
template< typename T >
void ApproximativeMap< T >::Add( const std::string& key, const T& value )
{
    keys_.push_back( key );
    values_.push_back( value );
}

// -----------------------------------------------------------------------------
// Name: ApproximativeMap::Find
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
template< typename T >
const T* ApproximativeMap< T >::Find( const std::string& key ) const
{
    return Find( key, defaultThreshold_ );
}

// -----------------------------------------------------------------------------
// Name: ApproximativeMap::Find
// Created: AGE 2008-04-04
// -----------------------------------------------------------------------------
template< typename T >
const T* ApproximativeMap< T >::Find( const std::string& key, int threshold ) const
{
    int index = internal::Find( key, keys_, threshold );
    if( index >= 0 )
        return & values_.at( index );
    return 0;
}

}