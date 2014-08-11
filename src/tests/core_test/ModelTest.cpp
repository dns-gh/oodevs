// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"
#include "core/Model.h"
#include "core/UserData.h"
#include "MockModelListener.h"
#include "MockModelVisitor.h"
#include "TextArchive.h"
#include <boost/numeric/conversion/cast.hpp>
#include <string>

namespace
{
    const uint8_t  uint8_max  = static_cast< uint8_t  >( -1 );
    const uint16_t uint16_max = static_cast< uint16_t >( -1 );
    const uint32_t uint32_max = static_cast< uint32_t >( -1 );
    const uint64_t uint64_max = static_cast< uint64_t >( -1 );
    const int8_t   int8_max   = static_cast< int8_t  >( uint8_max  >> 1 );
    const int16_t  int16_max  = static_cast< int16_t >( uint16_max >> 1 );
    const int32_t  int32_max  = static_cast< int32_t >( uint32_max >> 1 );
    const int64_t  int64_max  = static_cast< int64_t >( uint64_max >> 1 );

    template< typename Target, typename Source >
    Target store_and_retrieve( Source in )
    {
        core::Model model;
        model = in;
        Target out = model;
        return out;
    }

    template< typename T >
    void load_all_types_from_stored_type( T in )
    {
        BOOST_CHECK_EQUAL( static_cast< int8_t   >( in ), store_and_retrieve< int8_t   >( in ) );
        BOOST_CHECK_EQUAL( static_cast< int16_t  >( in ), store_and_retrieve< int16_t  >( in ) );
        BOOST_CHECK_EQUAL( static_cast< int32_t  >( in ), store_and_retrieve< int32_t  >( in ) );
        BOOST_CHECK_EQUAL( static_cast< int64_t  >( in ), store_and_retrieve< int64_t  >( in ) );
        BOOST_CHECK_EQUAL( static_cast< uint8_t  >( in ), store_and_retrieve< uint8_t  >( in ) );
        BOOST_CHECK_EQUAL( static_cast< uint16_t >( in ), store_and_retrieve< uint16_t >( in ) );
        BOOST_CHECK_EQUAL( static_cast< uint32_t >( in ), store_and_retrieve< uint32_t >( in ) );
        BOOST_CHECK_EQUAL( static_cast< uint64_t >( in ), store_and_retrieve< uint64_t >( in ) );
        BOOST_CHECK_EQUAL( static_cast< float    >( in ), store_and_retrieve< float    >( in ) );
        BOOST_CHECK_EQUAL( static_cast< double   >( in ), store_and_retrieve< double   >( in ) );
    }
}

BOOST_AUTO_TEST_CASE( model_stores_and_loads_all_numeric_types )
{
    load_all_types_from_stored_type< int8_t   >( int8_max );
    load_all_types_from_stored_type< int16_t  >( int8_max );
    load_all_types_from_stored_type< int32_t  >( int8_max );
    load_all_types_from_stored_type< int64_t  >( int8_max );
    load_all_types_from_stored_type< uint8_t  >( int8_max );
    load_all_types_from_stored_type< uint16_t >( int8_max );
    load_all_types_from_stored_type< uint32_t >( int8_max );
    load_all_types_from_stored_type< uint64_t >( int8_max );
    load_all_types_from_stored_type< float    >( int8_max );
    load_all_types_from_stored_type< double   >( int8_max );
}

namespace
{
    template< typename Target, typename Source >
    void check_store_and_retrieve_throws( Source in )
    {
        BOOST_CHECK_THROW( store_and_retrieve< Target >( in ), std::exception );
    }

    template< typename T >
    void check_all_unsigned_throw()
    {
        check_store_and_retrieve_throws< uint8_t,  T >( -1 );
        check_store_and_retrieve_throws< uint16_t, T >( -1 );
        check_store_and_retrieve_throws< uint32_t, T >( -1 );
        check_store_and_retrieve_throws< uint64_t, T >( -1 );
    }
}

BOOST_AUTO_TEST_CASE( model_retrieving_negative_number_as_unsigned_throws )
{
    check_all_unsigned_throw< int8_t  >();
    check_all_unsigned_throw< int16_t >();
    check_all_unsigned_throw< int32_t >();
    check_all_unsigned_throw< int64_t >();
    check_all_unsigned_throw< float   >();
    check_all_unsigned_throw< double  >();
}

BOOST_AUTO_TEST_CASE( model_signed_overflow_throws )
{
    check_store_and_retrieve_throws< int8_t,   int16_t >( int8_max  + 1 );
    check_store_and_retrieve_throws< uint8_t,  int16_t >( uint8_max + 1 );

    check_store_and_retrieve_throws< int8_t,   int32_t >( int8_max   + 1 );
    check_store_and_retrieve_throws< int16_t,  int32_t >( int16_max  + 1 );
    check_store_and_retrieve_throws< uint8_t,  int32_t >( uint8_max  + 1 );
    check_store_and_retrieve_throws< uint16_t, int32_t >( uint16_max + 1 );

    check_store_and_retrieve_throws< int8_t,   int64_t >( int8_max   + 1 );
    check_store_and_retrieve_throws< int16_t,  int64_t >( int16_max  + 1 );
    check_store_and_retrieve_throws< int32_t,  int64_t >( int32_max  + static_cast< int64_t >( 1 ) );
    check_store_and_retrieve_throws< uint8_t,  int64_t >( uint8_max  + 1 );
    check_store_and_retrieve_throws< uint16_t, int64_t >( uint16_max + 1 );
    check_store_and_retrieve_throws< uint32_t, int64_t >( uint32_max + static_cast< int64_t >( 1 ) );
}

BOOST_AUTO_TEST_CASE( model_unsigned_overflow_throws )
{
    check_store_and_retrieve_throws< int8_t,  uint8_t  >( int8_max + 1 );

    check_store_and_retrieve_throws< int8_t,  uint16_t >( int8_max  + 1 );
    check_store_and_retrieve_throws< int16_t, uint16_t >( int16_max + 1 );
    check_store_and_retrieve_throws< uint8_t, uint16_t >( uint8_max + 1 );

    check_store_and_retrieve_throws< int8_t,   uint32_t >( int8_max   + 1 );
    check_store_and_retrieve_throws< int16_t,  uint32_t >( int16_max  + 1 );
    check_store_and_retrieve_throws< int32_t,  uint32_t >( int32_max  + static_cast< uint32_t >( 1 ) );
    check_store_and_retrieve_throws< uint8_t,  uint32_t >( uint8_max  + 1 );
    check_store_and_retrieve_throws< uint16_t, uint32_t >( uint16_max + 1 );

    check_store_and_retrieve_throws< int8_t,   uint64_t >( int8_max   + 1 );
    check_store_and_retrieve_throws< int16_t,  uint64_t >( int16_max  + 1 );
    check_store_and_retrieve_throws< int32_t,  uint64_t >( int32_max  + static_cast< uint64_t >( 1 ) );
    check_store_and_retrieve_throws< int64_t,  uint64_t >( int64_max  + static_cast< uint64_t >( 1 ) );
    check_store_and_retrieve_throws< uint8_t,  uint64_t >( uint8_max  + 1 );
    check_store_and_retrieve_throws< uint16_t, uint64_t >( uint16_max + 1 );
    check_store_and_retrieve_throws< uint32_t, uint64_t >( uint32_max + static_cast< uint64_t >( 1 ) );
}

BOOST_AUTO_TEST_CASE( model_float_overflow_throws )
{
    check_store_and_retrieve_throws< int8_t,   float >( int8_max   + 1 );
    check_store_and_retrieve_throws< int16_t,  float >( int16_max  + 1 );
    check_store_and_retrieve_throws< uint8_t,  float >( uint8_max  + 1 );
    check_store_and_retrieve_throws< uint16_t, float >( uint16_max + 1 );
}

BOOST_AUTO_TEST_CASE( model_double_overflow_throws )
{
    check_store_and_retrieve_throws< int8_t,   double >( int8_max   + 1 );
    check_store_and_retrieve_throws< int16_t,  double >( int16_max  + 1 );
    check_store_and_retrieve_throws< int32_t,  double >( int32_max  + static_cast< double >( 1 ) );
    check_store_and_retrieve_throws< uint8_t,  double >( uint8_max  + 1 );
    check_store_and_retrieve_throws< uint16_t, double >( uint16_max + 1 );
    check_store_and_retrieve_throws< uint32_t, double >( uint32_max + static_cast< double >( 1 ) );
}

namespace
{
    template< typename Source >
    bool store_any_and_retrieve_boolean( bool value )
    {
        core::Model model;
        Source in = boost::numeric_cast< Source >( value ? 42 : 0 );
        model = in;
        bool out = model;
        return out;
    }
}

BOOST_AUTO_TEST_CASE( model_can_convert_all_numeric_types_to_boolean )
{
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< int8_t   >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< int16_t  >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< int32_t  >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< int64_t  >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< uint8_t  >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< uint16_t >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< uint32_t >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< uint64_t >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< float    >( false ) );
    BOOST_CHECK_EQUAL( false, store_any_and_retrieve_boolean< double   >( false ) );

    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< int8_t   >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< int16_t  >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< int32_t  >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< int64_t  >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< uint8_t  >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< uint16_t >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< uint32_t >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< uint64_t >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< float    >( true ) );
    BOOST_CHECK_EQUAL( true, store_any_and_retrieve_boolean< double   >( true ) );
}

BOOST_AUTO_TEST_CASE( model_storing_text_reads_it )
{
    BOOST_CHECK_EQUAL( "text", store_and_retrieve< std::string >( "text" ) );
}

BOOST_AUTO_TEST_CASE( model_storing_number_attempting_to_read_text_throws )
{
    check_store_and_retrieve_throws< std::string, int8_t   >( 1 );
    check_store_and_retrieve_throws< std::string, int16_t  >( 1 );
    check_store_and_retrieve_throws< std::string, int32_t  >( 1 );
    check_store_and_retrieve_throws< std::string, int64_t  >( 1 );
    check_store_and_retrieve_throws< std::string, uint8_t  >( 1 );
    check_store_and_retrieve_throws< std::string, uint16_t >( 1 );
    check_store_and_retrieve_throws< std::string, uint32_t >( 1 );
    check_store_and_retrieve_throws< std::string, uint64_t >( 1 );
    check_store_and_retrieve_throws< std::string, float    >( 1 );
    check_store_and_retrieve_throws< std::string, double   >( 1 );
}

BOOST_AUTO_TEST_CASE( model_storing_text_attempting_to_read_anything_but_text_throws )
{
    check_store_and_retrieve_throws< int8_t   >( "text" );
    check_store_and_retrieve_throws< int16_t  >( "text" );
    check_store_and_retrieve_throws< int32_t  >( "text" );
    check_store_and_retrieve_throws< int64_t  >( "text" );
    check_store_and_retrieve_throws< uint8_t  >( "text" );
    check_store_and_retrieve_throws< uint16_t >( "text" );
    check_store_and_retrieve_throws< uint32_t >( "text" );
    check_store_and_retrieve_throws< uint64_t >( "text" );
    check_store_and_retrieve_throws< float    >( "text" );
    check_store_and_retrieve_throws< double   >( "text" );
    check_store_and_retrieve_throws< bool     >( "text" );
}

BOOST_AUTO_TEST_CASE( retrieving_unstored_data_fails )
{
    const core::Model model;
    BOOST_CHECK_THROW( model[ "key" ], std::exception );
    BOOST_CHECK_THROW( model.GetData(), std::exception );
    BOOST_CHECK_THROW( static_cast< int >( model ), std::exception );
    BOOST_CHECK_THROW( model.GetElement( 0 ), std::exception );
}

BOOST_AUTO_TEST_CASE( model_adding_child_to_different_type_node_throws )
{
    {
        core::Model model;
        model = 3;
        BOOST_CHECK_THROW( model[ "key" ], std::exception );
    }
    {
        core::Model model;
        model.AddElement();
        BOOST_CHECK_THROW( model[ "key" ], std::exception );
    }
    {
        core::Model model;
        model.SetUserData( 42 );
        BOOST_CHECK_THROW( model[ "key" ], std::exception );
    }
    {
        core::Model model;
        model[ 42 ];
        BOOST_CHECK_THROW( model[ "key" ], std::exception );
    }
    {
        core::Model model;
        model[ "key" ];
        BOOST_CHECK_THROW( model[ 42 ], std::exception );
    }
}

BOOST_AUTO_TEST_CASE( model_adding_value_to_different_type_node_throws )
{
    {
        core::Model model;
        model.AddElement();
        BOOST_CHECK_THROW( model = 3, std::exception );
    }
    {
        core::Model model;
        model[ "child" ];
        BOOST_CHECK_THROW( model = 3, std::exception );
    }
    {
        core::Model model;
        model.SetUserData( 42 );
        BOOST_CHECK_THROW( model = 3, std::exception );
    }
}

BOOST_AUTO_TEST_CASE( model_adding_element_to_different_type_node_throws )
{
    {
        core::Model model;
        model = 3;
        BOOST_CHECK_THROW( model.AddElement(), std::exception );
    }
    {
        core::Model model;
        model[ "child" ];
        BOOST_CHECK_THROW( model.AddElement(), std::exception );
    }
    {
        core::Model model;
        model.SetUserData( 42 );
        BOOST_CHECK_THROW( model.AddElement(), std::exception );
    }
}

BOOST_AUTO_TEST_CASE( model_adding_user_data_to_different_type_node_throws )
{
    {
        core::Model model;
        model = 3;
        BOOST_CHECK_THROW( model.SetUserData( 42 ), std::exception );
    }
    {
        core::Model model;
        model[ "child" ];
        BOOST_CHECK_THROW( model.SetUserData( 42 ), std::exception );
    }
    {
        core::Model model;
        model.AddElement();
        BOOST_CHECK_THROW( model.SetUserData( 42 ), std::exception );
    }
}

BOOST_AUTO_TEST_CASE( empty_key_child_is_parent )
{
    core::Model parent;
    core::Model& child = parent[ "" ];
    BOOST_CHECK_EQUAL( &parent, &child );
}

BOOST_AUTO_TEST_CASE( model_child_can_be_added_to_parent_by_splitting_forward_slashs )
{
    core::Model parent;
    const int in = 3;
    {
        core::Model& child = parent[ "child/sub-child" ];
        child = in;
    }
    core::Model& child = parent[ "child" ][ "sub-child" ];
    int out = child;
    BOOST_CHECK_EQUAL( in, out );
}

BOOST_AUTO_TEST_CASE( model_provides_root_context )
{
    const core::Model root;
    BOOST_CHECK_EQUAL( "/", root.Context() );
}

BOOST_AUTO_TEST_CASE( model_provides_child_context )
{
    core::Model root;
    const core::Model& child = root[ "child/sub-child" ];
    BOOST_CHECK_EQUAL( "/child/sub-child", child.Context() );
}

BOOST_AUTO_TEST_CASE( model_provides_element_context )
{
    core::Model root;
    const core::Model& element = root[ "child" ].AddElement();
    BOOST_CHECK_EQUAL( "/child/0", element.Context() );
}

namespace
{
    bool CheckExceptionMessage( const std::exception& e, const std::string& message )
    {
        BOOST_CHECK_EQUAL( message, e.what() );
        return message == e.what();
    }
}
#define BOOST_CHECK_EXCEPTION_MESSAGE( expression, message ) \
    BOOST_CHECK_EXCEPTION( expression, std::exception, boost::bind( &CheckExceptionMessage, _1, message ) );

BOOST_AUTO_TEST_CASE( model_raising_an_exception_when_retrieving_child_provides_context )
{
    core::Model parent;
    const core::Model& child = parent[ "child/sub-child" ];
    BOOST_CHECK_EXCEPTION_MESSAGE( child[ "non-existing" ],
        "cannot find 'non-existing' in /child/sub-child as it contains nothing" );
}

BOOST_AUTO_TEST_CASE( model_raising_an_exception_when_retrieving_grand_child_provides_context )
{
    core::Model parent;
    parent[ "child" ];
    BOOST_CHECK_EXCEPTION_MESSAGE( const_cast< const core::Model& >( parent )[ "child/non-existing" ],
        "cannot find 'non-existing' in /child as it contains nothing" );
}

BOOST_AUTO_TEST_CASE( model_added_element_can_be_retrieved )
{
    core::Model model;
    model.AddElement() = 4;
    model.AddElement() = 7;
    BOOST_CHECK_EQUAL( 2u, model.GetSize() );
    BOOST_CHECK_EQUAL( 4, static_cast< int >( model.GetElement( 0 ) ) );
    BOOST_CHECK_EQUAL( 7, static_cast< int >( model.GetElement( 1 ) ) );
}

BOOST_AUTO_TEST_CASE( model_retrieving_out_of_range_element_throws )
{
    core::Model model;
    model.AddElement() = 4;
    model.AddElement() = 7;
    BOOST_CHECK_THROW( model.GetElement( 42 ), std::exception );
}

BOOST_AUTO_TEST_CASE( model_elements_size_is_zero_when_empty )
{
    core::Model model;
    BOOST_CHECK_EQUAL( 0u, model.GetSize() );
}

BOOST_AUTO_TEST_CASE( model_elements_can_be_removed )
{
    core::Model model;
    core::Model& element = model.AddElement();
    BOOST_CHECK_EQUAL( 1u, model.GetSize() );
    element.Remove();
    BOOST_CHECK_EQUAL( 0u, model.GetSize() );
    BOOST_CHECK_THROW( model.GetElement( 0 ), std::exception );
}

BOOST_AUTO_TEST_CASE( empty_model_can_be_copied_to_empty_model )
{
    core::Model lhs;
    {
        core::Model rhs;
        lhs = rhs;
    }
}

BOOST_AUTO_TEST_CASE( value_model_can_be_copied_to_value_model )
{
    core::Model lhs;
    lhs = 42;
    {
        core::Model rhs;
        rhs = 77;
        lhs = rhs;
    }
    BOOST_CHECK_EQUAL( 77, static_cast< int >( lhs ) );
}

BOOST_AUTO_TEST_CASE( empty_model_cannot_be_copied_to_value_model )
{
    core::Model lhs;
    lhs = 42;
    core::Model rhs;
    BOOST_CHECK_THROW( lhs = rhs, std::exception );
}

BOOST_AUTO_TEST_CASE( model_can_be_copied )
{
    core::Model lhs;
    {
        core::Model rhs;
        rhs[ "value" ] = 42;
        rhs[ "element" ].AddElement() = 23;
        rhs[ "data" ].SetUserData( 17 );
        lhs = rhs;
    }
    BOOST_CHECK_EQUAL( 42, static_cast< int >( lhs[ "value" ] ) );
    BOOST_CHECK_EQUAL( 1u, lhs[ "element" ].GetSize() );
    BOOST_CHECK_EQUAL( 23, static_cast< int >( lhs[ "element" ].GetElement( 0 ) ) );
    BOOST_CHECK_EQUAL( 17, lhs[ "data" ].GetUserData< int >() );
}

BOOST_AUTO_TEST_CASE( children_model_can_be_copied_to_children_model )
{
    core::Model lhs;
    lhs[ "child" ];
    {
        core::Model rhs;
        rhs[ "value" ] = 42;
        rhs[ "element" ].AddElement() = 23;
        rhs[ "data" ].SetUserData( 17 );
        lhs = rhs;
    }
    BOOST_CHECK_EQUAL( 42, static_cast< int >( lhs[ "value" ] ) );
    BOOST_CHECK_EQUAL( 1u, lhs[ "element" ].GetSize() );
    BOOST_CHECK_EQUAL( 23, static_cast< int >( lhs[ "element" ].GetElement( 0 ) ) );
    BOOST_CHECK_EQUAL( 17, lhs[ "data" ].GetUserData< int >() );
}

BOOST_AUTO_TEST_CASE( model_can_be_visited_if_empty )
{
    core::Model model;
    core::MockModelVisitor visitor;
    model.Accept( visitor );
}

BOOST_AUTO_TEST_CASE( model_can_store_shared_pointer_user_data )
{
    boost::shared_ptr< int > retrieved;
    {
        core::Model model;
        model.SetUserData( boost::shared_ptr< int >( new int( 3 ) ) );
        retrieved = model.GetUserData< boost::shared_ptr< int > >();
    }
    BOOST_CHECK_EQUAL( 3, *retrieved );
}

BOOST_AUTO_TEST_CASE( model_can_store_pointer_on_user_data )
{
    core::Model model;
    int data = 3;
    model.SetUserData( &data );
    const int retrieved = model.GetUserData< int >();
    BOOST_CHECK_EQUAL( 3, retrieved );
}

BOOST_AUTO_TEST_CASE( model_can_store_const_pointer_on_user_data )
{
    core::Model model;
    int data = 3;
    const int* pouet = &data;
    model.SetUserData( pouet );
    const int retrieved = model.GetUserData< const int >();
    BOOST_CHECK_EQUAL( 3, retrieved );
}

BOOST_AUTO_TEST_CASE( model_can_store_user_data_by_value )
{
    core::Model model;
    int data = 3;
    model.SetUserData( data );
    const int retrieved = model.GetUserData< int >();
    BOOST_CHECK_EQUAL( 3, retrieved );
}

BOOST_AUTO_TEST_CASE( model_can_retrieve_user_data_by_non_const_reference )
{
    core::Model model;
    int data = 3;
    model.SetUserData( data );
    int& retrieved = model.GetUserData< int >();
    BOOST_CHECK_EQUAL( 3, retrieved );
}

BOOST_AUTO_TEST_CASE( model_can_retrieve_user_data_by_pointer )
{
    core::Model model;
    int data = 3;
    model.SetUserData( data );
    int* retrieved = model.GetUserData< int* >();
    BOOST_REQUIRE( retrieved );
    BOOST_CHECK_EQUAL( 3, *retrieved );
}

BOOST_AUTO_TEST_CASE( model_throws_if_retrieving_user_data_when_none_was_added )
{
    core::Model model;
    BOOST_CHECK_THROW( model.GetUserData< int >(), std::exception );
}

BOOST_AUTO_TEST_CASE( model_without_value_throws_when_retrieving_value )
{
    const core::Model model;
    BOOST_CHECK_THROW( static_cast< int64_t >( model ), std::exception );
    BOOST_CHECK_THROW( static_cast< std::string >( model ), std::exception );
}

BOOST_AUTO_TEST_CASE( model_is_serializable_from_base_pointer )
{
    std::stringstream s;
    {
        boost::shared_ptr< core::Model > model( new core::Model() );
        *model = 3;
        boost::archive::text_oarchive a( s );
        a & model;
    }
    {
        boost::shared_ptr< core::Model > model;
        boost::archive::text_iarchive a( s );
        a & model;
        const int out = *model;
        BOOST_CHECK_EQUAL( 3, out );
    }
}

BOOST_AUTO_TEST_CASE( model_child_is_serializable )
{
    std::stringstream s;
    {
        core::Model model;
        model[ "key" ] = "this is some text";
        boost::archive::text_oarchive a( s );
        a & model;
    }
    {
        core::Model model;
        boost::archive::text_iarchive a( s );
        a & model;
        const std::string& text = model[ "key" ];
        BOOST_CHECK_EQUAL( "this is some text", text );
    }
}

BOOST_AUTO_TEST_CASE( model_element_is_serializable )
{
    std::stringstream s;
    {
        core::Model model;
        model.AddElement() = true;
        boost::archive::text_oarchive a( s );
        a & model;
    }
    {
        core::Model model;
        boost::archive::text_iarchive a( s );
        a & model;
        BOOST_REQUIRE_EQUAL( 1u, model.GetSize() );
        BOOST_CHECK( model.GetElement( 0 ) );
    }
}

BOOST_AUTO_TEST_CASE( model_element_is_serializable_separately )
{
    std::stringstream s;
    {
        core::Model model;
        model.AddElement() = true;
        boost::archive::text_oarchive a( s );
        const core::Model* const element = &model.GetElement( 0 );
        a & model & element;
    }
    {
        core::Model model;
        boost::archive::text_iarchive a( s );
        core::Model* element;
        a & model & element;
        BOOST_REQUIRE( element );
        BOOST_CHECK( *element );
    }
}

SWORD_USER_DATA_EXPORT( int )

BOOST_AUTO_TEST_CASE( model_user_data_is_serializable )
{
    std::stringstream s;
    {
        core::Model model;
        model.SetUserData( 42 );
        boost::archive::text_oarchive a( s );
        a & model;
    }
    {
        core::Model model;
        boost::archive::text_iarchive a( s );
        a & model;
        BOOST_CHECK_EQUAL( 42, model.GetUserData< int >() );
    }
}

BOOST_AUTO_TEST_CASE( model_deserialized_without_value_throws_when_retrieving_value )
{
    std::stringstream s;
    {
        core::Model model;
        boost::archive::text_oarchive a( s );
        a & model;
    }
    {
        core::Model model;
        boost::archive::text_iarchive a( s );
        a & model;
        BOOST_CHECK_THROW( static_cast< int64_t >( model ), std::exception );
    }
}

BOOST_AUTO_TEST_CASE( a_model_can_be_part_of_an_expression )
{
    {
        core::Model model;
        model = 3;
        BOOST_CHECK_EQUAL( 4, model + 1 );
        BOOST_CHECK_EQUAL( 4, 1 + model );
        BOOST_CHECK_EQUAL( 2, model - 1 );
        BOOST_CHECK_EQUAL( -2, 1 - model );
        BOOST_CHECK_EQUAL( 6, model * 2 );
        BOOST_CHECK_EQUAL( 6, 2 * model );
        BOOST_CHECK_EQUAL( 1.5, model / 2. );
        BOOST_CHECK_EQUAL( 1, 3. / model );
        BOOST_CHECK_EQUAL( true, model == 3 );
        BOOST_CHECK_EQUAL( true, 3 == model );
        BOOST_CHECK_EQUAL( false, model != 3 );
        BOOST_CHECK_EQUAL( false, 3 != model );
        BOOST_CHECK_EQUAL( false, 3 < model );
        BOOST_CHECK_EQUAL( false, 3 > model );
        BOOST_CHECK_EQUAL( true, 2 < model );
        BOOST_CHECK_EQUAL( true, 4 > model );
        BOOST_CHECK_EQUAL( true, 3 <= model );
        BOOST_CHECK_EQUAL( true, 3 >= model );
        BOOST_CHECK_EQUAL( true, 2 <= model );
        BOOST_CHECK_EQUAL( true, 4 >= model );
    }
    {
        core::Model model;
        model = "string";
        BOOST_CHECK_EQUAL( "some string", "some " + model );
        BOOST_CHECK_EQUAL( "string some", model + " some" );
        BOOST_CHECK( "string" == model );
        BOOST_CHECK( model == "string" );
        BOOST_CHECK( "not string" != model );
        BOOST_CHECK( model != "not string" );
    }
}

BOOST_AUTO_TEST_CASE( model_root_cannot_be_removed )
{
    core::Model model;
    BOOST_CHECK_THROW( model.Remove(), std::exception );
}

BOOST_AUTO_TEST_CASE( model_child_can_be_removed )
{
    {
        core::Model model;
        model[ "child" ].Remove();
        BOOST_CHECK_THROW( static_cast< const core::Model& >( model )[ "child" ], std::exception );
    }
    {
        core::Model model;
        model[ 42 ].Remove();
        BOOST_CHECK_THROW( static_cast< const core::Model& >( model )[ 42 ], std::exception );
    }
}

BOOST_AUTO_TEST_CASE( model_element_can_be_removed )
{
    core::Model model;
    model.AddElement();
    model.AddElement().Remove();
    BOOST_CHECK_EQUAL( 1u, model.GetSize() );
}

BOOST_AUTO_TEST_CASE( model_with_value_can_be_cleared )
{
    core::Model model;
    model = 3;
    model.Clear();
    core::MockModelVisitor visitor;
    model.Accept( visitor );
}

BOOST_AUTO_TEST_CASE( model_with_child_can_be_cleared )
{
    core::Model model;
    model[ "child" ];
    model.Clear();
    core::MockModelVisitor visitor;
    model.Accept( visitor );
}

BOOST_AUTO_TEST_CASE( model_with_element_can_be_cleared )
{
    core::Model model;
    model.AddElement();
    model.Clear();
    core::MockModelVisitor visitor;
    model.Accept( visitor );
}

BOOST_AUTO_TEST_CASE( model_child_with_name_ending_with_forward_slash_is_the_same_child_as_without )
{
    core::Model model;
    BOOST_CHECK( &model[ "child/" ] == &model[ "child" ] );
}

BOOST_AUTO_TEST_CASE( model_can_check_if_child_exists )
{
    {
        core::Model model;
        model[ "child" ];
        BOOST_CHECK( model.HasChild( "child" ) );
        BOOST_CHECK( !model.HasChild( "unknown" ) );
    }
    {
        core::Model model;
        model[ 42 ];
        BOOST_CHECK( model.HasChild( 42 ) );
        BOOST_CHECK( !model.HasChild( 1337 ) );
    }
}

BOOST_AUTO_TEST_CASE( model_follows_link_to_get_child )
{
    core::Model model;
    core::Model link;
    link.Link( model );
    const core::Model& child = model[ "child" ];
    BOOST_CHECK( &child == &const_cast< const core::Model& >( link )[ "child" ] );
}

BOOST_AUTO_TEST_CASE( model_follows_link_to_get_element )
{
    core::Model model;
    core::Model link;
    link.Link( model );
    model.AddElement();
    BOOST_CHECK_EQUAL( 1u, link.GetSize() );
}

BOOST_AUTO_TEST_CASE( model_follows_link_to_read_value )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    core::Model link;
    link.Link( child );
    child = 42;
    BOOST_CHECK( 42 == link );
}

BOOST_AUTO_TEST_CASE( model_follows_link_to_read_boolean_value )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    core::Model link;
    link.Link( child );
    child = true;
    BOOST_CHECK( link );
}

BOOST_AUTO_TEST_CASE( model_cannot_modify_linked_value )
{
    core::Model model;
    model[ "child/sub" ];
    core::Model& child = model[ "child" ];
    core::Model link;
    link.Link( child );
    BOOST_CHECK_THROW( link = 42, std::exception );
    const_cast< const core::Model& >( link )[ "sub" ];
    BOOST_CHECK_THROW( link[ "sub" ], std::exception );
}

BOOST_AUTO_TEST_CASE( model_cannot_clear_a_link )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    child.AddElement();
    core::Model link;
    link.Link( child );
    BOOST_REQUIRE_EQUAL( 1u, link.GetSize() );
    BOOST_CHECK_THROW( link.Clear(), std::exception );
    BOOST_CHECK_EQUAL( 1u, link.GetSize() );
}

BOOST_AUTO_TEST_CASE( model_can_remove_a_link )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    core::Model& link = model[ "link" ];
    link.Link( child );
    link.Remove();
    BOOST_CHECK_THROW( const_cast< const core::Model& >( model )[ "link" ], std::exception );
}

BOOST_AUTO_TEST_CASE( model_copies_link )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    child = 42;
    core::Model& link = model[ "link" ];
    link.Link( child );
    core::Model copy;
    copy = model;
    child = 43;
    copy[ "child" ] = 17;
    BOOST_CHECK( 43 == copy[ "link" ] );
    BOOST_CHECK( 43 == link );
}

BOOST_AUTO_TEST_CASE( model_provides_link_context )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    core::Model link;
    link.Link( child );
    child[ "sub-child" ];
    BOOST_CHECK_EQUAL( "/", link.Context() );
    BOOST_CHECK_EQUAL( "/child/sub-child", const_cast< const core::Model& >( link )["sub-child"].Context() );
}

BOOST_AUTO_TEST_CASE( model_visitor_follows_link )
{
    core::Model model;
    core::Model& child = model[ "child" ];
    child = 42;
    core::Model link;
    link.Link( child );
    core::MockModelVisitor visitor;
    MOCK_EXPECT( visitor.VisitInt64 ).once().with( 42 );
    link.Accept( visitor );
}
