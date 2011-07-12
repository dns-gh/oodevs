// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "XmlNode.h"
#pragma warning( push )
#pragma warning( disable: 4800 )
#include <boost/bind.hpp>
#pragma warning( pop )
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.hpp>

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
            >> xml::optional >> text_;
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
        >> xml::optional >> text_;
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
    attributes_[ name ] = xis.value< std::string >();
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
        return boost::lexical_cast< bool >( str );
    }
    else
        throw std::exception( "XmlNode impossible to find path" );
}

// -----------------------------------------------------------------------------
// Name: XmlNode::RemoveNode
// Created: RPD 2011-03-29
// -----------------------------------------------------------------------------
bool XmlNode::RemoveNode( const std::string& path )
{
    XmlNode* node = GetChildNode( path );
    if( !node )
        return false;

    XmlNode* parentNode = GetChildNode( path, true );
    if( !parentNode )
        return false;

    for( T_Nodes::iterator it = parentNode->children_.begin(); it != parentNode->children_.end(); ++it )
    {
        if( it->second == node )
        {
            parentNode->children_.erase( it );
            return true;
        }
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: XmlNode::HasNode
// Created: RPD 2011-03-30
// -----------------------------------------------------------------------------
bool XmlNode::HasNode( const std::string& path )
{
    if( IsAttribute( path ) && attributes_.find( path.substr( 1, path.size() - 1 ) ) != attributes_.end() )
        return true;
    std::string element = NextElement( path );
    T_Nodes::iterator it = children_.find( element );
    if( element.size() && it != children_.end() )
    {
        element = element != path ? path.substr( element.length() + 1 ) : "";
        if( !element.size() || it->second->HasNode( element ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: XmlNode::GetChildNode
// Created: RPD 2011-04-12
// -----------------------------------------------------------------------------
XmlNode* XmlNode::GetChildNode( const std::string& path, bool parent /*= false*/ )
{
    std::string element = NextElement( path );
    T_Nodes::iterator it = children_.find( element );
    if( element.size() && it != children_.end() )
    {
        element = element != path ? path.substr( element.length() + 1 ) : "";
        if( !element.size() )
            return parent? this : it->second;
        return it->second->GetChildNode( element, parent );
    }
    return 0;
}