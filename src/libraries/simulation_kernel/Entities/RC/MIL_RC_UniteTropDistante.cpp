#include "simulation_kernel_pch.h"

#include "MIL_RC_UniteTropDistante.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteTropDistante constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_UniteTropDistante::MIL_RC_UniteTropDistante( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_UniteTropDistante::~MIL_RC_UniteTropDistante()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteTropDistante destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteTropDistante::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypePion( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_unite_trop_distante );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteTropDistante destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteTropDistante::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypePion( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_unite_trop_distante );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_UniteTropDistante::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_UniteTropDistante::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

