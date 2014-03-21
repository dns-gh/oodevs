// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "xeuseudeu_test_pch.h"
#include "xeuseudeu/XsdToModel.h"
#include "core/MakeModel.h"
#include "core/Model.h"
#include "xeuseudeu/CopyModelVisitor.h"
#include "xeuseudeu/MakeModel.h"
#include "xeuseudeu/Tools.h"
#include <tools/TemporaryDirectory.h>
#include <tools/StdFileWrapper.h>
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
    void Check( const std::string& grammar, const sword::test::Model& expected )
    {
        tools::TemporaryDirectory dir( "xeuseudeu-", temp_directory );
        tools::Path path = dir.Path() / "test.xsd";
        {
            tools::Ofstream ofs( path );
            ofs << grammar;
        }
        core::Model actual;
        xsd::XsdToModel( path, actual );
        BOOST_CHECK_EQUAL( expected, actual );
    }
}

BOOST_AUTO_TEST_CASE( xsd_is_loaded_with_root_element )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:element name='rootnode' type='xs:string'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "xs:string" )
                                                              ( "schemas/test/types", sword::test::MakeModel() );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_restriction_on_base )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:boolean'/>"
    "   </xs:simpleType>"
    "   <xs:element name='rootnode' type='Simple'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:boolean" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_string_enumeration_restriction )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:string'>"
    "           <xs:enumeration value='A'/>"
    "           <xs:enumeration value='B'/>"
    "       </xs:restriction>"
    "   </xs:simpleType>"
    "   <xs:element name='rootnode' type='Simple'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:string" )
                                                              ( "schemas/test/types/Simple/restriction/enumeration", sword::test::MakeModel()
                                                                                                                   [ sword::test::MakeModel( "A" ) ]
                                                                                                                   [ sword::test::MakeModel( "B" ) ] );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_integer_enumeration_restriction )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:integer'>"
    "           <xs:enumeration value='12'/>"
    "           <xs:enumeration value='42'/>"
    "       </xs:restriction>"
    "   </xs:simpleType>"
    "   <xs:element name='rootnode' type='Simple'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:integer" )
                                                              ( "schemas/test/types/Simple/restriction/enumeration", sword::test::MakeModel()
                                                                                                                   [ sword::test::MakeModel( 12 ) ]
                                                                                                                   [ sword::test::MakeModel( 42 ) ] );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_string_restrictions )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:string'>"
    "           <xs:pattern value='[A-Z]*'/>"
    "           <xs:minLength value='4'/>"
    "           <xs:maxLength value='6'/>"
    "       </xs:restriction>"
    "   </xs:simpleType>"
    "   <xs:element name='rootnode' type='Simple'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:string" )
                                                              ( "schemas/test/types/Simple/restriction/pattern", "[A-Z]*" )
                                                              ( "schemas/test/types/Simple/restriction/minLength", 4 )
                                                              ( "schemas/test/types/Simple/restriction/maxLength", 6 );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_int_restrictions )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:integer'>"
    "           <xs:minInclusive value='4'/>"
    "           <xs:maxExclusive value='6'/>"
    "       </xs:restriction>"
    "   </xs:simpleType>"
    "   <xs:element name='rootnode' type='Simple'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:integer" )
                                                              ( "schemas/test/types/Simple/restriction/minInclusive", 4 )
                                                              ( "schemas/test/types/Simple/restriction/maxExclusive", 6 );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_union )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:union memberTypes='xs:boolean xs:float'/>"
    "   </xs:simpleType>"
    "   <xs:element name='rootnode' type='Simple'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/union", sword::test::MakeModel()
                                                                                                 [ sword::test::MakeModel( "xs:boolean" ) ]
                                                                                                 [ sword::test::MakeModel( "xs:float" ) ] );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_simple_type_with_list )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:simpleType name='Simple'>"
        "       <xs:list itemType='xs:boolean'/>"
        "   </xs:simpleType>"
        "   <xs:element name='rootnode' type='Simple'/>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/list", "xs:boolean" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_complex_type )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:complexType name='Complex'/>"
    "   <xs:element name='rootnode' type='Complex'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Complex/type", "complex" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_complex_type_with_builtin_attributes )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:complexType name='Complex'>"
    "       <xs:attribute name='A' type='xs:string' use='required'/>"
    "       <xs:attribute name='B' type='xs:integer' use='optional'/>"
    "       <xs:attribute name='C' type='xs:string' default='default value'/>"
    "       <xs:attribute name='D' type='xs:integer' default='42'/>"
    "       <xs:attribute name='E' type='xs:string' fixed='fixed value'/>"
    "       <xs:attribute name='F' type='xs:integer' fixed='42'/>"
    "   </xs:complexType>"
    "   <xs:element name='rootnode' type='Complex'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/attributes/A", sword::test::MakeModel( "type", "xs:string" )
                                                                                                                                 ( "use", "required" ) )
                                                              ( "schemas/test/types/Complex/attributes/B", sword::test::MakeModel( "type", "xs:integer" )
                                                                                                                                 ( "use", "optional" ) )
                                                              ( "schemas/test/types/Complex/attributes/C", sword::test::MakeModel( "type", "xs:string" )
                                                                                                                                 ( "default", "default value" ) )
                                                              ( "schemas/test/types/Complex/attributes/D", sword::test::MakeModel( "type", "xs:integer" )
                                                                                                                                 ( "default", 42 ) )
                                                              ( "schemas/test/types/Complex/attributes/E", sword::test::MakeModel( "type", "xs:string" )
                                                                                                                                 ( "fixed", "fixed value" ) )
                                                              ( "schemas/test/types/Complex/attributes/F", sword::test::MakeModel( "type", "xs:integer" )
                                                                                                                                 ( "fixed", 42 ) );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_complex_type_with_a_sequence )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:boolean'/>"
    "   </xs:simpleType>"
    "   <xs:complexType name='Complex'>"
    "       <xs:sequence>"
    "           <xs:element name='simple' type='Simple'/>"
    "           <xs:element name='occurs' type='Simple' minOccurs='0' maxOccurs='unbounded'/>"
    "       </xs:sequence>"
    "   </xs:complexType>"
    "   <xs:element name='rootnode' type='Complex'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:boolean" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/sequence/simple/type", "Simple" )
                                                              ( "schemas/test/types/Complex/sequence/occurs", sword::test::MakeModel( "type", "Simple" )
                                                                                                                                    ( "minOccurs", 0 )
                                                                                                                                    ( "maxOccurs", -1 ) );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_complex_type_with_a_choice )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:boolean'/>"
    "   </xs:simpleType>"
    "   <xs:complexType name='Complex'>"
    "       <xs:choice>"
    "           <xs:element name='simple' type='Simple'/>"
    "           <xs:element name='occurs' type='Simple' minOccurs='0' maxOccurs='unbounded'/>"
    "       </xs:choice>"
    "   </xs:complexType>"
    "   <xs:element name='rootnode' type='Complex'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:boolean" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/choice/simple/type", "Simple" )
                                                              ( "schemas/test/types/Complex/choice/occurs", sword::test::MakeModel( "type", "Simple" )
                                                                                                                                  ( "minOccurs", 0 )
                                                                                                                                  ( "maxOccurs", -1 ) );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( root_element_can_be_a_complex_type_with_all )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:restriction base='xs:boolean'/>"
    "   </xs:simpleType>"
    "   <xs:complexType name='Complex'>"
    "       <xs:all>"
    "           <xs:element name='simple' type='Simple'/>"
    "           <xs:element name='occurs' type='Simple' minOccurs='0' maxOccurs='1'/>"
    "       </xs:all>"
    "   </xs:complexType>"
    "   <xs:element name='rootnode' type='Complex'/>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:boolean" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/all/simple/type", "Simple" )
                                                              ( "schemas/test/types/Complex/all/occurs", sword::test::MakeModel( "type", "Simple" )
                                                                                                                               ( "minOccurs", 0 )
                                                                                                                               ( "maxOccurs", 1 ) );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( xsd_can_be_annotated )
{
    const std::string grammar =
    "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
    "   <xs:annotation>"
    "       <xs:documentation>schema doc</xs:documentation>"
    "   </xs:annotation>"
    "   <xs:simpleType name='Simple'>"
    "       <xs:annotation>"
    "           <xs:documentation>simple type doc</xs:documentation>"
    "       </xs:annotation>"
    "       <xs:restriction base='xs:boolean'/>"
    "   </xs:simpleType>"
    "   <xs:complexType name='Complex'>"
    "       <xs:annotation>"
    "           <xs:documentation>complex type doc</xs:documentation>"
    "       </xs:annotation>"
    "       <xs:attribute name='name' type='xs:string' use='required'>"
    "           <xs:annotation>"
    "               <xs:documentation xml:lang='en'>attribute doc en</xs:documentation>"
    "           </xs:annotation>"
    "       </xs:attribute>"
    "       <xs:sequence>"
    "           <xs:element name='simple' type='Simple'>"
    "               <xs:annotation>"
    "                   <xs:documentation>simple element doc</xs:documentation>"
    "               </xs:annotation>"
    "           </xs:element>"
    "       </xs:sequence>"
    "   </xs:complexType>"
    "   <xs:element name='rootnode' type='Simple'>"
    "       <xs:annotation>"
    "           <xs:documentation xml:lang='en'>root doc en</xs:documentation>"
    "           <xs:documentation xml:lang='fr'>root doc fr</xs:documentation>"
    "       </xs:annotation>"
    "   </xs:element>"
    "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/annotations/default", "schema doc" )
                                                              ( "schemas/test/elements/rootnode/type", "Simple" )
                                                              ( "schemas/test/elements/rootnode/annotations/en", "root doc en" )
                                                              ( "schemas/test/elements/rootnode/annotations/fr", "root doc fr" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:boolean" )
                                                              ( "schemas/test/types/Simple/annotations/default", "simple type doc" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/annotations/default", "complex type doc" )
                                                              ( "schemas/test/types/Complex/sequence/simple/type", "Simple" )
                                                              ( "schemas/test/types/Complex/sequence/simple/annotations/default", "simple element doc" )
                                                              ( "schemas/test/types/Complex/attributes/name/annotations/en", "attribute doc en" )
                                                              ( "schemas/test/types/Complex/attributes/name/type", "xs:string" )
                                                              ( "schemas/test/types/Complex/attributes/name/use", "required" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( element_can_have_key_constraint )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:complexType name='ComplexWithAttribute'>"
        "       <xs:attribute name='A' type='xs:string'/>"
        "   </xs:complexType>"
        "   <xs:complexType name='Complex'>"
        "       <xs:sequence>"
        "           <xs:element name='simple' type='ComplexWithAttribute'>"
        "               <xs:key name='simpleKey'>"
        "                   <xs:selector xpath='.'/>"
        "                   <xs:field xpath='@A'/>"
        "               </xs:key>"
        "           </xs:element>"
        "       </xs:sequence>"
        "   </xs:complexType>"
        "   <xs:element name='rootnode' type='ComplexWithAttribute'>"
        "       <xs:key name='rootKey'>"
        "           <xs:selector xpath='.'/>"
        "           <xs:field xpath='@A'/>"
        "       </xs:key>"
        "   </xs:element>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "ComplexWithAttribute" )
                                                              ( "schemas/test/elements/rootnode/constraints/rootKey/type", "key" )
                                                              ( "schemas/test/elements/rootnode/constraints/rootKey/selector", "." )
                                                              ( "schemas/test/elements/rootnode/constraints/rootKey/field", "@A" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/sequence/simple/type", "ComplexWithAttribute" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKey/type", "key" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKey/selector", "." )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKey/field", "@A" )
                                                              ( "schemas/test/types/ComplexWithAttribute/type", "complex" )
                                                              ( "schemas/test/types/ComplexWithAttribute/attributes/A/type", "xs:string" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( element_can_have_unique_constraint )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:complexType name='ComplexWithAttribute'>"
        "       <xs:attribute name='A' type='xs:string'/>"
        "   </xs:complexType>"
        "   <xs:complexType name='Complex'>"
        "       <xs:sequence>"
        "           <xs:element name='simple' type='ComplexWithAttribute'>"
        "               <xs:unique name='simpleUnique'>"
        "                   <xs:selector xpath='.'/>"
        "                   <xs:field xpath='@A'/>"
        "               </xs:unique>"
        "           </xs:element>"
        "       </xs:sequence>"
        "   </xs:complexType>"
        "   <xs:element name='rootnode' type='Complex'/>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/sequence/simple/type", "ComplexWithAttribute" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleUnique/type", "unique" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleUnique/selector", "." )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleUnique/field", "@A" )
                                                              ( "schemas/test/types/ComplexWithAttribute/type", "complex" )
                                                              ( "schemas/test/types/ComplexWithAttribute/attributes/A/type", "xs:string" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( element_can_have_keyref_constraint )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:complexType name='ComplexWithAttribute'>"
        "       <xs:attribute name='A' type='xs:string'/>"
        "   </xs:complexType>"
        "   <xs:complexType name='Complex'>"
        "       <xs:sequence>"
        "           <xs:element name='simple' type='ComplexWithAttribute'>"
        "               <xs:keyref name='simpleKeyref' refer='otherkey'>"
        "                   <xs:selector xpath='.'/>"
        "                   <xs:field xpath='@A'/>"
        "               </xs:keyref>"
        "           </xs:element>"
        "       </xs:sequence>"
        "   </xs:complexType>"
        "   <xs:element name='rootnode' type='Complex'/>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/sequence/simple/type", "ComplexWithAttribute" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKeyref/type", "keyref" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKeyref/selector", "." )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKeyref/field", "@A" )
                                                              ( "schemas/test/types/Complex/sequence/simple/constraints/simpleKeyref/refer", "otherkey" )
                                                              ( "schemas/test/types/ComplexWithAttribute/type", "complex" )
                                                              ( "schemas/test/types/ComplexWithAttribute/attributes/A/type", "xs:string" );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( xsd_can_include_another_xsd )
{
    const std::string includedGrammar =
         "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
         "   <xs:simpleType name='OutsideType'>"
         "       <xs:restriction base='xs:integer'/>"
         "   </xs:simpleType>"
         "</xs:schema>";
    const std::string mainGrammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:include schemaLocation='included.xsd'/>"
        "   <xs:element name='rootnode' type='OutsideType'/>"
        "</xs:schema>";
    tools::TemporaryDirectory dir( "xeuseudeu-", temp_directory );
    tools::Path included = dir.Path() / "included.xsd";
    tools::Path main = dir.Path() / "test.xsd";
    {
        tools::Ofstream includedStream( included );
        includedStream << includedGrammar;
        tools::Ofstream mainStream( main );
        mainStream << mainGrammar;
    }

    core::Model expected;
    expected = core::MakeModel( "schemas/included/elements", core::MakeModel() )
                              ( "schemas/included/types/OutsideType/type", "simple" )
                              ( "schemas/included/types/OutsideType/restriction/base", "xs:integer" )
                              ( "schemas/test/elements/rootnode/type", "OutsideType" )
                              ( "schemas/test/includes/included.xsd", core::MakeModel() )
                              ( "schemas/test/types", core::MakeModel() );
    expected[ "schemas/test/types/OutsideType" ].Link( expected[ "schemas/included/types/OutsideType" ] );

    core::Model actual;
    xsd::XsdToModel( main, actual );
    CompareModel( expected, actual );
}

BOOST_AUTO_TEST_CASE( attribute_with_simple_type_can_resolve_simple_type_as_builtin_types )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:simpleType name='Simple'>"
        "       <xs:restriction base='xs:integer'/>"
        "   </xs:simpleType>"
        "   <xs:simpleType name='Simple2'>"
        "       <xs:restriction base='Simple'/>"
        "   </xs:simpleType>"
        "   <xs:complexType name='Complex'>"
        "       <xs:attribute name='A' type='Simple' default='42'/>"
        "       <xs:attribute name='B' type='Simple2' fixed='42'/>"
        "   </xs:complexType>"
        "   <xs:element name='rootnode' type='Complex'/>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:integer" )
                                                              ( "schemas/test/types/Simple2/type", "simple" )
                                                              ( "schemas/test/types/Simple2/restriction/base", "Simple" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/attributes/A", sword::test::MakeModel( "type", "Simple" )
                                                                                                                                 ( "default", 42 ) )
                                                              ( "schemas/test/types/Complex/attributes/B", sword::test::MakeModel( "type", "Simple2" )
                                                                                                                                 ( "fixed", 42 ) );
    Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( attribute_with_simple_type_can_resolve_union_types_as_builtin_types )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:simpleType name='Simple'>"
        "       <xs:restriction base='xs:integer'/>"
        "   </xs:simpleType>"
        "   <xs:simpleType name='Simple2'>"
        "       <xs:union memberTypes='Simple xs:string'/>"
        "   </xs:simpleType>"
        "   <xs:complexType name='Complex'>"
        "       <xs:attribute name='A' type='Simple2' default='42'/>"
        "       <xs:attribute name='B' type='Simple2' fixed='fixed_value'/>"
        "   </xs:complexType>"
        "   <xs:element name='rootnode' type='Complex'/>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:integer" )
                                                              ( "schemas/test/types/Simple2/type", "simple" )
                                                              ( "schemas/test/types/Simple2/union", sword::test::MakeModel()
                                                                                                  [ sword::test::MakeModel( "Simple" ) ]
                                                                                                  [ sword::test::MakeModel( "xs:float" ) ] )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/attributes/A", sword::test::MakeModel( "type", "Simple2" )
                                                                                                                                 ( "default", 42 ) )
                                                              ( "schemas/test/types/Complex/attributes/B", sword::test::MakeModel( "type", "Simple2" )
                                                                                                                                 ( "fixed", "fixed_value" ) );
    BOOST_TODO;
    //Check( grammar, expected );
}

BOOST_AUTO_TEST_CASE( attribute_with_simple_type_can_resolve_list_types_as_a_list_of_builtin_type )
{
    const std::string grammar =
        "<xs:schema xmlns:xs='http://www.w3.org/2001/XMLSchema'>"
        "   <xs:simpleType name='Simple'>"
        "       <xs:restriction base='xs:integer'/>"
        "   </xs:simpleType>"
        "   <xs:simpleType name='Simple2'>"
        "       <xs:list itemType='Simple'/>"
        "   </xs:simpleType>"
        "   <xs:complexType name='Complex'>"
        "       <xs:attribute name='A' type='Simple2' default='42'/>"
        "       <xs:attribute name='B' type='Simple2' fixed='42 1337'/>"
        "   </xs:complexType>"
        "   <xs:element name='rootnode' type='Complex'/>"
        "</xs:schema>";
    const sword::test::Model expected = sword::test::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                                                              ( "schemas/test/types/Simple/type", "simple" )
                                                              ( "schemas/test/types/Simple/restriction/base", "xs:integer" )
                                                              ( "schemas/test/types/Simple2/type", "simple" )
                                                              ( "schemas/test/types/Simple2/list", "Simple" )
                                                              ( "schemas/test/types/Complex/type", "complex" )
                                                              ( "schemas/test/types/Complex/attributes/A", sword::test::MakeModel( "type", "Simple2" )
                                                                                                                                 ( "default", sword::test::MakeModel()
                                                                                                                                            [ sword::test::MakeModel( 42 ) ] ) )
                                                              ( "schemas/test/types/Complex/attributes/B", sword::test::MakeModel( "type", "Simple2" )
                                                                                                                                 ( "fixed", sword::test::MakeModel()
                                                                                                                                          [ sword::test::MakeModel( 42 ) ]
                                                                                                                                          [ sword::test::MakeModel( 1337 ) ] ) );
    BOOST_TODO;
    //Check( grammar, expected );
}
