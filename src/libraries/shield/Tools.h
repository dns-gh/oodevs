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

#pragma warning( push, 0 )
#include <google/protobuf/descriptor.h>
#pragma warning( pop )
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
    if( from.has_##from_field() ) \
        to->set_##to_field( from.from_field() )
#define CONVERT( field ) \
    CONVERT_TO( field, field )

#define CONVERT_ENUM_EXT( from_field, to_field, mapping, injective ) \
    { \
        const ::google::protobuf::FieldDescriptor* fromField = from.descriptor()->FindFieldByName( BOOST_PP_STRINGIZE( from_field ) ); \
        const ::google::protobuf::FieldDescriptor* toField = to->descriptor()->FindFieldByName( BOOST_PP_STRINGIZE( to_field ) ); \
        if( ! fromField || ! fromField->enum_type() ) \
            throw std::runtime_error( "enumeration field '" BOOST_PP_STRINGIZE( from_field ) "' not found in '" + from.descriptor()->full_name() + "'" ); \
        if( ! toField || ! toField->enum_type() ) \
            throw std::runtime_error( "enumeration field '" BOOST_PP_STRINGIZE( to_field ) "' not found in '" + to->descriptor()->full_name() + "'" ); \
        bool dummy = injective; \
        if( dummy && fromField->enum_type()->value_count() > toField->enum_type()->value_count() ) \
               throw std::runtime_error( "source values cannot all be mapped to destination values of field '" BOOST_PP_STRINGIZE( to_field ) "'" ); \
        if( fromField->enum_type()->value_count() != int( boost::assign::map_list_of mapping .size() ) ) \
                throw std::runtime_error( "missing values pair in mapping for field '" BOOST_PP_STRINGIZE( from_field ) "'" ); \
        if( from.has_##from_field() ) \
            to->set_##to_field( ConvertEnum( from.from_field(), boost::assign::map_list_of mapping ) ); \
    }
#define CONVERT_ENUM_TO( from_field, to_field, mapping ) \
    CONVERT_ENUM_EXT( from_field, to_field, mapping, true )
#define CONVERT_ENUM( field, mapping ) \
    CONVERT_ENUM_TO( field, field, mapping )
#define CONVERT_NON_INJECTIVE_ENUM_TO( from_field, to_field, mapping ) \
    CONVERT_ENUM_EXT( from_field, to_field, mapping, false )
#define CONVERT_NON_INJECTIVE_ENUM( from_field, to_field, mapping ) \
    CONVERT_NON_INJECTIVE_ENUM_TO( from_field, to_field, mapping )

#define CONVERT_ID_TO( from_field, to_field ) \
    if( from.has_##from_field() ) \
        to->mutable_##to_field()->set_id( from.from_field().id() )
#define CONVERT_ID( field ) \
    CONVERT_ID_TO( field, field )

#define CONVERT_LIST_TO( from_field, to_field, elem, callback ) \
    if( from.has_##from_field() ) \
    { \
        to->mutable_##to_field(); \
        for( int i = 0; i < from.from_field().elem().size(); ++i ) \
            callback( from.from_field().elem( i ), to->mutable_##to_field()->add_##elem() ); \
    }
#define CONVERT_LIST( field, elem, callback ) \
    CONVERT_LIST_TO( field, field, elem, callback )

#define CONVERT_ENUM_LIST( elem, mapping ) \
    for( int i = 0; i < from.elem().size(); ++i ) \
        to->add_##elem( ConvertEnum( from.elem( i ), boost::assign::map_list_of mapping ) )

#define CONVERT_CB_TO( from_field, to_field, callback ) \
    if( from.has_##from_field() ) \
        callback( from.from_field(), to->mutable_##to_field() )
#define CONVERT_CB( field, callback ) \
    CONVERT_CB_TO( field, field, callback )

#define CONVERT_DATE_TO( from_field, to_field ) \
    if( from.has_##from_field() && from.from_field().has_data() ) \
        to->mutable_##to_field()->set_data( from.from_field().data() )
#define CONVERT_DATE( field ) \
    CONVERT_DATE_TO( field, field )

#define CONVERT_NAME_TO( from_field, to_field ) \
    if( from.has_##from_field() && from.from_field().has_name() ) \
        to->mutable_##to_field()->set_name( from.from_field().name() )
#define CONVERT_NAME( field ) \
    CONVERT_NAME_TO( field, field )

#endif // shield_Tools_h
