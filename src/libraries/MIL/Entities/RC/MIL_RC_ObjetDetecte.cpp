// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC_ObjetDetecte.cpp $
// $Author: Nld $
// $Modtime: 31/03/05 10:15 $
// $Revision: 3 $
// $Workfile: MIL_RC_ObjetDetecte.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_ObjetDetecte.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObjetDetecte constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ObjetDetecte::MIL_RC_ObjetDetecte( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_ObjetDetecte::~MIL_RC_ObjetDetecte()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObjetDetecte destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObjetDetecte::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyObjectKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_objet_detecte, sender.GetKnowledgeGroup().GetKSQuerier() );
    asnMsg.Send();   
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObjetDetecte destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_ObjetDetecte::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceObjet( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    NET_ASN_Tools::CopyObjectKnowledge( diaParams[ 1 ], asnMsg.GetAsnMsg().cr.u.cr_objet_detecte, sender.GetKnowledgeGroup().GetKSQuerier() );
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_ObjetDetecte::Send
// Created: NLD 2005-03-30
// -----------------------------------------------------------------------------
void MIL_RC_ObjetDetecte::Send( const MIL_AgentPion& sender, E_RcType nType, const DEC_Knowledge_Object& param ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_objet_detecte = param.GetMosID();
    asnMsg.Send();
}
