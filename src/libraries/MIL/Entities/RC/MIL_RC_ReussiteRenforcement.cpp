// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_ReussiteRenforcement.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 5 $
// $Workfile: MIL_RC_ReussiteRenforcement.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_ReussiteRenforcement.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_ReussiteRenforcement constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ReussiteRenforcement::MIL_RC_ReussiteRenforcement( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ReussiteRenforcement::~MIL_RC_ReussiteRenforcement()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_ReussiteRenforcement destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ReussiteRenforcement::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypePion    ( diaParams[ 1 ] ) );
    assert( DEC_Tools::CheckTypeAutomate( diaParams[ 2 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );    
    
    ASN1T_CR_ReussiteRenforcement asnCR;    
    NET_ASN_Tools::CopyAgent   ( diaParams[ 1 ], asnCR.pion_renforcant   );
    NET_ASN_Tools::CopyAutomate( diaParams[ 2 ], asnCR.automate_renforce );
    asnMsg.GetAsnMsg().cr.u.cr_reussite_renforcement = &asnCR;
    
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ReussiteRenforcement destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ReussiteRenforcement::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypePion    ( diaParams[ 1 ] ) );
    assert( DEC_Tools::CheckTypeAutomate( diaParams[ 2 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );    
    
    ASN1T_CR_ReussiteRenforcement asnCR;    
    NET_ASN_Tools::CopyAgent   ( diaParams[ 1 ], asnCR.pion_renforcant   );
    NET_ASN_Tools::CopyAutomate( diaParams[ 2 ], asnCR.automate_renforce );
    asnMsg.GetAsnMsg().cr.u.cr_reussite_renforcement = &asnCR;
    
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ReussiteRenforcement::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ReussiteRenforcement::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}
