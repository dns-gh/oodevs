// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "core_test_pch.h"
#include "core/ModelPrinter.h"
#include "core/Model.h"
#include <boost/lexical_cast.hpp>
#include <sstream>

namespace
{
    std::string Print( const core::Model& model )
    {
        std::stringstream s;
        core::ModelPrinter( model, s, true, 0 );
        return s.str();
    }
}

BOOST_AUTO_TEST_CASE( model_is_printable )
{
    {
        core::Model model;
        model = 3;
        BOOST_CHECK_EQUAL( "3", Print( model ) );
    }
    {
        core::Model model;
        model[ "test" ] = 42;
        BOOST_CHECK_EQUAL( "{ \"test\": 42 }", Print( model ) );
    }
    {
        core::Model model;
        model[ "test" ];
        BOOST_CHECK_EQUAL( "{ \"test\":  }", Print( model ) );
    }
    {
        core::Model model;
        model[ "test/sub" ] = 42;
        BOOST_CHECK_EQUAL( "{ \"test\": { \"sub\": 42 } }", Print( model ) );
    }
    {
        core::Model model;
        model[ 17 ] = 42;
        model[ 2 ] = "pipo";
        BOOST_CHECK_EQUAL( "{ 2: \"pipo\", 17: 42 }", Print( model ) );
    }
    {
        core::Model model;
        model.AddElement() = 1;
        BOOST_CHECK_EQUAL( "[ 1 ]", Print( model ) );
    }
    {
        core::Model model;
        model.AddElement() = 1;
        model.AddElement() = "2";
        BOOST_CHECK_EQUAL( "[ 1, \"2\" ]", Print( model ) );
    }
    {
        core::Model model;
        model.AddElement() = 1;
        model.AddElement() = "2";
        BOOST_CHECK_EQUAL( "[ 1, \"2\" ]", Print( model ) );
    }
    {
        core::Model model;
        model.AddElement() = 1;
        model.AddElement() = "2";
        BOOST_CHECK_EQUAL( "[ 1, \"2\" ]", Print( model ) );
    }
    {
        core::Model model;
        model.AddElement() = 1;
        model.AddElement() = "2";
        BOOST_CHECK_EQUAL( "[ 1, \"2\" ]", Print( model ) );
    }
    {
        core::Model model;
        const int data = 0;
        model.SetUserData( &data );
        BOOST_CHECK_EQUAL( boost::lexical_cast< std::string >( &data ), Print( model ) );
    }
}

BOOST_AUTO_TEST_CASE( quotes_around_strings_are_optional )
{
    {
        core::Model model;
        model[ "test" ] = 42;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 0 );
        BOOST_CHECK_EQUAL( "{ test: 42 }", s.str() );
    }
    {
        core::Model model;
        model = "some string";
        std::stringstream s;
        core::ModelPrinter( model, s, false, 0 );
        BOOST_CHECK_EQUAL( "some string", s.str() );
    }
    {
        core::Model model;
        model[ "test" ] = "some string";
        std::stringstream s;
        core::ModelPrinter( model, s, false, 0 );
        BOOST_CHECK_EQUAL( "{ test: some string }", s.str() );
    }
    {
        core::Model model;
        model.AddElement() = "some string";
        std::stringstream s;
        core::ModelPrinter( model, s, false, 0 );
        BOOST_CHECK_EQUAL( "[ some string ]", s.str() );
    }
}

BOOST_AUTO_TEST_CASE( printed_model_output_can_be_indented )
{
    {
        core::Model model;
        model = 42;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "42", s.str() );
    }
    {
        core::Model model;
        model[ "test" ] = 42;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "{\n"
                           "  test: 42\n"
                           "}", s.str() );
    }
    {
        core::Model model;
        model[ "test" ][ "too" ] = 42;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "{\n"
                           "  test: \n"
                           "  {\n"
                           "    too: 42\n"
                           "  }\n"
                           "}", s.str() );
    }
    {
        core::Model model;
        model[ "test" ] = 42;
        model[ "too" ] = 123;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "{\n"
                           "  test: 42,\n"
                           "  too: 123\n"
                           "}", s.str() );
    }
    {
        core::Model model;
        model.AddElement() = 42;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "[\n"
                           "  42\n"
                           "]", s.str() );
    }
    {
        core::Model model;
        model.AddElement().AddElement() = 42;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "[\n"
                           "  [\n"
                           "    42\n"
                           "  ]\n"
                           "]", s.str() );
    }
    {
        core::Model model;
        model.AddElement() = 42;
        model.AddElement() = 123;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "[\n"
                           "  42,\n"
                           "  123\n"
                           "]", s.str() );
    }
    {
        core::Model model;
        core::Model& element = model.AddElement();
        element[ "test" ] = 42;
        element[ "too" ] = 123;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "[\n"
                           "  {\n"
                           "    test: 42,\n"
                           "    too: 123\n"
                           "  }\n"
                           "]", s.str() );
    }
    {
        core::Model model;
        core::Model& element = model.AddElement();
        element[ "test" ] = 42;
        element[ "too" ] = 123;
        core::Model& element2 = model.AddElement();
        element2[ "test" ] = 43;
        element2[ "too" ] = 654;
        std::stringstream s;
        core::ModelPrinter( model, s, false, 2 );
        BOOST_CHECK_EQUAL( "[\n"
                           "  {\n"
                           "    test: 42,\n"
                           "    too: 123\n"
                           "  },\n"
                           "  {\n"
                           "    test: 43,\n"
                           "    too: 654\n"
                           "  }\n"
                           "]", s.str() );
    }
}
