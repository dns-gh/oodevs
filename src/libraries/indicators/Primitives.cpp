// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Primitives.h"
#include "DataTypeFactory.h"
#include "Primitive.h"
#include <xeumeuleu/xml.h>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Primitives constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
Primitives::Primitives()
    : types_( new DataTypeFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Primitives destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
Primitives::~Primitives()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: Primitives::Load
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void Primitives::Load( const std::string& file )
{
    xml::xifstream xis( file );
    xis >> xml::start( "primitives" )
            >> xml::list( "primitive", *this, &Primitives::ReadPrimitive )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Primitives::ReadPrimitive
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
void Primitives::ReadPrimitive( xml::xistream& xis )
{
    Primitive* primitive = new Primitive( xis, *types_ );
    Register( primitive->GetName(), *primitive );
}

// -----------------------------------------------------------------------------
// Name: Primitives::Purge
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void Primitives::Purge()
{
    DeleteAll();
}
