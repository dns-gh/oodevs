// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "xeuseudeu_test_pch.h"
#include "xeuseudeu/CopyModelVisitor.h"
#include "xeuseudeu/Model.h"
#include "xeuseudeu/Tools.h"
#include <core/Model.h>
#include <core/MakeModel.h>

using namespace sword::test;

BOOST_AUTO_TEST_CASE( an_empty_model_matches_an_empty_constraint )
{
    Model model;
    core::Model actual;
    BOOST_CHECK( model.Check( actual ) );
}

BOOST_AUTO_TEST_CASE( an_empty_model_fails_to_match_a_value_constraint )
{
    Model model;
    model = 42;
    core::Model actual;
    BOOST_CHECK( ! model.Check( actual ) );
}

BOOST_AUTO_TEST_CASE( a_value_model_fails_to_match_an_empty_constraint )
{
    Model model;
    core::Model actual;
    actual = 42;
    BOOST_CHECK( ! model.Check( actual ) );
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_number_values )
{
    Model model;
    model = 42;
    {
        core::Model actual;
        actual = 42;
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual = 43;
        BOOST_CHECK( ! model.Check( actual ) );
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
        core::Model actual;
        actual = false;
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual = true;
        BOOST_CHECK( ! model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_string_values )
{
    Model model;
    model = "some string";
    {
        core::Model actual;
        actual = "some string";
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual = "some other string";
        BOOST_CHECK( ! model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_inner_constraints_on_values )
{
    Model model;
    model = mock::any;
    core::Model actual;
    actual = 42;
    BOOST_CHECK( model.Check( actual ) );
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_string_child_nodes )
{
    Model model;
    model[ "key" ];
    {
        core::Model actual;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key" ] = 42;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key" ][ "sub-key" ];
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key" ];
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key" ];
        actual[ "key 2" ];
        BOOST_CHECK( ! model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_accepts_integer_child_nodes )
{
    Model model;
    model[ 3 ];
    {
        core::Model actual;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ 3 ] = 42;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ 3 ][ 7 ];
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ 3 ];
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ 3 ];
        actual[ 4 ];
        BOOST_CHECK( ! model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_with_any_constraint_accepts_every_valid_model )
{
    Model model;
    model = mock::any;
    {
        core::Model actual;
        actual[ "key" ];
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.AddElement();
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual = 42;
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.SetData( boost::make_shared< core::UserData< int > >( 42 ) );
        BOOST_CHECK( model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_constraint_splits_child_node_path )
{
    Model model;
    model[ "key/sub-key" ];
    {
        core::Model actual;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key" ];
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key/sub-key" ];
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key/sub-key" ] = 42;
        BOOST_CHECK( ! model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_values_on_child_nodes )
{
    Model model;
    model[ "key" ] = mock::any;
    {
        core::Model actual;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual[ "key" ] = "something else";
        BOOST_CHECK( model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_element_nodes )
{
    Model model;
    model.AddElement();
    {
        core::Model actual;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.AddElement() = 42;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.AddElement().AddElement();
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.AddElement();
        BOOST_CHECK( model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.AddElement();
        actual.AddElement();
        BOOST_CHECK( ! model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_accepts_user_data )
{
    Model model;
    boost::shared_ptr< core::UserData_ABC > data = boost::make_shared< core::UserData< int > >( 42 );
    model.SetData( data );
    {
        core::Model actual;
        BOOST_CHECK( ! model.Check( actual ) );
    }
    {
        core::Model actual;
        actual.SetData( data );
        BOOST_CHECK( model.Check( actual ) );
    }
}

BOOST_AUTO_TEST_CASE( a_model_does_not_accept_empty_user_data )
{
    Model model;
    boost::shared_ptr< core::UserData_ABC > data;
    BOOST_CHECK_THROW( model.SetData( data ), std::exception );
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

BOOST_AUTO_TEST_CASE( test_copy_visitor )
{
    core::Model original;
    original  = core::MakeModel( "root/namedchildren/child1/int", -42 )
                               ( "root/namedchildren/child1/uint", static_cast< uint64_t >( 42 ) )
                               ( "root/namedchildren/child1/double", 13.37 )
                               ( "root/namedchildren/child1/string", "text" )
                               ( "root/namedchildren/child2", core::MakeModel() )
                               ( "root/identifiedchildren", core::MakeModel( 1, "text" )
                                                                           ( 2, 13.37 ) )
                               ( "root/elements", core::MakeModel()
                                                [ core::MakeModel( "Element1" ) ]
                                                [ core::MakeModel( 2 ) ] );
    Model copy;
    CopyModelVisitor visitor( copy );
    original.Accept( visitor );
    BOOST_CHECK_EQUAL( copy, original );
}
