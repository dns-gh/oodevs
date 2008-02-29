// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

namespace bml
{
    UtmLocation::UtmLocation( const char* utm, int height )
        : height_( height )
    {
        const std::string str( utm, 15 );
        gridZone_ = str.substr( 0, 3 );
        easting_  = str.substr( 3, 6 );
        northing_ = str.substr( 7, 6 );
    }

    xml::xostream& operator<<( xml::xostream& xos, const UtmLocation& location )
    {
        xos << xml::start( "msdl:UTM" )
                << xml::content( "msdl:UTMGridZone" , location.gridZone_ )
                << xml::content( "msdl:UTMEasting"  , location.easting_ )
                << xml::content( "msdl:UTMNorthing" , location.northing_ )
                << xml::content( "msdl:ElevationAGL", location.height_ )
            << xml::end();
        return xos;
    }
}