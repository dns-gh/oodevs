// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Helpers.h"
#include <tools/Exception.h>
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <map>

namespace
{
    const std::map< std::string, std::string > matcher = boost::assign::map_list_of
        // Primitive datatypes
        ( "xs:string"       , "string" )
        ( "xs:boolean"      , "bool" )
        ( "xs:decimal"      , "int32" )
        ( "xs:float"        , "float" )
        ( "xs:double"       , "double" )
        ( "xs:duration"     , "" )
        ( "xs:dateTime"     , "" )
        ( "xs:time"         , "" )
        ( "xs:date"         , "" )
        ( "xs:gYearMonth"   , "" )
        ( "xs:gYear"        , "" )
        ( "xs:gMonthDay"    , "" )
        ( "xs:gDay"         , "" )
        ( "xs:gMonth"       , "" )
        ( "xs:hexBinary"    , "" )
        ( "xs:base64Binary" , "" )
        ( "xs:anyURI"       , "" )
        ( "xs:QName"        , "" )
        ( "xs:NOTATION"     , "" )

        // Derived datatypes
        ( "xs:normalizedString"    , "" )
        ( "xs:token"               , "" )
        ( "xs:language"            , "" )
        ( "xs:NMTOKEN"             , "" )
        ( "xs:NMTOKENS"            , "" )
        ( "xs:Name"                , "" )
        ( "xs:NCName"              , "" )
        ( "xs:ID"                  , "" )
        ( "xs:IDREF"               , "" )
        ( "xs:IDREFS"              , "" )
        ( "xs:ENTITY"              , "" )
        ( "xs:ENTITIES"            , "" )
        ( "xs:integer"             , "int32" )
        ( "xs:nonPositiveInteger"  , "int32" )
        ( "xs:negativeInteger"     , "int32" )
        ( "xs:long"                , "int64" )
        ( "xs:int"                 , "int32" )
        ( "xs:short"               , "" )
        ( "xs:byte"                , "" )
        ( "xs:nonNegativeInteger"  , "int32" )
        ( "xs:unsignedLong"        , "uint64" )
        ( "xs:unsignedInt"         , "uint32" )
        ( "xs:unsignedShort"       , "" )
        ( "xs:unsignedByte"        , "" )
        ( "xs:positiveInteger"     , "" )
        ;
}

bool xsd::IsBuiltinXsdType( const std::string& type )
{
    return matcher.find( type ) != matcher.end();
}

std::string xsd::XsdTypeToProtoType( const std::string& type )
{
    auto it = matcher.find( type );
    if( it == matcher.end() )
        return type;
    if( it->second.empty() )
        throw MASA_EXCEPTION( "Xsd Type not implemented: " + type );
    assert( !it->second.empty() );
    return it->second;
}

std::string xsd::XsdNameToProtoName( const std::string& name )
{
    return boost::replace_all_copy( name, "-", "_" );
}

std::string xsd::ProtoNameToXsdName( const std::string& name )
{
    return boost::replace_all_copy( name, "_", "-" );
}

std::string xsd::XsdEnumToProtoEnum( const std::string& name )
{
    return boost::replace_all_copy( name, " ", "_" );
}

std::string xsd::ProtoEnumToXsdEnum( const std::string& name )
{
    return boost::replace_all_copy( name, "_", " " );
}
