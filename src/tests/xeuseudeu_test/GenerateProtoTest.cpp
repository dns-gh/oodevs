// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "xeuseudeu_test_pch.h"
#include "core/Model.h"
#include "core/MakeModel.h"
#include "xeuseudeu/GenerateProto.h"
#include <tools/TemporaryDirectory.h>
#include <tools/StdFileWrapper.h>

namespace
{
    void Check( const core::Model& schema, const std::string& expected, const std::string& packageName = "", bool generateDoc = false, const std::string& language = "" )
    {
        tools::TemporaryDirectory dir( "xeuseudeu-", temp_directory );
        tools::Path path = dir.Path() / "test.proto";

        xsd::GenerateProto( schema[ "schemas/test" ], path, packageName, generateDoc, language );
        tools::Ifstream ifs( path );
        std::stringstream sstr;
        sstr << ifs.rdbuf();
        BOOST_CHECK_EQUAL( sstr.str(), expected );
    }
}

BOOST_AUTO_TEST_CASE( generate_header )
{
    const std::string expected =
        "// *****************************************************************************\n"
        "//\n"
        "// This file is part of a MASA library or program.\n"
        "// Refer to the included end-user license agreement for restrictions.\n"
        "//\n"
        "// Copyright (c) MASA Group\n"
        "//\n"
        "// This file was generated, do not modify it.\n"
        "//\n"
        "// *****************************************************************************\n"
        "\n"
        "package packageName;\n"
        "\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/types/", core::MakeModel() );
    Check( schema, expected, "packageName", true );
}

BOOST_AUTO_TEST_CASE( generate_documentation_from_annotation )
{
    const std::string expected =
        "// *****************************************************************************\n"
        "//\n"
        "// This file is part of a MASA library or program.\n"
        "// Refer to the included end-user license agreement for restrictions.\n"
        "//\n"
        "// Copyright (c) MASA Group\n"
        "//\n"
        "// This file was generated, do not modify it.\n"
        "//\n"
        "// *****************************************************************************\n"
        "\n"
        "package packageName;\n"
        "\n"
        "/** @file test.proto\n"
        " *  @brief schema doc en\n"
        " *  @{\n"
        " */\n"
        "\n"
        "/** @}*/\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                            ( "schemas/test/annotations/en", "schema doc en" )
                            ( "schemas/test/types", core::MakeModel() );
    Check( schema, expected, "packageName", true, "en" );
}

BOOST_AUTO_TEST_CASE( generate_import_from_includes )
{
    const std::string expected =
        "import \"other1.proto\";\n"
        "import \"other2.proto\";\n"
        "\n"
        "package packageName;\n"
        "\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/includes/other1.xsd", core::MakeModel() )
                            ( "schemas/test/includes/other2.xsd", core::MakeModel() )
                            ( "schemas/test/types", core::MakeModel() );
    Check( schema, expected, "packageName" );
}

BOOST_AUTO_TEST_CASE( generate_message_from_simple_type )
{
    const std::string expected =
        "message Simple {\n"
        "    required bool value = 1;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                            ( "schemas/test/types/Simple/type", "simple" )
                            ( "schemas/test/types/Simple/restriction/base", "xs:boolean" );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_simple_type_enumeration )
{
    const std::string expected =
        "enum Simple {\n"
        "    First_value = 0;\n"
        "    Second_value = 1;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Simple" )
                            ( "schemas/test/types/Simple/type", "simple" )
                            ( "schemas/test/types/Simple/restriction/base", "xs:string" )
                            ( "schemas/test/types/Simple/restriction/enumeration", core::MakeModel()
                                                                                 [ core::MakeModel( "First value" ) ]
                                                                                 [ core::MakeModel( "Second value" ) ] );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_simple_type_with_union )
{
    const std::string expected =
        "message SimpleUnion {\n"
        "    enum Type {\n"
        "        UNION_string = 0;\n"
        "        UNION_int32 = 1;\n"
        "    }\n"
        "\n"
        "    required Type type = 1;\n"
        "    optional string string_value = 2;\n"
        "    optional int32 int32_value = 3;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleUnion" )
                            ( "schemas/test/types/SimpleUnion/type", "simple" )
                            ( "schemas/test/types/SimpleUnion/union", core::MakeModel()
                                                                [ core::MakeModel( "xs:string" ) ]
                                                                [ core::MakeModel( "xs:integer" ) ] );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_simple_type_with_list )
{
    const std::string expected =
        "message SimpleList {\n"
        "    repeated bool values = 1;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "SimpleList" )
                            ( "schemas/test/types/SimpleList/type", "simple" )
                            ( "schemas/test/types/SimpleList/list", "xs:boolean" );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_complex_type_with_builtin_attributes )
{
    const std::string expected =
        "message Complex {\n"
        "    required string A = 1;\n"
        "    optional int32 B = 2;\n"
        "    optional string C = 3 [default = \"default value\"];\n"
        "    optional int32 D_E = 4 [default = 42];\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                            ( "schemas/test/types/Complex/type", "complex" )
                            ( "schemas/test/types/Complex/attributes/A", core::MakeModel( "type", "xs:string" )
                                                                                        ( "use", "required" ) )
                            ( "schemas/test/types/Complex/attributes/B", core::MakeModel( "type", "xs:integer" )
                                                                                        ( "use", "optional" ) )
                            ( "schemas/test/types/Complex/attributes/C", core::MakeModel( "type", "xs:string" )
                                                                                        ( "default", "default value" ) )
                            ( "schemas/test/types/Complex/attributes/D-E", core::MakeModel( "type", "xs:integer" )
                                                                                          ( "default", 42 ) );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_complex_type_with_simple_type_attributes )
{
    const std::string expected =
        "message Complex {\n"
        "    required Simple A = 1;\n"
        "    optional Simple2 B = 2;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                            ( "schemas/test/types/Complex/type", "complex" )
                            ( "schemas/test/types/Complex/attributes/A", core::MakeModel( "type", "Simple" )
                                                                                        ( "use", "required" )
                                                                                        ( "default", 42 ) )
                            ( "schemas/test/types/Complex/attributes/B", core::MakeModel( "type", "Simple2" ) );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_complex_type_with_sequence )
{
    const std::string expected =
        "message Complex {\n"
        "    required Simple a = 1;\n"
        "    repeated Simple b = 2;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                            ( "schemas/test/types/Complex/type", "complex" )
                            ( "schemas/test/types/Complex/sequence/a/type", "Simple" )
                            ( "schemas/test/types/Complex/sequence/b", core::MakeModel( "type", "Simple" )
                                                                                      ( "minOccurs", 0 )
                                                                                      ( "maxOccurs", -1 ) );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_complex_type_with_choice )
{
    const std::string expected =
        "message Complex {\n"
        "    enum Type {\n"
        "        CHOICE_a = 0;\n"
        "        CHOICE_b = 1;\n"
        "    }\n"
        "\n"
        "    required Type type = 1;\n"
        "    optional Simple a = 2;\n"
        "    repeated Simple b = 3;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                            ( "schemas/test/types/Complex/type", "complex" )
                            ( "schemas/test/types/Complex/choice/a/type", "Simple" )
                            ( "schemas/test/types/Complex/choice/b", core::MakeModel( "type", "Simple" )
                                                                                    ( "minOccurs", 0 )
                                                                                    ( "maxOccurs", -1 ) );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_message_from_complex_type_with_all )
{
    const std::string expected =
        "message Complex {\n"
        "    required Simple a = 1;\n"
        "    optional Simple b = 2;\n"
        "}\n\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/elements/rootnode/type", "Complex" )
                            ( "schemas/test/types/Complex/type", "complex" )
                            ( "schemas/test/types/Complex/all/a/type", "Simple" )
                            ( "schemas/test/types/Complex/all/b", core::MakeModel( "type", "Simple" )
                                                                                 ( "minOccurs", 0 )
                                                                                 ( "maxOccurs", 1 ) );
    Check( schema, expected );
}

BOOST_AUTO_TEST_CASE( generate_complete_message_with_annotations )
{
    const std::string expected =
        "// *****************************************************************************\n"
        "//\n"
        "// This file is part of a MASA library or program.\n"
        "// Refer to the included end-user license agreement for restrictions.\n"
        "//\n"
        "// Copyright (c) MASA Group\n"
        "//\n"
        "// This file was generated, do not modify it.\n"
        "//\n"
        "// *****************************************************************************\n"
        "\n"
        "import \"other1.proto\";\n"
        "import \"other2.proto\";\n"
        "\n"
        "package packageName;\n"
        "\n"
        "/** @file test.proto\n"
        " *  @brief schema doc en\n"
        " *  @{\n"
        " */\n"
        "\n"
        "/** @struct Complex\n"
        " *  @brief complex type doc\n"
        " */\n"
        "message Complex {\n"
        "    //! simple element doc\n"
        "    required Simple simple = 1;\n"
        "    //! name attribute doc en\n"
        "    optional string name = 2;\n"
        "}\n"
        "\n"
        "/** @struct Simple\n"
        " *  @brief simple type doc\n"
        " */\n"
        "message Simple {\n"
        "    required bool value = 1;\n"
        "}\n"
        "\n"
        "/** @}*/\n";
    core::Model schema;
    schema = core::MakeModel( "schemas/test/annotations/default", "schema doc" )
                            ( "schemas/test/annotations/en", "schema doc en" )
                            ( "schemas/test/includes/other1.xsd", core::MakeModel() )
                            ( "schemas/test/includes/other2.xsd", core::MakeModel() )
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
                            ( "schemas/test/types/Complex/attributes/name/annotations/en", "name attribute doc en" )
                            ( "schemas/test/types/Complex/attributes/name/type", "xs:string" );
    Check( schema, expected, "packageName", true, "en" );
}
