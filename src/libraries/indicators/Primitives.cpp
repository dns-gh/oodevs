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
#include <xeumeuleu/xml.hpp>

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
void Primitives::Load( xml::xistream& xis )
{
    xis >> xml::start( "primitives" )
            >> xml::list( "primitive", [&]( xml::xistream& x )
                {
                    Primitive* primitive = new Primitive( x, *types_ );
                    Register( primitive->GetName(), *primitive );
                } )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Primitives::Purge
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
void Primitives::Purge()
{
    DeleteAll();
}
