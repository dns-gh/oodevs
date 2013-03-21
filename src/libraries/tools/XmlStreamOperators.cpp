// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "XmlStreamOperators.h"
#include <tools/Path.h>
#include <xeumeuleu/xml.h>

xml::xostream& tools::operator<<( xml::xostream& xos, const tools::Path& path )
{
    std::string text = path.ToUTF8();
    std::replace( text.begin(), text.end(), '\\', '/' );
    return xos << text;
}

xml::xistream& tools::operator>>( xml::xistream& xis, tools::Path& path )
{
    std::string tmp;
    xis >> tmp;
    path = tools::Path::FromUTF8( tmp );
    return xis;
}
