// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Agent_ABC.h"
#include "Report_ABC.h"
#include "RC.h"
#include "App.h"
#include "Trace.h"
#include "Net_Def.h"
#include "FireResult.h"
#include "LineManager.h"

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Agent_ABC constructor
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
Agent_ABC::Agent_ABC()
: reportVector_ ()
, reportPoints_ ()
, fireResults_  ()
, bListened_    ( false )
, nCurrentMission_    ( 0 )
, nCurrentLeftLimit_  ( 0 )
, nCurrentRightLimit_ ( 0 )
{
}
// -----------------------------------------------------------------------------
// Name: Agent_ABC destructor
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
Agent_ABC::~Agent_ABC()
{
    DeleteAllRCs();
    DeleteAllTraces();
    fireResults_.clear();
}

// =============================================================================
// REPORTS Trace/RCs...
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Agent_ABC::DeleteAllRCs
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void Agent_ABC::DeleteAllRCs()
{
    for( IT_ReportVector it = reportVector_.begin(); it != reportVector_.end(); )
    {
        if( (*it)->IsRCType() )
        {
            delete *it;
            it = reportVector_.erase( it );
            continue;
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::DeleteAllTraces
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void Agent_ABC::DeleteAllTraces()
{
    for( IT_ReportVector it = reportVector_.begin(); it != reportVector_.end(); )
    {
        if( (*it)->GetType() == Report_ABC::eTrace )
        {
            delete *it;
            it = reportVector_.erase( it );
            continue;
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::DeleteReport
// Created: HME 2005-10-07
// -----------------------------------------------------------------------------
void Agent_ABC::DeleteReport( Report_ABC& report )
{
    IT_ReportVector it = std::find( reportVector_.begin(), reportVector_.end(), &report );
    assert( it != reportVector_.end() );
    reportVector_.erase( it );
    delete &report;
}

//-----------------------------------------------------------------------------
// Name: Agent::OnReceiveMsgCR
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void Agent_ABC::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    RC& rc = *new RC( *this );
    rc.Initialize( asnMsg );
    reportVector_.push_back( &rc );

    App::GetApp().NotifyReportCreated( *this, rc );
}


//-----------------------------------------------------------------------------
// Name: Agent::OnReceiveTraceMsg
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void Agent_ABC::OnReceiveTraceMsg( DIN_Input& msg )
{
    Trace& trace = *new Trace( *this, msg );
    reportVector_.push_back( &trace );

    App::GetApp().NotifyReportCreated( *this, trace );
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::OnReceiveDebugDrawPointsMsg
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void Agent_ABC::OnReceiveDebugDrawPointsMsg( DIN::DIN_Input& msg )
{
    uint32 nTmp;
    msg >> nTmp;

    reportPoints_.clear();
    reportPoints_.reserve( nTmp );
    for( uint i = 0; i < nTmp; ++i )
    {
        MT_Vector2D vPos;
        msg >> vPos;
        reportPoints_.push_back( vPos );
    }
}

// =============================================================================
// FIRE RESULTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Agent_ABC::OnReceiveMsgStopFire
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void Agent_ABC::OnReceiveMsgStopFire( const ASN1T_FireDamagesPion& asnMsg )
{
    fireResults_.push_back( new FireResult( asnMsg ) );
    if( fireResults_.size() > 20 )
        fireResults_.erase( fireResults_.begin() );
}
    
// -----------------------------------------------------------------------------
// Name: Agent_ABC::OnReceiveMsgStopFire
// Created: SBO 2005-12-06
// -----------------------------------------------------------------------------
void Agent_ABC::OnReceiveMsgStopFire( const ASN1T_FireDamagesPopulation& asnMsg )
{
    fireResults_.push_back( new FireResult( asnMsg ) );
    if( fireResults_.size() > 20 )
        fireResults_.erase( fireResults_.begin() );
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::DeleteAllFireResults
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void Agent_ABC::DeleteAllFireResults()
{
    for( CIT_FireResults it = fireResults_.begin(); it != fireResults_.end(); ++it )
        delete *it;
    fireResults_.clear();
}

//-----------------------------------------------------------------------------
// Name: Agent_ABC::OnReceiveMsgWaitForOrderConduite
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void Agent_ABC::OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    RC& rc = *new RC( *this );
    rc.Initialize( asnMsg );
    reportVector_.push_back( &rc );

    App::GetApp().NotifyReportCreated( *this, rc );
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::OnSendMissionOrder
// Created: HME 2006-01-30
// -----------------------------------------------------------------------------
void Agent_ABC::OnSendMissionOrder( ASN1T_MsgPionOrder order )
{
    sentOrders_[ order.order_id ] = order;
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::OnReceiveMission
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
void Agent_ABC::OnReceiveMission( const ASN1T_MsgPionOrderAck& asnMsg )
{
    if( (int)asnMsg.error_code == 0 )
    {
        IT_MissionOrders itOrder = sentOrders_.find( asnMsg.order_id );
        if( itOrder != sentOrders_.end() )
        {
            nCurrentMission_ =  itOrder->second.mission.t;
            if( itOrder->second.m.oid_limite_droitePresent )
                nCurrentLeftLimit_ = itOrder->second.oid_limite_gauche;
            else
                nCurrentLeftLimit_ = 0;
            if( itOrder->second.m.oid_limite_gauchePresent )
                nCurrentRightLimit_ = itOrder->second.oid_limite_droite;
            else
                nCurrentRightLimit_ = 0;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_ABC::GetCurrentMission
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
int Agent_ABC::GetCurrentMission() const
{
    return nCurrentMission_;
}

// -----------------------------------------------------------------------------
// Name: Agent::OnReceiveMission
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
void Agent_ABC::OnReceiveMission( const ASN1T_MsgPionOrder& asnMsg )
{
    nCurrentMission_ = asnMsg.mission.t;
    if( asnMsg.m.oid_limite_droitePresent )
        nCurrentLeftLimit_ = asnMsg.oid_limite_gauche;
    else
        nCurrentLeftLimit_ = 0;
    if( asnMsg.m.oid_limite_gauchePresent )
        nCurrentRightLimit_ = asnMsg.oid_limite_droite;
    else
        nCurrentRightLimit_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Agent::RightLimit
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
TacticalLine_ABC* Agent_ABC::GetRightLimit() const 
{
    if( nCurrentRightLimit_ == 0 )
        return 0;

    return App::GetApp().GetLineManager().FindLine( nCurrentRightLimit_ );
}

// -----------------------------------------------------------------------------
// Name: Agent::LeftLimit
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
TacticalLine_ABC* Agent_ABC::GetLeftLimit() const
{
    if( nCurrentRightLimit_ == 0 )
        return 0;

    return App::GetApp().GetLineManager().FindLine( nCurrentLeftLimit_ );
}