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
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.h>

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
    attributes_[ name ] = xml::attribute< std::string >( xis, name );
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
        xos << xml::end();
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
        return path.length() && path[ 0 ] == '@';
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
