// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "XmlToModel.h"
#include "Helpers.h"
#include "core/Model.h"
#include "core/NamedChildrenVisitor.h"
#include <tools/Exception.h>
#include <tools/Path.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable: 4180 )
#include <boost/assign/list_of.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <map>

namespace
{
    void ReadNode( xml::xistream& xis, const core::Model& schema, const std::string& type, core::Model& node );

    bool ReadNotImplementedType( const std::string&, core::Model& )
    {
        throw MASA_EXCEPTION( "Xsd Type not implemented" );
    }
    template< typename T >
    bool ReadBuiltin( const std::string& content, core::Model& model )
    {
        std::istringstream ss( content );
        T value;
        ss >> std::boolalpha >> value >> std::ws;
        if( !ss || !ss.eof() )
            return false;
        model = value;
        return true;
    }
    template<>
    bool ReadBuiltin< std::string >( const std::string& content, core::Model& model )
    {
        model = content;
        return true;
    }
    typedef bool ( *T_Matcher )( const std::string&, core::Model& );
    const std::map< std::string, T_Matcher > matcher = boost::assign::map_list_of
        ( "xs:string"             , &ReadBuiltin< std::string > )
        ( "xs:boolean"            , &ReadBuiltin< bool > )
        ( "xs:float"              , &ReadBuiltin< float > )
        ( "xs:double"             , &ReadBuiltin< double > )
        ( "xs:byte"               , &ReadBuiltin< int16_t > )
        ( "xs:short"              , &ReadBuiltin< int16_t > )
        ( "xs:int"                , &ReadBuiltin< int32_t > )
        ( "xs:integer"            , &ReadBuiltin< int32_t > )
        ( "xs:decimal"            , &ReadBuiltin< int32_t > )
        ( "xs:long"               , &ReadBuiltin< int64_t > )
        ( "xs:unsignedByte"       , &ReadBuiltin< uint16_t > )
        ( "xs:unsignedShort"      , &ReadBuiltin< uint16_t > )
        ( "xs:unsignedInt"        , &ReadBuiltin< uint32_t > )
        ( "xs:unsignedLong"       , &ReadBuiltin< uint64_t > )
        ( "xs:positiveInteger"    , &ReadBuiltin< int32_t > )
        ( "xs:negativeInteger"    , &ReadBuiltin< int32_t > )
        ( "xs:nonPositiveInteger" , &ReadBuiltin< int32_t > )
        ( "xs:nonNegativeInteger" , &ReadBuiltin< int32_t > )
        ( "xs:duration"           , &ReadNotImplementedType )
        ( "xs:dateTime"           , &ReadNotImplementedType )
        ( "xs:time"               , &ReadNotImplementedType )
        ( "xs:date"               , &ReadNotImplementedType )
        ( "xs:gYearMonth"         , &ReadNotImplementedType )
        ( "xs:gYear"              , &ReadNotImplementedType )
        ( "xs:gMonthDay"          , &ReadNotImplementedType )
        ( "xs:gDay"               , &ReadNotImplementedType )
        ( "xs:gMonth"             , &ReadNotImplementedType )
        ( "xs:hexBinary"          , &ReadNotImplementedType )
        ( "xs:base64Binary"       , &ReadNotImplementedType )
        ( "xs:anyURI"             , &ReadNotImplementedType )
        ( "xs:QName"              , &ReadNotImplementedType )
        ( "xs:NOTATION"           , &ReadNotImplementedType )
        ( "xs:normalizedString"   , &ReadNotImplementedType )
        ( "xs:token"              , &ReadNotImplementedType )
        ( "xs:language"           , &ReadNotImplementedType )
        ( "xs:NMTOKEN"            , &ReadNotImplementedType )
        ( "xs:NMTOKENS"           , &ReadNotImplementedType )
        ( "xs:Name"               , &ReadNotImplementedType )
        ( "xs:NCName"             , &ReadNotImplementedType )
        ( "xs:ID"                 , &ReadNotImplementedType )
        ( "xs:IDREF"              , &ReadNotImplementedType )
        ( "xs:IDREFS"             , &ReadNotImplementedType )
        ( "xs:ENTITY"             , &ReadNotImplementedType )
        ( "xs:ENTITIES"           , &ReadNotImplementedType );

    const std::string& ResolveType( const std::string& type, const core::Model& schema )
    {
        std::size_t antiCycleProtection = 1337;
        auto it = matcher.find( type );
        while( it == matcher.end() && --antiCycleProtection )
            it = matcher.find( schema[ "types" ][ type ][ "restriction/base" ] );
        return it->first;
    }
    void ReadSimpleRestriction( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "restriction" ) )
            return;
        core::Model value;
        matcher.at( ResolveType( type[ "restriction/base" ], schema ) )( xis.value< std::string >(), value );
        if( type[ "restriction" ].HasChild( "enumeration" ) )
        {
            const core::Model& enumeration = type[ "restriction/enumeration" ];
            for( std::size_t index = 0; index < enumeration.GetSize(); ++index )
                if( enumeration.GetElement( index ) == static_cast< std::string >( value ) )
                    node[ "value" ] = index;
        }
        else
            node[ "value" ] = value;
    }
    void ReadSimpleUnion( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "union" ) )
            return;
        for( std::size_t index = 0; index != type[ "union" ].GetSize(); ++index )
        {
            const std::string builtinType = ResolveType( type[ "union" ].GetElement( index ), schema );
            core::Model value;
            if( matcher.at( builtinType )( xis.value< std::string >(), value ) )
            {
                node[ xsd::XsdTypeToProtoType( builtinType ) + "-value" ] = value;
                return;
            }
        }
    }
    void ReadSimpleType( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        ReadSimpleRestriction( xis, schema, type, node );
        ReadSimpleUnion( xis, schema, type, node );
    }
    void ReadAttribute( const std::string& key, const core::Model& child,
                        xml::xistream& xis, const core::Model& schema, core::Model& node )
    {
        core::Model value;
        if( child.HasChild( "fixed" ) )
            value = child[ "fixed" ];
        if( child.HasChild( "default" ) )
            value = child[ "default" ];
        if( ( child.HasChild( "use" ) && child[ "use" ] == "required" ) || xis.has_attribute( key ) )
            matcher.at( ResolveType( child[ "type" ], schema ) )( xis.attribute< std::string >( key ), value );
        node[ key ] = value;
    }
    class KeyVisitor : public core::ModelVisitor_ABC
    {
    public:
        KeyVisitor( core::Model& indexedList, const core::Model& child )
            : indexedList_( indexedList )
            , child_      ( child )
        {}
        virtual void Visit( int64_t key )
        {
            indexedList_[ boost::numeric_cast< std::size_t >( key ) ].Link( child_ );
        }
        virtual void Visit( uint64_t key )
        {
            indexedList_[ boost::numeric_cast< std::size_t >( key ) ].Link( child_ );
        }
        virtual void Visit( const std::string& key )
        {
            indexedList_[ key ].Link( child_ );
        }
        virtual void Visit( double ) { throw MASA_EXCEPTION( "Cannot index value with 'double' key" ); }
        virtual void Visit( const std::string&, const core::Model& ) {}
        virtual void Visit( std::size_t, const core::Model& ) {}
        virtual void Visit( const core::Model& ) {}
        virtual void Visit( const boost::shared_ptr< core::UserData_ABC >& ) {}
    private:
        core::Model& indexedList_;
        const core::Model& child_;
    };

    void ReadConstraint( const std::string& keyName, const core::Model& constraint, core::Model& node )
    {
        if( constraint[ "type" ] != "key" )
            return;
        const std::string& selector = constraint[ "selector" ];
        const std::string field = boost::erase_first_copy( static_cast< std::string >( constraint[ "field" ] ), "@" );
        const core::Model& children = node[ "elements" ][ selector ][ "list" ];
        core::Model& indexedList = node[ "elements" ][ selector ][ keyName ];
        for( std::size_t i = 0; i < children.GetSize(); ++i )
        {
            const core::Model& child = children.GetElement( i );
            const core::Model& key = child[ "attributes" ][ field ];
            KeyVisitor visitor( indexedList, child );
            key.Accept( visitor );
        }
    }
    void ReadElementList( xml::xistream& xis, const core::Model& schema, const std::string& type, core::Model& node )
    {
        ReadNode( xis, schema, type, node.AddElement() );
    }
    void ReadConstraints( const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "constraints" ) )
            return;
        core::NamedChildrenVisitor( type[ "constraints" ], boost::bind( &ReadConstraint, _1, _2, boost::ref( node ) ) );
    }
    void ReadSequenceElement( const std::string& key, const core::Model& child,
                              xml::xistream& xis, const core::Model& schema, core::Model& node )
    {
        if( child.HasChild( "maxOccurs" ) && ( child[ "maxOccurs" ] > 1 || child[ "maxOccurs" ] == -1 ) )
            xis >> xml::list( key, boost::bind( &ReadElementList, _1, boost::cref( schema ), boost::cref( child[ "type" ] ), boost::ref( node[ "elements" ][ key ][ "list" ] ) ) );
        else
            xis >> xml::list( key, boost::bind( &ReadNode, _1, boost::cref( schema ), boost::cref( child[ "type" ] ), boost::ref( node[ "elements" ][ key ] ) ) );
        ReadConstraints( child, node[ "elements" ][ key ] );
    }
    void ReadChoiceElement( const std::string& key, const core::Model& child,
                            xml::xistream& xis, const core::Model& schema, core::Model& node )
    {
        if( !xis.has_child( key ) )
            return;
        ReadSequenceElement( key, child, xis, schema, node );
    }
    void ReadAttributes( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "attributes" ) )
            return;
        core::NamedChildrenVisitor( type[ "attributes" ], boost::bind( &ReadAttribute, _1, _2, boost::ref( xis ), boost::cref( schema ), boost::ref( node[ "attributes" ] ) ) );
    }
    void ReadSequence( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "sequence" ) )
            return;
        core::NamedChildrenVisitor( type[ "sequence" ], boost::bind( &ReadSequenceElement, _1, _2, boost::ref( xis ), boost::cref( schema ), boost::ref( node ) ) );
    }
    void ReadChoice( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "choice" ) )
            return;
        core::NamedChildrenVisitor( type[ "choice" ], boost::bind( &ReadChoiceElement, _1, _2, boost::ref( xis ), boost::cref( schema ), boost::ref( node ) ) );
    }
    void ReadAll( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        if( !type.HasChild( "all" ) )
            return;
        core::NamedChildrenVisitor( type[ "all" ], boost::bind( &ReadChoiceElement, _1, _2, boost::ref( xis ), boost::cref( schema ), boost::ref( node ) ) );
    }
    void ReadComplexType( xml::xistream& xis, const core::Model& schema, const core::Model& type, core::Model& node )
    {
        ReadAttributes( xis, schema, type, node );
        ReadSequence( xis, schema, type, node );
        ReadChoice( xis, schema, type, node );
        ReadAll( xis, schema, type, node );
    }
    void ReadNode( xml::xistream& xis, const core::Model& schema, const std::string& type, core::Model& node )
    {
        const core::Model& typeNode = schema[ "types" ][ type ];
        const std::string& kind = typeNode[ "type" ];
        if( kind == "simple" )
            ReadSimpleType( xis, schema, typeNode, node );
        else
        {
            if( kind != "complex" )
                throw MASA_EXCEPTION( "type kind must be 'complex' or 'simple' instead of " + kind );
            ReadComplexType( xis, schema, typeNode, node );
        }
    }
    void ReadRootNode( const std::string& node, xml::xistream& xis, const core::Model& schema, core::Model& model )
    {
        ReadNode( xis, schema, schema[ "elements" ][ node ][ "type" ], model[ node ] );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlToModel
// Created: ABR 2013-08-14
// -----------------------------------------------------------------------------
void xsd::XmlToModel( const core::Model& schema, const tools::Path& input, core::Model& model )
{
    xml::xifstream xis( input.ToUTF8() );
    XmlToModel( schema, xis, model );
}

// -----------------------------------------------------------------------------
// Name: XmlToModel
// Created: ABR 2013-08-14
// -----------------------------------------------------------------------------
void xsd::XmlToModel( const core::Model& schema, xml::xistream& xis, core::Model& model )
{
    xis >> xml::list( boost::bind( &ReadRootNode, _2, _3, boost::cref( schema ), boost::ref( model )) );
}
