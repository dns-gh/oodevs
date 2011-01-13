// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_Tools_h
#define shield_Tools_h

#include <boost/assign/list_of.hpp>

namespace shield
{
    template< typename E1, typename E2 >
    E2 ConvertEnum( E1 e, const boost::assign_detail::generic_list< std::pair< E1, E2 > >& list )
    {
        const std::map< E1, E2 > map( list );
        std::map< E1, E2 >::const_iterator it = map.find( e );
        if( it == map.end() )
            throw std::runtime_error( "invalid enum value while converting to shield protocol" );
        return it->second;
    }
    template< typename E1, typename E2 >
    E1 ConvertEnum( E2 e, const boost::assign_detail::generic_list< std::pair< E1, E2 > >& list )
    {
        const std::map< E1, E2 > map( list );
        for( std::map< E1, E2 >::const_iterator it = map.begin(); it != map.end(); ++it )
            if( it->second == e )
                return it->first;
        throw std::runtime_error( "invalid enum value while converting to sword protocol" );
    }
}

#define CONVERT_TO( from_field, to_field ) \
    if( from.has_##from_field() ) to->set_##to_field( from.from_field() )
#define CONVERT( field ) \
    CONVERT_TO( field, field )

#define CONVERT_ENUM_TO( from_field, to_field, mapping ) \
    if( from.has_##from_field() ) to->set_##to_field( ConvertEnum( from.from_field(), boost::assign::map_list_of mapping ) )
#define CONVERT_ENUM( field, mapping ) \
    CONVERT_ENUM_TO( field, field, mapping )

#define CONVERT_ENUM_EXT( field, type, mapping ) \
    assert( from.type##_descriptor()->value_count() == to->type##_descriptor()->value_count() ); \
    assert( from.type##_descriptor()->value_count() == int( boost::assign::map_list_of mapping .size() ) ); \
    CONVERT_ENUM( field, mapping )

#define CONVERT_ID_TO( from_field, to_field ) \
    to->mutable_##to_field()->set_id( from.from_field().id() )
#define CONVERT_ID( field ) \
    CONVERT_ID_TO( field, field )

#define CONVERT_LIST_TO( from_field, to_field, elem, callback ) \
    if( from.has_##from_field() ) \
        for( int i = 0; i < from.from_field().elem().size(); ++i ) \
            callback( from.from_field().elem( i ), to->mutable_##to_field()->add_##elem() )
#define CONVERT_LIST( field, elem, callback ) \
    CONVERT_LIST_TO( field, field, elem, callback )

#define CONVERT_ENUM_LIST( elem, mapping ) \
    for( int i = 0; i < from.elem().size(); ++i ) \
        to->add_##elem( ConvertEnum( from.elem( i ), boost::assign::map_list_of mapping ) )

#endif // shield_Tools_h
