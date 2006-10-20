// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "CrossingSiteAttributes.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::CrossingSiteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
CrossingSiteAttributes::CrossingSiteAttributes( xml::xistream& xis )
{
    xis >> start( "specific-attributes" )
            >> content( "width", (int&)width_ )
            >> content( "depth", (int&)depth_ )
            >> content( "speed", (int&)speed_ )
            >> content( "construction-needed", (bool&)needsConstruction_ )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
CrossingSiteAttributes::~CrossingSiteAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Site de franchissement" ) )
                .Display( tools::translate( "Object", "Largeur:" ), width_ * Units::meters )
                .Display( tools::translate( "Object", "Profondeur:" ), depth_ * Units::meters )
                .Display( tools::translate( "Object", "Vitesse du courant:" ), speed_ * Units::metersPerSecond )
                .Display( tools::translate( "Object", "Berges à aménager:" ), needsConstruction_ );
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetWidth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetWidth( unsigned int value )
{
    width_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetDepth
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetDepth( unsigned int value )
{
    depth_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetFlowSpeed
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetFlowSpeed( unsigned int value )
{
    speed_ = value;
}
    
// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::SetConstruction
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::SetConstruction( bool need )
{
    needsConstruction_ = need;
}

// -----------------------------------------------------------------------------
// Name: CrossingSiteAttributes::DoSerialize
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void CrossingSiteAttributes::DoSerialize( xml::xostream& xos ) const
{
    xos << start( "specific-attributes" )
            << content( "width", int( width_ ) )
            << content( "depth", int( depth_ ) )
            << content( "speed", int( speed_ ) )
            << content( "construction-needed", needsConstruction_ )
        << end();
}
