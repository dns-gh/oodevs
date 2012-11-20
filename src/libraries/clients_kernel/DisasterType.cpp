// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DisasterType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DisasterType constructor
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
DisasterType::DisasterType( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ )
            >> xml::start( "contamination" )
                >> xml::list( "threshold", *this, &DisasterType::ReadContaminationThresholds )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DisasterType destructor
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
DisasterType::~DisasterType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterType::ReadContaminationThresholds
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void DisasterType::ReadContaminationThresholds( xml::xistream& xis )
{
    contamination_[ xis.attribute< double > ( "value" ) ] = xis.attribute< std::string >( "color" );
}

// -----------------------------------------------------------------------------
// Name: DisasterType::GetName
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
const std::string& DisasterType::GetName() const
{
    return name_;
}
