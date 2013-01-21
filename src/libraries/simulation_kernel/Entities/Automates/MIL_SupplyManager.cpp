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
#include "MIL_AgentServer.h"
#include <boost/foreach.hpp>

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
        if( std::find( previous.begin(), previous.end(), pion ) == previous.end() )
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
    // Pas de RC si RC envoy� au tick pr�c�dent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    bool result = ( nCurrentTick > ( nTickRcSupplyQuerySent_ + 1 ) || nTickRcSupplyQuerySent_ == 0 );
    nTickRcSupplyQuerySent_ = nCurrentTick;
    return result;
}
