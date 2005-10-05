// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC_ObstacleEnAttenteActivation.cpp $
// $Author: Nld $
// $Modtime: 31/03/05 10:15 $
// $Revision: 3 $
// $Workfile: MIL_RC_ObstacleEnAttenteActivation.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_ObstacleEnAttenteActivation.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObstacleEnAttenteActivation constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ObstacleEnAttenteActivation::MIL_RC_ObstacleEnAttenteActivation( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ObstacleEnAttenteActivation::~MIL_RC_ObstacleEnAttenteActivation()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObstacleEnAttenteActivation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObstacleEnAttenteActivation::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyObjectKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_obstacle_en_attente_activation, sender.GetKnowledgeGroup().GetKSQuerier() );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObstacleEnAttenteActivation destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObstacleEnAttenteActivation::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyObjectKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_obstacle_en_attente_activation, sender.GetKnowledgeGroup().GetKSQuerier() );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObstacleEnAttenteActivation::Send
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObstacleEnAttenteActivation::Send( const MIL_Population& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    assert( false );
}