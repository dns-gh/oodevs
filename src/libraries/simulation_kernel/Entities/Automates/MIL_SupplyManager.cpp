// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_SupplyManager.h"
#include <boost/range/algorithm.hpp>
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::NotifySuperiorNotAvailable
// Created: MCO 2012-12-11
// -----------------------------------------------------------------------------
void MIL_SupplyManager::NotifySuperiorNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
{
    PostEvent( MIL_Report::eRC_LogNoSuperior, dotationCategory, requesters );
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::NotifyStockNotAvailable
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::NotifyStockNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
{
    PostEvent( MIL_Report::eRC_LogNoStock, dotationCategory, requesters );
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::PostEvent
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::PostEvent( MIL_Report::E_DecisionalReport report, const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
{
    const T_Requesters& previous = previousNotifications_[ std::make_pair( report, &dotationCategory ) ];
    BOOST_FOREACH( T_Requesters::value_type pion, requesters )
    {
        if( boost::find( previous, pion ) == previous.end() )
            MIL_Report::PostEvent( *pion, report, dotationCategory );
        currentNotifications_[ std::make_pair( report, &dotationCategory ) ].push_back( pion );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::Clean
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::Clean()
{
    previousNotifications_ = currentNotifications_;
    currentNotifications_.clear();
}
