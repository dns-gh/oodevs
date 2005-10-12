// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Agent_ABC.h"
#include "MOS_Report_ABC.h"
#include "MOS_RC.h"
#include "MOS_App.h"
#include "MOS_Trace.h"

using namespace DIN;

// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC constructor
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
MOS_Agent_ABC::MOS_Agent_ABC()
{
}
// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC destructor
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
MOS_Agent_ABC::~MOS_Agent_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC::DeleteAllRCs
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void MOS_Agent_ABC::DeleteAllRCs()
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
// Name: MOS_Agent_ABC::DeleteAllTraces
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
void MOS_Agent_ABC::DeleteAllTraces()
{
    for( IT_ReportVector it = reportVector_.begin(); it != reportVector_.end(); )
    {
        if( (*it)->GetType() == MOS_Report_ABC::eTrace )
        {
            delete *it;
            it = reportVector_.erase( it );
            continue;
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent_ABC::DeleteReport
// Created: HME 2005-10-07
// -----------------------------------------------------------------------------
void MOS_Agent_ABC::DeleteReport( MOS_Report_ABC& report )
{
    IT_ReportVector it = std::find( reportVector_.begin(), reportVector_.end(), &report );
    assert( it != reportVector_.end() );
    reportVector_.erase( it );
    delete &report;
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveMsgCR
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent_ABC::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    MOS_RC& rc = *new MOS_RC( *this );
    rc.Initialize( asnMsg );
    reportVector_.push_back( &rc );

    MOS_App::GetApp().NotifyReportCreated( *this, rc );
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::OnReceiveTraceMsg
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_Agent_ABC::OnReceiveTraceMsg( DIN_Input& msg )
{
    MOS_Trace& trace = *new MOS_Trace( *this );
    trace.Initialize( msg );
    reportVector_.push_back( &trace );

    MOS_App::GetApp().NotifyReportCreated( *this, trace );
}