// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorTypeResolver.h"
#include "IndicatorPrimitiveParameter.h"
#include "IndicatorType.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver constructor
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
IndicatorTypeResolver::IndicatorTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver destructor
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
IndicatorTypeResolver::~IndicatorTypeResolver()
{
    // NOTHING
}

namespace
{
    void AddPlaceholders( const std::string& type, std::map< std::string, std::string >& dictionary )
    {
        boost::regex rx( "(_[0-9]+)" );
        boost::sregex_iterator end;
        for( boost::sregex_iterator it( type.begin(), type.end(), rx ); it != end; ++it )
            for( unsigned int i = 1; i < it->size(); ++i )
                if( (*it)[i].matched )
                {
                    const std::string match( (*it)[i].first, (*it)[i].second );
                    if( ! dictionary[ match ].empty() )
                        dictionary[ match ] = "";
                }
    }

    bool IsAbstract( const std::string& type )
    {
        return boost::find_first( type, "_" );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver::AddElement
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void IndicatorTypeResolver::AddElement( const IndicatorType& instance, const IndicatorType& definition )
{
    if( IsAbstract( definition.ToString() ) )
    {
        AddPlaceholders( definition.ToString(), dictionary_ );
        instances_[ &definition ] = &instance;
    }
}

namespace
{
    std::string& ReplaceBaseTypes( std::string& type )
    {
        boost::replace_all( type, "key", "unsigned long" );
        boost::replace_all( type, "damage", "float" );
        boost::replace_all( type, "resources", "int" );
        // $$$$ SBO 2009-04-15: TODO: add resources, equiments...
        return type;
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver::Resolve
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
std::string IndicatorTypeResolver::Resolve( const std::string& type ) const
{
    const_cast< IndicatorTypeResolver* >( this )->Update(); // $$$$ SBO 2009-04-15: 
    std::string result( type );
    for( std::map< std::string, std::string >::const_iterator it = dictionary_.begin(); it != dictionary_.end(); ++it )
        if( ! it->second.empty() )
            boost::replace_all( result, it->first, it->second );
    if( IsAbstract( result ) )
        throw std::exception( ( "Unresolved type parameter: " + result ).c_str() );
    return ReplaceBaseTypes( result );
}

namespace
{
    std::vector< std::string > Split( const std::string& type )
    {
        std::string str = boost::replace_first_copy( type, "list(", "" );
        boost::replace_last( str, ")", "" );
        std::vector< std::string > result;
        return boost::split( result, str, boost::algorithm::is_any_of( "," ) );
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver::Update
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void IndicatorTypeResolver::Update()
{
    for( std::map< const IndicatorType*, const IndicatorType* >::const_iterator itI = instances_.begin(); itI != instances_.end(); ++itI )
    {
        std::vector< std::string > def = Split( itI->first->ToString() );
        std::vector< std::string > val = Split( itI->second->Resolve() );
        if( def.size() != val.size() )
            ConvertTypes( def, val );
        for( std::vector< std::string >::const_iterator it = def.begin(); it != def.end(); ++it )
        {
            std::map< std::string, std::string >::iterator itD = dictionary_.find( *it );
            if( itD != dictionary_.end() && itD->second.empty() )
                itD->second = val.at( std::distance< std::vector< std::string >::const_iterator >( def.begin(), it ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver::ConvertTypes
// Created: SBO 2009-04-15
// $$$$ SBO 2009-04-15: Try to convert types from list(key,val) to list(val)
// -----------------------------------------------------------------------------
void IndicatorTypeResolver::ConvertTypes( std::vector< std::string >& type1, std::vector< std::string >& type2 ) const
{
    if( type1.size() > type2.size() )
        type1.erase( type1.begin(), type1.begin() + ( type1.size() - type2.size() ) );
    else if( type2.size() > type1.size() )
        type2.erase( type2.begin(), type2.begin() + ( type2.size() - type1.size() ) );
    if( type1.size() == 0 || type2.size() == 0 )
        throw std::exception( "Cannot convert list parameters." );
}

// -----------------------------------------------------------------------------
// Name: IndicatorTypeResolver::ToSimpleType
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
std::string IndicatorTypeResolver::ToSimpleType( const std::string& type )
{
    const std::vector< std::string > list = Split( type );
    if( list.empty() )
        throw std::exception( "Indicator has no type." );
    return list.back();
}
