// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Where.h"
#include "Point.h"
#include "dispatcher/Agent_ABC.h"
#include <xeumeuleu/xml.h>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: Where constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Where::Where( const dispatcher::Agent_ABC& agent )
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
xml::xostream& plugins::bml::operator<<( xml::xostream& xos, const Where& data )
{
    xos << xml::start( "C_BML_Where" )
            << xml::start( "WhereInstance" );
    Point point( data.agent_.GetPosition().X(), data.agent_.GetPosition().Y() );
    point.Serialize( xos, "AbstractAbsolutePoint" );
    xos     << xml::end
        << xml::end;
    return xos;
}
