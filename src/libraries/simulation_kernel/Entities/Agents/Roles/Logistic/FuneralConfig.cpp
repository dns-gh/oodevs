// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FuneralConfig.h"
#include "FuneralPackagingResource.h"
#include <boost/foreach.hpp>

using namespace logistic;

std::vector< const FuneralPackagingResource* > FuneralConfig::orderedPackagingResources_;
double FuneralConfig::fakeTransporterSpeed_( 0 );

// -----------------------------------------------------------------------------
// Name: FuneralConfig::Initialize
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void FuneralConfig::Initialize( xml::xistream& xis, unsigned tickDuration )
{
    xis >> xml::start( "funeral" )
            >> xml::start( "fake-transporter" )
                >> xml::attribute( "speed", fakeTransporterSpeed_ )
            >> xml::end
            >> xml::start( "packagings" )
                >> xml::list( "packaging", &FuneralConfig::ReadPackagingResource )
        >> xml::end;

    if( fakeTransporterSpeed_ <= 0 )
        xis.error( "invalid speed" );

    fakeTransporterSpeed_ *= 1000. * tickDuration / 3600.;
}

// -----------------------------------------------------------------------------
// Name: FuneralConfig::Terminate
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void FuneralConfig::Terminate()
{
    BOOST_FOREACH( const FuneralPackagingResource* data, orderedPackagingResources_ )
        delete data;
    orderedPackagingResources_.clear();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: FuneralConfig::ReadPackagingResource
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void FuneralConfig::ReadPackagingResource( xml::xistream& xis )
{
    orderedPackagingResources_.push_back( new FuneralPackagingResource( xis ) );
}

// -----------------------------------------------------------------------------
// Name: FuneralConfig::GetOrderedPackagingResources
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
void FuneralConfig::GetOrderedPackagingResources( std::vector< const FuneralPackagingResource* >& container, const FuneralPackagingResource* startingFrom )
{
    if( !startingFrom )
        container = orderedPackagingResources_;
    else
    {
        std::vector< const FuneralPackagingResource* >::iterator it = std::find( orderedPackagingResources_.begin(), orderedPackagingResources_.end(), startingFrom );
        if( it != orderedPackagingResources_.end() )
            std::copy( ++it, orderedPackagingResources_.end(), std::back_inserter( container ) );
    }
}

// -----------------------------------------------------------------------------
// Name: FuneralConfig::GetFakeTransporterSpeed
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
double FuneralConfig::GetFakeTransporterSpeed()
{
    return fakeTransporterSpeed_;
}
