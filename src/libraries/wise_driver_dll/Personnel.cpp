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
Personnel::Personnel( const MsgsSimToClient::HumanDotations::HumanDotation& message )
    : rank_( message.rang() )
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
void Personnel::Update( const MsgsSimToClient::HumanDotations::HumanDotation& message )
{
    total_ = message.nb_total();
    available_ = message.nb_operationnels();
    dead_ = message.nb_morts();
    wounded_ = message.nb_blesses();
    mentalDiseased_ = message.nb_blesses_mentaux();
    contaminated_ = message.nb_contamines_nbc();
    inTreatment_ = message.nb_dans_chaine_sante();
    assignedToMaintenance_ = message.nb_utilises_pour_maintenance();
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
