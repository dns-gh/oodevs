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

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_child_nodes )
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

BOOST_AUTO_TEST_CASE( a_model_resets_user_data )
{
    Model model;
    model.SetData( boost::make_shared< core::UserData< int > >( 42 ) );
    model.SetData( boost::make_shared< core::UserData< void* > >( static_cast< void* >( 0 ) ) );
    {
        core::Model expected;
        BOOST_CHECK( model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_empty_user_data )
{
    Model model;
    boost::shared_ptr< core::UserData_ABC > data;
    model.SetData( data );
    {
        core::Model expected;
        BOOST_CHECK( model.Check( expected ) );
    }
    {
        core::Model expected;
        expected.SetData( data );
        BOOST_CHECK( model.Check( expected ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_is_printable )
{
    Model model;
    model = 42;
    model[ "child" ];
    model[ 0u ];
    BOOST_CHECK( ! boost::lexical_cast< std::string >( model ).empty() );
}
