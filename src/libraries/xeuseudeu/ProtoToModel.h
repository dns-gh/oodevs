// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef xsd_ProtoToModel_h
#define xsd_ProtoToModel_h

#include "Constraints.h"

namespace core
{
    class Model;
}

namespace google
{
namespace protobuf
{
    class Message;
}
}

// *****************************************************************************
//
// ProtoToModel( const core::Model& schema, const google::protobuf::Message& message,
//               core::Model& model, T_Constraints constraints = T_Constraints() )
//
// This function uses 'schema' to fill 'model' with the data stored in 'message'.
// 'constraints' is an optional parameter used to describe the constraints to apply
// to the resulting 'model'.
//
// Each node element can have the following children:
// - "value": required if simple type, forbidden otherwise. It stores the value of
//            this type.
// - "attributes": forbidden if simple type, optional otherwise. It stores named
//                 children with the attribute name as key and the attribute value
//                 as name.
// - "elements": forbidden if simple type, optional otherwise. It stores element
//               nodes as children.
// - "list": forbidden if simple type, optional otherwise. If the parent element
//           has multiple instance of this element, it stores the list of each
//           instance of this element.
//
// If the element is a complex type, at least one the possible sub child is required.
//
// If key constraints select this element (that means it's a list, so it already
// has a list child), the element will have a children for each key constraint,
// named as the key is. Under one key node, there will be named children or
// identified children (depending on the key attribute type) with the attributes
// value as key to allow an easy access to each instance.
// The key instance of the data are symbolic links to the original instance (the
// one stored in the "list" child)
//
// For the following examples, we assume that the input schema has a root node
// named root, with a different type for each example. SchemaTypes refer
// to schema[ "types" ], and RootNode refer to model[ "root" ].
//
// ex: simple type
//      SchemaTypes[ "SimpleType" ][ "type" ] = "simple"
//                                 [ "restriction" ] = "xs:boolean"
//   and
//      SimpleType message;
//      message.set_value( true )
//   result in:
//      RootNode[ "value" ] = 1
//
// ex: complex type with a root node of type ComplexWithKeysConstraints
//  SchemaTypes[ "ComplexWithKeys" ][ "type" ] = "complex"
//             [ "ComplexWithKeys" ][ "attributes" ][ "A" ][ "type" ] = "xs:string"
//                                                         [ "use" ] = "required"
//             [ "ComplexWithKeys" ][ "attributes" ][ "B" ][ "type" ] = "xs:integer"
//                                                         [ "use" ] = "required"
//             [ "ComplexWithKeysList" ][ "type" ] = "complex"
//             [ "ComplexWithKeysConstraints" ][ "sequence" ][ "occurs" ][ "type" ] = "ComplexWithKeys"
//                                                                       [ "minOccurs" ] = 0
//                                                                       [ "maxOccurs" ] = -1
//             [ "ComplexWithKeysConstraints" ][ "type" ] = "complex"
//             [ "ComplexWithKeysConstraints" ][ "all" ][ "occurs-list" ][ "type" ] = "ComplexWithAttributes"
//                                                                       [ "minOccurs" ] = 1
//                                                                       [ "maxOccurs" ] = 1
//                                                                       [ "constraints" ][ "a-key" ][ "type" ] = "key"
//                                                                                                   [ "selector" ] = "occurs"
//                                                                                                   [ "field" ] = "@A"
//                                                                                        [ "b-key" ][ "type" ] = "key"
//                                                                                                   [ "selector" ] = "occurs"
//                                                                                                   [ "field" ] = "@B"
//   and
//      ComplexWithKeysConstraints expectedMessage;
//      ComplexWithKeys* first = expectedMessage.mutable_occurs_list()->add_occurs();
//      first->set_a( "first key" );
//      first->set_b( 42 );
//      ComplexWithKeys* second = expectedMessage.mutable_occurs_list()->add_occurs();
//      second->set_a( "second key" );
//      second->set_b( 1337 );
//   result in:
//      RootNode[ "elements ][ "occurs-list" ][ "elements" ][ "occurs" ][ "list" ] = [ core::Model( "attributes/A", "first key" )
//                                                                                                ( "attributes/B", 42 ),
              //                                                                       core::Model( "attributes/A", "second key" )
//                                                                                                ( "attributes/B", 1337 ) ];
//      RootNode[ "elements/occurs-list/elements/occurs/a-key/first key/" ] .Link( RootNode[ "elements/occurs-list/elements/occurs/list" ].GetElement( 0 ) );
//      RootNode[ "elements/occurs-list/elements/occurs/a-key/second key/" ].Link( RootNode[ "elements/occurs-list/elements/occurs/list" ].GetElement( 1 ) );
//      RootNode[ "elements/occurs-list/elements/occurs/b-key/" ][ 42 ]     .Link( RootNode[ "elements/occurs-list/elements/occurs/list" ].GetElement( 0 ) );
//      RootNode[ "elements/occurs-list/elements/occurs/b-key/" ][ 1337 ]   .Link( RootNode[ "elements/occurs-list/elements/occurs/list" ].GetElement( 1 ) );
//
// *****************************************************************************

namespace xsd
{
    void ProtoToModel( const core::Model& schema, const google::protobuf::Message& message, core::Model& model,
                       T_Constraints constraints = T_Constraints() );
    void ProtoToModel( const core::Model& schema, int enumValue, core::Model& model );
}

#endif // xsd_ProtoToModel_h
