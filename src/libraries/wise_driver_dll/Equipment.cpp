// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Equipment.h"
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/iwisedriversink.h>
#include <wise/WISEAttributeGroupConverter.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Equipment constructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Equipment::Equipment( const sword::EquipmentDotations::EquipmentDotation& message )
    : type_( message.type().id() )
    , available_( 0 )
    , unAvailable_( 0 )
    , repairable_( 0 )
    , inMaintenance_( 0 )
    , captured_( 0 )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: Equipment destructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Equipment::~Equipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Equipment::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Equipment::Update( const sword::EquipmentDotations::EquipmentDotation& message )
{
    available_ = message.available();
    unAvailable_ = message.unavailable();
    repairable_ = message.repairable() + message.on_site_fixable();
    inMaintenance_ = message.repairing();
    captured_ = message.captured();
}

// -----------------------------------------------------------------------------
// Name: Equipment::AddAttributeGroup
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Equipment::AddAttributeGroup( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache ) const
{
    CWISEAttributeGroupTemplate groupTemplate;
    groupTemplate.Add( L"Type", long( type_ ) );
    groupTemplate.Add( L"AvailableCount", long( available_ ) );
    groupTemplate.Add( L"UnavailableCount", long( unAvailable_ ) );
    groupTemplate.Add( L"RepairableCount", long( repairable_ ) );
    groupTemplate.Add( L"InMaintenanceCount", long( inMaintenance_ ) );
    groupTemplate.Add( L"CapturedCount", long( captured_ ) );

    CWISEAttributeGroup group;
    CHECK_WISE_RESULT_EX( CWISEAttributeGroupConverter::TemplateToValueGroup( groupTemplate, &cache, L"Equipment", group ) );
    list.push_back( group );
}
