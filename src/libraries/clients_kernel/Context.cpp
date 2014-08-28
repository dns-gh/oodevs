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

namespace
{
    template< typename T >
    typename T::const_iterator LowerBound( const T& strings, const std::string& key )
    {
        return std::lower_bound( strings.begin(), strings.end(), key, [&]( const boost::shared_ptr< LocalizedString >& translation, const std::string& key ) { return translation->Key() < key; } );
    }
    template< typename T >
    typename T::const_iterator UpperBound( const T& strings, const std::string& key )
    {
        return std::upper_bound( strings.begin(), strings.end(), key, [&]( const std::string& key, const boost::shared_ptr< LocalizedString >& translation ) { return key < translation->Key(); } );
    }
    template< typename T >
    std::pair< typename T::const_iterator, typename T::const_iterator > EqualRange( const T& strings, const std::string& key )
    {
        return std::make_pair( LowerBound( strings, key ), UpperBound( strings, key ) );
    }
    template< typename T >
    void Sort( T& strings )
    {
        std::sort( strings.begin(), strings.end(),
            [&]( const boost::shared_ptr< LocalizedString >& lhs, const boost::shared_ptr< LocalizedString >& rhs )
            {
                return lhs->Key() < rhs->Key();
            } );
    }
}

// -----------------------------------------------------------------------------
// Name: Context::operator[]
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const boost::shared_ptr< LocalizedString >& Context::operator[]( const std::string& key )
{
    const auto it = LowerBound( strings_, key );
    if( it != strings_.end() && (*it)->Key() == key )
        return *it;
    return *strings_.insert( it, boost::make_shared< LocalizedString >( key ) );
}

// -----------------------------------------------------------------------------
// Name: Context::SetKey
// Created: SLI 2014-08-26
// -----------------------------------------------------------------------------
void Context::SetKey( const boost::shared_ptr< LocalizedString >& translation, const std::string& key )
{
    const auto range = EqualRange( strings_, translation->Key() );
    for( auto it = range.first; it != range.second; ++it )
        if( *it == translation )
        {
            strings_.erase( it );
            translation->SetKey( key );
            strings_.insert( LowerBound( strings_, key ), translation );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: Context::SwapKey
// Created: SLI 2014-08-27
// -----------------------------------------------------------------------------
bool Context::SwapKey( const std::string& oldKey, const std::string& newKey )
{
    for( auto it = strings_.begin(); it != strings_.end(); ++it )
        (*it)->SwapKey( oldKey, newKey );
    return false;
}

// -----------------------------------------------------------------------------
// Name: Context::Apply
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool Context::Apply( const std::function< bool( LocalizedString& ) >& functor )
{
    for( auto it = strings_.begin(); it != strings_.end(); ++it )
        if( *it && functor( **it ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Context::CheckUniqueTranslation
// Created: SLI 2014-08-25
// -----------------------------------------------------------------------------
bool Context::CheckUniqueTranslation( const LocalizedString& translation ) const
{
    auto range = EqualRange( strings_, translation.Key() );
    for( auto it = range.first; it != range.second; ++it )
        if( it->get() != &translation && translation != **it )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Context::CleanTranslations
// Created: SLI 2014-08-26
// -----------------------------------------------------------------------------
void Context::CleanTranslations()
{
    strings_.erase( std::remove_if( strings_.begin(), strings_.end(),
                                    [&]( const boost::shared_ptr< LocalizedString >& translation ){ return translation.unique(); } ),
                    strings_.end() );
}

// -----------------------------------------------------------------------------
// Name: Context::IsEmpty
// Created: SLI 2014-08-26
// -----------------------------------------------------------------------------
bool Context::IsEmpty() const
{
    return strings_.empty();
}

// -----------------------------------------------------------------------------
// Name: Context::MergeDuplicateTranslations
// Created: SLI 2014-08-26
// -----------------------------------------------------------------------------
void Context::MergeDuplicateTranslations()
{
    for( auto lhs = strings_.begin(); lhs != strings_.end(); ++lhs )
    {
        for( auto rhs = lhs + 1; rhs != strings_.end(); )
        {
            if( ( *lhs )->Key() == ( *rhs )->Key() && **lhs == **rhs )
                rhs = strings_.erase( rhs );
            else
                ++rhs;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Context::HasDuplicateErrors
// Created: SLI 2014-08-26
// -----------------------------------------------------------------------------
bool Context::HasDuplicateErrors() const
{
    for( auto lhs = strings_.begin(); lhs != strings_.end(); ++lhs )
        for( auto rhs = lhs + 1; rhs != strings_.end(); ++rhs )
            if( ( *lhs )->Key() == ( *rhs )->Key() && **lhs != **rhs )
                return true;
    return false;
}
