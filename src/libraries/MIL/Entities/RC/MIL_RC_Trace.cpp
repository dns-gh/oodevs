// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_Trace.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 18:33 $
// $Revision: 1 $
// $Workfile: MIL_RC_Trace.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RC_Trace.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_Trace constructor
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
MIL_RC_Trace::MIL_RC_Trace( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_Trace::~MIL_RC_Trace()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_Trace destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_Trace::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );

    std::string strTmp( diaParams[ 1 ].ToString() );
    asnMsg.GetAsnMsg().cr.u.cr_trace = strTmp.c_str();
    asnMsg.Send();       
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_Trace destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_Trace::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    
    std::string strTmp( diaParams[ 1 ].ToString() );
    asnMsg.GetAsnMsg().cr.u.cr_trace = strTmp.c_str();
    asnMsg.Send();       
}

