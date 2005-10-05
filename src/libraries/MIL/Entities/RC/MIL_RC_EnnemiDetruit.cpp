// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_EnnemiDetruit.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 5 $
// $Workfile: MIL_RC_EnnemiDetruit.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_EnnemiDetruit.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnnemiDetruit constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_EnnemiDetruit::MIL_RC_EnnemiDetruit( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_EnnemiDetruit::~MIL_RC_EnnemiDetruit()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnnemiDetruit destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_EnnemiDetruit::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_ennemi_detruit, sender.GetKnowledgeGroup().GetKSQuerier() );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnnemiDetruit destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_EnnemiDetruit::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgentKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_ennemi_detruit, sender.GetKnowledgeGroup().GetKSQuerier() );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnnemiDetruit::Send
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void MIL_RC_EnnemiDetruit::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*parameters*/ ) const
{
    assert( false );
}
