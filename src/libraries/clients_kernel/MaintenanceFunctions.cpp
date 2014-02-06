// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MaintenanceFunctions.h"
#include "BreakdownType.h"
#include "ComponentType.h"
#include "EquipmentType.h"

#include "ENT/ENT_Tr.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::TowCapacity constructor
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
MaintenanceFunctions::TowCapacity::TowCapacity( xml::xistream& xis )
    : capacity_( xis.attribute< float >( "capacity" ) )
    , loadTime_( xis.attribute< std::string >( "loading-time" ) )
    , unloadTime_( xis.attribute< std::string >( "unloading-time" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::RepairCapacity constructor
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
MaintenanceFunctions::RepairCapacity::RepairCapacity( xml::xistream& xis )
    : maxRepairTime_( boost::none )
{
    const std::string maxRepairTime = xis.attribute< std::string >( "max-reparation-time", "" );
    const std::string types = xis.attribute( "type", "" );
    if( !maxRepairTime.empty() && maxRepairTime != "0s" )
        maxRepairTime_ = maxRepairTime;
    for( int i = 0; i < eNbrBreakdownType; ++i )
    {
        E_BreakdownType type = static_cast< E_BreakdownType >( i );
        const std::string& typeStr = ENT_Tr::ConvertFromBreakdownType( type );
        if( types.find( typeStr ) != std::string::npos )
            supportedTypes_.push_back( type );
    }
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions constructor
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
MaintenanceFunctions::MaintenanceFunctions( xml::xistream& xis )
    : towCapacity_( boost::none )
{
    xis >> xml::list( "towing", [&] ( xml::xistream& xis )
            {
                towCapacity_ = MaintenanceFunctions::TowCapacity( xis );
            } )
        >> xml::list( "repairing", [&] ( xml::xistream& xis )
            {
                const std::string categoryStr = xis.attribute< std::string >( "category" );
                E_BreakdownNTI category = ENT_Tr::ConvertToBreakdownNTI( categoryStr );
                if( repairCapacities_.find( category ) != repairCapacities_.end() )
                    throw MASA_EXCEPTION( "Category " + categoryStr + " already defined" );
                repairCapacities_.insert( std::make_pair( category, MaintenanceFunctions::RepairCapacity( xis ) ) );
            } );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions destructor
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
MaintenanceFunctions::~MaintenanceFunctions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::CanHaul
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
bool MaintenanceFunctions::CanHaul( const EquipmentType& equipment ) const
{
    return towCapacity_ && towCapacity_->capacity_ >= equipment.GetWeight();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::CanHaul
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
bool MaintenanceFunctions::CanHaul( const ComponentType& component ) const
{
    return towCapacity_ && towCapacity_->capacity_ >= component.GetWeight();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::CanRepair
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
bool MaintenanceFunctions::CanRepair( const BreakdownType& breakdown ) const
{
    auto repairCapacity = repairCapacities_.find( breakdown.GetNTI() );
    return repairCapacity!= repairCapacities_.end() &&
           std::find( repairCapacity->second.supportedTypes_.begin(),
                      repairCapacity->second.supportedTypes_.end(),
                      breakdown.GetType() ) != repairCapacity->second.supportedTypes_.end();
    // $$$$ 2014-02-06: TODO compute TheoricRepairTime in BreakdownType, then check
    // repairCapacity->maxRepairTime_ && breakdown->GetTheoricRepairTime() < repairCapacity->maxRepairTime_
}
