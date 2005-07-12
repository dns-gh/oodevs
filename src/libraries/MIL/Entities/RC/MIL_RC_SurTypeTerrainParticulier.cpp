// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_SurTypeTerrainParticulier.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:21 $
// $Revision: 4 $
// $Workfile: MIL_RC_SurTypeTerrainParticulier.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RC_SurTypeTerrainParticulier.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_SurTypeTerrainParticulier constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_SurTypeTerrainParticulier::MIL_RC_SurTypeTerrainParticulier( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_SurTypeTerrainParticulier::~MIL_RC_SurTypeTerrainParticulier()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_SurTypeTerrainParticulier destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_SurTypeTerrainParticulier::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyEnumeration( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_sur_type_terrain_particulier );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_SurTypeTerrainParticulier destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_SurTypeTerrainParticulier::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyEnumeration( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_sur_type_terrain_particulier );
    asnMsg.Send();
}

