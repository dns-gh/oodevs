// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_ObservationTirIndirect.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 2 $
// $Workfile: MIL_RC_ObservationTirIndirect.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_RC_ObservationTirIndirect.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObservationTirIndirect constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ObservationTirIndirect::MIL_RC_ObservationTirIndirect( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ObservationTirIndirect::~MIL_RC_ObservationTirIndirect()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObservationTirIndirect destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObservationTirIndirect::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& /*diaParams*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();   
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObservationTirIndirect destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObservationTirIndirect::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& /*diaParams*/ ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObservationTirIndirect::Send
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void MIL_RC_ObservationTirIndirect::Send( const MIL_AgentPion& sender, E_RcType nType, const MIL_Effect_IndirectFire& fire ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    
    asnMsg.GetAsnMsg().cr.u.cr_observation_tir_indirect = fire.GetFireID();
    asnMsg.Send();    
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObservationTirIndirect destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObservationTirIndirect::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}