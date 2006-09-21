// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticRouteAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::LogisticRouteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
LogisticRouteAttributes::~LogisticRouteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Itinéraire logistique" ) )
                .Display( tools::translate( "Object", "Equipé:" ), equipped_ )
                .Display( tools::translate( "Object", "Débit:" ), flow_ * Units::vehiclesPerHour )
                .Display( tools::translate( "Object", "Largeur:" ), width_ * Units::meters )
                .Display( tools::translate( "Object", "Longueur:" ), length_ * Units::meters )
                .Display( tools::translate( "Object", "Poids supporté:" ), maxWeight_ * Units::tons );
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetFlow
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetFlow( unsigned int value )
{
    flow_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetWidth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetWidth( unsigned int value )
{
    width_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetLength
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetLength( unsigned int value )
{
    length_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetMaxWeight
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetMaxWeight( unsigned int value )
{
    maxWeight_ = value;
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::SetEquipped
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::SetEquipped( bool value )
{
    equipped_ = value;
}

// -----------------------------------------------------------------------------
// Name: LogisticRouteAttributes::DoSerialize
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticRouteAttributes::DoSerialize( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << content( "flow", int( flow_ ) )
            << content( "width", int( width_ ) )
            << content( "length", int( length_ ) )
            << content( "max-weight", int( maxWeight_ ) )
            << content( "equipped", equipped_ )
        << end();
}
