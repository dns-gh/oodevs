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
    : dotationCategory_( PHY_DotationType::FindDotationCategory( xis.attribute< std::string >( "resource" ) ) )
    , processDuration_ ( 0 )
    , terminal_        ( xis.attribute< bool >( "terminal" ) )
{
    if( !dotationCategory_ )
        throw MASA_EXCEPTION( xis.context() + "Invalid resource " );
    tools::ReadTimeAttribute( xis, "process-duration", processDuration_ );
    if( processDuration_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "Invalid process duration" );
    processDuration_ = static_cast< unsigned int >( MIL_Tools::ConvertSecondsToSim( processDuration_ ) );
}

// -----------------------------------------------------------------------------
// Name: FuneralPackagingResource::GetDotationCategory
// Created: NLD 2011-08-24
// -----------------------------------------------------------------------------
const PHY_DotationCategory& FuneralPackagingResource::GetDotationCategory() const
{
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
