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
#include "ADN_XmlInput_Helper.h"

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
// Name: ADN_AvailabilityWarning::GetNodeName
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
std::string ADN_AvailabilityWarning::GetNodeName()
{
    return std::string();
}
    
// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning::GetItemName
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
std::string ADN_AvailabilityWarning::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning::ReadArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarning::ReadArchive( ADN_XmlInput_Helper& input, const std::string& fieldName )
{
    input.Section( fieldName );
    input.ReadAttribute( "pourcentageMoyensDisponibles", percentage_ );
    input.EndSection();
}
    
// -----------------------------------------------------------------------------
// Name: ADN_AvailabilityWarning::WriteArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_AvailabilityWarning::WriteArchive( MT_OutputArchive_ABC& output, const std::string& fieldName )
{
    output.Section( fieldName );
    output.WriteAttribute( "pourcentageMoyensDisponibles", percentage_.GetData() );
    output.EndSection();
}
