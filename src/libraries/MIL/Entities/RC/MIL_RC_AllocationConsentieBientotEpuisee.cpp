// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_AllocationConsentieBientotEpuisee.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 16:06 $
// $Revision: 1 $
// $Workfile: MIL_RC_AllocationConsentieBientotEpuisee.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_AllocationConsentieBientotEpuisee.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_AllocationConsentieBientotEpuisee constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_AllocationConsentieBientotEpuisee::MIL_RC_AllocationConsentieBientotEpuisee( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_AllocationConsentieBientotEpuisee::~MIL_RC_AllocationConsentieBientotEpuisee()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_AllocationConsentieBientotEpuisee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_AllocationConsentieBientotEpuisee::Send( const MIL_AgentPion& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_AllocationConsentieBientotEpuisee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_AllocationConsentieBientotEpuisee::Send( const MIL_Automate& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_AllocationConsentieBientotEpuisee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_AllocationConsentieBientotEpuisee::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_AllocationConsentieBientotEpuisee::Send
// Created: NLD 2005-03-09
// -----------------------------------------------------------------------------
void MIL_RC_AllocationConsentieBientotEpuisee::Send( const MIL_Automate& sender, E_RcType nType, const PHY_DotationCategory& category ) const
{
   NET_ASN_MsgCR asnMsg;
   FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
   asnMsg.GetAsnMsg().cr.u.cr_allocation_consentie_bientot_epuisee = category.GetMosID();
   asnMsg.Send();
}
