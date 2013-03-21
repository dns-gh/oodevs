// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __XmlStreamOperators_h_
#define __XmlStreamOperators_h_

namespace xml
{
    class xistream;
    class xostream;
}

namespace tools
{
    class Path;

    xml::xostream& operator<<( xml::xostream& xos, const tools::Path& path );
    xml::xistream& operator>>( xml::xistream& xis, tools::Path& path );
}

#endif // __XmlStreamOperators_h_
