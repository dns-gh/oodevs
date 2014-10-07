// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ModelToProto.h"
#include "Helpers.h"
#include "core/Model.h"
#include "core/NamedChildrenVisitor.h"
#include <tools/Exception.h>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>
#pragma warning( push )
#pragma warning( disable: 4244 4267 )
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <map>

using namespace google::protobuf;

namespace
{
    template< typename BaseType, void ( Reflection::*Setter )( Message*, const FieldDescriptor*, BaseType ) const >
    void SetSingle( const core::Model& model, const Reflection& reflection, Message& message, const FieldDescriptor& field )
    {
        ( reflection.*Setter )( &message, &field, static_cast< BaseType >( model ) );
    }
    template< typename BaseType, void ( Reflection::*Setter )( Message*, const FieldDescriptor*, BaseType ) const >
    void SetMultiple( const core::Model& model, const Reflection& reflection, Message& message, const FieldDescriptor& field, size_t index )
    {
        ( reflection.*Setter )( &message, &field, model.GetElement( index ) );
    }
    void SetSingleEnum( const core::Model& model, const Reflection& reflection, Message& message, const FieldDescriptor& field )
    {
        const EnumDescriptor* enumDescriptor = field.enum_type();
        if( !enumDescriptor )
            throw MASA_EXCEPTION( "Can't find enum descriptor for field: " + field.name() );
        reflection.SetEnum( &message, &field, enumDescriptor->FindValueByNumber( static_cast< int32_t >( model ) ) );
    }
    void SetMultipleEnum( const core::Model& model, const Reflection& reflection, Message& message, const FieldDescriptor& field, size_t index )
    {
        const EnumDescriptor* enumDescriptor = field.enum_type();
        if( !enumDescriptor )
            throw MASA_EXCEPTION( "Can't find enum descriptor for field: " + field.name() );
        reflection.AddEnum( &message, &field, enumDescriptor->FindValueByNumber( model.GetElement( index ) ) );
    }
    void SetSingleMessage( const core::Model& model, const Reflection& reflection, Message& message, const FieldDescriptor& field )
    {
        Message* childMessage = reflection.MutableMessage( &message, &field );
        if( !childMessage )
            throw MASA_EXCEPTION( "Can't find mutable message for field: " + field.name() );
        xsd::ModelToProto( model, *childMessage );
    }
    void SetMultipleMessage( const core::Model& model, const Reflection& reflection, Message& message, const FieldDescriptor& field, size_t index )
    {
        Message* childMessage = reflection.AddMessage( &message, &field );
        if( !childMessage )
            throw MASA_EXCEPTION( "Can't find mutable message for field: " + field.name() );
        xsd::ModelToProto( model.GetElement( index ), *childMessage );
    }

    typedef void ( *T_SingleSetter )( const core::Model&, const Reflection&, Message&, const FieldDescriptor& );
    typedef void ( *T_MultipleSetter )( const core::Model&, const Reflection&, Message&, const FieldDescriptor&, size_t );
    typedef std::pair< T_SingleSetter, T_MultipleSetter > T_Setters;

#define DECLARE_SETTER( CPPTYPE, TYPE, NAME )                                                                                           \
    ( FieldDescriptor::CPPTYPE, std::make_pair< T_SingleSetter, T_MultipleSetter >( &SetSingle< TYPE , &Reflection::Set##NAME >,      \
                                                                                      &SetMultiple< TYPE , &Reflection::Add##NAME > ) ) \

    const std::map< FieldDescriptor::CppType, T_Setters > setters = boost::assign::map_list_of
        DECLARE_SETTER( CPPTYPE_INT32, int32_t, Int32 )
        DECLARE_SETTER( CPPTYPE_INT64, int64_t, Int64 )
        DECLARE_SETTER( CPPTYPE_UINT32, uint32_t, UInt32 )
        DECLARE_SETTER( CPPTYPE_UINT64, uint64_t, UInt64 )
        DECLARE_SETTER( CPPTYPE_DOUBLE, double, Double )
        DECLARE_SETTER( CPPTYPE_FLOAT, float, Float )
        DECLARE_SETTER( CPPTYPE_BOOL, bool, Bool )
        DECLARE_SETTER( CPPTYPE_STRING, const std::string&, String )
        ( FieldDescriptor::CPPTYPE_ENUM, std::make_pair< T_SingleSetter, T_MultipleSetter >( &SetSingleEnum, &SetMultipleEnum ) )
        ( FieldDescriptor::CPPTYPE_MESSAGE, std::make_pair< T_SingleSetter, T_MultipleSetter >( &SetSingleMessage, &SetMultipleMessage ) );

    void Functor( Message& msg, const Descriptor& descriptor, const Reflection& reflection, const std::string& key, const core::Model& child )
    {
        if( key == "attributes" || key == "elements" )
        {
            xsd::ModelToProto( child, msg );
            return;
        }
        const FieldDescriptor* field = descriptor.FindFieldByName( xsd::XsdNameToProtoName( key ) );
        if( !field )
            throw MASA_EXCEPTION( "Unknown field descriptor: " + key );
        if( descriptor.enum_type_count() == 1 )
            if( const FieldDescriptor* typeField = descriptor.FindFieldByName( "type" ) )
                if( typeField->cpp_type() == FieldDescriptor::CPPTYPE_ENUM )
                    if( const EnumDescriptor* enumTypeDescriptor = typeField->enum_type() )
                    {
                        const std::string enumName = boost::algorithm::ends_with( key, "-value" )
                            ? "UNION_" + boost::algorithm::erase_last_copy( key, "-value" )
                            : "CHOICE_" + key;
                        if( const EnumValueDescriptor* enumValueDescriptor = enumTypeDescriptor->FindValueByName( enumName ) )
                            reflection.SetEnum( &msg, typeField, enumValueDescriptor );
                    }
        FieldDescriptor::CppType type = field->cpp_type();
        if( !field->is_repeated() )
            setters.at( type ).first( child, reflection, msg, *field );
        else
        {
            const core::Model& realChild = child.HasChild( "list" ) ? child[ "list" ] : child;
            for( size_t index = 0; index < realChild.GetSize(); ++index )
                setters.at( type ).second( realChild, reflection, msg, *field, index );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: xsd::ModelToProto
// Created: ABR 2013-08-09
// -----------------------------------------------------------------------------
void xsd::ModelToProto( const core::Model& model, google::protobuf::Message& msg )
{
    const Descriptor& descriptor = *msg.GetDescriptor();
    const Reflection& reflection = *msg.GetReflection();
    core::NamedChildrenVisitor( model, boost::bind( &Functor, boost::ref( msg ), boost::cref( descriptor ), boost::cref( reflection ), _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: xsd::ModelToProto
// Created: ABR 2013-08-09
// -----------------------------------------------------------------------------
void xsd::ModelToProto( const core::Model& model, int& enumMsg )
{
    enumMsg = model[ "value" ];
}
