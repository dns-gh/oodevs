// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_PointTenuParEni.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 4 $
// $Workfile: MIL_RC_PointTenuParEni.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_RC_PointTenuParEni.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_PointTenuParEni constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PointTenuParEni::MIL_RC_PointTenuParEni( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PointTenuParEni::~MIL_RC_PointTenuParEni()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_PointTenuParEni destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PointTenuParEni::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_point_tenu_par_eni, sender.GetKnowledgeGroup() );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PointTenuParEni destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PointTenuParEni::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_point_tenu_par_eni, sender.GetKnowledgeGroup() );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PointTenuParEni::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PointTenuParEni::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}