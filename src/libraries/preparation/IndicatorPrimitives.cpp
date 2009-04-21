// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorPrimitives.h"
#include "IndicatorPrimitive.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitives constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorPrimitives::IndicatorPrimitives()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitives destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
IndicatorPrimitives::~IndicatorPrimitives()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitives::Load
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void IndicatorPrimitives::Load( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "primitives" )
            >> xml::list( "primitive", *this, &IndicatorPrimitives::ReadPrimitive )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitives::ReadPrimitive
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
void IndicatorPrimitives::ReadPrimitive( xml::xistream& xis )
{
    IndicatorPrimitive* primitive = new IndicatorPrimitive( xis );
    Register( primitive->GetName(), *primitive );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPrimitives::Purge
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void IndicatorPrimitives::Purge()
{
    DeleteAll();
}
