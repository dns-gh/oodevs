// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "xeuseudeu_test_pch.h"
#include "xeuseudeu/ModeltoXml.h"
#include "xeuseudeu/XmlToModel.h"
#include "core/MakeModel.h"
#include "core/Model.h"
#include "xeuseudeu/CopyModelVisitor.h"
#include "xeuseudeu/MakeModel.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    void CompareModel( const core::Model& lhs, const core::Model& rhs )
    {
        sword::test::Model newLhs;
        sword::test::CopyModelVisitor visitor( newLhs );
        lhs.Accept( visitor );
        BOOST_CHECK_EQUAL( newLhs, rhs );
    }
    void Check( const core::Model& model, const std::string& xml, const std::string& xsd = "test" )
    {
        const core::Model& schema = model[ "schemas" ][ xsd ];
        const core::Model& expectedData = model[ "data" ];

        core::Model actualData;
        xml::xistringstream xis( xml );
        xsd::XmlToModel( schema, xis, actualData );
        CompareModel( expectedData, actualData );
    }
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_restriction )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleRestriction" )
                           ( "schemas/test/types/SimpleRestriction/type", "simple" )
                           ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                           ( "data/rootnode/value", 0 );

    const std::string xml = "<rootnode>false</rootnode>";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_string_enumeration )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleStringEnumeration" )
                           ( "schemas/test/types/SimpleStringEnumeration/type", "simple" )
                           ( "schemas/test/types/SimpleStringEnumeration/restriction/base", "xs:string" )
                           ( "schemas/test/types/SimpleStringEnumeration/restriction/enumeration", core::MakeModel()
                                                                                                 [ core::MakeModel( "A" ) ]
                                                                                                 [ core::MakeModel( "B" ) ] )
                           ( "data/rootnode/value", 1 );

    const std::string xml = "<rootnode>B</rootnode>";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_string_restrictions )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleString" )
                           ( "schemas/test/types/SimpleString/type", "simple" )
                           ( "schemas/test/types/SimpleString/restriction/base", "xs:string" )
                           ( "schemas/test/types/SimpleString/restriction/pattern", "[A-Z]*" )
                           ( "schemas/test/types/SimpleString/restriction/minLength", 4 )
                           ( "schemas/test/types/SimpleString/restriction/maxLength", 6 )
                           ( "data/rootnode/value", "ABCDE" );

    const std::string xml = "<rootnode>ABCDE</rootnode>";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_integer_restrictions )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleInteger" )
                           ( "schemas/test/types/SimpleInteger/type", "simple" )
                           ( "schemas/test/types/SimpleInteger/restriction/base", "xs:integer" )
                           ( "schemas/test/types/SimpleInteger/restriction/minInclusive", 4 )
                           ( "schemas/test/types/SimpleInteger/restriction/maxExclusive", 6 )
                           ( "data/rootnode/value", 5 );

    const std::string xml = "<rootnode>5</rootnode>";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_restriction_of_another_simple_type )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "B" )
                           ( "schemas/test/types/A/type", "simple" )
                           ( "schemas/test/types/A/restriction/base", "xs:integer" )
                           ( "schemas/test/types/B/type", "simple" )
                           ( "schemas/test/types/B/restriction/base", "A" )
                           ( "data/rootnode/value", 5 );

    const std::string xml = "<rootnode>5</rootnode>";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_union )
{
    {
        core::Model model;
        model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleUnion" )
                               ( "schemas/test/types/SimpleUnion/type", "simple" )
                               ( "schemas/test/types/SimpleUnion/union", core::MakeModel()
                                                                       [ core::MakeModel( "xs:integer" ) ]
                                                                       [ core::MakeModel( "xs:string" ) ] )
                               ( "data/rootnode/string-value", "ABCD" );
        const std::string xml = "<rootnode>ABCD</rootnode>";
        Check( model, xml );
    }
    {
        core::Model model;
        model = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleUnion" )
                               ( "schemas/test/types/SimpleUnion/type", "simple" )
                               ( "schemas/test/types/SimpleUnion/union", core::MakeModel()
                                                                       [ core::MakeModel( "xs:integer" ) ]
                                                                       [ core::MakeModel( "xs:string" ) ] )
                               ( "data/rootnode/int32-value", 42 );
        const std::string xml = "<rootnode>42</rootnode>";
        Check( model, xml );
    }
}

BOOST_AUTO_TEST_CASE( serialize_xml_simple_list_of_simple_type )
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

    const std::string xml = "<rootnode>true false true</rootnode>";
    BOOST_TODO;
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_with_attribute )
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

    const std::string xml = "<rootnode A=\"A value\" B=\"42\" C=\"C value\" D=\"42\" E=\"fixed value\" F=\"42\"/>";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_sequence )
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

    const std::string xml = "<rootnode>\n"
                                    "  <simple>false</simple>\n"
                                    "  <occurs>true</occurs>\n"
                                    "  <occurs>false</occurs>\n"
                                    "  <occurs>true</occurs>\n"
                                    "</rootnode>\n";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_choice )
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

        const std::string xml = "<rootnode>\n"
                                        "  <simple>false</simple>\n"
                                        "</rootnode>\n";
        Check( model, xml );
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

        const std::string xml = "<rootnode>\n"
                                        "  <occurs>true</occurs>\n"
                                        "  <occurs>false</occurs>\n"
                                        "  <occurs>true</occurs>\n"
                                        "</rootnode>\n";
        Check( model, xml );
    }
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_all )
{
    core::Model model;
    model = core::MakeModel( "schemas/test/elements/rootnode/type", "ComplexAllElements" )
                           ( "schemas/test/types/SimpleRestriction/type", "simple" )
                           ( "schemas/test/types/SimpleRestriction/restriction/base", "xs:boolean" )
                           ( "schemas/test/types/ComplexAllElements/type", "complex" )
                           ( "schemas/test/types/ComplexAllElements/all/simple/type", "SimpleRestriction" )
                           ( "schemas/test/types/ComplexAllElements/all/occurs", core::MakeModel( "type", "SimpleRestriction" )
                                                                                                     ( "minOccurs", 0 )
                                                                                                     ( "maxOccurs", 1 ) )
                           ( "data/rootnode/elements/simple/value", 0 )
                           ( "data/rootnode/elements/occurs/value", 1 );

    const std::string xml = "<rootnode>\n"
                                    "  <simple>false</simple>\n"
                                    "  <occurs>true</occurs>\n"
                                    "</rootnode>\n";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_with_key_constraints )
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
                           ( "schemas/test/types/ComplexWithKeyConstraints/type", "complex" )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list", core::MakeModel( "type", "ComplexWithKeysList" )
                                                                                                             ( "minOccurs", 1 )
                                                                                                             ( "maxOccurs", 1 ) )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list/constraints/a-key/type", "key" )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list/constraints/a-key/selector", "occurs" )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list/constraints/a-key/field", "@A" )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list/constraints/b-key/type", "key" )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list/constraints/b-key/selector", "occurs" )
                           ( "schemas/test/types/ComplexWithKeyConstraints/all/occurs-list/constraints/b-key/field", "@B" )
                           ( "data/rootnode/elements/occurs-list/elements/occurs/list", core::MakeModel()
                                                                                      [ core::MakeModel( "attributes/A", "first key" )
                                                                                                       ( "attributes/B", 42 ) ]
                                                                                      [ core::MakeModel( "attributes/A", "second key" )
                                                                                                       ( "attributes/B", 1337 ) ] );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/a-key/first key/" ] .Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 0 ) );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/a-key/second key/" ].Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 1 ) );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/b-key/" ][ 42 ]     .Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 0 ) );
    model[ "data/rootnode/elements/occurs-list/elements/occurs/b-key/" ][ 1337 ]   .Link( model[ "data/rootnode/elements/occurs-list/elements/occurs/list" ].GetElement( 1 ) );

    const std::string xml = "<rootnode>\n"
                                    "  <occurs-list>\n"
                                    "     <occurs A=\"first key\" B=\"42\"/>\n"
                                    "     <occurs A=\"second key\" B=\"1337\"/>\n"
                                    "  </occurs-list>\n"
                                    "</rootnode>\n";
    Check( model, xml );
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_with_unique_constraints )
{
    BOOST_TODO;
}

BOOST_AUTO_TEST_CASE( serialize_xml_complex_with_keyref_constraints )
{
    BOOST_TODO;
}
