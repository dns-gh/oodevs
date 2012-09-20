// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "module_tester_test_pch.h"
#include "module_tester/Model.h"
#include <core/Model.h>

using namespace sword::test;

BOOST_AUTO_TEST_CASE( an_empty_model_matches_an_empty_constraint )
{
    Model model;
    core::Model expected;
    BOOST_CHECK( model.Check( expected ) );
}

BOOST_AUTO_TEST_CASE( an_empty_model_fails_to_match_a_value_constraint )
{
    Model model;
    model = 42;
    core::Model expected;
    BOOST_CHECK( ! model.Check( expected ) );
}

BOOST_AUTO_TEST_CASE( a_value_model_fails_to_match_an_empty_constraint )
{
    Model model;
    core::Model expected;
    expected = 42;
    BOOST_CHECK( ! model.Check( expected ) );
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_number_values )
{
    Model model;
    model = 42;
    {
        core::Model expected;
        expected = 42;
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected = 43;
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_all_number_types )
{
    Model model;
    model = 42;
    model = 42u;
    model = 42.;
    model = 42.f;
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_boolean_values )
{
    Model model;
    model = false;
    {
        core::Model expected;
        expected = false;
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected = true;
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_string_values )
{
    Model model;
    model = "some string";
    {
        core::Model expected;
        expected = "some string";
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected = "some other string";
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_inner_constraints_on_values )
{
    Model model;
    model = mock::any;
    core::Model expected;
    expected = 42;
    BOOST_CHECK( model.Check( expected ) );
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_string_child_nodes )
{
    Model model;
    model[ "key" ];
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key" ] = 42;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key" ][ "sub-key" ];
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key" ];
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key" ];
        expected[ "key 2" ];
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_integer_child_nodes )
{
    Model model;
    model[ 3 ];
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ 3 ] = 42;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ 3 ][ 7 ];
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ 3 ];
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ 3 ];
        expected[ 4 ];
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_with_any_constraint_accepts_every_valid_model )
{
    Model model;
    model = mock::any;
    {
        core::Model expected;
        expected[ "key" ];
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.AddElement();
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected = 42;
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.SetData( boost::make_shared< core::UserData< int > >( 42 ) );
        BOOST_CHECK( model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_splits_child_node_path )
{
    Model model;
    model[ "key/sub-key" ];
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key" ];
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key/sub-key" ];
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key/sub-key" ] = 42;
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_values_on_child_nodes )
{
    Model model;
    model[ "key" ] = mock::any;
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected[ "key" ] = "something else";
        BOOST_CHECK( model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_element_nodes )
{
    Model model;
    model.AddElement();
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.AddElement() = 42;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.AddElement().AddElement();
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.AddElement();
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.AddElement();
        expected.AddElement();
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_user_data )
{
    Model model;
    boost::shared_ptr< core::UserData_ABC > data = boost::make_shared< core::UserData< int > >( 42 );
    model.SetData( data );
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.SetData( data );
        BOOST_CHECK( model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_does_not_accept_empty_user_data )
{
    Model model;
    boost::shared_ptr< core::UserData_ABC > data;
    BOOST_CHECK_THROW( model.SetData( data ), std::exception );
}

BOOST_AUTO_TEST_CASE( a_model_can_be_marked_for_remove )
{
    Model model;
    model.MarkForRemove();
    {
        core::Model expected;
        expected.MarkForRemove();
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        BOOST_CHECK( ! model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_is_printable )
{
    Model model;
    Model& element = model.AddElement();
    element[ "test" ] = 42;
    element[ "too" ] = 123;
    Model& element2 = model.AddElement();
    element2[ "test" ] = 43;
    element2[ "too" ] = mock::any;
    BOOST_CHECK_EQUAL( "[\n"
                        "  {\n"
                        "    test: 42,\n"
                        "    too: 123\n"
                        "  },\n"
                        "  {\n"
                        "    test: 43,\n"
                        "    too: any\n"
                        "  }\n"
                        "]", boost::lexical_cast< std::string >( model ) );
}
