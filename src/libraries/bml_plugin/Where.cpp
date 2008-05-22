// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Where.h"
#include "Point.h"
#include "dispatcher/Agent.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: Where constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Where::Where( const dispatcher::Agent& agent )
    : agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Where destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Where::~Where()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
xml::xostream& bml::operator<<( xml::xostream& xos, const Where& data )
{
    xos << xml::start( "C_BML_Where" )
            << xml::start( "WhereInstance" );
    Point point( data.agent_.position_.latitude, data.agent_.position_.longitude );
    point.Serialize( xos );
    xos     << xml::end()
        << xml::end();
    return xos;
}
