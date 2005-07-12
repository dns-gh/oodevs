// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_SituationLogDefavorable.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 4 $
// $Workfile: MIL_RC_SituationLogDefavorable.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RC_SituationLogDefavorable.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_SituationLogDefavorable constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_SituationLogDefavorable::MIL_RC_SituationLogDefavorable( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_SituationLogDefavorable::~MIL_RC_SituationLogDefavorable()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_SituationLogDefavorable destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_SituationLogDefavorable::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyEnumeration( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_situation_log_defavorable );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_SituationLogDefavorable destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_SituationLogDefavorable::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyEnumeration( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_situation_log_defavorable );
    asnMsg.Send();
}

