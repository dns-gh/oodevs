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
    xml::xostream& operator<<( xml::xostream& xos, const Namespaces& )
    {
        xos << xml::attribute( "xmlns", "urn:sisostds:bml:coalition:draft:cbml:1" )
            << xml::attribute( "xmlns:jc3iedm", "http://www.mip-site.org" )
            << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
        return xos;
    }
}
