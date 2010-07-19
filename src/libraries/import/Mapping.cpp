// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Mapping.h"
#include "Warnings.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: Mapping constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Mapping::Mapping( const std::string& file, Warnings& warnings )
    : warnings_( warnings )
    , maxId_   ( 0 )
{
    xml::xifstream xis( file );
    xis >> xml::start( "mapping" )
            >> xml::list( "association", *this, &Mapping::ReadAssociation )
            >> xml::list( "automat", *this, &Mapping::ReadAutomat )
            >> xml::list( "unit", *this, &Mapping::ReadUnit )
            >> xml::list( "mission", *this, &Mapping::ReadMission );
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
// Name: Mapping::ReadMission
// Created: MCO 2010-07-16
// -----------------------------------------------------------------------------
void Mapping::ReadMission( xml::xistream& xis )
{
    ReadAssociation( xis );
    std::string key;
    xis >> xml::attribute( "key", key )
        >> xml::list( "parameter", *this, &Mapping::ReadParameter, key );
}

// -----------------------------------------------------------------------------
// Name: Mapping::ReadParameter
// Created: MCO 2010-07-16
// -----------------------------------------------------------------------------
void Mapping::ReadParameter( xml::xistream& xis, const std::string& key )
{
    std::string name, type, location;
    xis >> xml::attribute( "name", name )
        >> xml::attribute( "type", type )
        >> xml::attribute( "location", location );
    missions_[ key ].push_back( boost::make_tuple( name, type, location ) );
}

// -----------------------------------------------------------------------------
// Name: Mapping::operator[]
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
std::string Mapping::operator[]( const std::string& id ) const
{
    std::map< std::string, std::string >::const_iterator it = mapping_.find( id );
    if( it != mapping_.end() )
        return it->second;
    warnings_.Add( id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: Mapping::AddId
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
unsigned int Mapping::AddId( const std::string& id )
{
    mapping_[ id ] = boost::lexical_cast< std::string >( ++maxId_ );
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
    warnings_.Add( type );
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
// Name: Mapping::Serialize
// Created: LDC 2010-07-19
// -----------------------------------------------------------------------------
void Mapping::Serialize( xml::xostream& xos, const std::string& id, const std::vector< std::vector< Position > >& tacticals ) const
{
    xos << xml::attribute( "id", (*this)[ id ] )
        << xml::attribute( "name", "" );
    std::size_t i = 0;
    for( std::vector< std::vector< Position > >::const_iterator it = tacticals.begin(); it != tacticals.end(); ++it, ++i )
    {
        xos << xml::start( "parameter" )
                << xml::attribute( "name", GetParameter< 0 >( id, i ) )
                << xml::attribute( "type", GetParameter< 1 >( id, i ) )
                << xml::start( "location" )
                    << xml::attribute( "type", GetParameter< 2 >( id, i ) );
        for( std::vector< Position >::const_iterator itPos = it->begin(); itPos != it->end(); ++itPos )
            xos << xml::start( "point" )
                    << xml::attribute( "coordinates", *itPos )
                << xml::end;
        xos     << xml::end
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Mapping::GetParameter
// Created: LDC 2010-07-19
// -----------------------------------------------------------------------------
template< int I >
std::string Mapping::GetParameter( const std::string& id, std::size_t index ) const
{
    CIT_Missions it = missions_.find( id );
    if( it == missions_.end() )
    {
        warnings_.Add( id );
        return "?";
    }
    if( index >= it->second.size() )
        return "?"; // $$$$ MCO : add warning ?
    return it->second[ index ].get< I >();
}
