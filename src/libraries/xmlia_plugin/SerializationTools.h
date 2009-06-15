// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SerializationTools_h_
#define __SerializationTools_h_

namespace xml
{
    class xostream;
}

namespace plugins
{
namespace xmlia
{
    struct Namespaces {};
    xml::xostream& operator<<( xml::xostream& xos, const Namespaces& );
    std::string NS( const std::string& tag, const std::string& preferred );
}
}

#endif // __SerializationTools_h_
