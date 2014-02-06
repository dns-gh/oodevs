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
// Name: MaintenanceFunctions::NTI constructor
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
MaintenanceFunctions::Towing::Towing( xml::xistream& xis )
    : capacity_( xis.attribute< float >( "capacity" ) )
    , loadTime_( xis.attribute< std::string >( "loading-time" ) )
    , unloadTime_( xis.attribute< std::string >( "unloading-time" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::NTI constructor
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
MaintenanceFunctions::NTI::NTI( xml::xistream& xis )
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
    : towing_( boost::none )
{
    xis >> xml::list( "towing", [&] ( xml::xistream& xis )
            {
                towing_ = MaintenanceFunctions::Towing( xis );
            } )
        >> xml::list( "repairing", [&] ( xml::xistream& xis )
            {
                const std::string categoryStr = xis.attribute< std::string >( "category" );
                E_BreakdownNTI category = ENT_Tr::ConvertToBreakdownNTI( categoryStr );
                if( repairing_.find( category ) != repairing_.end() )
                    throw MASA_EXCEPTION( "Category " + categoryStr + " already defined" );
                repairing_.insert( std::make_pair( category, MaintenanceFunctions::NTI( xis ) ) );
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
    return towing_ && towing_->capacity_ >= equipment.GetWeight();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::CanHaul
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
bool MaintenanceFunctions::CanHaul( const ComponentType& component ) const
{
    return towing_ && towing_->capacity_ >= component.GetWeight();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceFunctions::CanRepair
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
bool MaintenanceFunctions::CanRepair( const BreakdownType& breakdown ) const
{
    auto nti = repairing_.find( breakdown.GetNTI() );
    return nti != repairing_.end() &&
           std::find( nti->second.supportedTypes_.begin(),
                      nti->second.supportedTypes_.end(),
                      breakdown.GetType() ) != nti->second.supportedTypes_.end();
    // $$$$ 2014-02-06: TODO compute TheoricRepairTime in BreakdownType, then check
    // nti->maxRepairTime_ && breakdown->GetTheoricRepairTime() < nti->maxRepairTime_
}
