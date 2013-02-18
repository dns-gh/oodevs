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
void MIL_SupplyManager::NotifySuperiorNotAvailable( const T_Requesters& requesters )
{
    for( auto it = requesters.begin(); it != requesters.end(); ++it )
    {
        if( std::find( previousReportNotifications_.begin(), previousReportNotifications_.end(), *it ) == previousReportNotifications_.end() )
        {
            if( std::find( currentReportNotifications_.begin(), currentReportNotifications_.end(), *it ) == currentReportNotifications_.end() )
            {
                MIL_Report::PostEvent( **it, MIL_Report::eRC_LogNoSuperior );
                currentReportNotifications_.push_back( *it );
            }
        }
        else if( std::find( currentReportNotifications_.begin(), currentReportNotifications_.end(), *it ) == currentReportNotifications_.end() )
            currentReportNotifications_.push_back( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::NotifyStockNotAvailable
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::NotifyStockNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters )
{
    const T_Requesters& previous = previousDotationNotifications_[ &dotationCategory ];
    for( auto it = requesters.begin(); it != requesters.end(); ++it )
    {
        if( std::find( previous.begin(), previous.end(), *it ) == previous.end() )
            MIL_Report::PostEvent( **it, MIL_Report::eRC_LogNoStock, dotationCategory );
        currentDotationNotifications_[ &dotationCategory ].push_back( *it );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::Clean
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
void MIL_SupplyManager::Clean()
{
    previousDotationNotifications_ = currentDotationNotifications_;
    previousReportNotifications_ = currentReportNotifications_;
    currentDotationNotifications_.clear();
    currentReportNotifications_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_SupplyManager::SendSupplyNeededReport
// Created: MCO 2012-12-19
// -----------------------------------------------------------------------------
bool MIL_SupplyManager::SendSupplyNeededReport()
{
    // Pas de RC si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    bool result = ( nCurrentTick > ( nTickRcSupplyQuerySent_ + 1 ) || nTickRcSupplyQuerySent_ == 0 );
    nTickRcSupplyQuerySent_ = nCurrentTick;
    return result;
}
