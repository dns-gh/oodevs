// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_UniteEnnemieReconnue.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 4 $
// $Workfile: MIL_RC_UniteEnnemieReconnue.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_RC_UniteEnnemieReconnue.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/DEC_Knowledge_Agent.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteEnnemieReconnue constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_UniteEnnemieReconnue::MIL_RC_UniteEnnemieReconnue( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_UniteEnnemieReconnue::~MIL_RC_UniteEnnemieReconnue()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteEnnemieReconnue destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteEnnemieReconnue::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_unite_ennemie_reconnue, sender.GetKnowledgeGroup() );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteEnnemieReconnue destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteEnnemieReconnue::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_unite_ennemie_reconnue, sender.GetKnowledgeGroup() );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteEnnemieReconnue::Send
// Created: NLD 2005-03-30
// -----------------------------------------------------------------------------
void MIL_RC_UniteEnnemieReconnue::Send( const MIL_AgentPion& sender, E_RcType nType, const DEC_Knowledge_Agent& param ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_unite_ennemie_reconnue = param.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteEnnemieReconnue::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteEnnemieReconnue::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

