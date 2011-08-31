// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Personnel.h"
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/iwisedriversink.h>
#include <wise/WISEAttributeGroupConverter.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Personnel constructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Personnel::Personnel( const sword::HumanDotations::HumanDotation& message )
    : rank_( message.rank() )
    , total_( 0 )
    , available_( 0 )
    , dead_( 0 )
    , wounded_( 0 )
    , mentalDiseased_( 0 )
    , contaminated_( 0 )
    , inTreatment_( 0 )
    , assignedToMaintenance_( 0 )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: Personnel destructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Personnel::~Personnel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Personnel::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Personnel::Update( const sword::HumanDotations::HumanDotation& message )
{
    // $$$$ ABR 2011-08-31: Need to change Personnel architecture to match with story 644. No time for that and no need to maintain wise_drivers so ... commented.
    //total_ = message.total();
    //available_ = message.operational();
    //dead_ = message.dead();
    //wounded_ = message.wounded();
    //mentalDiseased_ = message.mentally_wounded();
    //contaminated_ = message.contaminated();
    //inTreatment_ = message.healing();
    //assignedToMaintenance_ = message.maintenance();
}

// -----------------------------------------------------------------------------
// Name: Personnel::AddAttributeGroup
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Personnel::AddAttributeGroup( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache ) const
{
    CWISEAttributeGroupTemplate groupTemplate;
    groupTemplate.Add( L"Rank", long( rank_ ) );
    groupTemplate.Add( L"TotalCount", long( total_ ) );
    groupTemplate.Add( L"AvailableCount", long( available_ ) );
    groupTemplate.Add( L"DeadCount", long( dead_ ) );
    groupTemplate.Add( L"WoundedCount", long( wounded_ ) );
    groupTemplate.Add( L"MentalDiseasedCount", long( mentalDiseased_ ) );
    groupTemplate.Add( L"ContaminatedCount", long( contaminated_ ) );
    groupTemplate.Add( L"InTreatmentCount", long( inTreatment_ ) );
    groupTemplate.Add( L"AssignedToMaintenanceCount", long( assignedToMaintenance_ ) );

    CWISEAttributeGroup group;
    CHECK_WISE_RESULT_EX( CWISEAttributeGroupConverter::TemplateToValueGroup( groupTemplate, &cache, L"Personnel", group ) );
    list.push_back( group );
}
