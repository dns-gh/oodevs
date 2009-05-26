// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "ElementTypeResolver.h"
#include "DataType_ABC.h"
#include "Element_ABC.h"
#include "PrimitiveParameter.h"
#include "clients_kernel/Tools.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: ElementTypeResolver constructor
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
ElementTypeResolver::ElementTypeResolver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElementTypeResolver destructor
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
ElementTypeResolver::~ElementTypeResolver()
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
// Name: ElementTypeResolver::AddElement
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void ElementTypeResolver::AddElement( const DataType_ABC& definition, const DataType_ABC& instance )
{
    if( definition != instance )
        throw std::exception( tools::translate( "Indicators", "Expected '%1' got '%2'." ).arg( definition.ToString().c_str() ).arg( instance.ToString().c_str() ) );
    const std::string def = definition.ToString();
    if( IsAbstract( def ) )
    {
        AddPlaceholders( def, dictionary_ );
        instances_[ def ] = &instance;
    }
}

namespace
{
    std::string ReplaceBaseElementTypes( const std::string& type )
    {
        std::string result( type );
        // $$$$ SBO 2009-05-12: Variable types
        boost::replace_all( result, "unit list", "unsigned long" );
        boost::replace_all( result, "unit", "unsigned long" );
        boost::replace_all( result, "dotation list", "unsigned long" );
        boost::replace_all( result, "equipment list", "unsigned long" );
        boost::replace_all( result, "zone", "position" );
        // $$$$ SBO 2009-05-12: Extractor types
        boost::replace_all( result, "key", "unsigned long" );
        boost::replace_all( result, "equipment-type", "unsigned long" );
        boost::replace_all( result, "resource-type", "unsigned long" );
        boost::replace_all( result, "damage", "float" );
        boost::replace_all( result, "resources", "int" );
        // $$$$ SBO 2009-05-12: Function parameter types
        boost::replace_all( result, "operator", "string" );
        // $$$$ SBO 2009-04-15: TODO: put this into a config file...
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ElementTypeResolver::Resolve
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
std::string ElementTypeResolver::Resolve( const std::string& type ) const
{
    const_cast< ElementTypeResolver* >( this )->Update(); // $$$$ SBO 2009-04-15: 
    std::string result( type );
    for( std::map< std::string, std::string >::const_iterator it = dictionary_.begin(); it != dictionary_.end(); ++it )
        if( ! it->second.empty() )
            boost::replace_all( result, it->first, it->second );
    if( IsAbstract( result ) )
        throw std::exception( tools::translate( "Indicators", "Unresolved type parameter: %1." ).arg( result.c_str() ).ascii() );
    return ReplaceBaseElementTypes( result );
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
// Name: ElementTypeResolver::Update
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
void ElementTypeResolver::Update()
{
    for( T_Instances::const_iterator itI = instances_.begin(); itI != instances_.end(); ++itI )
    {
        std::vector< std::string > def = Split( itI->first );
        std::vector< std::string > val = Split( itI->second->Resolve() );
        if( def.size() != val.size() )
            ConvertElementTypes( def, val );
        for( std::vector< std::string >::const_iterator it = def.begin(); it != def.end(); ++it )
        {
            std::map< std::string, std::string >::iterator itD = dictionary_.find( *it );
            if( itD != dictionary_.end() && itD->second.empty() )
                itD->second = val.at( std::distance< std::vector< std::string >::const_iterator >( def.begin(), it ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ElementTypeResolver::ConvertElementTypes
// Created: SBO 2009-04-15
// $$$$ SBO 2009-04-15: Try to convert types from list(key,val) to list(val)
// -----------------------------------------------------------------------------
void ElementTypeResolver::ConvertElementTypes( std::vector< std::string >& type1, std::vector< std::string >& type2 ) const
{
    if( type1.size() > type2.size() )
        type1.erase( type1.begin(), type1.begin() + ( type1.size() - type2.size() ) );
    else if( type2.size() > type1.size() )
        type2.erase( type2.begin(), type2.begin() + ( type2.size() - type1.size() ) );
    if( type1.size() == 0 || type2.size() == 0 )
        throw std::exception( tools::translate( "Indicators", "Cannot convert list parameter." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ElementTypeResolver::ToSimpleType
// Created: SBO 2009-04-15
// -----------------------------------------------------------------------------
std::string ElementTypeResolver::ToSimpleType( const std::string& type )
{
    const std::vector< std::string > list = Split( type );
    if( list.empty() )
        throw std::exception( tools::translate( "Indicators", " has no type." ).ascii() );
    return list.back();
}

// -----------------------------------------------------------------------------
// Name: ElementTypeResolver::IsCompatible
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
bool ElementTypeResolver::IsCompatible( const std::string& lhs, const std::string& rhs )
{
    return IsAbstract( lhs ) || IsAbstract( rhs ) || ReplaceBaseElementTypes( lhs ) == ReplaceBaseElementTypes( rhs );
}
