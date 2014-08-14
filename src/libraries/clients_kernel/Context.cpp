// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Context.h"
#include "LocalizedString.h"
#include <boost/make_shared.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Context constructor
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
Context::Context()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Context destructor
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
Context::~Context()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Context::find
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< LocalizedString > >::iterator Context::find( const std::string& key )
{
    for( auto it = begin(); it != end(); ++it )
        if( ( *it )->Key() == key )
            return it;
    return end();
}

// -----------------------------------------------------------------------------
// Name: Context::find
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const std::vector< boost::shared_ptr< LocalizedString > >::const_iterator Context::find( const std::string& key ) const
{
    for( auto it = begin(); it != end(); ++it )
        if( ( *it )->Key() == key )
            return it;
    return end();
}

// -----------------------------------------------------------------------------
// Name: Context::operator[]
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
boost::shared_ptr< LocalizedString > Context::operator[]( const std::string& key )
{
    for( auto it = begin(); it != end(); ++it )
        if( ( *it )->Key() == key )
            return *it;
    return CreateNew( key );
}

// -----------------------------------------------------------------------------
// Name: Context::operator[]
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const boost::shared_ptr< LocalizedString >& Context::operator[]( const std::string& key ) const
{
    for( auto it = begin(); it != end(); ++it )
        if( ( *it )->Key() == key )
            return *it;
    throw MASA_EXCEPTION( std::string( "Key not found: " ) + key );
}

// -----------------------------------------------------------------------------
// Name: Context::at
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const boost::shared_ptr< LocalizedString >& Context::at( const std::string& key ) const
{
    return ( *this )[ key ];
}

// -----------------------------------------------------------------------------
// Name: Context::CreateNew
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
boost::shared_ptr< LocalizedString > Context::CreateNew( const std::string& key )
{
    push_back( boost::make_shared< LocalizedString >( key ) );
    return back();
}

// -----------------------------------------------------------------------------
// Name: Context::Apply
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool Context::Apply( const std::function< bool( LocalizedString& ) >& functor )
{
    for( auto it = begin(); it != end(); ++it )
        if( *it && functor( **it ) )
            return true;
    return false;
}
