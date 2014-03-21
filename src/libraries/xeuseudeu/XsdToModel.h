// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_XsdToModel_h
#define xsd_XsdToModel_h

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
// XsdToModel( const tools::Path& schema, core::Model& model )
//
// This function loads an xsd file 'schema' and stores it to a core::Model 'model'.
//
// The content of the xsd file is stored in model[ "schema" ][ FILENAME ].
// ex: test.xsd becomes model[ "schema" ][ "test" ]
//
// For convenience, model[ "schema" ][ FILENAME ] will be referred as FileModel
// in the following documentation.
//
// The includes defined in the xsd file are stored in FileModel[ "includes" ].
// These are string value leaves with the path to the include file as value.
//
// The elements defined in the xsd file are stored in FileModel[ "elements" ].
// These nodes contain a named children 'type', that stores their type name.
// ex: an element MyElement of type MyType result in:
//     FileModel[ "elements" ][ "MyElement" ][ "type" ] = MyType
//
// The types defined in the xsd file are stored in FileModel[ "types" ].
// These nodes contain a named children 'type', that stores their complexity:
// 'simple' or 'complex' depending on their xsd type.
// ex: a simple type MyType result in:
//     FileModel[ "types" ][ "MyType" ][ "type" ] = "simple"
//
// Simple types must have one of the following children:
// - "restriction": must have a "base" child to describe the base type, and some
//   of the following nodes, depending on the base type:
//    - "minInclusive", "maxInclusive", "minExclusive", "maxExclusive": if the base
//      type is a numeric type. These are value children.
//    - "pattern", "minLength", "maxLength": if the base type is a string type.
//      These are value children, integer for min and max length and a regexp validating
//      the string for "pattern".
//    - "enumeration" a list of the allowed values.
// ex: a type MyRestriction that is an xs:integer between 0 and 10:
//      FileModel[ "types" ][ "MyRestriction" ][ "restriction" ][ "base" ] = "xs:integer"
//                                                              [ "minInclusive" ] = 0
//                                                              [ "maxInclusive" ] = 10
// - "union": a list of element that describe the possible types.
// ex: a type MyUnion that is a union between MyType1 and MyType2 result in:
//     FileModel[ "types" ][ "MyUnion" ][ "union" ] = [ core::Model( "MyType1" ),
//                                                      core::Model( "MyType2" ) ]
// - "list": a named child with the type as leaf
// ex: a type MyList that is a list of type MyType result in:
//     FileModel[ "types" ][ "MyList" ][ "list" ] = "MyType"
//
// Complex types can have an "attributes" child, with the following sub child:
// - "type": required, defines the attribute's type.
// - "use": optional, can be "required" or "optional". "optional" by default.
// - "default": optional, stores the default value.
// - "fixed": optional, defines the node as a constant value.
// ex: a type MyComplex with an optional attribute MyAttribute of type xs:string becomes:
//     FileModel[ "types" ][ "MyComplex" ][ "attributes" ][ "MyAttributes" ][ "type" ] = "xs:string"
//                                                                          [ "minOccurs" ] = 0
//
// Complex types can also have one of the following children to store the allowed
// sub-elements:
// - "sequence": each element can be optional, required, or repeated.
// - "choice": only one of the sub elements is allowed. It can be optional, required,
//             or repeated.
// - "all": each element can be optional or required.
// Those sub-elements can have the following nodes:
// - "type": required, it defines the type of the element.
// - "minOccurs": optional, 1 by default.
// - "maxOccurs": optional, 1 by default.
// - "constraints": this is a list of the associated constraints.
// ex: a type MyComplex with a sequence of sub-elements of type MySubElement1 and MySubElement2
//     that both can occur 0 or X times and that both are of type MySubType, results in:
//     FileModel[ "types" ][ "MyComplex" ][ "sequence" ][ "MySubElement1" ][ "type" ] = "MySubType"
//                                                                         [ "minOccurs" ] = 0
//                                                                         [ "maxOccurs" ] = -1
//                                                      [ "MySubElement2" ][ "type" ] = "MySubType"
//                                                                         [ "minOccurs" ] = 0
//                                                                         [ "maxOccurs" ] = -1
//
// A constraint node has the following child:
// - "type": required, can be "key", "unique", or "keyref".
// - "selector": required, is the X-Path request that selects the node where the
//               the constraint will be applied.
// - "field": required, the target attribute where the constraint will be applied.
// - "refer": required if type is "keyref", forbidden otherwise. It's the X-Path request
//            that identifies the linked node.
// ex: a type MyComplex with a sequence of sub-elements of type MySubElement that
//     is required and of type MySubType and that have a key constraint name MyConstraint
//     on the attribute MyAttribute that we can found on the current node result in:
//     FileModel[ "types" ][ "MyComplex" ][ "sequence" ][ "MySubElement" ][ "type" ] = "MySubType"
//                                                                        [ "constraints" ][ "type" ] = "key"
//                                                                                         [ "selector" ] = "."
//                                                                                         [ "field" ] = "@MyAttribute"
//
// All these nodes can have an "annotations" child that stores the documentation
// defined in the xsd, depending on their language.
// ex: an element MyElement that is annotated without a specific language as
//     "Default annotation" and that is annotated in English as "English annotation"
//     result in:
//     FileModel[ "elements" ][ "MyElement" ][ "annotations" ][ "default" ] = "Default annotation"
//                                                            [ "en" ] = "English annotation"
//
// *****************************************************************************

namespace xsd
{
    void XsdToModel( const tools::Path& schema, core::Model& model );
}

#endif // xsd_XsdToModel_h
