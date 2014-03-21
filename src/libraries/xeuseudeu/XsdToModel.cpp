// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "XsdToModel.h"
#include "core/Model.h"
#include "core/NamedChildrenVisitor.h"
#include <tools/Path.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#pragma warning( push )
#pragma warning( disable: 4180 )
#include <boost/assign/list_of.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <map>

using namespace xsd;

namespace
{
    // -----------------------------------------------------------------------------
    // Constraints
    // -----------------------------------------------------------------------------
    void ReadConstraint( xml::xisubstream xis, core::Model& element, const std::string& type )
    {
        const std::string name = xis.attribute< std::string >( "name" );
        std::string selector;
        std::string field;
        core::Model& constraint = element[ "constraints" ][ name ];
        constraint[ "type" ] = type;
        xis >> xml::start( "selector" )
                >> xml::attribute( "xpath", selector )
            >> xml::end
            >> xml::start( "field" )
                >> xml::attribute( "xpath", field )
            >> xml::end;
        constraint[ "selector" ] = selector;
        constraint[ "field" ] = field;
        if( type == "keyref" )
             constraint[ "refer" ] = xis.attribute< std::string >( "refer" );
    }
    void ReadConstraints( xml::xisubstream xis, core::Model& element )
    {
        xis >> xml::list( "key", boost::bind( &ReadConstraint, _1, boost::ref( element ), "key" ) )
            >> xml::list( "unique", boost::bind( &ReadConstraint, _1, boost::ref( element ), "unique" ) )
            >> xml::list( "keyref", boost::bind( &ReadConstraint, _1, boost::ref( element ), "keyref" ) );
    }

    // -----------------------------------------------------------------------------
    // Annotation
    // -----------------------------------------------------------------------------
    void ReadDocumentation( xml::xisubstream xis, core::Model& model )
    {
        const std::string language = xis.attribute( "lang", "default" );
        model[ "annotations" ][ language ] = xis.value< std::string >();
    }
    void ReadAnnotation( xml::xisubstream xis, core::Model& model )
    {
        xis >> xml::optional >> xml::start( "annotation" )
                >> xml::list( "documentation", boost::bind( &ReadDocumentation, _1, boost::ref( model ) ) );
    }

    // -----------------------------------------------------------------------------
    // Built-in type
    // -----------------------------------------------------------------------------
    template< typename T >
    void ReadBuiltin( xml::xisubstream xis, const std::string& attribute, core::Model& model )
    {
        model = xis.attribute< T >( attribute );
    }
    typedef void ( *T_Matcher )( xml::xisubstream, const std::string&, core::Model& );
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
        ( "xs:nonNegativeInteger" , &ReadBuiltin< int32_t > );

    std::string ResolveType( const std::map< std::string, std::string >& resolver, const std::string& type )
    {
        return matcher.find( type ) != matcher.end() ? type : resolver.at( type );
    }

    // -----------------------------------------------------------------------------
    // Simple type
    // -----------------------------------------------------------------------------
    void ReadEnumeration( xml::xisubstream xis, const std::map< std::string, std::string >& resolver, core::Model& restriction, const std::string& base )
    {
        matcher.at( ResolveType( resolver, base ) )( xis, "value", restriction[ "enumeration" ].AddElement() );
    }
    template< typename T >
    void ReadFacet( xml::xisubstream xis, core::Model& restriction, const std::string& facet )
    {
        ReadBuiltin< T >( xis, "value", restriction[ facet ] );
    }
    void ReadFacet( xml::xisubstream xis, const std::map< std::string, std::string >& resolver, core::Model& restriction, const std::string& facet, const std::string& base )
    {
        matcher.at( ResolveType( resolver, base ) )( xis, "value", restriction[ facet ] );
    }
    void ReadRestriction( xml::xisubstream xis, const std::map< std::string, std::string >& resolver, core::Model& type )
    {
        std::string base = xis.attribute< std::string >( "base" );
        core::Model& restriction = type[ "restriction" ];
        restriction[ "base" ] = base;
        xis >> xml::list( "enumeration" , boost::bind( &ReadEnumeration, _1, boost::cref( resolver ), boost::ref( restriction ), boost::cref( base ) ) )
            >> xml::list( "pattern"     , boost::bind( &ReadFacet< std::string >, _1, boost::ref( restriction ), "pattern" ) )
            >> xml::list( "length"      , boost::bind( &ReadFacet< uint32_t >, _1, boost::ref( restriction ), "length" ) )
            >> xml::list( "maxLength"   , boost::bind( &ReadFacet< uint32_t >, _1, boost::ref( restriction ), "maxLength" ) )
            >> xml::list( "minLength"   , boost::bind( &ReadFacet< uint32_t >, _1, boost::ref( restriction ), "minLength" ) )
            >> xml::list( "maxExclusive", boost::bind( &ReadFacet, _1, boost::cref( resolver ), boost::ref( restriction ), "maxExclusive", base ) )
            >> xml::list( "maxInclusive", boost::bind( &ReadFacet, _1, boost::cref( resolver ), boost::ref( restriction ), "maxInclusive", base ) )
            >> xml::list( "minExclusive", boost::bind( &ReadFacet, _1, boost::cref( resolver ), boost::ref( restriction ), "minExclusive", base ) )
            >> xml::list( "minInclusive", boost::bind( &ReadFacet, _1, boost::cref( resolver ), boost::ref( restriction ), "minInclusive", base ) );
    }
    void ReadList( xml::xisubstream xis, core::Model& type )
    {
        type[ "list" ] = xis.attribute< std::string >( "itemType" );
    }
    void ReadUnion( xml::xisubstream xis, core::Model& type )
    {
        const std::string memberTypes = xis.attribute< std::string >( "memberTypes" );
        std::vector< std::string > buffer;
        boost::split( buffer, memberTypes, boost::algorithm::is_space() );
        BOOST_FOREACH( const std::string& member, buffer )
            type[ "union" ].AddElement() = member;
    }
    void ReadSimpleType( xml::xisubstream xis, const std::map< std::string, std::string >& resolver, core::Model& types )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        xis >> xml::list( "restriction", boost::bind( &ReadRestriction, _1, boost::cref( resolver ), boost::ref( types[ name ] ) ) )
            >> xml::list( "union", boost::bind( &ReadUnion, _1, boost::ref( types[ name ] ) ) )
            >> xml::list( "list", boost::bind( &ReadList, _1, boost::ref( types[ name ] ) ) );
        ReadAnnotation( xis, types[ name ] );
        types[ name ][ "type" ] = "simple";
    }

    // -----------------------------------------------------------------------------
    // Complex type
    // -----------------------------------------------------------------------------
    void ReadAttribute( xml::xisubstream xis, const std::map< std::string, std::string >& resolver, core::Model& type )
    {
        std::string name;
        std::string typeStr;
        std::string use;
        xis >> xml::attribute( "name", name )
            >> xml::attribute( "type", typeStr )
            >> xml::optional >> xml::attribute( "use", use );
        core::Model& attribute = type[ "attributes" ][ name ];
        attribute[ "type" ] = typeStr;
        if( !use.empty() )
            attribute[ "use" ] = use;
        if( xis.has_attribute( "default" ) )
            matcher.at( ResolveType( resolver, typeStr ) )( xis, "default", attribute[ "default" ] );
        if( xis.has_attribute( "fixed" ) )
            matcher.at( ResolveType( resolver, typeStr ) )( xis, "fixed", attribute[ "fixed" ] );
        ReadAnnotation( xis, attribute );
    }
    void ReadElement( xml::xisubstream xis, core::Model& elements )
    {
        std::string name;
        std::string typeStr;
        xis >> xml::attribute( "name", name )
            >> xml::attribute( "type", typeStr );
        core::Model& element = elements[ name ];
        if( xis.has_attribute( "minOccurs" ) )
            element[ "minOccurs" ] = xis.attribute< int >( "minOccurs" );
        if( xis.has_attribute( "maxOccurs" ) )
        {
            const std::string maxOccurs = xis.attribute< std::string >( "maxOccurs" );
            element[ "maxOccurs" ] = maxOccurs == "unbounded" ? -1 : boost::lexical_cast< int >( maxOccurs );
        }
        ReadAnnotation( xis, element );
        ReadConstraints( xis, element );
        element[ "type" ] = typeStr;
    }
    void ReadElements( xml::xisubstream xis, core::Model& type, const std::string& nodeName )
    {
        xis >> xml::list( "element", boost::bind( &ReadElement, _1, boost::ref( type[ nodeName ] ) ) );
    }
    void ReadComplexType( xml::xisubstream xis, const std::map< std::string, std::string >& resolver, core::Model& types )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        xis >> xml::list( "attribute", boost::bind( &ReadAttribute, _1, boost::cref( resolver ), boost::ref( types[ name ] ) ) )
            >> xml::list( "all", boost::bind( &ReadElements, _1, boost::ref( types[ name ] ), "all" ) )
            >> xml::list( "choice", boost::bind( &ReadElements, _1, boost::ref( types[ name ] ), "choice" ) )
            >> xml::list( "sequence", boost::bind( &ReadElements, _1, boost::ref( types[ name ] ), "sequence" ) );
        ReadAnnotation( xis, types[ name ] );
        types[ name ][ "type" ] = "complex";
    }

    // -----------------------------------------------------------------------------
    // Include
    // -----------------------------------------------------------------------------
    void LinkTypeFunctor( core::Model& dest, const std::string& key, const core::Model& source )
    {
        dest[ key ].Link( source );
    }
    void ReadInclude( xml::xisubstream xis, core::Model& model, const std::string& schemaName, const tools::Path& currentDirectory )
    {
        const tools::Path includedPath = tools::Path::FromUTF8( xis.attribute< std::string >( "schemaLocation" ) );
        model[ "schemas" ][ schemaName ][ "includes" ][ includedPath.ToUTF8() ];
        xsd::XsdToModel( currentDirectory / includedPath, model );
        core::NamedChildrenVisitor( model[ "schemas" ][ includedPath.BaseName().ToUTF8() ][ "types" ],
                                    boost::bind( &LinkTypeFunctor, boost::ref( model[ "schemas" ][ schemaName ][ "types" ] ), _1, _2 ) );
    }

    // -----------------------------------------------------------------------------
    // Simple type resolver
    // -----------------------------------------------------------------------------
    void ResolveAllSimpleType( const tools::Path& schema, std::map< std::string, std::string >& resolver );
    void ResolveExternalSimpleType( xml::xisubstream xis, std::map< std::string, std::string >& resolver, const tools::Path& base )
    {
        const std::string schemaLocation = xis.attribute< std::string >( "schemaLocation" );
        const tools::Path schema = tools::Path::FromUTF8( schemaLocation );
        ResolveAllSimpleType( base / schema, resolver );
    }
    void ResolveSimpleType( xml::xisubstream xis, std::map< std::string, std::string >& resolver, bool& modified )
    {
        const std::string name = xis.attribute< std::string >( "name" );
        if( resolver.find( name ) != resolver.end() )
            return;
        std::string type;
        if( xis.has_child( "restriction" ) )
            xis >> xml::start( "restriction" ) >> xml::attribute( "base", type );
        else if( xis.has_child( "union" ) )
            xis >> xml::start( "union" ) >> xml::attribute( "memberTypes", type );
        else if( xis.has_child( "list" ) )
            xis >> xml::start( "list" ) >> xml::attribute( "itemType", type );
        else
            assert( false );
        if( std::find_if( type.begin(), type.end(), boost::algorithm::is_space() ) != type.end() ) // TODO: Handle union of SimpleType
        {
            std::vector< std::string > types;
            boost::split( types, type, boost::algorithm::is_space() );
            type = *types.begin();
        }
        if( matcher.find( type ) != matcher.end() ) // if target type is a built-in
        {
            resolver[ name ] =  type;
            modified = true;
        }
        else if( resolver.find( type ) != resolver.end() ) // else if it's a known type
        {
            resolver[ name ] = resolver.at( type );
            modified = true;
        }
    }
    void ResolveAllSimpleType( const tools::Path& schema, std::map< std::string, std::string >& resolver )
    {
        xml::xifstream xis( schema.ToUTF8().c_str() );
        xis >> xml::start( "schema" )
                >> xml::list( "include", boost::bind( &ResolveExternalSimpleType, _1, boost::ref( resolver ), boost::cref( schema.Parent() ) ) );
        bool modified = true;
        while( modified )
        {
            modified = false;
            xis >> xml::list( "simpleType", boost::bind( &ResolveSimpleType, _1, boost::ref( resolver ), boost::ref( modified ) ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: xsd::XsdToModel
// Created: ABR 2013-08-09
// -----------------------------------------------------------------------------
void xsd::XsdToModel( const tools::Path& schema, core::Model& model )
{
    xml::xifstream xis( schema.ToUTF8().c_str() );
    const std::string schemaName = schema.BaseName().ToUTF8();
    std::map< std::string, std::string > resolver;
    ResolveAllSimpleType( schema, resolver );
    xis >> xml::start( "schema" )
            >> xml::list( "include", boost::bind( &ReadInclude, _1, boost::ref( model ), boost::cref( schemaName ), boost::cref( schema.Parent() ) ) )
            >> xml::list( "simpleType", boost::bind( &ReadSimpleType, _1, boost::cref( resolver ), boost::ref( model[ "schemas" ][ schemaName ][ "types" ] ) ) )
            >> xml::list( "complexType", boost::bind( &ReadComplexType, _1, boost::cref( resolver ), boost::ref( model[ "schemas" ][ schemaName ][ "types" ] ) ) );
    ReadElements( xis, model, "schemas/" + schemaName + "/elements" );
    ReadAnnotation( xis, model[ "schemas" ][ schemaName ] );
}
