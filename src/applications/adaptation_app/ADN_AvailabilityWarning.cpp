// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_AvailabilityWarning.h"

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning constructor
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
ADN_AvailabilityWarning::ADN_AvailabilityWarning()
    : percentage_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning destructor
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
ADN_AvailabilityWarning::~ADN_AvailabilityWarning()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning::ReadArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarning::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "availability-threshold", percentage_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning::WriteArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarning::WriteArchive( xml::xostream& output )
{
    output << xml::start( "resource-availability-alert" )
            << xml::attribute( "availability-threshold", percentage_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning::WriteArchive
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarning::WriteArchive( xml::xostream& output, const std::string& attribute, const std::string& value )
{
    output << xml::start( "resource-availability-alert" )
            << xml::attribute( "availability-threshold", percentage_ )
            << xml::attribute( attribute, value )
           << xml::end;
}
