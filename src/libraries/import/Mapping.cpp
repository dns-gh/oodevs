// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Mapping.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: Mapping constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Mapping::Mapping( const std::string& file )
    : maxId_( 0 )
{
    xml::xifstream xis( file );
    xis >> xml::start( "mapping" )
            >> xml::list( "association", *this, &Mapping::ReadAssociation )
            >> xml::list( "automat", *this, &Mapping::ReadAutomat )
            >> xml::list( "unit", *this, &Mapping::ReadUnit );
}

// -----------------------------------------------------------------------------
// Name: Mapping destructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Mapping::~Mapping()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mapping::ReadAssociation
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Mapping::ReadAssociation( xml::xistream& xis )
{
    std::string key, value;
    xis >> xml::attribute( "key", key )
        >> xml::attribute( "value", value );
    mapping_[ key ] = value;
}

// -----------------------------------------------------------------------------
// Name: Mapping::ReadAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Mapping::ReadAutomat( xml::xistream& xis )
{
    ReadAssociation( xis );
    std::string key;
    xis >> xml::attribute( "key", key );
    automats_.insert( key );
}

// -----------------------------------------------------------------------------
// Name: Mapping::ReadUnit
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Mapping::ReadUnit( xml::xistream& xis )
{
    ReadAssociation( xis );
    std::string key, superior;
    xis >> xml::attribute( "key", key )
        >> xml::attribute( "superior", superior );
    units_[ key ] = superior;
    if( xis.attribute( "command-post", false ) )
        commandPosts_.insert( key );
}

// -----------------------------------------------------------------------------
// Name: Mapping::operator[]
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
std::string Mapping::operator[]( const std::string& key ) const
{
    std::map< std::string, std::string >::const_iterator it = mapping_.find( key );
    if( it != mapping_.end() )
        return it->second;
    unknownKeys_.insert( key );
    return key;
}

// -----------------------------------------------------------------------------
// Name: Mapping::AddWarning
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Mapping::AddWarning( const std::string& id )
{
    unknownKeys_.insert( id );
}

// -----------------------------------------------------------------------------
// Name: Mapping::LogWarnings
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void Mapping::LogWarnings( std::ostream& os ) const
{
    for( std::set< std::string >::const_iterator it = unknownKeys_.begin(); it != unknownKeys_.end(); ++it )
        os << "Identifiant non reconnu: " << *it << std::endl;
}

// -----------------------------------------------------------------------------
// Name: Mapping::AddId
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
unsigned int Mapping::AddId( const std::string& editorId )
{
    mapping_[ editorId ] = boost::lexical_cast< std::string >( ++maxId_ );
    return maxId_;
}

// -----------------------------------------------------------------------------
// Name: Mapping::AddId
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
unsigned int Mapping::AddId()
{
    ++maxId_;
    return maxId_;
}

// -----------------------------------------------------------------------------
// Name: Mapping::IsUnit
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
bool Mapping::IsUnit( const std::string& type ) const
{
    return units_.find( type ) != units_.end();
}

// -----------------------------------------------------------------------------
// Name: Mapping::IsAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
bool Mapping::IsAutomat( const std::string& type ) const
{
    return automats_.find( type ) != automats_.end();
}

// -----------------------------------------------------------------------------
// Name: Mapping::GetSuperiorId
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
std::string Mapping::GetSuperiorId( const std::string& type ) const
{
    std::map< std::string, std::string >::const_iterator it = units_.find( type );
    if( it != units_.end() )
        return it->second;
    unknownKeys_.insert( type );
    return type;
}

// -----------------------------------------------------------------------------
// Name: Mapping::IsCommandPost
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
bool Mapping::IsCommandPost( const std::string& type ) const
{
    return commandPosts_.find( type ) != commandPosts_.end();
}

// -----------------------------------------------------------------------------
// Name: Mapping::GetMissionParameterName
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
std::string Mapping::GetMissionParameterName( int /*index*/ ) const
{
    return "Location";
}

// -----------------------------------------------------------------------------
// Name: Mapping::GetMissionParameterType
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
std::string Mapping::GetMissionParameterType( int /*index*/ ) const
{
    return "point";
}

// -----------------------------------------------------------------------------
// Name: Mapping::GetMissionParameterLocationType
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
std::string Mapping::GetMissionParameterLocationType( int /*index*/ ) const
{
    return "point";
}
