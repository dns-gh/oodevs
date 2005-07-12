// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC_PionDecontamine.cpp $
// $Author: Nld $
// $Modtime: 26/04/05 11:52 $
// $Revision: 2 $
// $Workfile: MIL_RC_PionDecontamine.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RC_PionDecontamine.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_PionDecontamine constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PionDecontamine::MIL_RC_PionDecontamine( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_PionDecontamine::~MIL_RC_PionDecontamine()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_PionDecontamine destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PionDecontamine::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_pion_decontamine );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PionDecontamine destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_PionDecontamine::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_pion_decontamine );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_PionDecontamine::Send
// Created: NLD 2005-04-26
// -----------------------------------------------------------------------------
void MIL_RC_PionDecontamine::Send( const MIL_AgentPion& sender, E_RcType nType, const MIL_Agent_ABC& param ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_pion_decontamine = param.GetID();
    asnMsg.Send();
}
