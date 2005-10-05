// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_TransportUnitePrete.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 14:31 $
// $Revision: 1 $
// $Workfile: MIL_RC_TransportUnitePrete.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_TransportUnitePrete.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_TransportUnitePrete constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_TransportUnitePrete::MIL_RC_TransportUnitePrete( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_TransportUnitePrete::~MIL_RC_TransportUnitePrete()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_TransportUnitePrete destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TransportUnitePrete::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_transport_unite_prete );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_TransportUnitePrete destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TransportUnitePrete::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyAgent( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_transport_unite_prete );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_TransportUnitePrete::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TransportUnitePrete::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

