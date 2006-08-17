// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_ProgressionVersEni.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 5 $
// $Workfile: MIL_RC_ProgressionVersEni.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_RC_ProgressionVersEni.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_ProgressionVersEni constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ProgressionVersEni::MIL_RC_ProgressionVersEni( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ProgressionVersEni::~MIL_RC_ProgressionVersEni()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_ProgressionVersEni destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ProgressionVersEni::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_progression_vers_eni, sender.GetKnowledgeGroup() );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ProgressionVersEni destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ProgressionVersEni::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_progression_vers_eni, sender.GetKnowledgeGroup() );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ProgressionVersEni::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ProgressionVersEni::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}
