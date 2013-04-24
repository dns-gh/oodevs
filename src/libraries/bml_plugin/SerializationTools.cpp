// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.hpp>

namespace plugins
{
namespace bml
{
    xml::xostream& operator<<( xml::xostream& xos, const Namespaces& )
    {
        xos << xml::attribute( "xmlns", "urn:sisostds:bml:coalition:draft:cbml:1" )
            << xml::attribute( "xmlns:jc3iedm", "http://www.mip-site.org" )
            << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
        return xos;
    }

//    std::string NS( const std::string& tag, const std::string& preferred )
//    {
//        if( preferred == "jc3iedm" )
//            return "jc3iedm:" + tag;
//        if( preferred == "xsi" )
//            return "xsi:" + tag;
//        return tag;
//    }

    std::string NS( const std::string& tag, const std::string& preferred )
    {
        if( preferred == "cbml" )
            return "ns2:" + tag;
        if( preferred == "xsi" )
            return "xsi:" + tag;
        return tag;
    }
}
}