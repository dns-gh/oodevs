// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_MissionTerminee.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 16:23 $
// $Revision: 4 $
// $Workfile: MIL_RC_MissionTerminee.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_MissionTerminee.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_MissionTerminee constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_MissionTerminee::MIL_RC_MissionTerminee( E_RC nRC )
    : MIL_RC( nRC )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_MissionTerminee::~MIL_RC_MissionTerminee()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_MissionTerminee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_MissionTerminee::Send( const MIL_AgentPion& sender, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    const_cast< MIL_AgentPion& >( sender ).GetOrderManager().OnReceiveRCMissionFinished();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_MissionTerminee destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_MissionTerminee::Send( const MIL_Automate& sender, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    const_cast< MIL_Automate& >( sender ).GetOrderManager().OnReceiveRCMissionFinished();
}

