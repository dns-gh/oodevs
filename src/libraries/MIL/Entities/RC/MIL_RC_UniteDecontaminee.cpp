// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC_UniteDecontaminee.cpp $
// $Author: Nld $
// $Modtime: 26/04/05 11:52 $
// $Revision: 2 $
// $Workfile: MIL_RC_UniteDecontaminee.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_UniteDecontaminee.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteDecontaminee constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_UniteDecontaminee::MIL_RC_UniteDecontaminee( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_UniteDecontaminee::~MIL_RC_UniteDecontaminee()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteDecontaminee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteDecontaminee::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_unite_decontaminee );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteDecontaminee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteDecontaminee::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_unite_decontaminee );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteDecontaminee::Send
// Created: NLD 2005-04-26
// -----------------------------------------------------------------------------
void MIL_RC_UniteDecontaminee::Send( const MIL_AgentPion& sender, E_RcType nType, const MIL_Agent_ABC& param ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_unite_decontaminee = param.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteDecontaminee::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteDecontaminee::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

