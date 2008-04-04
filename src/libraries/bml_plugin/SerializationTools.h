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

namespace bml
{
    struct UtmLocation
    {
        UtmLocation( const char* utm, int height );
        UtmLocation( double latitude, double longitude, int height );
        std::string gridZone_, easting_, northing_;
        int height_;
    };

    xml::xostream& operator<<( xml::xostream& xos, const UtmLocation& location );
}

#endif // __SerializationTools_h_
