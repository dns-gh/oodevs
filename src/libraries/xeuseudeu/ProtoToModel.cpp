// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "ProtoToModel.h"
#include "Helpers.h"
#include "core/Model.h"
#include "core/ModelVisitor_ABC.h"
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

using namespace google::protobuf;

namespace
{
    class ApplyConstraintsVisitor : public core::ModelVisitor_ABC
    {
    public:
        explicit ApplyConstraintsVisitor( core::Model& keyNode, const core::Model& elementNode )
            : keyNode_    ( keyNode )
            , elementNode_( elementNode )
        {}
    public:
        virtual void Visit( int64_t value )
        {
            if( value < 0 )
                throw MASA_EXCEPTION( "negative indexes are not supported" );
            keyNode_[ static_cast< std::size_t >( value ) ].Link( elementNode_ );
        }
        virtual void Visit( uint64_t /*value*/ ) {}
        virtual void Visit( double /*value*/ ) {}
        virtual void Visit( const std::string& value )
        {
            keyNode_[ value ].Link( elementNode_ );
        }
        virtual void Visit( std::size_t /*key*/, const core::Model& /*child*/ ) {}
        virtual void Visit( const core::Model& /*element*/ ) {}
        virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& /*data*/ ) {}
        virtual void Visit( const std::string& /*key*/, const core::Model& /*child*/ ) {}
    private:
        core::Model& keyNode_;
        const core::Model& elementNode_;
    };

    template< typename BaseType, BaseType ( Reflection::*Extractor )( const Message&, const FieldDescriptor* ) const >
    void ExtractSingle( const core::Model& /*schema*/, core::Model& model, const Reflection& reflection,
                        const Message& message, const FieldDescriptor& field, xsd::T_Constraints& /*constraints*/ )
    {
        model = ( reflection.*Extractor )( message, &field );
    }
    template< typename BaseType, BaseType ( Reflection::*Extractor )( const Message&, const FieldDescriptor*, int ) const >
    void ExtractMultiple( const core::Model& /*schema*/, core::Model& model, const Reflection& reflection,
                          const Message& message, const FieldDescriptor& field, int index, xsd::T_Constraints& /*constraints*/ )
    {
        model.AddElement() = ( reflection.*Extractor )( message, &field, index );
    }
    void ExtractSingleEnum( const core::Model& /*schema*/, core::Model& model, const Reflection& reflection,
                            const Message& message, const FieldDescriptor& field, xsd::T_Constraints& /*constraints*/ )
    {
        const EnumValueDescriptor* enumDescriptor = reflection.GetEnum( message, &field );
        if( !enumDescriptor )
            throw MASA_EXCEPTION( "Can't find enum value descriptor for field: " + field.name() );
        model = enumDescriptor->number();
    }
    void ExtractMultipleEnum( const core::Model& /*schema*/, core::Model& model, const Reflection& reflection,
                              const Message& message, const FieldDescriptor& field, int index, xsd::T_Constraints& /*constraints*/ )
    {
        const EnumValueDescriptor* enumDescriptor = reflection.GetRepeatedEnum( message, &field, index );
        if( !enumDescriptor )
            throw MASA_EXCEPTION( "Can't find enum value descriptor for field: " + field.name() );
        model.AddElement() = enumDescriptor->number();
    }
    void ExtractSingleMessage( const core::Model& schema, core::Model& model, const Reflection& reflection,
                               const Message& message, const FieldDescriptor& field, xsd::T_Constraints& constraints )
    {
        xsd::ProtoToModel( schema, reflection.GetMessage( message, &field ), model, constraints );
    }
    void ExtractMultipleMessage( const core::Model& schema, core::Model& model, const Reflection& reflection,
                                 const Message& message, const FieldDescriptor& field, int index, xsd::T_Constraints& constraints )
    {
        const Message& child = reflection.GetRepeatedMessage( message, &field, index );
        const Descriptor* descriptor = message.GetDescriptor();
        if( !descriptor )
            throw MASA_EXCEPTION( "Can't find message descriptor for message: " + message.GetTypeName() );
        if( schema[ "types" ][ descriptor->name() ][ "type" ] == "simple" )
        {
            xsd::ProtoToModel( schema, child, model.AddElement(), constraints );
            return;
        }
        std::vector< std::string > constraintsToApply;
        for( auto it = constraints.begin(); it != constraints.end(); ++it )
        {
            std::string& selector = it->second.selector_;
            if( !selector.empty() && selector[ 0 ] == '/' )
                boost::erase_first( selector, "/" );
            boost::erase_first( selector, field.name() );
            if( !selector.empty() && selector[ 0 ] == '/' )
                boost::erase_first( selector, "/" );
            if( selector.empty() && it->second.type_ == "key" )
                constraintsToApply.push_back( it->first );
        }
        core::Model& elementNode = model[ "list" ].AddElement();
        xsd::ProtoToModel( schema, child, elementNode, constraints );
        for( auto it = constraintsToApply.begin(); it != constraintsToApply.end(); ++it )
        {
            std::string targetField = constraints.at( *it ).field_;
            if( std::find_if( targetField.begin(), targetField.end(), boost::algorithm::is_space() ) != targetField.end() )
                throw MASA_EXCEPTION( "Invalid key field: " + targetField );
            boost::erase_first( targetField, "@" );
            if( !elementNode.HasChild( "attributes/" + targetField ) )
                throw MASA_EXCEPTION( "Can't find field " + targetField + " on element " + field.name() );
            
            ApplyConstraintsVisitor visitor( model[ *it ], elementNode );
            elementNode[ "attributes" ][ targetField ].Accept( visitor );
        }
    }

    typedef void ( *T_SingleExtractor )( const core::Model&, core::Model&, const Reflection&, const Message&, const FieldDescriptor&, xsd::T_Constraints& );
    typedef void ( *T_MultipleExtractor )( const core::Model&, core::Model&, const Reflection&, const Message&, const FieldDescriptor&, int, xsd::T_Constraints& );
    typedef std::pair< T_SingleExtractor, T_MultipleExtractor > T_Extractors;

#define DECLARE_EXTRACTOR( CPPTYPE, TYPE, NAME )                                                                                                            \
    ( FieldDescriptor::CPPTYPE, std::make_pair< T_SingleExtractor, T_MultipleExtractor >( &ExtractSingle< TYPE , &Reflection::Get##NAME >,                \
                                                                                            &ExtractMultiple< TYPE , &Reflection::GetRepeated##NAME > ) )   \

    const std::map< FieldDescriptor::CppType, T_Extractors > extractors = boost::assign::map_list_of
        DECLARE_EXTRACTOR( CPPTYPE_INT32, int32_t, Int32 )
        DECLARE_EXTRACTOR( CPPTYPE_INT64, int64_t, Int64 )
        DECLARE_EXTRACTOR( CPPTYPE_UINT32, uint32_t, UInt32 )
        DECLARE_EXTRACTOR( CPPTYPE_UINT64, uint64_t, UInt64 )
        DECLARE_EXTRACTOR( CPPTYPE_DOUBLE, double, Double )
        DECLARE_EXTRACTOR( CPPTYPE_FLOAT, float, Float )
        DECLARE_EXTRACTOR( CPPTYPE_BOOL, bool, Bool )
        DECLARE_EXTRACTOR( CPPTYPE_STRING, std::string, String )
        ( FieldDescriptor::CPPTYPE_ENUM, std::make_pair< T_SingleExtractor, T_MultipleExtractor >( &ExtractSingleEnum, &ExtractMultipleEnum ) )
        ( FieldDescriptor::CPPTYPE_MESSAGE, std::make_pair< T_SingleExtractor, T_MultipleExtractor >( &ExtractSingleMessage, &ExtractMultipleMessage ) );

    void ConstraintVisitor( const std::string& key, const core::Model& child, xsd::T_Constraints& constraints )
    {
        if( constraints.find( key ) != constraints.end() )
            throw MASA_EXCEPTION( "Constraint already registered: " + key );
        if( !child.HasChild( "type" ) || !child.HasChild( "selector" ) || !child.HasChild( "field" ) )
            throw MASA_EXCEPTION( "Invalid constraint: " + key );
        std::string refer;
        if( child[ "type" ] == "keyref" )
        {
            if( !child.HasChild( "refer" ) )
                throw MASA_EXCEPTION( "Can't find refer node under keyref constraint: " + key );
            refer = child[ "refer" ];
        }
        constraints[ key ] = xsd::Constraint( child[ "type" ], child[ "selector" ], child[ "field" ], refer );
    }
    void ExtractConstraintsFromSchema( const core::Model& schema, xsd::T_Constraints& constraints )
    {
        if( !schema.HasChild( "constraints" ) )
            return;
        core::NamedChildrenVisitor( schema[ "constraints" ], boost::bind( &ConstraintVisitor, _1, _2, boost::ref( constraints ) ) );
    }
    void Extract( const core::Model& schema, core::Model& model, const Message& message, const Reflection& reflection,
                  const FieldDescriptor& field, FieldDescriptor::CppType type, xsd::T_Constraints& constraints )
    {
        if( !field.is_repeated() )
            extractors.at( type ).first( schema, model, reflection, message, field, constraints );
        else
            for( int index = 0; index < reflection.FieldSize( message, &field ); ++index )
                extractors.at( type ).second( schema, model, reflection, message, field, index, constraints );
    }
    bool ExtractTypeWithEnum( const std::string& typeName, const core::Model& schema, core::Model& model,
                              const Message& message, const Descriptor& descriptor, const Reflection& reflection,
                              xsd::T_Constraints& constraints, const std::string& suffix = "", const std::string& childName = "" )
    {
        if( !schema[ "types" ][ descriptor.name() ].HasChild( typeName ) )
            return false;
        const FieldDescriptor* typeField = descriptor.FindFieldByName( "type" );
        if( descriptor.enum_type_count() != 1 || typeField == 0 || typeField->cpp_type() != FieldDescriptor::CPPTYPE_ENUM )
            throw MASA_EXCEPTION( "Invalid type " + typeName );
        const EnumValueDescriptor* typeDescriptor = reflection.GetEnum( message, typeField );
        if( !typeDescriptor )
            throw MASA_EXCEPTION( "Can't find enum value descriptor for field: type" );
        std::string fieldName = boost::algorithm::erase_first_copy( typeDescriptor->name(), boost::algorithm::to_upper_copy( typeName ) + "_" );
        if( !suffix.empty() )
            fieldName += "_" + suffix;
        const FieldDescriptor* field = descriptor.FindFieldByName( fieldName );
        if( !field )
            throw MASA_EXCEPTION( "Can't find field descriptor for field: " + fieldName );
        fieldName = xsd::ProtoNameToXsdName( fieldName );

        if( schema[ "types" ][ descriptor.name() ][ "type" ] == "complex" )
            ExtractConstraintsFromSchema( schema[ "types" ][ descriptor.name() ][ typeName ][ fieldName ], constraints );
        Extract( schema, childName.empty() ? model[ fieldName ] : model[ childName ][ fieldName ], message, reflection, *field, field->cpp_type(), constraints );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: xsd::ProtoToModel
// Created: ABR 2013-08-09
// -----------------------------------------------------------------------------
void xsd::ProtoToModel( const core::Model& schema, const Message& message, core::Model& model,
                        xsd::T_Constraints constraints /* = xsd::T_Constraints() */ )
{
    const Descriptor* descriptor = message.GetDescriptor();
    const Reflection* reflection = message.GetReflection();
    if( !descriptor )
        throw MASA_EXCEPTION( "Can't find descriptor for message: " + message.GetTypeName() );
    if( !reflection )
        throw MASA_EXCEPTION( "Can't find reflection for message: " + message.GetTypeName() );

    if( ExtractTypeWithEnum( "union", schema, model, message, *descriptor, *reflection, constraints, "value" ) ||
        ExtractTypeWithEnum( "choice", schema, model, message, *descriptor, *reflection, constraints, "", "elements" ) )
        return;

    const core::Model& typeNode = schema[ "types" ][ descriptor->name() ];
    for( int i = 0; i < descriptor->field_count(); ++i )
    {
        const FieldDescriptor* field = descriptor->field( i );
        if( !field )
            throw MASA_EXCEPTION( "Can't find field descriptor" );
        const std::string fieldName = ProtoNameToXsdName( field->name() );
        core::Model* targetNode = 0;
        if( typeNode.HasChild( "attributes/" + fieldName ) )
            targetNode = &model[ "attributes" ][ fieldName ];
        else if( typeNode.HasChild( "sequence/" + fieldName ) )
        {
            targetNode = &model[ "elements" ][ fieldName ];
            ExtractConstraintsFromSchema( typeNode[ "sequence" ][ fieldName ], constraints );
        }
        else if( typeNode.HasChild( "all/" + fieldName ) )
        {
            targetNode = &model[ "elements" ][ fieldName ];
            ExtractConstraintsFromSchema( typeNode[ "all" ][ fieldName ], constraints );
        }
        else
            targetNode = &model[ fieldName ];
        Extract( schema, *targetNode, message, *reflection, *field, field->cpp_type(), constraints );
    }
}

// -----------------------------------------------------------------------------
// Name: xsd::ProtoToModel
// Created: ABR 2013-08-09
// -----------------------------------------------------------------------------
void xsd::ProtoToModel( const core::Model& /*schema*/, int enumValue, core::Model& model )
{
    model[ "value" ] = enumValue;
}
