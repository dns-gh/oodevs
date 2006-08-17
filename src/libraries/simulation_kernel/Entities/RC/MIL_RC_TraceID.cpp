// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_TraceID.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 18:41 $
// $Revision: 1 $
// $Workfile: MIL_RC_TraceID.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_RC_TraceID.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Populations/MIL_Population.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_TraceID constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_TraceID::MIL_RC_TraceID( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_TraceID::~MIL_RC_TraceID()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_TraceID destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TraceID::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_trace_id = (uint)diaParams[ 1 ].ToFloat();
    asnMsg.Send();       
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_TraceID destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TraceID::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_trace_id = (uint)diaParams[ 1 ].ToFloat();
    asnMsg.Send();       
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_TraceID::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TraceID::Send( const MIL_Population& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_trace_id = (uint)diaParams[ 1 ].ToFloat();
    asnMsg.Send();       
}

