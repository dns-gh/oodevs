// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_GenerateProto_h
#define xsd_GenerateProto_h

#include <string>

namespace core
{
    class Model;
}

namespace tools
{
    class Path;
}

// *****************************************************************************
//
// GenerateProto( const core::Model& schema, const tools::Path& path,
//                const std::string& package, bool generateDoc,
//                const std::string& language )
// This function uses 'schema' to generate a .proto file where 'path' is the path
// to this proto file.
// 'package' is the namespace of the protobuf file. If empty, no namespace is set.
// 'generateDoc' indicates if the documentation is generated in the resulting file.
// 'language' is the language selected for the documentation in the .proto file.
// If empty, the default annotation will be used.
//
// ex: package
//      package = packageName
//   results in:
//      package packageName;
//
// ex: includes
//      schema[ "includes" ][ "other1.xsd"]
//   results in:
//      import "other1.proto";
//
// ex: simple type
//      schema[ "types" ][ "SimpleType" ][ "type" ] = "simple"
//                                       [ "restriction" ][ "base" ] = "xs:boolean"
//   results in:
//      message SimpleType {
//          required bool value = 1;
//      }
//
// ex: union type
//      schema[ "types" ][ "SimpleUnion" ][ "type" ] = "simple"
//                                        [ "union" ][ "base" ] = [ core::Model( "xs:string" ),
//                                                                  core::Model( "xs:integer" ) ]
//   results in:
//      message SimpleUnion {
//          enum Type {
//              UNION_string = 1;
//              UNION_int32 = 2;
//          }
//          required Type type = 1;
//          optional string string_value = 2;
//          optional int32 int32_value = 3;
//      }
//
// ex: complex type
//      schema[ "types" ][ "ComplexSequence" ][ "type" ] = "simple"
//                                            [ "sequence" ][ "a" ][ "type" ] = "SimpleType"
//                                                          [ "b" ][ "type" ] = "SimpleType"
//                                                                 [ "minOccurs" ] = 0
//                                                                 [ "maxOccurs" ] = -1
//   results in:
//      message ComplexSequence {
//          required SimpleType a = 1;
//          repeated SimpleType b = 2;
//      }
//
// ex: choice type
//      schema[ "types" ][ "ComplexChoice" ][ "type" ] = "simple"
//                                            [ "sequence" ][ "a" ][ "type" ] = "SimpleType"
//                                                          [ "b" ][ "type" ] = "SimpleType"
//                                                                 [ "minOccurs" ] = 0
//                                                                 [ "maxOccurs" ] = -1
//   results in:
//      message ComplexChoice {
//          enum Type {
//              CHOICE_a = 1;
//              CHOICE_b = 2;
//          }
//          required Type type = 1;
//          optional SimpleType a = 2;
//          repeated SimpleType b = 3;
//      }
//
// *****************************************************************************

namespace xsd
{
    void GenerateProto( const core::Model& schema, const tools::Path& path,
                        const std::string& package, bool generateDoc,
                        const std::string& language );
}

#endif // xsd_GenerateProto_h
