// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "QueryBuilder.h"
#include "Shape_ABC.h"

#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <functional>

using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: QueryBuilder constructor
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
QueryBuilder::QueryBuilder( const std::string& table )
    : table_ ( table )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder destructor
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
QueryBuilder::~QueryBuilder()
{
    // NOTHING
}

namespace 
{
    
    void replace_if( std::string& value, char compare )
    {
        for ( std::string::iterator it = value.begin(); it != value.end(); ++it )
        {
            if ( *it == compare )             
                it = value.insert( it, compare ) + 1;            
        }
    }
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::SetField
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::SetId( const std::string& field )
{
	idField_ = field;
    AddParameter( T_Parameter( field, "nextval('" + table_ + "_" + idField_ + "_seq')" ) );
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::GetSequence
// Created: MPT 2009-09-28
// -----------------------------------------------------------------------------
std::string QueryBuilder::GetSequence() const
{
	return table_ + "_" + idField_ + "_seq";
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::GetSequence
// Created: AME 2009-10-12
// -----------------------------------------------------------------------------
std::string QueryBuilder::GetTableName() const{
	return table_;
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::SetField
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::SetField( const std::string& field, const std::string& value )
{
    std::string clean( value );
    replace_if( clean, '\'' );
    AddParameter( T_Parameter( field, "'" + clean + "'" ) );
}
    
// -----------------------------------------------------------------------------
// Name: QueryBuilder::SetField
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::SetField( const std::string& field, int value )
{
    AddParameter( T_Parameter( field, boost::lexical_cast< std::string >( value ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: QueryBuilder::SetField
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::SetField( const std::string& field, long value )
{
    AddParameter( T_Parameter( field, boost::lexical_cast< std::string >( value ) ) );
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::SetField
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::SetField( const std::string& field, double value )
{
    AddParameter( T_Parameter( field, boost::lexical_cast< std::string >( value ) ) );
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::SetGeometry
/*
INSERT INTO sword.unitforces (id, public_oid, shape)
VALUES ( nextval('sword.unitforces_id_seq'), 13, st_point(33.1, 3.2, 0) );
*/
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::SetGeometry( const Shape_ABC& shape )
{
    std::stringstream geometry;
    shape.Serialize( geometry );
    AddParameter( T_Parameter( "shape", geometry.str() ) );
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::AddParameter
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
void QueryBuilder::AddParameter( const T_Parameter& parameter )
{
    parameters_.push_back( parameter );
}

namespace 
{
    template< typename pair_type >
    struct select1st : public std::unary_function< const pair_type&, typename const pair_type::first_type& >
    {
        const typename pair_type::first_type &operator()(const pair_type &v) const
        {
            return v.first;
        }
    };

    template< typename pair_type >
    struct select2nd : public std::unary_function< const pair_type&, typename const pair_type::second_type& >
    {
        const typename pair_type::second_type &operator()(const pair_type &v) const
        {
            return v.second;
        }
    };

    template< typename pair_type >
    struct update_builder : public std::unary_function< const pair_type&, typename const pair_type::second_type& >
    {
        const typename pair_type::second_type operator()( const pair_type &v ) const
        {
            return v.first + "=" + v.second;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::CreateInsert
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
std::string QueryBuilder::BuildInsert() const
{
    std::stringstream rows;
    std::transform( parameters_.begin(), parameters_.end() - 1, 
        std::ostream_iterator< std::string >( rows, "," ), select1st< T_Parameter >() );
    rows << select1st< T_Parameter >()( *(parameters_.end() - 1) );

    std::stringstream values;
    std::transform( parameters_.begin(), parameters_.end() - 1, 
        std::ostream_iterator< std::string >( values, "," ), select2nd< T_Parameter >() );
    values << select2nd< T_Parameter >()( *(parameters_.end() - 1) );
    return "INSERT INTO " + table_ + " (" + rows.str() + ") VALUES (" + values.str() + ")";
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::CreateInsert
// Created: JCR 2009-04-24
// -----------------------------------------------------------------------------
std::string QueryBuilder::BuildUpdate( const std::string& clause ) const
{
    /*
        final String update = "" + new SQLString( rows_, ',' )
                                    .append( "=" ).append( values );
        return "UPDATE " + table_ + " SET " + update + " WHERE " + clause;
    */

    std::stringstream rows;
    std::transform( parameters_.begin(), parameters_.end() - 1, 
        std::ostream_iterator< std::string >( rows, "," ), update_builder< T_Parameter >() );
    rows << update_builder< T_Parameter >()( *(parameters_.end() - 1) );
    
    if( clause.empty() )
        return "UPDATE " + table_ + " SET " + rows.str();
    else
        return "UPDATE " + table_ + " SET " + rows.str() + " WHERE " + clause;
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::BuildDelete
// Created: JCR 2009-04-28
// -----------------------------------------------------------------------------
std::string QueryBuilder::BuildDelete( const std::string& clause ) const
{
    if( clause.empty() )
        return "DELETE FROM " + table_;
    else
        return "DELETE FROM " + table_ + " WHERE " + clause;
}

// -----------------------------------------------------------------------------
// Name: QueryBuilder::IsValid
// Created: JCR 2009-04-28
// -----------------------------------------------------------------------------
bool QueryBuilder::IsValid() const
{
    return ! parameters_.empty();
}
