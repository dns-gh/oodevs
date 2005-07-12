// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Rc/MIL_RC_TireParCivil.cpp $
// $Author: Nld $
// $Modtime: 30/03/05 11:57 $
// $Revision: 2 $
// $Workfile: MIL_RC_TireParCivil.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RC_TireParCivil.h"

#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_TireParCivil constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_TireParCivil::MIL_RC_TireParCivil( E_RC nRC, int nAsnID )
    : MIL_RC( nRC, nAsnID )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_TireParCivil::~MIL_RC_TireParCivil()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_TireParCivil destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TireParCivil::Send( const MIL_AgentPion& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();   
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_TireParCivil destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_TireParCivil::Send( const MIL_Automate& sender, E_RcType nType, DIA_Parameters& diaParams ) const
{
    assert( DEC_Tools::CheckTypeConnaissanceAgent( diaParams[ 1 ] ) );

    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();
    assert( false );
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_TireParCivil::Send
// Created: NLD 2005-03-30
// -----------------------------------------------------------------------------
void MIL_RC_TireParCivil::Send( const MIL_AgentPion& sender, E_RcType nType, const MIL_Agent_ABC& param ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.GetAsnMsg().cr.u.cr_tire_par_civil = param.GetID();
    asnMsg.Send();
}
