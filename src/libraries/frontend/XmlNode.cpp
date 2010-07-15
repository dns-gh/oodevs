// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "XmlNode.h"
#pragma warning( push )
#pragma warning( disable: 4702 4800 )
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <cctype>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: XmlNode constructor
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
XmlNode::XmlNode( const std::string& filename )
{
    if( boost::filesystem::exists( filename ) )
    {
        xml::xifstream xis( filename );
        xis >> xml::attributes( *this, &XmlNode::ReadAttribute )
            >> xml::list( *this, &XmlNode::ReadChild )
            >> xml::optional() >> text_;
    }
}

// -----------------------------------------------------------------------------
// Name: XmlNode constructor
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
XmlNode::XmlNode( xml::xistream& xis )
{
    xis >> xml::attributes( *this, &XmlNode::ReadAttribute )
        >> xml::list( *this, &XmlNode::ReadChild )
        >> xml::optional() >> text_;
}

// -----------------------------------------------------------------------------
// Name: XmlNode constructor
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
XmlNode::XmlNode()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmlNode destructor
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
XmlNode::~XmlNode()
{
    for( T_Nodes::const_iterator it = children_.begin(); it != children_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: XmlNode::ReadAttribute
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void XmlNode::ReadAttribute( const std::string& name, xml::xistream& xis )
{
    attributes_[ name ] = xis.attribute< std::string >( name );
}

// -----------------------------------------------------------------------------
// Name: XmlNode::ReadChild
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void XmlNode::ReadChild( const std::string& name, xml::xistream& xis )
{
    children_[ name ] = new XmlNode( xis );
}

namespace
{
    bool IsCData( const std::string& text )
    {
        // $$$$ SBO 2008-02-27: not enough
        return std::find_if( text.begin(), text.end(), boost::bind( std::logical_not< bool >(), boost::bind( isprint, _1 ) ) ) != text.end();
    }
}

// -----------------------------------------------------------------------------
// Name: XmlNode::Serialize
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void XmlNode::Serialize( xml::xostream& xos ) const
{
    for( T_Attributes::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        xos << xml::attribute( it->first, it->second );
    for( T_Nodes::const_iterator it = children_.begin(); it != children_.end(); ++it )
    {
        xos << xml::start( it->first );
        it->second->Serialize( xos );
        xos << xml::end;
    }
    if( !text_.empty() )
        if( IsCData( text_ ) )
            xos << xml::cdata( text_ );
        else
            xos << text_;
}

namespace
{
    std::string NextElement( const std::string& path )
    {
        const size_t index = path.find_first_of( '/' );
        return index == path.npos ? path : path.substr( 0, index );
    }

    bool IsAttribute( const std::string& path )
    {
        return !path.empty() && path[ 0 ] == '@';
    }

    bool IsOptionalAttribute( const std::string& path )
    {
        return !path.empty() && path[ path.size() - 1 ] == '?';
    }

    std::string CleanAttribute( const std::string& path, bool optional )
    {
        return path.substr( 1, path.size() - ( optional ? 2 : 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: XmlNode::SetStringValue
// Created: SBO 2008-02-27
// -----------------------------------------------------------------------------
void XmlNode::SetStringValue( const std::string& path, const std::string& value )
{
    if( path.empty() || IsAttribute( path ) )
    {
        if( IsAttribute( path ) )
            attributes_[ path.substr( 1 ) ] = value;
        else
            text_ = value;
        return;
    }
    std::string element = NextElement( path );
    XmlNode*& node = children_[ element ];
    if( !node )
        node = new XmlNode();
    element = element != path ? path.substr( element.length() + 1 ) : "";
    node->SetStringValue( element, value );
}


// -----------------------------------------------------------------------------
// Name: XmlNode::GetStringValue
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
bool XmlNode::GetStringValue( const std::string& path, std::string& value ) const
{
    if( path.empty() || IsAttribute( path ) )
    {
        if( IsAttribute( path ) )
        {
            bool optional = IsOptionalAttribute( path );
            T_Attributes::const_iterator it = attributes_.find( CleanAttribute( path, optional ) ) ;
            if( it == attributes_.end() )
                return optional;
            else
               value = (*it).second;
        }
        else
            value = text_;
        return true ;
    }
    std::string element = NextElement( path );
    T_Nodes::const_iterator it = children_.find( element ) ;
    if( it == children_.end() )
        return false ;
    else
    {
        element = element != path ? path.substr( element.length() + 1 ) : "";
        return (*it).second->GetStringValue( element, value ) ;
    }
}

// -----------------------------------------------------------------------------
// Name: XmlNode::GetValue partial implementation
// Created: RDS 2008-08-19
// -----------------------------------------------------------------------------
template<>
bool XmlNode::GetValue<bool>( const std::string& path ) const
{
    std::string str ;
    if( GetStringValue( path, str ) )
    {
        std::transform( str.begin(), str.end(), str.begin(), std::tolower );
        if( str =="false" )
            return false ;
        if( str =="true" )
            return true ;
        return boost::lexical_cast<bool>(str);
    }
    else
        throw ( std::exception( "XmlNode impossible to find path" ) ) ;
}