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
#include "MIL_Time_ABC.h"
#include <boost/range/algorithm.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_SupplyManager );

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::MIL_SupplyManager
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
MIL_SupplyManager::MIL_SupplyManager()
    : nTickRcSupplyQuerySent_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::~MIL_SupplyManager
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
MIL_SupplyManager::~MIL_SupplyManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::NotifySuperiorNotAvailable
// Created: MCO 2012-12-11
// -----------------------------------------------------------------------------
void MIL_SupplyManager::NotifySuperiorNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
{
    PostEvent( report::eRC_LogNoSuperior, dotationCategory, requesters );
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::NotifyStockNotAvailable
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::NotifyStockNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
{
    PostEvent( report::eRC_LogNoStock, dotationCategory, requesters );
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::PostEvent
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::PostEvent( const MIL_DecisionalReport& report, const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
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

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::SendSupplyNeededReport
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
bool MIL_SupplyManager::SendSupplyNeededReport()
{
    // Pas de RC si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    bool result = ( nCurrentTick > ( nTickRcSupplyQuerySent_ + 1 ) || nTickRcSupplyQuerySent_ == 0 );
    nTickRcSupplyQuerySent_ = nCurrentTick;
    return result;
}
