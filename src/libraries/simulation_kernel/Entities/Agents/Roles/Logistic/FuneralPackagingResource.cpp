// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FuneralPackagingResource.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "tools/Codec.h"
#include "Tools/MIL_Tools.h"

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: FuneralPackagingResource constructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralPackagingResource::FuneralPackagingResource( xml::xistream& xis )
{
    std::string resource;
    xis >> xml::attribute( "resource", resource );
    dotationCategory_ = PHY_DotationType::FindDotationCategory( resource );
    if( !dotationCategory_ )
        xis.error( "Invalid resource " );

    tools::ReadTimeAttribute( xis, "process-duration", processDuration_ );
    if( processDuration_ < 0 )
        xis.error( "Invalid process duration" );
    processDuration_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( processDuration_ ) );

    xis >> xml::attribute( "terminal", terminal_ );
}

// -----------------------------------------------------------------------------
// Name: FuneralPackagingResource destructor
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
FuneralPackagingResource::~FuneralPackagingResource()
{
    // NOTHING
}

// =============================================================================
// Getters
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralPackagingResource::GetDotationCategory
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& FuneralPackagingResource::GetDotationCategory() const
{
    assert( dotationCategory_ );
    return *dotationCategory_;
}

// -----------------------------------------------------------------------------
// Name: FuneralPackagingResource::GetProcessDuration
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
unsigned FuneralPackagingResource::GetProcessDuration() const
{
    return processDuration_;
}

// -----------------------------------------------------------------------------
// Name: FuneralPackagingResource::IsTerminal
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
bool FuneralPackagingResource::IsTerminal() const
{
    return terminal_;
}
