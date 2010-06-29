// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ConstructionCapacity.h"
#include "ObjectPrototype.h"
#include "BuildableCapacity.h"
#include "ImprovableCapacity.h"
#include "Entities\Agents\Units\Dotations\PHY_ConsumptionType.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: ConstructionCapacity constructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
ConstructionCapacity::ConstructionCapacity( ObjectPrototype& prototype, xml::xistream& xis )
    : default_ ( PHY_ConsumptionType::FindConsumptionType( xml::attribute< std::string >( xis, "default-consumption-mode" ) ) )
    , unit_type_ ( eRaw )
    , prototype_ ( prototype )
{
    std::string unit_type( xml::attribute< std::string >( xis, "unit-type" ) );
    if( unit_type == "raw" )
        unit_type_ = eRaw;
    else if( unit_type == "density" )
        unit_type_ = eDensity;
    else
        throw std::runtime_error( "unknown unit-type : " + unit_type + ". Must be 'raw' or 'density'" );
}

// -----------------------------------------------------------------------------
// Name: ConstructionCapacity destructor
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
ConstructionCapacity::~ConstructionCapacity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionCapacity::AddCapacity
// Created: JCR 2008-08-25
// -----------------------------------------------------------------------------
void ConstructionCapacity::AddCapacity( const std::string& capacity, xml::xistream& xis ) const
{
    if( capacity == "buildable" )
        prototype_.AddCapacity( new BuildableCapacity( *default_, unit_type_, xis ) );
    if( capacity == "improvable" )
        prototype_.AddCapacity( new ImprovableCapacity( *default_, unit_type_, xis ) );
}
