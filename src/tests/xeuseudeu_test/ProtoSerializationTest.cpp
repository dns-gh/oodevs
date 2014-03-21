// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "xeuseudeu_test_pch.h"

#include "xeuseudeu/ModeltoProto.h"
#include "xeuseudeu/ProtoToModel.h"

#include "core/MakeModel.h"
#include "core/Model.h"
#include "xeuseudeu/CopyModelVisitor.h"
#include "xeuseudeu/MakeModel.h"

#pragma warning( push, 0 )
#include "proto/included.pb.h"
#include "proto/test.pb.h"
#pragma warning( pop )

using namespace google::protobuf;

namespace
{
    void CompareModel( const core::Model& lhs, const core::Model& rhs )
    {
        sword::test::Model newLhs;
        sword::test::CopyModelVisitor visitor( newLhs );
        lhs.Accept( visitor );
        BOOST_CHECK_EQUAL( newLhs, rhs );
    }
    template< typename T >
    void CheckProtoToModel( const core::Model& schema, const core::Model& expectedData, const T& expectedMessage )
    {
        core::Model actualData;
        xsd::ProtoToModel( schema, expectedMessage, actualData );
        CompareModel( expectedData, actualData );
    }
    template< typename T >
    void Check( const core::Model& model, const T& expectedMessage, const std::string& xsd = "test", const std::string& rootnode = "rootnode" )
    {
        const core::Model& expectedData = model[ "data" ][ rootnode ];

        CheckProtoToModel( model[ "schemas" ][ xsd ], expectedData, expectedMessage );

        T actualMessage;
        xsd::ModelToProto( expectedData, actualMessage );
        BOOST_CHECK_EQUAL( expectedMessage.DebugString(), actualMessage.DebugString() );
    }
    void CheckEnum( const core::Model& model, int expectedValue, int defaultValue, const std::string& xsd = "test", const std::string& rootnode = "rootnode" )
    {
        const core::Model& expectedData = model[ "data" ][ rootnode ];

        CheckProtoToModel( model[ "schemas" ][ xsd ], expectedData, expectedValue );

        int actualValue = defaultValue;
        xsd::ModelToProto( expectedData, actualValue );
        BOOST_CHECK_EQUAL( expectedValue, actualValue );
    }
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_restriction )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleRestriction" )
                           ( "schemas/test/types/SimpleRestriction/type", "simple" )
                           ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                           ( "data/rootnode/value", 0 );

    test::SimpleRestriction expectedMessage;
    expectedMessage.set_value( false );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_string_enumeration )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleStringEnumeration" )
                           ( "schemas/test/types/SimpleStringEnumeration/type", "simple" )
                           ( "schemas/test/types/SimpleStringEnumeration/restriction/base", "xs:string" )
                           ( "schemas/test/types/SimpleStringEnumeration/restriction/enumeration", core::MakeModel()
                                                                                                 [ core::MakeModel( "A" ) ]
                                                                                                 [ core::MakeModel( "B" ) ] )
                           ( "data/rootnode/value", 1 );
    test::SimpleStringEnumeration expectedMessage = test::B;
    CheckEnum( model, expectedMessage, test::SimpleStringEnumeration_MIN );
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_string_restrictions )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleString" )
                           ( "schemas/test/types/SimpleString/type", "simple" )
                           ( "schemas/test/types/SimpleString/restriction/base", "xs:string" )
                           ( "schemas/test/types/SimpleString/restriction/pattern", "[A-Z]*" )
                           ( "schemas/test/types/SimpleString/restriction/minLength", 4 )
                           ( "schemas/test/types/SimpleString/restriction/maxLength", 6 )
                           ( "data/rootnode/value", "ABCDE" );
    test::SimpleString expectedMessage;
    expectedMessage.set_value( "ABCDE" );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_integer_restrictions )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleInteger" )
                           ( "schemas/test/types/SimpleInteger/type", "simple" )
                           ( "schemas/test/types/SimpleInteger/restriction/base", "xs:integer" )
                           ( "schemas/test/types/SimpleInteger/restriction/minInclusive", 4 )
                           ( "schemas/test/types/SimpleInteger/restriction/maxExclusive", 6 )
                           ( "data/rootnode/value", 5 );
    test::SimpleInteger expectedMessage;
    expectedMessage.set_value( 5 );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_union )
{
    {
        core::Model model;
        model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleUnion" )
                               ( "schemas/test/types/SimpleUnion/type", "simple" )
                               ( "schemas/test/types/SimpleUnion/union", core::MakeModel()
                                                                       [ core::MakeModel( "xs:string" ) ]
                                                                       [ core::MakeModel( "xs:integer" ) ] )
                               ( "data/rootnode/string-value", "ABCD" );
        test::SimpleUnion expectedMessage;
        expectedMessage.set_type( test::SimpleUnion::UNION_string );
        expectedMessage.set_string_value( "ABCD" );
        Check( model, expectedMessage );
    }
    {
        core::Model model;
        model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleUnion" )
                               ( "schemas/test/types/SimpleUnion/type", "simple" )
                               ( "schemas/test/types/SimpleUnion/union", core::MakeModel()
                                                                       [ core::MakeModel( "xs:string" ) ]
                                                                       [ core::MakeModel( "xs:integer" ) ] )
                               ( "data/rootnode/int32-value", 42 );
        test::SimpleUnion expectedMessage;
        expectedMessage.set_type( test::SimpleUnion::UNION_int32 );
        expectedMessage.set_int32_value( 42 );
        Check( model, expectedMessage );
    }
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_list )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleList" )
                           ( "schemas/test/types/SimpleList/type", "simple" )
                           ( "schemas/test/types/SimpleList/list", "xs:boolean" )
                           ( "data/rootnode/values", core::MakeModel()
                                                   [ core::MakeModel( 1 ) ]
                                                   [ core::MakeModel( 0 ) ]
                                                   [ core::MakeModel( 1 ) ] );
    test::SimpleList expectedMessage;
    expectedMessage.add_values( true );
    expectedMessage.add_values( false );
    expectedMessage.add_values( true );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_simple_list_of_simple_type )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleListOfSimpleType" )
                           ( "schemas/test/types/SimpleRestriction/type", "simple" )
                           ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                           ( "schemas/test/types/SimpleListOfSimpleType/type", "simple" )
                           ( "schemas/test/types/SimpleListOfSimpleType/list", "SimpleRestriction" )
                           ( "data/rootnode/values", core::MakeModel()
                                                   [ core::MakeModel( "value", 1 ) ]
                                                   [ core::MakeModel( "value", 0 ) ]
                                                   [ core::MakeModel( "value", 1 ) ] );
    test::SimpleListOfSimpleType expectedMessage;
    expectedMessage.add_values()->set_value( true );
    expectedMessage.add_values()->set_value( false );
    expectedMessage.add_values()->set_value( true );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_with_attribute )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexWithAttributes" )
                           ( "schemas/test/types/ComplexWithAttributes/type", "complex" )
                           ( "schemas/test/types/ComplexWithAttributes/attributes/A", core::MakeModel( "type", "xs:string" )
                                                                                                     ( "use", "required" ) )
                           ( "schemas/test/types/ComplexWithAttributes/attributes/B", core::MakeModel( "type", "xs:integer" )
                                                                                                     ( "use", "optional" ) )
                           ( "schemas/test/types/ComplexWithAttributes/attributes/C", core::MakeModel( "type", "xs:string" )
                                                                                                     ( "default", "default value" ) )
                           ( "schemas/test/types/ComplexWithAttributes/attributes/D", core::MakeModel( "type", "xs:integer" )
                                                                                                     ( "default", 42 ) )
                           ( "schemas/test/types/ComplexWithAttributes/attributes/E", core::MakeModel( "type", "xs:string" )
                                                                                                     ( "fixed", "fixed value" ) )
                           ( "schemas/test/types/ComplexWithAttributes/attributes/F", core::MakeModel( "type", "xs:integer" )
                                                                                                     ( "fixed", 42 ) )
                           ( "data/rootnode/attributes/A", "A value" )
                           ( "data/rootnode/attributes/B", 42 )
                           ( "data/rootnode/attributes/C", "C value" )
                           ( "data/rootnode/attributes/D", 42 )
                           ( "data/rootnode/attributes/E", "fixed value" )
                           ( "data/rootnode/attributes/F", 42 );
    test::ComplexWithAttributes expectedMessage;
    expectedMessage.set_a( "A value" );
    expectedMessage.set_b( 42 );
    expectedMessage.set_c( "C value" );
    expectedMessage.set_d( 42 );
    expectedMessage.set_e( "fixed value" );
    expectedMessage.set_f( 42 );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_sequence )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexSequenceElements" )
                           ( "schemas/test/types/SimpleRestriction/type", "simple" )
                           ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                           ( "schemas/test/types/ComplexSequenceElements/type", "complex" )
                           ( "schemas/test/types/ComplexSequenceElements/sequence/simple/type", "SimpleRestriction" )
                           ( "schemas/test/types/ComplexSequenceElements/sequence/occurs", core::MakeModel( "type", "SimpleRestriction" )
                                                                                                          ( "minOccurs", 0 )
                                                                                                          ( "maxOccurs", -1 ) )
                           ( "data/rootnode/elements/simple/value", 0 )
                           ( "data/rootnode/elements/occurs/list", core::MakeModel()
                                                                 [ core::MakeModel( "value", 1 ) ]
                                                                 [ core::MakeModel( "value", 0 ) ]
                                                                 [ core::MakeModel( "value", 1 ) ] );
    test::ComplexSequenceElements expectedMessage;
    expectedMessage.mutable_simple()->set_value( false );
    expectedMessage.add_occurs()->set_value( true );
    expectedMessage.add_occurs()->set_value( false );
    expectedMessage.add_occurs()->set_value( true );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_choice )
{
    {
        core::Model model;
        model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexChoiceElements" )
                               ( "schemas/test/types/SimpleRestriction/type", "simple" )
                               ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                               ( "schemas/test/types/ComplexChoiceElements/type", "complex" )
                               ( "schemas/test/types/ComplexChoiceElements/choice/simple/type", "SimpleRestriction" )
                               ( "schemas/test/types/ComplexChoiceElements/choice/occurs", core::MakeModel( "type", "SimpleRestriction" )
                                                                                                          ( "minOccurs", 0 )
                                                                                                          ( "maxOccurs", -1 ) )
                               ( "data/rootnode/elements/simple/value", 0 );
        test::ComplexChoiceElements expectedMessage;
        expectedMessage.set_type( test::ComplexChoiceElements::CHOICE_simple );
        expectedMessage.mutable_simple()->set_value( false );
        Check( model, expectedMessage );
    }
    {
        core::Model model;
        model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexChoiceElements" )
                               ( "schemas/test/types/SimpleRestriction/type", "simple" )
                               ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                               ( "schemas/test/types/ComplexChoiceElements/type", "complex" )
                               ( "schemas/test/types/ComplexChoiceElements/choice/simple/type", "SimpleRestriction" )
                               ( "schemas/test/types/ComplexChoiceElements/choice/occurs", core::MakeModel( "type", "SimpleRestriction" )
                                                                                                          ( "minOccurs", 0 )
                                                                                                          ( "maxOccurs", -1 ) )
                               ( "data/rootnode/elements/occurs/list", core::MakeModel()
                                                                     [ core::MakeModel( "value", 1 ) ]
                                                                     [ core::MakeModel( "value", 0 ) ]
                                                                     [ core::MakeModel( "value", 1 ) ] );
        test::ComplexChoiceElements expectedMessage;
        expectedMessage.set_type( test::ComplexChoiceElements::CHOICE_occurs );
        expectedMessage.add_occurs()->set_value( true );
        expectedMessage.add_occurs()->set_value( false );
        expectedMessage.add_occurs()->set_value( true );
        Check( model, expectedMessage );
    }
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_all )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexAllElements" )
                           ( "schemas/test/types/SimpleRestriction/type", "simple" )
                           ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                           ( "schemas/test/types/ComplexAllElements/type", "complex" )
                           ( "schemas/test/types/ComplexAllElements/sequence/simple/type", "SimpleRestriction" )
                           ( "schemas/test/types/ComplexAllElements/sequence/occurs", core::MakeModel( "type", "SimpleRestriction" )
                                                                                                     ( "minOccurs", 0 )
                                                                                                     ( "maxOccurs", -1 ) )
                           ( "data/rootnode/elements/simple/value", 0 )
                           ( "data/rootnode/elements/occurs/value", 1 );
    test::ComplexAllElements expectedMessage;
    expectedMessage.mutable_simple()->set_value( false );
    expectedMessage.mutable_occurs()->set_value( true );
    Check( model, expectedMessage );
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_with_key_constraints )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexWithKeyConstraints" )
                           ( "schemas/test/types/ComplexWithKeys/type", "complex" )
                           ( "schemas/test/types/ComplexWithKeys/attributes/A", core::MakeModel( "type", "xs:string" )
                                                                                               ( "use", "required" ) )
                           ( "schemas/test/types/ComplexWithKeys/attributes/B", core::MakeModel( "type", "xs:integer" )
                                                                                               ( "use", "required" ) )
                           ( "schemas/test/types/ComplexWithKeysList/type", "complex" )
                           ( "schemas/test/types/ComplexWithKeysList/sequence/occurs", core::MakeModel( "type", "ComplexWithKeys" )
                                                                                                      ( "minOccurs", 0 )
                                                                                                      ( "maxOccurs", -1 ) )
                           ( "schemas/test/types/ComplexWithKeysConstraints/type", "complex" )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list", core::MakeModel( "type", "ComplexWithKeysList" )
                                                                                                             ( "minOccurs", 1 )
                                                                                                             ( "maxOccurs", 1 ) )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list/constraints/a-key/type", "key" )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list/constraints/a-key/selector", "occurs" )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list/constraints/a-key/field", "@A" )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list/constraints/b-key/type", "key" )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list/constraints/b-key/selector", "occurs" )
                           ( "schemas/test/types/ComplexWithKeysConstraints/all/occurs-list/constraints/b-key/field", "@B" )
                           ( "data/rootnode/elements/occurs-list/elements/occurs/list", core::MakeModel()
                                                                                      [ core::MakeModel( "attributes/A", "first key" )
                                                                                                       ( "attributes/B", 42 ) ]
                                                                                      [ core::MakeModel( "attributes/A", "second key" )
                                                                                                       ( "attributes/B", 1337 ) ] );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/a-key/first key/" ] .Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 0 ) );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/a-key/second key/" ].Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 1 ) );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/b-key/" ][ 42 ]     .Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 0 ) );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/b-key/" ][ 1337 ]   .Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 1 ) );

    {
        test::ComplexWithKeysConstraints expectedMessage;
        ::test::ComplexWithKeys* first = expectedMessage.mutable_occurs_list()->add_occurs();
        first->set_a( "first key" );
        first->set_b( 42 );
        ::test::ComplexWithKeys* second = expectedMessage.mutable_occurs_list()->add_occurs();
        second->set_a( "second key" );
        second->set_b( 1337 );
        Check( model, expectedMessage );
    }

    {
        // Negative keys are not handled
        test::ComplexWithKeysConstraints expectedMessage;
        ::test::ComplexWithKeys* first = expectedMessage.mutable_occurs_list()->add_occurs();
        first->set_a( "first key" );
        first->set_b( 42 );
        ::test::ComplexWithKeys* second = expectedMessage.mutable_occurs_list()->add_occurs();
        second->set_a( "second key" );
        second->set_b( -1337 );

        core::Model actualData;
        BOOST_REQUIRE_THROW(
            xsd::ProtoToModel( model[ "schemas/test" ], expectedMessage, actualData ),
            tools::Exception );
    }
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_with_unique_constraints )
{
    BOOST_TODO;
}

BOOST_AUTO_TEST_CASE( serialize_proto_complex_with_keyref_constraints )
{
    BOOST_TODO;
}

BOOST_AUTO_TEST_CASE( shutdown_protobuf_library )
{
    google::protobuf::ShutdownProtobufLibrary();
}
